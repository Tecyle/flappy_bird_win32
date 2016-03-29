#pragma once

typedef struct PEBirdObject
{
	double cx;
	double cy;
	double angle;
	double width;
	double height;

	double hv;
	double vv;

	bool isDead;
} PEBirdObject;

typedef struct PEPipeObject
{
	double upLength;
	double downLength;
	double cx;
	double width;
} PEPipeObject;

typedef struct PEGroundObject
{
	double height;
	double cy;
} PEGroundObject;

void PhysicEngine_tick(int tickCount);  // tick = 1/20 s