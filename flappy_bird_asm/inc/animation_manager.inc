IFNDEF	_ANIMATION_MANAGER_INC_
_ANIMATION_MANAGER_INC_	equ	<1>

include		image_manager.inc

Animation		struct 
	; ���뵭����������
	alphaStart		SDWORD		?
	alphaEnd		SDWORD		?
	nowAlpha		SDWORD		?
	fadeStep		SDWORD		?			; ��¼����ÿ�� tick Ҫ�������� alpha
	fadeLoop		BOOL		?
	fadeEnable		BOOL		?
	; ��֡������������
	frameCount		SDWORD		?
	frameIndex		SDWORD		?
	frameStep		SDWORD		?			; ��¼����ÿ�� frame ͣ�����ٸ� tick
	frameTickCount	SDWORD		?
	frameLoop		BOOL		?
	frameEnable		BOOL		?
	; �ƶ���������
	sx				SDWORD		?
	sy				SDWORD		?
	ex				SDWORD		?
	ey				SDWORD		?
	nx				SDWORD		?
	ny				SDWORD		?
	transXStep		SDWORD		?			; ��¼����ÿ�� tick �� x ������ֵ
	transYStep		SDWORD		?			; ��¼����ÿ�� tick �� y ������ֵ
	transLoop		BOOL		?
	transEnable		BOOL		?
Animation		ends
AnimationPtr	typedef	ptr Animation

; ���뵭������
FadeAnimation_init					proto stdcall	o : AnimationPtr, from : DWORD, to : DWORD, duringTime : SDWORD, _loop : BOOL
FadeAnimation_tick					proto stdcall	o : AnimationPtr
FadeAnimation_getAlpha				proto stdcall	o : AnimationPtr
FadeAnimation_finished				proto stdcall	o : AnimationPtr

; ��֡����
FrameAnimation_init					proto stdcall	o : AnimationPtr, frameCount : SDWORD, duringTime : SDWORD, _loop : BOOL
FrameAnimation_tick					proto stdcall	o : AnimationPtr
FrameAnimation_getFrameIndex		proto stdcall	o : AnimationPtr
FrameAnimation_finished				proto stdcall	o : AnimationPtr

; �ƶ�����
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