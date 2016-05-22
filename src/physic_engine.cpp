#include "stdafx.h"
#include "physic_engine.h"
#include "image_manager.h"
#include <math.h>
#include <time.h>

PEBirdObject pe_bird;
PEGroundObject pe_ground;
PEPipeObject pe_pipes[4];

#define PI 3.1415926
#define REAL_TO_PIXEL(x) ((x) / 10)
#define PIXEL_TO_REAL(x) ((x) * 10)

static double g_gravity = 80.0;

const int g_pipeInterval = 1400.0;

//////////////////////////////////////////////////////////////////////////
//
typedef enum BirdState
{
	BirdState_floating,
	BirdState_free,
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

	double birdTickCount;		///< 小鸟的计时器
	bool isBirdDropped;			///< 小鸟是否已经死在地板上了
	bool canBeHigher;
	double birdVSpeed;			///< 小鸟的横向速度
	double birdHSpeed;			///< 小鸟的纵向速度

	int groundMovingLength;

	bool pipePassed[4];
} PhysicEngine;

static PhysicEngine g_physicEngine;

// 计算浮动中的小鸟的位置
void _PhysicEngine_birdFloating()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	// 浮动是一个简谐运动，其公式为： x = Acos(wt+p)
	// 在游戏中，仅改变小鸟的 y 坐标，而不改变小鸟的 x 坐标
	o->birdTickCount += 1.0;
	if (o->birdTickCount > 50.0)
		o->birdTickCount = 0.0;
	double dx = 50.0 * cos(2.0 * PI * 0.02 * o->birdTickCount);
	bird->cy = REAL_TO_PIXEL(PIXEL_TO_REAL(bird->oy) + dx);
}

// 计算自由飞行中小鸟的位置
void _PhysicEngine_birdFree()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);
	double tick = 0.15;
	// 如果小鸟已经死亡并且已经掉在地上，就不再改变状态
	if (o->isBirdDropped)
		return;

	double vvPre = o->birdVSpeed;
	o->birdVSpeed += g_gravity * tick;	// v1 = v0 + gt
	o->birdVSpeed = o->birdVSpeed > 700.0 ? 700.0 : o->birdVSpeed;
	double vs = 0.5 * (vvPre + o->birdVSpeed) * tick;	// s = 1/2 * (v0 + v1) * t
	bird->cy += REAL_TO_PIXEL(vs);
	// 如果小鸟落地，则不再掉落
	if (bird->cy + bird->halfHeight >= ground->cy - ground->halfHeight)
		bird->cy = ground->cy - ground->halfHeight - bird->halfHeight;

	// 小鸟的朝向就是合成速度的朝向
	bird->angle = atan(pe_bird.vv / pe_bird.hv);
	// TODO : 上升过程中的小鸟的朝向应该是个固定值
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
	}
}

// 移动地面
void _PhysicEngine_groundTick()
{
	PhysicEngine* o = &g_physicEngine;

	if (o->groundState == GroundState_stop)
		return;

	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);
	o->groundMovingLength = (o->groundMovingLength + 1) % (ground->halfWidth * 2 - SCENE_WIDTH);
	ground->cx += o->groundMovingLength;
}

// 移动管道
void _PhysicEngine_movingPipes()
{
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);
	for (size_t idxPipe = 0; idxPipe < 4; ++idxPipe)
	{
		upPipes[idxPipe].cx -= 1;
		downPipes[idxPipe].cy -= 1;
	}
}

static double _PhysicEngine_getRandPipeUp()
{
	// 上端管子的开口坐标应该是 40 ~ ground - 40
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

// 检查游戏中的精灵状态，主要有：
// * 判断是不是小鸟撞到东西死掉了
// * 判断小鸟是不是飞到最高处了
// * 判断小鸟是不是刚刚通过一个管子
void _PhysicEngine_checkSpiritState()
{
	PhysicEngine* o = &g_physicEngine;
	Spirit* bird = ImageManager_getSpirit(SpiritType_bird);
	Spirit* ground = ImageManager_getSpirit(SpiritType_ground);
	Spirit* upPipes = ImageManager_getSpirit(SpiritType_upPipes);
	Spirit* downPipes = ImageManager_getSpirit(SpiritType_downPipes);

	if (o->birdState == BirdState_floating)
		return;
	if (o->birdState == BirdState_dead && o->isBirdDropped)
		return;

	// 检查小鸟有没有撞到地面
	if (bird->cy + bird->halfHeight >= ground->cy - ground->halfHeight)
	{
		o->birdState = BirdState_dead;
		o->isBirdDropped = true;
	}
	if (o->birdState == BirdState_dead)
		return;
	// 检测小鸟有没有撞到水管
	for (size_t idxPipe = 0; idxPipe < 4; ++idxPipe)
	{
		Spirit* upPipe = upPipes + idxPipe;
		Spirit* downPipe = downPipes + idxPipe;
		// 垂直高度，是否在两个管道之间
		if (bird->cy + bird->halfHeight <= downPipe->cy - downPipe->halfHeight
			&& bird->cy - bird->halfHeight >= upPipe->cy + upPipe->halfHeight)
			continue;
		// 水平检查，是否碰到管道
		if (bird->cx + bird->halfWidth >= upPipe->cx - upPipe->halfWidth
			&& bird->cx - bird->halfWidth <= upPipe->cx + upPipe->halfWidth)
		{
			o->birdState = BirdState_dead;
			break;
		}
	}
	// 检查小鸟有没有飞到最高
	o->canBeHigher = bird->cy > 40;
}

// 计算游戏中的
// 重新计算游戏中的位置逻辑
// 游戏的帧速率是 60 fps，所以每一次 tick 的时间间隔是 1/60 秒
// 游戏中可以运动的物体主要有三个：
// * 小鸟：主要有浮动、自由、死亡三种状态
// * 地面：主要有移动、停止两种状态
// * 管道：主要有未出现、移动和停止三种状态
void PhysicEngine_tick()
{
	_PhysicEngine_birdTick();
	_PhysicEngine_groundTick();
	_PhysicEngine_pipesTick();
}

void PhysicEngine_setBirdPos(int ox, int oy)
{
	pe_bird.angle = 0.0;
	pe_bird.cx = ox;
	pe_bird.cy = pe_bird.oy = oy;
	pe_bird.hv = 200.0;
	pe_bird.vv = 0.0;
	pe_bird.width = 150.0;
	pe_bird.height = 150.0;
	pe_bird.isDead = false;
	pe_bird.isStopped = false;
	pe_bird.canBeHigher = true;
}

void PhysicEngine_reset()
{
	pe_ground.cy = PhysicEngine_pixelToRealCoord(SCENE_HEIGHT - 112);
	srand(time(NULL));
	_PhysicEngine_initPipes();
}


static void _PhysicEngine_pipeTick(double t)
{
	for (size_t idxPipe = 0; idxPipe < sizeof(pe_pipes) / sizeof(PEPipeObject); ++idxPipe)
	{
		PEPipeObject* pipe = &pe_pipes[idxPipe];
		pipe->cx -= 30 * 20 / 60;
	}
}

static void _PhysicEngine_circlePipe()
{
	for (size_t i = 0; i < sizeof(pe_pipes) / sizeof(PEPipeObject); ++i)
	{
		PEPipeObject* pipe = &pe_pipes[i];
		if (pipe->cx + pipe->width < 0.0)
		{
			pipe->cx += 4.0 * g_pipeInterval;
			pipe->cyUp = _getRandPipeUp();
			pipe->cyDown = pipe->cyUp + 1200.0;
			pipe->pipePassed = false;
			break;
		}
	}
}

static void _PhysicEngine_checkHit()
{
	if (pe_bird.isStopped)
		return;
	// 检查小鸟有没有撞到地面
	if (pe_bird.cy + pe_bird.height >= pe_ground.cy)
	{
		pe_bird.isDead = true;
		pe_bird.isStopped = true;
	}
	if (pe_bird.isDead)
		return;
	// 检测小鸟有没有撞到水管
	for (size_t idxPipe = 0; idxPipe < sizeof(pe_pipes) / sizeof(PEPipeObject); ++idxPipe)
	{
		PEPipeObject* pipe = &pe_pipes[idxPipe];
		// 垂直高度，是否在两个管道之间
		if (pe_bird.cy + pe_bird.height <= pipe->cyDown
			&& pe_bird.cy - pe_bird.height >= pipe->cyUp)
			continue;
		// 水平检查，是否碰到管道
		if (pe_bird.cx + pe_bird.width >= pipe->cx - pipe->width
			&& pe_bird.cx - pe_bird.width <= pipe->cx + pipe->width)
		{
			pe_bird.isDead = true;
			break;
		}
	}
	// 检查小鸟有没有飞到最高
	pe_bird.canBeHigher = pe_bird.cy > 40;
}

void PhysicEngine_tick(int tickCount)
{
	double t = tickCount * 0.15;
	_PhysicEngine_birdTick(t);
	_PhysicEngine_pipeTick(t);
	_PhysicEngine_checkHit();
	_PhysicEngine_circlePipe();
}

void PhysicEngine_dieTick(int tickCount)
{
	double t = tickCount * 0.15;
	_PhysicEngine_birdTick(t);
	_PhysicEngine_checkHit();
}

void PhysicEngine_float(int tickCount)
{
	// 浮动是一个简谐运动，其公式为： x = Acos(wt+p)
	static double t = 0;
	t += tickCount;
	if (t > 50)
		t = 0.0;
	double dx = 50.0 * cos(2.0 * PI * 0.02 * t);
	pe_bird.cy = pe_bird.oy + dx;
}

int PhysicEngine_realToPixelCoord(double x)
{
	return (int)(x / 10);
}

double PhysicEngine_pixelToRealCoord(int x)
{
	return x * 10.0;
}

void PhysicEngine_BirdFly()
{
	if (pe_bird.canBeHigher)
		pe_bird.vv = -300.0;
}

bool PhysicEngine_passedPipe()
{
	for (size_t i = 0; i < sizeof(pe_pipes) / sizeof(PEPipeObject); ++i)
	{
		PEPipeObject* pipe = &pe_pipes[i];
		// 如果某个 pipe 已经通过小鸟的位置，但是没有标记，则说明是刚通过的
		if (!pipe->pipePassed)
		{
			if (pipe->cx < pe_bird.cx)
			{
				pipe->pipePassed = true;
				return true;
			}
		}
	}
	return false;
}

