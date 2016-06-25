include		stdafx.inc
include		physic_engine.inc
include		image_manager.inc
include		animation_manager.inc

BirdState			typedef	DWORD
BirdState_floating	equ		0
BirdState_free		equ		1
BirdState_fixed		equ		2
BirdState_dead		equ		3

GroundState			typedef	DWORD
GroundState_moving	equ		0
GroundState_stop	equ		1

PipeState			typedef	DWORD
PipeState_unset		equ		0
PipeState_moving	equ		1
PipeState_stop		equ		2

PhysicEngine		struct
	birdState			BirdState	?
	groundState			GroundState	?
	pipeState			PipeState	?

	birdTickCount		double		?		; 小鸟的计时器
	isBirdDropped		bool		?		; 小鸟是否已经死在地板上了
	canBeHigher			bool		?
	birdVSpeed			double		?		; 小鸟的横向速度
	birdHSpeed			double		?		; 小鸟的纵向速度

	groundMovingLength	SDWORD		?

	pipePassed			bool		4 dup(?)
PhysicEngine		ends

	.const
g_gravity				double		100.0
g_pipeInterval			SDWORD		1400
g_fb_w					double		0.04
g_fb_A					double		50.0
g_scale					double		10.0
g_bf_tick				double		0.15
g_bf_maxVSpeed			double		700.0

	.data
g_physicEngine			PhysicEngine	<>

	.code
; 计算浮动中的小鸟的位置
_PhysicEngine_birdFloating	proc stdcall private uses eax ebx
	local	@tpf : double
	local	@tp : DWORD

	assume	ebx : ptr Spirit
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax
	; 浮动是一个简谐运动，其公式为： x = Acos(wt+p)
	; 在游戏中，仅改变小鸟的 y 坐标，而不改变小鸟的 x 坐标
	finit
	fld1
	fld		g_sceMgr.birdTickCount
	fadd
	fstp	g_sceMgr.birdTickCount
	mov		@tp, 50
	fild	@tp
	;	ZF	PF	CF
	;>	0	0	0
	;<	0	0	1
	;=	1	0	0
	fcomi	g_sceMgr.birdTickCount
	.if		CARRY?
		fldz
		fstp	g_sceMgr.birdTickCount
	.endif
	
	finit
	fldpi
	fmul	g_fb_w
	fmul	g_sceMgr.birdTickCount
	fcos
	fstp	@tpf
	fld		g_fb_A
	fmul	@tpf
	fstp	@tpf
	fild	[ebx].oy
	fmul	g_scale
	fadd	@tpf
	fdiv	g_scale
	fistp	[ebx].cy
	assume	ebx : nothing
	ret
_PhysicEngine_birdFloating	endp

; 计算自由飞行中小鸟的位置
_PhysicEngine_birdFree		proc stdcall private uses eax ebx edx
	local	@vvPre : double
	local	@tp : SDWORD

	assume	ebx : ptr Spirit
	assume	edx : ptr Spirit
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax
	invoke	ImageManager_getSpirit, SpiritType_ground
	mov		edx, eax
	
	.if		g_physicEngine.isBirdDropped != FALSE
		ret
	.endif

	push	g_physicEngine.isBirdDropped
	pop		@vvPre

	finit
	fld		g_gravity
	fmul	g_bf_tick
	fadd	g_physicEngine.birdVSpeed
	fcomi	g_bf_maxVSpeed
	.if		!CARRY? && !ZERO
		fstp	g_physicEngine.birdVSpeed
	.else
		push	g_bf_maxVSpeed
		pop		g_physicEngine.birdVSpeed
	.endif
	mov		@tp, 2
	finit
	fild	@tp
	fld		g_physicEngine.birdVSpeed
	fadd	@vvPre
	fmul	g_bf_tick
	fdiv	st(1)
	fdiv	g_scale
	fiadd	[ebx].cy
	fistp	[ebx].cy
	; 如果小鸟落地，则不再掉落
	mov		eax, [ebx].cy
	add		eax, [ebx].halfHeight
	mov		@tp, eax
	mov		eax, [edx].cy
	sub		eax, [edx].halfHeight
	.if		@tp >= eax
		mov		eax, [edx].cy
		sub		eax, [edx].halfHeight
		sub		eax, [ebx].halfHeight
		mov		[ebx].cy, eax
	.endif
	; // 小鸟的朝向就是合成速度的朝向
	fld		g_physicEngine.birdVSpeed
	fdiv	g_physicEngine.birdHSpeed
	fstp	@vvPre
	invoke	crt_atan, @vvPre
	mov		[ebx].angle, eax
	assume	ebx : nothing
	assume	edx : nothing
	ret
_PhysicEngine_birdFree		endp

_PhysicEngine_birdTick		proc stdcall private uses eax
	.if		g_physicEngine.birdState == BirdState_floating
		invoke	_PhysicEngine_birdFloating
	.elseif	g_physicEngine.birdState == BirdState_free
		invoke	_PhysicEngine_birdFree
	.elseif	g_physicEngine.birdState == BirdState_dead
		invoke	_PhysicEngine_birdFree
	.endif
	ret
_PhysicEngine_birdTick		endp

; 移动地面
_PhysicEngine_groundTick	proc stdcall private uses eax ebx edx
	assume	ebx : ptr Spirit
	.if		g_physicEngine.groundState == GroundState_stop
		ret
	.endif

	mov		edx, 2
	mov		eax, g_physicEngine.groundMovingLength
	add		eax, edx
	mov		edx, 0
	mov		ebx, 24
	div		ebx
	mov		g_physicEngine.groundMovingLength, edx
	invoke	ImageManager_getSpirit, SpiritType_ground
	mov		ebx, eax
	mov		eax, [ebx]._cx
	sub		eax, g_physicEngine.groundMovingLength
	mov		[ebx]._cx, eax
	assume	ebx : nothing
	ret
_PhysicEngine_groundTick	endp

; 移动管道
_PhysicEngine_movingPipes	proc stdcall private uses eax ebx ecx edx
	local	@cxInc : SDWORD

	assume	ebx : ptr Spirit
	assume	edx : ptr Spirit
	mov		@cxInc, 2
	invoke	ImageManager_getSpirit, SpiritType_upPipes
	mov		ebx, eax
	invoke	ImageManager_getSpirit, SpiritType_downPipes
	mov		edx, eax
	mov		ecx, 0
	.while	ecx < 4
		sub		[ebx]._cx, @cxInc
		sub		[edx]._cx, @cxInc
		inc		ecx
		add		ebx, sizeof SpiritPtr
		add		edx, sizeof SpiritPtr
	.endw
	assume	ebx : nothing
	assume	edx : nothing
	ret
_PhysicEngine_movingPipes	endp

_PhysicEngine_getRandPipeUp	proc stdcall private uses ebx edx
	; 上端管子的开口坐标应该是 40 ~ ground - 40
	local	@res : double
	local	@tp : SDWORD

	invoke	crt_rand
	mov		edx, 0
	mov		ebx, 200
	div		ebx
	div		ebx
	add		edx, 40
	mov		edx, eax
	mov		edx, 0
	mov		ebx, 10
	mul		ebx
	mov		@tp, eax
	finit
	fild	@tp
	fstp	@res
	mov		eax, @res
	ret
_PhysicEngine_getRandPipeUp	endp

_PhysicEngine_initPipes		proc stdcall private uses eax ebx ecx edx
	local	@r2pFactor : SDWORD
	local	@cxPipe : SDWORD
	local	@pixelPipeInterval : SDWORD
	local	@tp : double

	assume	ebx : ptr Spirit
	assume	edx : ptr Spirit
	mov		edx, 0
	mov		eax, g_pipeInterval
	div		@r2pFactor
	mov		@pixelPipeInterval, eax
	mov		edx, 288
	add		eax, edx
	mov		@cxPipe, eax
	mov		@r2pFactor, 10
	invoke	ImageManager_getSpirit, SpiritType_upPipes
	mov		ebx, eax
	invoke	ImageManager_getSpirit, SpiritType_downPipes
	mov		edx, eax
	mov		ecx, 0

	mov		eax, offset g_physicEngine.pipePassed
	push	eax

	.while	ecx < 4
		push	@cxPipe
		pop		[ebx]._cx
		mov		eax, @cxPipe
		add		eax, @pixelPipeInterval
		mov		@cxPipe, eax
		invoke	_PhysicEngine_getRandPipeUp
		mov		@tp, eax
		finit
		fild	@r2pFactor
		fld		@tp
		fdiv
		fisub	[ebx].halfHeight
		fistp	[ebx].cy

		push	[ebx]._cx
		pop		[edx]._cx
		mov		eax, 120
		add		eax, [ebx].cy
		add		eax, [ebx].halfHeight
		add		eax, [edx].halfHeight
		mov		[edx].cy, eax

		pop		eax
		mov		[eax], false
		add		eax, sizeof bool
		push	eax

		inc		ecx
		add		ebx, sizeof SpiritPtr
		add		edx, sizeof SpiritPtr
	.endw
	pop		eax
	assume	ebx : nothing
	assume	edx : nothing
	ret
_PhysicEngine_initPipes		endp

_PhysicEngine_loopPipes		proc stdcall private uses eax ebx ecx edx
	local	@upPipes : SpiritPtr
	local	@downPipes : SpiritPtr
	local	@pixelPipeInterval : SDWORD	; todo
	local	@tp : double
	local	@r2pFactor : SDWORD
	
	mov		edx, 0
	mov		eax, g_pipeInterval
	mov		ecx, 10
	div		ecx
	mov		@pixelPipeInterval, eax
	mov		@r2pFactor, 10

	assume	ebx : ptr Spirit
	invoke	ImageManager_getSpirit, SpiritType_upPipes
	mov		@upPipes, eax
	invoke	ImageManager_getSpirit, SpiritType_downPipes
	mov		@downPipes, eax
	mov		ecx, 0
	.while	ecx < 4
		mov		ebx, @upPipes
		mov		eax, sizeof SpiritPtr
		mul		ecx
		add		ebx, eax
		
		mov		eax, [ebx]._cx
		add		eax, [ebx].halfWidth
		.if		eax < 0
			mov		eax, @pixelPipeInterval
			shl		eax, 2
			add		eax, [ebx]._cx
			mov		[ebx]._cx, eax
			finit
			invoke	_PhysicEngine_getRandPipeUp
			mov		@tp, eax
			fild	@r2pFactor
			fld		@tp
			fdiv
			fisub	[ebx].halfHeight
			fistp	[ebx].cy

			assume	edx : ptr Spirit
			mov		eax, sizeof SpiritPtr
			mul		ecx
			mov		edx, @downPipes
			add		edx, eax
			push	[ebx]._cx
			pop		[edx]._cx
			mov		eax, 120
			add		eax, [ebx].cy
			add		eax, [ebx].halfHeight
			add		eax, [edx].halfHeight
			mov		[edx].cy, eax
			assume	edx : nothing

			mov		eax, sizeof bool
			mul		ecx
			mov		edx, offset g_physicEngine.pipePassed
			add		edx, eax
			mov		[edx], false

			ret
		.endif
		inc		ecx
	.endw
	ret
_PhysicEngine_loopPipes		endp

_PhysicEngine_pipesTick		proc stdcall private uses eax
	.if		g_physicEngine.pipeState == PipeState_moving
		invoke	_PhysicEngine_movingPipes
		invoke	_PhysicEngine_loopPipes
	.endif
	ret
_PhysicEngine_pipesTick		endp



	end