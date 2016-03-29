#include "stdafx.h"
#include "physic_engine.h"

PEBirdObject bird;
PEGroundObject ground;
PEPipeObject pipes[4];

static double g_gravity = 100.0;

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

void PhysicEngine_reset()
{

}

static void _PhysicEngine_birdTick(double t)
{
	if (bird.isDead && bird.cy + bird.height >= ground.cy)
		return;
	
	double vvPre = bird.vv;
	bird.vv = g_gravity * t;
	double vs = 0.5 * (vvPre + bird.vv) * t;
	bird.cy += vs;
	if (bird.cy >= ground.cy)
		bird.cy = ground.cy;

	bird.angle = _arctan(bird.vv)
}

void PhysicEngine_tick(int tickCount)
{

}
