#include "stdafx.h"
#include "physic_engine.h"
#include <math.h>

PEBirdObject pe_bird;
PEGroundObject pe_ground;
PEPipeObject pe_pipes[4];

static double g_gravity = 100.0;

#define PI 3.1415926

static double _pow(double x, int n)
{
	double res = 1.0;
	while (n--)
		res *= x;
	return res;
}

static double _arctan(double x)
{
	double res = 0.0;

	for (int i = 1; i <= 5; ++i)
	{
		res += (i % 2 == 1 ? 1.0 : -1.0) * _pow(x, i) / i;
	}

	return res;
}

#define abs(x) ((x) > 0 ? (x) : (-(x)))

double _sin(double x)
{
	const double B = 1.2732395447;
	const double C = -0.4052847346;
	const double P = 0.2310792853;//0.225;
	while (x > 0.5 * PI)
		x -= 2.0 * PI;
	float y = B * x + C * x * abs(x);
	y = P * (y * abs(y) - y) + y;
	return y;
}

static double _cos(double x)
{
	double res =
		1.0 - _pow(x, 2) / 2.0 + _pow(x, 4) / 24.0 - _pow(x, 6) / 720.0
		+ _pow(x, 8) / 40320.0 - _pow(x, 10) / 3628800.0;
	assert(res < 1 && res > -1);
	return res;
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
}

void PhysicEngine_reset()
{
	pe_ground.cy = PhysicEngine_pixelToRealCoord(SCENE_HEIGHT - 112);
}

static void _PhysicEngine_birdTick(double t)
{
	// 如果小鸟已经死亡并且已经掉在地上，就不再改变状态
	if (pe_bird.isStopped)
		return;
	
	double vvPre = pe_bird.vv;
	pe_bird.vv += g_gravity * t;	// v1 = v0 + gt
	double vs = 0.5 * (vvPre + pe_bird.vv) * t;	// s = 1/2 * (v0 + v1) * t
	pe_bird.cy += vs;
	// 如果小鸟落地，则不再掉落
	if (pe_bird.cy + pe_bird.height >= pe_ground.cy)
		pe_bird.cy = pe_ground.cy - pe_bird.height;

	// 小鸟的朝向就是合成速度的朝向
	pe_bird.angle = _arctan(pe_bird.vv / pe_bird.hv);
}

static void _PhysicEngine_pipeTick(double t)
{
	for (size_t idxPipe = 0; idxPipe < sizeof(pe_pipes) / sizeof(PEPipeObject); ++idxPipe)
	{
		PEPipeObject* pipe = &pe_pipes[idxPipe];
		pipe->cx -= pe_bird.hv * t;
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
		if (pe_bird.cy + pe_bird.height <= pe_ground.cy - pipe->downLength
			&& pe_bird.cy - pe_bird.height >= pipe->upLength)
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
	pe_bird.canBeHigher = pe_bird.cy > 20;
}

void PhysicEngine_tick(int tickCount)
{
	double t = tickCount * 0.15;
	_PhysicEngine_birdTick(t);
	_PhysicEngine_pipeTick(t);
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
	pe_bird.vv = -300.0;
}
