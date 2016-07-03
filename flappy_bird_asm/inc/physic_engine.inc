IFNDEF	_PHYSIC_ENGINE_INC_
_PHYSIC_ENGINE_INC_	equ <1>

PhysicEngine_init								proto stdcall
PhysicEngine_tick								proto stdcall
PhysicEngine_movingGround						proto stdcall moving:BOOL
PhysicEngine_showPipes							proto stdcall show:BOOL
PhysicEngine_movingPipes						proto stdcall moving:BOOL
PhysicEngine_floatingBird						proto stdcall
PhysicEngine_prepareBird						proto stdcall
PhysicEngine_freeBird							proto stdcall
PhysicEngine_birdFly							proto stdcall
PhysicEngine_isBirdDead							proto stdcall
PhysicEngine_isBirdStopped						proto stdcall
PhysicEngine_passedPipe							proto stdcall

ENDIF