#include "stdafx.h"
#include "physic_engine.h"
#include <math.h>
#include <time.h>

PEBirdObject pe_bird;
PEGroundObject pe_ground;
PEPipeObject pe_pipes[4];

#define PI 3.1415926

static double g_gravity = 80.0;

const int g_pipeInterval = 1400.0;

static double _getRandPipeUp()
{
	// �϶˹��ӵĿ�������Ӧ���� 40 ~ ground - 40
	return (double)(rand() % (280 - 40 - 40) + 40) * 10.0;
}

static void _PhysicEngine_initPipes()
{
	int cxPipe = SCENE_WIDTH * 10.0 + g_pipeInterval;
	for (size_t i = 0; i < sizeof(pe_pipes) / sizeof(PEPipeObject); ++i)
	{
		PEPipeObject* pipe = &pe_pipes[i];
		pipe->cx = cxPipe;
		cxPipe += g_pipeInterval;
		pipe->width = 260.0;
		pipe->cyUp = _getRandPipeUp();
		pipe->cyDown = pipe->cyUp + 1200.0;
	}
}

void PhysicEngine_setBirdPos(int ox, int oy)
{
	pe_bird.angle = 0;
	pe_bird.cx = ox;
	pe_bird.cy = pe_bird.oy = oy;
	pe_bird.hv = 50.0;
	pe_bird.vv = 0.0;
	pe_bird.width = 120.0;
	pe_bird.height = 120.0;
	pe_bird.isDead = false;
	pe_bird.canBeHigher = true;
}

void PhysicEngine_reset()
{
	pe_ground.cy = PhysicEngine_pixelToRealCoord(SCENE_HEIGHT - 112);
	srand(time(NULL));
	_PhysicEngine_initPipes();
}

static void _PhysicEngine_birdTick(double t)
{
	// ���С���Ѿ����������Ѿ����ڵ��ϣ��Ͳ��ٸı�״̬
	if (pe_bird.isStopped)
		return;
	
	double vvPre = pe_bird.vv;
	pe_bird.vv += g_gravity * t;	// v1 = v0 + gt
	pe_bird.vv = pe_bird.vv > 500.0 ? 500.0 : pe_bird.vv;
	double vs = 0.5 * (vvPre + pe_bird.vv) * t;	// s = 1/2 * (v0 + v1) * t
	pe_bird.cy += vs;
	// ���С����أ����ٵ���
	if (pe_bird.cy + pe_bird.height >= pe_ground.cy)
		pe_bird.cy = pe_ground.cy - pe_bird.height;

	// С��ĳ�����Ǻϳ��ٶȵĳ���
	//pe_bird.angle = _arctan(pe_bird.vv / pe_bird.hv);
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
			break;
		}
	}
}

static void _PhysicEngine_checkHit()
{
	if (pe_bird.isStopped)
		return;
	// ���С����û��ײ������
	if (pe_bird.cy + pe_bird.height >= pe_ground.cy)
	{
		pe_bird.isDead = true;
		pe_bird.isStopped = true;
	}
	if (pe_bird.isDead)
		return;
	// ���С����û��ײ��ˮ��
	for (size_t idxPipe = 0; idxPipe < sizeof(pe_pipes) / sizeof(PEPipeObject); ++idxPipe)
	{
		PEPipeObject* pipe = &pe_pipes[idxPipe];
		// ��ֱ�߶ȣ��Ƿ��������ܵ�֮��
		if (pe_bird.cy + pe_bird.height <= pipe->cyDown
			&& pe_bird.cy - pe_bird.height >= pipe->cyUp)
			continue;
		// ˮƽ��飬�Ƿ������ܵ�
		if (pe_bird.cx + pe_bird.width >= pipe->cx - pipe->width
			&& pe_bird.cx - pe_bird.width <= pipe->cx + pipe->width)
		{
			pe_bird.isDead = true;
			break;
		}
	}
	// ���С����û�зɵ����
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

void PhysicEngine_float(int tickCount)
{
	// ������һ����г�˶����乫ʽΪ�� x = Acos(wt+p)
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

