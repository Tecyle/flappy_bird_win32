#pragma once

void PhysicEngine_init();
void PhysicEngine_tick();
void PhysicEngine_movingGround(bool moving);
void PhysicEngine_showPipes(bool show);
void PhysicEngine_movingPipes(bool moving);
void PhysicEngine_floatingBird();
void PhysicEngine_fixBird();
void PhysicEngine_freeBird();
void PhysicEngine_birdFly();
bool PhysicEngine_isBirdDead();
bool PhysicEngine_isBirdStopped();
bool PhysicEngine_passedPipe();


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