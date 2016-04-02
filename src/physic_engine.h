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
	double cyUp;
	double cyDown;
	double cx;
	double width;
	bool pipePassed;
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
void PhysicEngine_reset();
void PhysicEngine_BirdFly();
void PhysicEngine_dieTick(int tickCount);
bool PhysicEngine_passedPipe();

extern PEBirdObject pe_bird;
extern PEGroundObject pe_ground;
extern PEPipeObject pe_pipes[4];