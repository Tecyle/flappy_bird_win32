include		stdafx.inc
include		animation_manager.inc

	.data
g_aniMgr		AnimationManager	<>

	.code

;////////////////////////////////////////////////////////////////////////
; FadeAnimation
FadeAnimation_init		proc stdcall uses eax ebx ecx edx	o : AnimationPtr, from : SDWORD, to : SDWORD, duringTime : SDWORD, _loop : BOOL
	local	@tickCount : SDWORD
	
	assume	ebx : AnimationPtr
	mov		ebx, o
	mov		[ebx].fadeEnable, TRUE
	push	from
	pop		[ebx].alphaStart
	push	to
	pop		[ebx].alphaEnd
	push	_loop
	pop		[ebx].fadeLoop
	push	from
	pop		[ebx].nowAlpha
	mov		eax, 60
	mov		edx, 0
	mul		duringTime
	mov		ecx, 1000
	div		ecx
	mov		@tickCount, eax
	.if		to > from
		mov		eax, to
		sub		eax, from
	.else
		mov		eax, from
		sub		eax, to
	.endif
	mov		edx, 0
	div		@tickCount
	mov		[ebx].fadeStep, eax
	assume	ebx : nothing
	ret
FadeAnimation_init		endp

FadeAnimation_tick		proc stdcall public uses eax ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	.if		[ebx].fadeEnable == FALSE
		ret
	.endif

	.if		[ebx].nowAlpha == [ebx].alphaEnd
		.if		[ebx].fadeLoop
			push	[ebx].alphaStart
			pop		[ebx].nowAlpha
		.else
			mov		[ebx].fadeEnable, FALSE
		.endif
		ret
	.endif

	.if		[ebx].alphaEnd > [ebx].alphaStart
		mov		eax, [ebx].nowAlpha
		add		eax, [ebx].fadeStep
		mov		[ebx].nowAlpha, eax
		.if		eax > [ebx].alphaEnd
			push	[ebx].alphaEnd
			pop		[ebx].nowAlpha
		.endif
	.else
		mov		eax, [ebx].nowAlpha
		sub		eax, [ebx].fadeStep
		mov		[ebx].nowAlpha, eax
		.if		eax < [ebx].alphaEnd
			push	[ebx].alphaEnd
			pop		[ebx].nowAlpha
		.endif
	.endif
	assume	ebx : nothing
	ret
FadeAnimation_tick		endp

FadeAnimation_getAlpha	proc stdcall public uses ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	mov		eax, [ebx].nowAlpha
	assume	ebx : nothing
	ret
FadeAnimation_getAlpha	endp

FadeAnimation_finished	proc stdcall public uses ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	.if		[ebx].fadeLoop == FALSE && [ebx].nowAlpha == [ebx].alphaEnd
		mov		eax, TRUE
	.else
		mov		eax, FALSE
	.endif
	assume	ebx : nothing
	ret
FadeAnimation_finished	endp

;////////////////////////////////////////////////////////////////////////
; FrameAniamtion
FrameAnimation_init		proc stdcall public uses eax ebx ecx edx	o : AnimationPtr, frameCount : SDWORD, duringTime : SDWORD, _loop BOOL
	local	@tickCount : SDWORD

	assume	ebx : AnimationPtr
	mov		ebx, o
	push	frameCount
	pop		[ebx].frameCount
	mov		[ebx].frameIndex, 0
	mov		[ebx].frameEnable, TRUE
	push	_loop
	pop		[ebx].frameLoop
	mov		edx, 0
	mov		eax, 60
	mul		duringTime
	mov		ecx, 1000
	mov		edx, 0
	div		ecx
	mov		@tickCount, eax
	mov		edx, 0
	div		frameCount
	mov		[ebx].frameStep, eax
	mov		[ebx].frameTickCount, 0
	assume	ebx : nothing
	ret
FrameAnimation_init		endp

FrameAnimation_tick		proc stdcall public uses eax ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	.if		[ebx].frameEnable == FALSE
		ret
	.endif

	inc		[ebx].frameTickCount
	.if		[ebx].frameTickCount < [ebx].frameStep
		ret
	.else
		mov		[ebx].frameTickCount, 0
	.endif

	mov		eax, [ebx].frameCount
	dec		eax
	.if		[ebx].frameIndex == eax
		.if		[ebx].frameLoop == TRUE
			mov		[ebx].frameIndex, 0
		.else
			mov		[ebx].frameEnable, FALSE
		.endif
		ret
	.endif

	inc		[ebx].frameIndex
	assume	ebx : nothing
	ret
FrameAnimation_tick		endp

FrameAnimation_getFrameIndex	proc stdcall public uses ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	mov		eax, [ebx].frameIndex
	assume	ebx : nothing
	ret
FrameAnimation_getFrameIndex	endp

FrameAnimation_finished	proc stdcall public uses ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	mov		eax, [ebx].frameCount
	dec		eax
	.if		[ebx].frameLoop == FALSE && [ebx].frameIndex == eax
		mov		eax, TRUE
	.else
		mov		eax, FALSE
	.endif
	assume	ebx : nothing
	ret
FrameAnimation_finished	endp

;////////////////////////////////////////////////////////////////////////
; TransAnimation
TransAnimation_init		proc stdcall public uses eax ebx ecx edx	o : AnimationPtr, sx : SDWORD, sy : SDWORD, \
						ex : SDWORD, ey : SDWORD, duringTime : SDWORD, _loop : BOOL
	local	@tickCount : SDWORD

	assume	ebx : AnimationPtr
	mov		ebx, o
	mov		[ebx].transEnable, TRUE
	push	sx
	pop		[ebx].sx
	push	sy
	pop		[ebx].sy
	push	ex
	pop		[ebx].ex
	push	ey
	pop		[ebx].ey
	push	sx
	pop		[ebx].nx
	push	sy
	pop		[ebx].ny
	push	_loop
	pop		[ebx].transLoop
	mov		eax, 60
	mov		edx, 0
	mul		[ebx].duringTime
	mov		ecx, 1000
	div		ecx
	mov		@tickCount, eax
	mov		eax, ex
	sub		eax, sx
	mov		edx, 0
	div		@tickCount
	mov		[ebx].transXStep, eax
	mov		eax, ey
	sub		eax, sy
	mov		edx, 0
	div		@tickCount
	mov		[ebx].transYStep
	.if		ex != sx && [ebx].transXStep == 0
		mov		eax, ex
		sub		eax, sx
		mov		[ebx].transXStep, eax
	.endif
	.if		ey != sy && [ebx].transYStep == 0
		mov		eax, ey
		sub		eax, sy
		mov		[ebx].transYStep, eax
	.endif
	assume	ebx : nothing
	ret
TransAnimation_init		endp

TransAnimation_tick		proc stdcall public uses eax ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	.if		[ebx].transEnable == FALSE
		ret
	.endif

	.if		[ebx].nx == [ebx].ex && [ebx].ny == [ebx].ey
		.if		[ebx].transLoop == TRUE
			push	[ebx].sx
			pop		[ebx].nx
			push	[ebx].sy
			pop		[ebx].ny
		.else
			mov		[ebx].transEnable, FALSE
		.endif
		ret
	.endif

	mov		eax, [ebx].nx
	add		eax, [ebx].transXStep
	mov		[ebx].nx, eax
	mov		eax, [ebx].ny
	add		eax, [ebx].transYStep
	mov		[ebx].ny, eax
	.if		([ebx].transXStep > 0 && [ebx].nx > [ebx].ex) || ([ebx].transXStep < 0 && [ebx].nx < [ebx].ex)
		push	[ebx].ex
		pop		[ebx].nx
	.endif
	.if		([ebx].transXStep > 0 && [ebx].nx > [ebx].ex) || ([ebx].transXStep < 0 && [ebx].nx < [ebx].ex)
		push	[ebx].ex
		pop		[ebx].nx
	.endif

	assume	ebx : nothing
	ret
TransAnimation_tick		endp

TransAnimation_getXY	proc stdcall public uses ebx	o : AnimationPtr, x : ptr SDWORD, y : ptr SDWORD
	assume	ebx : AnimationPtr
	mov		ebx, o
	push	[ebx].nx
	pop		[x]
	push	[ebx].ny
	pop		[y]
	assume	ebx : nothing
	ret
TransAnimation_getXY	endp

TransAnimation_finished	proc stdcall public uses ebx	o : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, o
	.if		[ebx].transLoop == FALSE && [ebx].nx == [ebx].ex && [ebx].ny == [ebx].ey
		mov		eax, TRUE
	.else
		mov		eax, FALSE
	.endif
	assume	ebx : nothing
	ret
TransAnimation_finished	endp

;////////////////////////////////////////////////////////////////////////
; AnimationManager
_AnimationManager_initAnimation	proc stdcall private uses ebx	ani : AnimationPtr
	assume	ebx : AnimationPtr
	mov		ebx, ani
	mov		[ebx].fadeEnable, FALSE
	mov		[ebx].frameEnable, FALSE
	mov		[ebx].transEnable, FALSE
	assume	ebx : nothing
	ret
_AnimationManager_initAnimation	endp

AnimationManager_init			proc stdcall public
	mov		g_aniMgr.aniCount, 0
	ret
AnimationManager_init			endp

AnimationManager_allocAnimation	proc stdcall public uses ebx edx
	.if		g_aniMgr.aniCount == 32
		mov		eax, NULL
	.endif

	mov		eax, sizeof Animation
	mov		edx, 0
	mov		ebx, g_aniMgr.aniCount
	mul		ebx
	add		eax, offset g_aniMgr.ani
	inc		g_aniMgr.aniCount
	invoke	_AnimationManager_initAnimation, eax
	ret
AnimationManager_allocAnimation	endp

AnimationManager_tick			proc stdcall public uses eax ecx edx
	mov		ecx, 0
	.while	ecx < g_aniMgr.aniCount
		mov		eax, sizeof Animation
		mov		edx, 0
		mul		ecx
		add		eax, offset g_aniMgr.ani
		invoke	FadeAnimation_tick, eax
		invoke	FrameAnimation_tick, eax
		invoke	TransAnimation_tick, eax
		inc		ecx
	.endw
	ret
AnimationManager_tick			endp
	end