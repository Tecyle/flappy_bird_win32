#pragma once

typedef struct PEBirdObject
{
	double oy;
	double cx;
	double cy;
	double angle;
	double width;
	double height;

	double hv;
	double vv;

	bool isDead;
	bool isStopped;
	bool canBeHigher;
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
void PhysicEngine_setBirdPos(int ox, int oy);
void PhysicEngine_float(int tickCount);
int PhysicEngine_realToPixelCoord(double x);
double PhysicEngine_pixelToRealCoord(int x);

extern PEBirdObject pe_bird;
extern PEGroundObject pe_ground;
extern PEPipeObject pe_pipes[4];