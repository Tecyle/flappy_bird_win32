#include "stdafx.h"
#include "physic_engine.h"
#include "image_manager.h"
#include "animation_manager.h"
#include <math.h>
#include <time.h>

#define PI 3.1415926
#define REAL_TO_PIXEL(x) ((x) / 10)
#define PIXEL_TO_REAL(x) ((x) * 10)

static double g_gravity = 80.0;
const int g_pipeInterval = 1400;

//////////////////////////////////////////////////////////////////////////
//
typedef enum BirdState
{
	BirdState_floating,
	BirdState_free,
	BirdState_fixed,
	BirdState_dead
} BirdState;

typedef enum GroundState
{
	GroundState_moving,
	GroundState_stop
} GroundState;

typedef enum PipeState
{
	PipeState_unset,
	PipeState_moving,
	PipeState_stop
} PipeState;

typedef struct PhysicEngine
{
	BirdState birdState;
	GroundState groundState;
	PipeState pipeState;

	double birdTickCount;		///< С��ļ�ʱ��
	bool isBirdDropped;			///< С���Ƿ��Ѿ����ڵذ�����
	bool canBeHigher;
	double birdVSpeed;			///< С��ĺ����ٶ�
	double birdHSpeed;			///< С��������ٶ�

	int groundMovingLength;

	bool pipePassed[4];
} PhysicEngine;

static PhysicEngine g_physicEngine;

// ���㸡���е�С���λ��
void _PhysicEngine_birdFloating()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	// ������һ����г�˶����乫ʽΪ�� x = Acos(wt+p)
	// ����Ϸ�У����ı�С��� y ���꣬�����ı�С��� x ����
	o->birdTickCount += 1.0;
	if (o->birdTickCount > 50.0)
		o->birdTickCount = 0.0;
	double dx = 50.0 * cos(2.0 * PI * 0.02 * o->birdTickCount);
	bird->cy = (int)(REAL_TO_PIXEL(PIXEL_TO_REAL(bird->oy) + dx));
}

// �������ɷ�����С���λ��
void _PhysicEngine_birdFree()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);
	double tick = 0.15;
	// ���С���Ѿ����������Ѿ����ڵ��ϣ��Ͳ��ٸı�״̬
	if (o->isBirdDropped)
		return;

	double vvPre = o->birdVSpeed;
	o->birdVSpeed += g_gravity * tick;	// v1 = v0 + gt
	o->birdVSpeed = o->birdVSpeed > 700.0 ? 700.0 : o->birdVSpeed;
	double vs = 0.5 * (vvPre + o->birdVSpeed) * tick;	// s = 1/2 * (v0 + v1) * t
	bird->cy += (int)(REAL_TO_PIXEL(vs));
	// ���С����أ����ٵ���
	if (bird->cy + bird->halfHeight >= ground->cy - ground->halfHeight)
		bird->cy = ground->cy - ground->halfHeight - bird->halfHeight;

	// С��ĳ�����Ǻϳ��ٶȵĳ���
	bird->angle = atan(o->birdVSpeed / o->birdHSpeed);
	// TODO : ���������е�С��ĳ���Ӧ���Ǹ��̶�ֵ
}

void _PhysicEngine_birdTick()
{
	PhysicEngine* o = &g_physicEngine;
	switch (o->birdState)
	{
	case BirdState_floating:
		_PhysicEngine_birdFloating();
		break;
	case BirdState_free:
	case BirdState_dead:
		_PhysicEngine_birdFree();
		break;
	case BirdState_fixed:
		break;
	}
}

// �ƶ�����
void _PhysicEngine_groundTick()
{
	PhysicEngine* o = &g_physicEngine;

	if (o->groundState == GroundState_stop)
		return;

	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);
	o->groundMovingLength = (o->groundMovingLength + 1) % 24;
	ground->cx = ground->ox - o->groundMovingLength;
}

// �ƶ��ܵ�
void _PhysicEngine_movingPipes()
{
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);
	for (size_t idxPipe = 0; idxPipe < 4; ++idxPipe)
	{
		upPipes[idxPipe].cx -= 1;
		downPipes[idxPipe].cx -= 1;
	}
}

static double _PhysicEngine_getRandPipeUp()
{
	// �϶˹��ӵĿ�������Ӧ���� 40 ~ ground - 40
	return (double)(rand() % (280 - 40 - 40) + 40) * 10.0;
}

void _PhysicEngine_initPipes()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);
	int cxPipe = 288 + REAL_TO_PIXEL(g_pipeInterval);
	for (size_t i = 0; i < 4; ++i)
	{
		Spirit* upPipe = upPipes + i;
		Spirit* downPipe = downPipes + i;

		upPipe->cx = cxPipe;
		cxPipe += REAL_TO_PIXEL(g_pipeInterval);
		upPipe->cy = (int)REAL_TO_PIXEL(_PhysicEngine_getRandPipeUp()) - upPipe->halfHeight;
		
		downPipe->cx = upPipe->cx;
		downPipe->cy = upPipe->cy + upPipe->halfHeight + 120 + downPipe->halfHeight;

		o->pipePassed[i] = false;
	}
}

void _PhysicEngine_loopPipes()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);

	for (size_t i = 0; i < 4; ++i)
	{
		Spirit* upPipe = upPipes + i;
		Spirit* downPipe = downPipes + i;
		if (upPipe->cx + upPipe->halfWidth < 0)
		{
			upPipe->cx += REAL_TO_PIXEL(4 * g_pipeInterval);
			upPipe->cy = (int)REAL_TO_PIXEL(_PhysicEngine_getRandPipeUp()) - upPipe->halfHeight;

			downPipe->cx = upPipe->cx;
			downPipe->cy = upPipe->cy + upPipe->halfHeight + 120 + downPipe->halfHeight;

			o->pipePassed[i] = false;
			break;
		}
	}
}

void _PhysicEngine_pipesTick()
{
	PhysicEngine* o = &g_physicEngine;

	switch (o->pipeState)
	{
	case PipeState_stop:
	case PipeState_unset:
		break;
	case PipeState_moving:
		_PhysicEngine_movingPipes();
		_PhysicEngine_loopPipes();
		break;
	}
}

// �����Ϸ�еľ���״̬����Ҫ�У�
// * �ж��ǲ���С��ײ������������
// * �ж�С���ǲ��Ƿɵ���ߴ���
// * �ж�С���ǲ��Ǹո�ͨ��һ������
void _PhysicEngine_checkBirdState()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);

	if (o->birdState == BirdState_floating || o->birdState == BirdState_fixed)
		return;
	if (o->birdState == BirdState_dead && o->isBirdDropped)
		return;

	// ���С����û��ײ������
	if (bird->cy + bird->halfHeight >= ground->cy - ground->halfHeight)
	{
		o->birdState = BirdState_dead;
		bird->ani->frameEnable = false;
		o->isBirdDropped = true;
	}
	if (o->birdState == BirdState_dead)
		return;
	// ���С����û��ײ��ˮ��
	for (size_t idxPipe = 0; idxPipe < 4; ++idxPipe)
	{
		Spirit* upPipe = upPipes + idxPipe;
		Spirit* downPipe = downPipes + idxPipe;
		// ��ֱ�߶ȣ��Ƿ��������ܵ�֮��
		if (bird->cy + bird->halfHeight <= downPipe->cy - downPipe->halfHeight
			&& bird->cy - bird->halfHeight >= upPipe->cy + upPipe->halfHeight)
			continue;
		// ˮƽ��飬�Ƿ������ܵ�
		if (bird->cx + bird->halfWidth >= upPipe->cx - upPipe->halfWidth
			&& bird->cx - bird->halfWidth <= upPipe->cx + upPipe->halfWidth)
		{
			o->birdState = BirdState_dead;
			bird->ani->frameEnable = false;
			break;
		}
	}
	// ���С����û�зɵ����
	o->canBeHigher = bird->cy > 40;
}

// ������Ϸ�е�
// ���¼�����Ϸ�е�λ���߼�
// ��Ϸ��֡������ 60 fps������ÿһ�� tick ��ʱ������ 1/60 ��
// ��Ϸ�п����˶���������Ҫ��������
// * С����Ҫ�и��������ɡ���������״̬
// * ���棺��Ҫ���ƶ���ֹͣ����״̬
// * �ܵ�����Ҫ��δ���֡��ƶ���ֹͣ����״̬
void PhysicEngine_tick()
{
	_PhysicEngine_birdTick();
	_PhysicEngine_groundTick();
	_PhysicEngine_pipesTick();
	_PhysicEngine_checkBirdState();
}

bool PhysicEngine_isBirdDead()
{
	PhysicEngine* o = &g_physicEngine;
	return o->birdState == BirdState_dead;
}

void PhysicEngine_init()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);

	ground->cx = 144;
	ground->cy = 456;
	srand((unsigned int)time(NULL));
	for (size_t i = 0; i < 4; ++i)
	{
		o->pipePassed[i] = 0;
	}
}

bool PhysicEngine_isBirdStopped()
{
	PhysicEngine* o = &g_physicEngine;
	return o->isBirdDropped;
}

void PhysicEngine_birdFly()
{
	PhysicEngine* o = &g_physicEngine;
	if (o->birdState != BirdState_free)
		return;
	if (o->canBeHigher)
		o->birdVSpeed = -300.0;
}

void PhysicEngine_floatingBird()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	Animation* frameAnimation = bird->ani;

	bird->cx = 130;
	bird->cy = 210;
	bird->ox = 130;
	bird->oy = 210;
	bird->angle = 0.0;
	o->birdState = BirdState_floating;
	o->isBirdDropped = false;
	FrameAnimation_init(frameAnimation, 4, 1000, true);
}

void PhysicEngine_fixBird()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	Animation* frameAnimation = bird->ani;

	bird->cx = 80;
	bird->cy = 220;
	bird->ox = 80;
	bird->oy = 220;
	bird->angle = 0.0;
	o->birdState = BirdState_fixed;
	o->isBirdDropped = false;
	FrameAnimation_init(frameAnimation, 4, 1000, true);
}

void PhysicEngine_freeBird()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);

	bird->angle = 0.0;
	o->birdState = BirdState_free;
	o->birdHSpeed = 200.0;
	o->birdVSpeed = 0.0;
	o->isBirdDropped = false;
}

bool PhysicEngine_passedPipe()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);

	for (size_t i = 0; i < 4; ++i)
	{
		// ���ĳ�� pipe �Ѿ�ͨ��С���λ�ã�����û�б�ǣ���˵���Ǹ�ͨ����
		if (!o->pipePassed[i])
		{
			Spirit* upPipe = upPipes + i;
			if (upPipe->cx < bird->cx)
			{
				o->pipePassed[i] = true;
				return true;
			}
		}
	}
	return false;
}

void PhysicEngine_showPipes(bool show)
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);

	for (size_t i = 0; i < 4; ++i)
	{
		Spirit* upPipe = upPipes + i;
		Spirit* downPipe = downPipes + i;
		upPipe->visiable = show;
		downPipe->visiable = show;
	}

	if (!show)
		o->pipeState = PipeState_unset;
}

void PhysicEngine_movingPipes(bool moving)
{
	PhysicEngine* o = &g_physicEngine;
	if (moving)
	{
		if (o->pipeState != PipeState_moving)
		{
			o->pipeState = PipeState_moving;
			_PhysicEngine_initPipes();
		}
	}
	else
	{
		o->pipeState = PipeState_stop;
	}
}

void PhysicEngine_movingGround(bool moving)
{
	PhysicEngine* o = &g_physicEngine;
	o->groundState = moving ? GroundState_moving : GroundState_stop;
}
