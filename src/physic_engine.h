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
