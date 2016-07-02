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

	birdTickCount		double		?		; С��ļ�ʱ��
	isBirdDropped		bool		?		; С���Ƿ��Ѿ����ڵذ�����
	canBeHigher			bool		?
	birdVSpeed			double		?		; С��ĺ����ٶ�
	birdHSpeed			double		?		; С��������ٶ�

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
; ���㸡���е�С���λ��
_PhysicEngine_birdFloating	proc stdcall private uses eax ebx
	local	@tpf : double
	local	@tp : DWORD

	assume	ebx : ptr Spirit
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax
	; ������һ����г�˶����乫ʽΪ�� x = Acos(wt+p)
	; ����Ϸ�У����ı�С��� y ���꣬�����ı�С��� x ����
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

; �������ɷ�����С���λ��
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
	; ���С����أ����ٵ���
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
	; // С��ĳ�����Ǻϳ��ٶȵĳ���
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

; �ƶ�����
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

; �ƶ��ܵ�
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
	; �϶˹��ӵĿ�������Ӧ���� 40 ~ ground - 40
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

; �����Ϸ�еľ���״̬����Ҫ�У�
; * �ж��ǲ���С��ײ������������
; * �ж�С���ǲ��Ƿɵ���ߴ���
; * �ж�С���ǲ��Ǹո�ͨ��һ������
_PhysicEngine_checkBirdState	proc stdcall private uses eax ebx ecx edx
	local	@bird : SpiritPtr
	local	@ground : SpiritPtr
	local	@upPipes : SpiritPtr
	local	@downPipes : SpiritPtr
	local	@upPipe : SpiritPtr
	local	@downPipe : SpiritPtr

	assume	ebx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		@bird, eax
	invoke	ImageManager_getSpirit, SpiritType_ground
	mov		@ground, eax
	invoke	ImageManager_getSpirit, SpiritType_upPipes
	mov		@upPipes, eax
	invoke	ImageManager_getSpirit, SpiritType_downPipes
	mov		@downPipes, eax

	.if		g_physicEngine.birdState == BirdState_floating || g_physicEngine.birdState == BirdState_fixed
		ret
	.endif
	.if		g_physicEngine.birdState == BirdState_dead && g_physicEngine.isBirdDropped
		ret
	.endif
	; ���С����û��ײ������
	mov		ebx, @bird
	mov		eax, [ebx].cy
	add		eax, [ebx].halfHeight
	mov		ebx, @ground
	mov		edx, [ebx].cy
	sub		edx, [ebx].halfHeight
	.if		eax >= edx
		mov		g_physicEngine.birdState, BirdState_dead
		assume	edx : AnimationPtr
		mov		ebx, @bird
		mov		edx, [ebx].ani
		mov		[edx].frameEnable, FALSE
		assume	edx : nothing
		mov		g_physicEngine.isBirdDropped, TRUE
	.endif
	.if		g_physicEngine.birdState == BirdState_dead
		ret
	.endif
	; ���С����û��ײ��ˮ��
	mov		ecx, 0
	.while	ecx < 4
		mov		eax, sizeof SpiritPtr
		mov		edx, 0
		mul		ecx
		mov		ebx, @upPipes
		add		ebx, eax
		mov		@upPipe, ebx
		mov		ebx, @downPipes
		add		ebx, eax
		mov		@downPipe, ebx
		; ��ֱ�߶ȣ��Ƿ��������ܵ�֮��
		mov		ebx, @bird
		mov		eax, [ebx].cy
		add		eax, [ebx].halfHeight
		mov		ebx, @downPipe
		mov		edx, [ebx].cy
		sub		edx, [ebx].halfHeight
		.if		eax <= edx
			mov		ebx, @bird
			mov		eax, [ebx].cy
			sub		eax, [ebx].halfHeight
			mov		ebx, @upPipe
			mov		edx, [ebx].cy
			add		edx, [ebx].halfHeight
			.if		eax >= edx
				inc		ecx
				.continue
			.endif
		.endif
		; ˮƽ��飬�Ƿ������ܵ�
		mov		ebx, @bird
		mov		eax, [ebx]._cx
		add		eax, [ebx].halfWidth
		mov		ebx, @upPipe
		mov		edx, [ebx]._cx
		sub		edx, [ebx].halfWidth
		.if		eax >= edx
			mov		ebx, @bird
			mov		eax, [ebx]._cx
			sub		eax, [ebx].halfWidth
			mov		ebx, @upPipe
			mov		edx, [ebx]._cx
			add		edx, [ebx].halfWidth
			.if		eax <= edx
				mov		g_physicEngine.birdState, BirdState_dead
				assume	edx : AnimationPtr
				mov		ebx, @bird
				mov		edx, [ebx].ani;
				mov		[edx].frameEnable, FALSE
				.break
			.endif
		.endif
		inc		ecx
	.endw
	mov		ebx, @bird
	.if		[ebx].cy > 40
		mov		g_phyiscEngine.canBeHigher, TRUE
	.else
		mov		g_phyiscEngine.canBeHigher, FALSE
	.endif
	assume	ebx : nothing
	ret
_PhysicEngine_checkBirdState	endp

; ������Ϸ�е�
; ���¼�����Ϸ�е�λ���߼�
; ��Ϸ��֡������ 60 fps������ÿһ�� tick ��ʱ������ 1/60 ��
; ��Ϸ�п����˶���������Ҫ��������
; * С����Ҫ�и��������ɡ���������״̬
; * ���棺��Ҫ���ƶ���ֹͣ����״̬
; * �ܵ�����Ҫ��δ���֡��ƶ���ֹͣ����״̬
PhysicEngine_tick			proc public stdcall uses eax
	invoke	_PhysicEngine_birdTick
	invoke	_PhysicEngine_groundTick
	invoke	_PhysicEngine_pipesTick
	invoke	_PhysicEngine_checkBirdState
	ret
PhysicEngine_tick			endp

PhysicEngine_isBirdDead		proc public stdcall
	.if		g_physicEngine.birdState == BirdState_dead
		mov		eax, TRUE
	.else
		mov		eax, FALSE
	.endif
	ret
PhysicEngine_isBirdDead		endp

PhysicEngine_init			proc stdcall public uses eax ebx ecx
	assume	ebx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_ground
	mov		ebx, eax
	mov		[ebx]._cx, 144
	mov		[ebx].cy, 456
	invoke	GetTickCount
	invoke	ctr_srand, eax
	assume	ebx : nothing
	mov		ecx, 0
	mov		ebx, offset g_physicEngine.pipePassed
	.while	ecx < 4
		mov		[ebx], FALSE
		add		ebx, sizeof BOOL
		inc		ecx
	.endw
	ret
PhysicEngine_init			endp

PhysicEngine_isBirdStopped	proc stdcall public
	mov		eax, g_physicEngine.isBirdDropped
	ret
PhysicEngine_isBirdStopped	endp

PhysicEngine_birdFly		proc stdcall public uses eax
	local	@tp : SDWORD

	.if		g_physicEngine.birdState != BirdState_free
		ret
	.endif
	.if		g_physicEngine.canBeHigher == TRUE
		mov		@tp, -300
		finit
		fild	@tp
		fstp	g_physicEngine.birdVSpeed
	.endif
	ret
PhysicEngine_birdFly		endp

PhysicEngine_floatingBird	proc stdcall public uses eax ebx ecx
	assume	ebx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax
	mov		ecx, [ebx].ani

	mov		[ebx]._cx, 144
	mov		[ebx].cy, 230
	mov		[ebx].ox, 144
	mov		[ebx].oy, 230
	finit
	fldz
	fstp	[ebx].angle
	mov		g_physicEngine.birdState, BirdState_floating
	mov		g_physicEngine.isBirdDropped, FALSE
	invoke	FrameAnimation_init, ecx, 4, 500, TRUE
	assume	ebx : nothing
	ret
PhysicEngine_floatingBird	endp

PhysicEngine_prepareBird	proc stdcall public uses eax ebx ecx
	assume	ebx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax
	mov		ecx, [ebx].ani

	mov		[ebx]._cx, 90
	mov		[ebx].cy, 230
	mov		[ebx].ox, 90
	mov		[ebx].oy, 230
	finit
	fldz
	fstp	[ebx].angle
	mov		g_physicEngine.birdState, BirdState_floating
	mov		g_physicEngine.isBirdDropped, FALSE
	invoke	FrameAnimation_init, ecx, 4, 600, TRUE
	assume	ebx : nothing
	ret
PhysicEngine_prepareBird	endp

PhysicEngine_freeBird		proc stdcall public uses eax ebx
	local	@tp : SDWORD

	assume	ebx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax

	finit
	fldz
	fst		[ebx].angle
	fstp	g_physicEngine.birdVSpeed
	mov		g_physicEngine.birdState, BirdState_free
	mov		@tp, 200
	fild	@tp
	fstp	g_physicEngine.birdHSpeed
	mov		g_physicEngine.isBirdDropped, FALSE
	assume	ebx : nothing
	ret
PhysicEngine_freeBird		endp

PhysicEngine_passedPipe		proc stdcall public uses ebx ecx edx
	local	@upPipes : SpiritPtr
	local	@pipePassed : ptr BOOL

	assume	ebx : SpiritPtr
	assume	edx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_upPipes
	mov		@upPipes, eax
	invoke	ImageManager_getSpirit, SpiritType_bird
	mov		ebx, eax

	mov		ecx, 0
	.while	ecx < 4
		mov		eax, sizeof BOOL
		mov		edx, 0
		mul		ecx
		add		eax, offset g_physicEngine.pipePassed
		.if		[eax] == FALSE
			mov		@pipePassed, eax
			mov		eax, sizeof SpiritPtr
			mov		edx, 0
			mul		ecx
			add		eax, @upPipes
			mov		edx, eax
			.if		[ebx]._cx > [edx]._cx
				mov		[@pipePassed], TRUE
				mov		eax, TRUE
				ret
			.endif
		.endif
		inc		ecx
	.endw
	mov		eax, FALSE
	assume	ebx : nothing
	assume	edx : nothing
	ret
PhysicEngine_passedPipe		endp

PhysicEngine_showPipes		proc stdcall public uses ebx ecx edx	show : BOOL
	local	@upPipes : SpiritPtr
	local	@downPipes : SpiritPtr

	assume	ebx : SpiritPtr
	invoke	ImageManager_getSpirit, SpiritType_upPipes
	mov		@upPipes, eax
	invoke	ImageManager_getSpirit, SpiritType_downPipes
	mov		@downPipes, eax

	mov		ecx, 0
	.while	ecx < 4
		mov		eax, sizeof SpiritPtr
		mov		edx, 0
		mul		ecx
		mov		ebx, @upPipes
		add		ebx, eax
		push	show
		pop		[ebx].visiable
		mov		ebx, @downPipes
		add		ebx, eax
		push	show
		pop		[ebx].visiable
		inc		ecx
	.endw

	.if		show == FALSE
		mov		g_physicEngine.pipeState, PipeState_unset
	.endif
	assume	ebx : nothing
	ret
PhysicEngine_showPipes		endp

PhysicEngine_movingPipes	proc stdcall public uses eax	moving : BOOL
	.if		moving != FALSE
		.if		g_physicEngine.pipeState != PipeState_moving
			mov		g_physicEngine.pipeState, PipeState_moving
			invoke	_PhysicEngine_initPipes
		.endif
	.else
		mov		g_physicEngine.pipeState, PipeState_stop
	.endif
	ret
PhysicEngine_movingPipes	endp

PhysicEngine_movingGround	proc stdcall public	moving : BOOL
	.if		moving == TRUE
		mov		g_physicEngine.groundState, GroundState_moving
	.else
		mov		g_physicEngine.groundState, GroundState_stop
	.endif
	ret
PhysicEngine_movingGround	endp

	end