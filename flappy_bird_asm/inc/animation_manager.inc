IFNDEF	_ANIMATION_MANAGER_INC_
_ANIMATION_MANAGER_INC_	equ	<1>

include		image_manager.inc

Animation		struct 
	; 淡入淡出动画参数
	alphaStart		SDWORD		?
	alphaEnd		SDWORD		?
	nowAlpha		SDWORD		?
	fadeStep		SDWORD		?			; 记录的是每个 tick 要增长多少 alpha
	fadeLoop		BOOL		?
	fadeEnable		BOOL		?
	; 逐帧动画动画参数
	frameCount		SDWORD		?
	frameIndex		SDWORD		?
	frameStep		SDWORD		?			; 记录的是每个 frame 停留多少个 tick
	frameTickCount	SDWORD		?
	frameLoop		BOOL		?
	frameEnable		BOOL		?
	; 移动动画参数
	sx				SDWORD		?
	sy				SDWORD		?
	ex				SDWORD		?
	ey				SDWORD		?
	nx				SDWORD		?
	ny				SDWORD		?
	transXStep		SDWORD		?			; 记录的是每个 tick 后 x 的增长值
	transYStep		SDWORD		?			; 记录的是每个 tick 后 y 的增长值
	transLoop		BOOL		?
	transEnable		BOOL		?
Animation		ends
AnimationPtr	typedef	ptr Animation

; 淡入淡出动画
FadeAnimation_init					proto stdcall	o : AnimationPtr, from : DWORD, to : DWORD, duringTime : SDWORD, _loop : BOOL
FadeAnimation_tick					proto stdcall	o : AnimationPtr
FadeAnimation_getAlpha				proto stdcall	o : AnimationPtr
FadeAnimation_finished				proto stdcall	o : AnimationPtr

; 逐帧动画
FrameAnimation_init					proto stdcall	o : AnimationPtr, frameCount : SDWORD, duringTime : SDWORD, _loop : BOOL
FrameAnimation_tick					proto stdcall	o : AnimationPtr
FrameAnimation_getFrameIndex		proto stdcall	o : AnimationPtr
FrameAnimation_finished				proto stdcall	o : AnimationPtr

; 移动动画
TransAnimation_init					proto stdcall	o : AnimationPtr, sx : SDWORD, sy : SDWORD, ex : SDWORD, ey : SDWORD, duringTime : SDWORD, _loop : BOOL
TransAnimation_tick					proto stdcall	o : AnimationPtr
TransAnimation_getXY				proto stdcall	o : AnimationPtr, x : ptr SDWORD, y : ptr SDWORD
TransAnimation_finished				proto stdcall	o : AnimationPtr

AnimationManager	struct 
	ani			Animation		32 dup(?)
	aniCount	DWORD			?
AnimationManager	ends
AnimationManagerPtr	typedef ptr Animation

AnimationManager_init				proto stdcall
AnimationManager_allocAnimation		proto stdcall
AnimationManager_tick				proto stdcall


ENDIF