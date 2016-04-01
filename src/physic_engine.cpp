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
	// 上端管子的开口坐标应该是 40 ~ ground - 40
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
	// 如果小鸟已经死亡并且已经掉在地上，就不再改变状态
	if (pe_bird.isStopped)
		return;
	
	double vvPre = pe_bird.vv;
	pe_bird.vv += g_gravity * t;	// v1 = v0 + gt
	pe_bird.vv = pe_bird.vv > 500.0 ? 500.0 : pe_bird.vv;
	double vs = 0.5 * (vvPre + pe_bird.vv) * t;	// s = 1/2 * (v0 + v1) * t
	pe_bird.cy += vs;
	// 如果小鸟落地，则不再掉落
	if (pe_bird.cy + pe_bird.height >= pe_ground.cy)
		pe_bird.cy = pe_ground.cy - pe_bird.height;

	// 小鸟的朝向就是合成速度的朝向
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

