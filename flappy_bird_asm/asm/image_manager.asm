include		stdafx.inc
include		image_manager.inc
include		score_manager.inc
include		msimg32.inc

includelib	msimg32.lib			; AlphaBlend 函数需要用到这个库

TRANSPRENT_COLOR	equ	00FF00BAh	; 定义透明色，紫色

	.data
img_dayBackground		Image<>
img_nightBackground		Image<>
img_ground				Image<>
img_txtGetReady			Image<>
img_txtGameOver			Image<>
img_txtFlappyBird		Image<>
img_txtCopyright		Image<>
img_smallNum			Image	10	dup(<>)
img_middleNum			Image	10	dup(<>)
img_largeNum			Image	10	dup(<>)
img_scorePane			Image<>
img_sliverMedal			Image<>
img_goldenMedal			Image<>
img_copperMedal			Image<>
img_help				Image<>
img_orangePipeUp		Image<>
img_orangePipeDown		Image<>
img_greenPipeUp			Image<>
img_greenPipeDown		Image<>
img_yellowBird			Image	3	dup(<>)
img_redBird				Image	3	dup(<>)
img_blueBird			Image	3	dup(<>)
img_btPlay				Image<>
img_btRank				Image<>
img_btRate				Image<>
img_btContinue			Image<>
img_btPause				Image<>
img_btShare				Image<>
img_btMenu				Image<>
img_btOk				Image<>
img_new					Image<>
img_black				Image<>
img_white				Image<>

sp_ground				Spirit<>
sp_bird					Spirit<>
sp_sky					Spirit<>
sp_upPipes				Spirit	4	dup(<>)
sp_downPipes			Spirit	4	dup(<>)
sp_txtGetReady			Spirit<>
sp_txtGameOver			Spirit<>
sp_txtCopyright			Spirit<>
sp_txtFlappyBird		Spirit<>
sp_largeScore			Spirit<>
sp_helpInfo				Spirit<>
sp_btPlay				Spirit<>
sp_btRank				Spirit<>
sp_btRate				Spirit<>
sp_scoreBoard			Spirit<>
sp_blackFade			Spirit<>
sp_whiteFade			Spirit<>

g_imgMgr				ImageManager<>

g_spirits				SpiritPtr	offset sp_ground, \			; SpiritType_ground
									offset sp_sky, \			; SpiritType_sky
									offset sp_bird,	\			; SpiritType_bird
									offset sp_upPipes, \		; SpiritType_upPipes
									offset sp_downPipes, \		; SpiritType_downPipes
									offset sp_txtGetReady, \	; SpiritType_txtGetReady
									offset sp_txtGameOver, \	; SpiritType_txtGameOver
									offset sp_txtCopyright		; SpiritType_txtCopyRight
						SpiritPtr	offset sp_txtFlappyBird, \	; SpiritType_txtFlappyBird
									offset sp_largeScore, \		; SpiritType_largeScore
									offset sp_helpInfo, \		; SpiritType_helpInfo
									offset sp_btPlay, \			; SpiritType_btPlay
									offset sp_btRank, \			; SpiritType_btRank
									offset sp_btRate, \			; SpiritType_btRate
									offset sp_scoreBoard, \		; SpiritType_scoreBoard
									offset sp_blackFade, \		; SpiritType_blackFade
									offset sp_whiteFade			; SpiritType_whiteFade

imgMedal				ImagePtr	offset img_goldenMedal, \
									offset img_sliverMedal, \
									offset img_copperMedal, \
									NULL

	.code
_rotateHdc			proc private	hdc : HDC, angle : REAL8, _cx : SDWORD, cy : SDWORD
	local	@xform : XFORM
	local	@tpv : REAL8

	invoke	SetGraphicsMode, hdc, GM_ADVANCED
	finit
	fld		angle
	fcos
	fst		@xform.eM11
	fstp	@xform.eM22
	fld		angle
	fsin
	fst		@xform.eM12
	fchs
	fstp	@xform.eM21
	fld		angle
	fcos
	fimul	_cx
	fchs
	fiadd	_cx
	fstp	@tpv
	fld		angle
	fsin
	fimul	cy
	fadd	@tpv
	fstp	@xform.ex
	fld		angle
	fcos
	fimul	cy
	fchs
	fiadd	cy
	fstp	@tpv
	fld		angle
	fsin
	fimul	_cx
	fchs
	fadd	@tpv
	fstp	@xform.ey
	invoke	SetWorldTransform, hdc, addr @xform
	ret
_rotateHdc			endp

_restoreHdc			proc private	hdc : HDC
	local	@xform : XFORM

	finit
	fld1
	fst		@xform.eM11
	fstp	@xform.eM22
	fldz
	fst		@xform.eM12
	fst		@xform.eM21
	fst		@xform.ex
	fstp	@xform.ey
	invoke	SetWorldTransform, hdc, addr @xform
	invoke	SetGraphicsMode, hdc, GM_COMPATIBLE
	ret
_restoreHdc			endp

_getDigitNum		proc private uses ebx edx	num : DWORD
	local	@digitNum : DWORD

	mov		@digitNum, 1
	mov		eax, num
	xor		edx, edx
	mov		ebx, 10
	div		ebx
	.while	eax != 0
		inc		@digitNum
		xor		edx, edx
		div		ebx
	.endw
	mov		eax, @digitNum
	ret
_getDigitNum		endp

_pow				proc private uses ecx ebx	num : DWORD, power : DWORD
	local	@res : DWORD
	
	mov		@res, 1
	mov		ecx, 0
	mov		ebx, num
	.while	ecx < power
		mov		eax, @res
		mul		ebx
		mov		@res, eax
		inc		ecx
	.endw
	mov		eax, @res
	ret
_pow				endp

_getDigit			proc private uses ebx edx	num : DWORD, index : DWORD
	local	@sn : DWORD

	invoke	_pow, 10, index
	mov		@sn, eax
	mov		edx, 0
	mov		eax, num
	div		@sn
	mov		edx, 0
	mov		ebx, 10
	div		ebx
	mov		eax, edx
	ret
_getDigit			endp

;////////////////////////////////////////////////////////////////////////
; Image 对象相关方法
Image_construct		proc public uses ebx	o : ImagePtr, x : SDWORD, y : SDWORD, _width : SDWORD, height : SDWORD
	assume	ebx : ptr Image
	mov		ebx, o
	push	x
	pop		[ebx].x
	push	y
	pop		[ebx].y
	push	_width
	pop		[ebx]._width
	push	height
	pop		[ebx].height
	assume	ebx : nothing
	ret
Image_construct		endp

;////////////////////////////////////////////////////////////////////////
; Spirit 对象相关方法
Spirit_construct	proc public uses eax ebx ecx	o : SpiritPtr, img : ImagePtr, _cx : SDWORD, cy : SDWORD
	assume	ebx : ptr Spirit
	assume	ecx : ptr Image
	mov		ebx, o
	push	img
	pop		[ebx].image
	push	_cx
	pop		[ebx]._cx
	push	cy
	pop		[ebx].cy
	push	_cx
	pop		[ebx].ox
	push	cy
	pop		[ebx].oy
	mov		ecx, img
	mov		eax, [ecx]._width
	sal		eax, 1
	mov		[ebx].halfWidth, eax
	mov		eax, [ecx].height
	sal		eax, 1
	mov		[ebx].halfHeight, eax
	mov		[ebx].visiable, TRUE
	mov		[ebx].ani, NULL
	assume	ebx : nothing
	assume	ecx : nothing
	ret
Spirit_construct	endp

Spirit_draw			proc public uses ebx ecx	o : SpiritPtr
	local	@srcHdc : HDC
	local	@dstHdc : HDC
	local	@x : SDWORD
	local	@y : SDWORD

	assume	ebx : ptr Spirit
	assume	ecx : ptr Animation
	mov		ebx, o
	.if		[ebx].visiable == FALSE
		ret
	.endif
	mov		ecx, [ebx].ani
	.if		ecx != NULL && [ecx].transEnable
		invoke	TransAnimation_getXY, ecx, addr [ebx]._cx, addr [ebx].cy
	.endif
	assume	ecx : ptr Image
	mov		ecx, [ebx].image
	push	[ebx]._cx
	pop		@x
	mov		eax, [ebx].halfWidth
	sub		@x, eax
	push	[ebx].cy
	pop		@y
	mov		eax, [ebx].halfHeight
	sub		@y, eax
	invoke	TransparentBlt, @dstHdc, @x, @y, [ecx]._width, [ecx].height, \
			@srcHdc, [ecx].x, [ecx].y, [ecx]._width, [ecx].height, TRANSPRENT_COLOR
	assume	ebx : nothing
	assume	ecx : nothing
	ret
Spirit_draw			endp

Spirit_drawBird		proc public uses ebx ecx edx	o : SpiritPtr
	local	@srcHdc : HDC
	local	@dstHdc : HDC
	local	@frameIndex : SDWORD
	local	@x : SDWORD
	local	@y : SDWORD

	assume	ebx : ptr Spirit
	assume	ecx : ptr Image
	mov		ebx, o
	.if		[ebx].visiable == FALSE
		ret
	.endif
	push	g_imgMgr.imgHdc
	pop		@srcHdc
	push	g_imgMgr.dstHdc
	pop		@dstHdc
	; 小鸟的扇动翅膀动画
	invoke	FrameAnimation_getFrameIndex, [ebx].ani
	.if		eax == 3
		mov		eax, 1
	.endif
	mov		@frameIndex, eax
	mov		ecx, [ebx].image
	mov		edx, 0
	mov		eax, sizeof Image
	mul		@frameIndex
	add		ecx, eax
	; 旋转小鸟
	invoke	_rotateHdc, @dstHdc, [ebx].angle, [ebx]._cx, [ebx].cy
	mov		eax, [ebx]._cx
	sub		eax, [ebx].halfWidth
	mov		@x, eax
	mov		eax, [ebx].cy
	mov		eax, [ebx].halfHeight
	mov		@y, eax
	invoke	TransparentBlt, @dstHdc, @x, @y, [ecx]._width, [ecx].height, \
			@srcHdc, [ecx].x, [ecx].y, [ecx]._width, [ecx].height, TRANSPRENT_COLOR
	invoke	_restoreHdc, @dstHdc
	assume	ebx : nothing
	assume	ecx : nothing
	ret
Spirit_drawBird		endp

Spirit_drawPipes	proc public uses ebx ecx edx	o : SpiritPtr
	local	@srcHdc : HDC
	local	@dstHdc : HDC
	local	@x : SDWORD
	local	@y : SDWORD

	push	g_imgMgr.imgHdc
	pop		@srcHdc
	push	g_imgMgr.dstHdc
	pop		@dstHdc
	assume	ebx : ptr Spirit
	assume	edx : ptr Image
	mov		ecx, 0
	mov		ebx, o
	.while	ecx < 4
		mov		edx, [ebx].image
		.if		[ebx].visiable != FALSE
			mov		eax, [ebx]._cx
			sub		eax, [ebx].halfWidth
			mov		@x, eax
			mov		eax, [ebx].cy
			mov		eax, [ebx].halfHeight
			mov		@y, eax
			invoke	TransparentBlt, @dstHdc, @x, @y, [edx]._width, [edx].height, \
					@srcHdc, [edx].x, [edx].y, [edx]._width, [edx].height, TRANSPRENT_COLOR		
		.endif
		inc		ecx
		add		ebx, sizeof Spirit
	.endw
	assume	ebx : nothing
	assume	edx : nothing
	ret
Spirit_drawPipes	endp

Spirit_drawLargeScore	proc public uses ebx	o : SpiritPtr
	local	@srcHdc : HDC
	local	@dstHdc : HDC

	push	g_imgMgr.imgHdc
	pop		@srcHdc
	push	g_imgMgr.dstHdc
	pop		@dstHdc
	assume	ebx : ptr Spirit
	mov		ebx, o
	.if		[ebx].visiable == FALSE
		ret
	.endif
	invoke	ScoreManager_getCurrentScore
	invoke	ImageManager_drawNumber, eax, [ebx]._cx, [ebx].cy, NumberSize_large, NumberAlign_center
	assume	ebx : nothing
	ret
Spirit_drawLargeScore	endp

Spirit_drawScoreBorad	proc public uses eax ebx ecx edx	o : SpiritPtr
	local	@srcHdc : HDC
	local	@dstHdc : HDC
	local	@x : SDWORD
	local	@y : SDWORD

	push	g_imgMgr.imgHdc
	pop		@srcHdc
	push	g_imgMgr.dstHdc
	pop		@dstHdc
	assume	ebx : ptr Spirit
	mov		ebx, o
	.if		[ebx].visiable == FALSE
		ret
	.endif
	invoke	TransAnimation_getXY, [ebx].ani, addr [ebx]._cx, addr [ebx].cy
	; 先绘制底板
	assume	ecx : ptr Image
	mov		ecx, offset img_scorePane
	mov		eax, [ebx]._cx
	sub		eax, [ebx].halfWidth
	mov		@x, eax
	mov		eax, [ebx].cy
	sub		eax, [ebx].halfHeight
	mov		@y, eax
	invoke	TransparentBlt, @dstHdc, @x, @y, [ecx]._width, [ecx].height, \
			@srcHdc, [ecx].x, [ecx].y, [ecx]._width, [ecx].height, TRANSPRENT_COLOR
	; 再绘制当前得分
	mov		eax, [ebx]._cx
	add		eax, 90
	mov		@x, eax
	mov		eax, [ebx].cy
	sub		eax, 15
	mov		@y, eax
	invoke	ScoreManager_getCurrentScore
	invoke	ImageManager_drawNumber, eax, @x, @y, NumberSize_middle, NumberAlign_right
	; 再绘制最高得分
	mov		eax, [ebx]._cx
	add		eax, 90
	mov		@x, eax
	mov		eax, [ebx].cy
	sub		eax, 28
	mov		@y, eax
	invoke	ScoreManager_getHighScore
	invoke	ImageManager_drawNumber, eax, @x, @y, NumberSize_middle, NumberAlign_right
	; 再决定绘制什么奖牌
	invoke	ScoreManager_getMedalType
	mov		ecx, sizeof Image
	mov		edx, 0
	mul		ecx
	mov		ecx, offset imgMedal
	add		ecx, eax
	.if		ecx != NULL
		mov		eax, [ebx]._cx
		sub		eax, 88
		mov		@x, eax
		mov		eax, [ebx].cy
		sub		eax, 14
		mov		@y, eax
		invoke	TransparentBlt, @dstHdc, @x, @y, [ecx]._width, [ecx].height, \
				@srcHdc, [ecx].x, [ecx].y, [ecx]._width, [ecx].height, TRANSPRENT_COLOR
	.endif

	assume	ecx : nothing
	assume	ebx : nothing
	ret
Spirit_drawScoreBorad	endp

Spirit_drawFade		proc public uses eax ebx	o : SpiritPtr
	local	@srcHdc : HDC
	local	@dstHdc : HDC

	push	g_imgMgr.imgHdc
	pop		@srcHdc
	push	g_imgMgr.dstHdc
	pop		@dstHdc
	assume	ebx : ptr Spirit
	mov		ebx, o
	.if		[ebx].visiable == FALSE
		ret
	.endif
	invoke	FadeAnimation_getAlpha, [ebx].ani
	invoke	ImageManager_drawFadeCover, [ebx].image, eax
	assume	ebx : nothing
	ret
Spirit_drawFade		endp

ImageManager_drawSpirit	proc public uses eax ebx edx	spiritType : DWORD
	assume	ebx : ptr Spirit
	mov		ebx, offset g_spirits
	mov		eax, sizeof SpiritPtr
	mov		edx, 0
	mul		spiritType
	add		ebx, eax
	mov		eax, spiritType
	.if		eax == SpiritType_bird
		invoke	Spirit_drawBird, ebx
	.elseif	eax == SpiritType_upPipes || eax == SpiritType_downPipes
		invoke	Spirit_drawPipes, ebx
	.elseif	eax == SpiritType_largeScore
		invoke	Spirit_drawLargeScore, ebx
	.elseif	eax == SpiritType_scoreBoard
		invoke	Spirit_drawScoreBorad, ebx
	.elseif	eax == SpiritType_blackFade || eax == SpiritType_whiteFade
		invoke	Spirit_drawFade, ebx
	.else
		invoke	Spirit_draw, ebx
	.endif
	assume	ebx : nothing
	ret
ImageManager_drawSpirit	endp

ImageManager_getSpirit	proc public uses ebx edx	spirit : DWORD
	assume	ebx : ptr Spirit
	mov		ebx, offset g_spirits
	mov		eax, sizeof SpiritPtr
	mov		edx, 0
	mul		spirit
	add		ebx, eax
	mov		eax, SpiritPtr ptr [ebx]
	assume	ebx : nothing
	ret
ImageManager_getSpirit	endp

ImageManager_randomSkyAndBird	proc public uses eax edx
	local	@d : DWORD

	invoke	GetTickCount
	xor		edx, edx
	mov		@d, 3
	div		@d
	.if		edx == 0
		push	offset img_blueBird
	.elseif	edx == 1
		push	offset img_yellowBird
	.else
		push	offset img_redBird
	.endif
	pop		sp_bird.image

	invoke	GetTickCount
	xor		edx, edx
	mov		@d, 2
	div		@d
	.if		edx == 0
		push	offset img_dayBackground
	.else
		push	offset img_nightBackground
	.endif
	pop		sp_sky.image
	ret
ImageManager_randomSkyAndBird	endp

ImageManager_initAll	proc public uses ebx ecx edx	hInstance : HINSTANCE, hdc : HDC
	push	hInstance
	pop		g_imgMgr.hInstance
	push	hdc
	pop		g_imgMgr.dstHdc
	invoke	LoadBitmap, hInstance, IDB_SCENE
	mov		g_imgMgr.imgScene, eax
	.if		eax == NULL
		mov		eax, FALSE
		ret
	.endif

	invoke	CreateCompatibleDC, hdc
	mov		g_imgMgr.imgHdc, eax
	invoke	SelectObject, g_imgMgr.imgHdc, g_imgMgr.imgScene

	invoke	Image_construct, offset img_dayBackground, 0, 0, SCENE_WIDTH, SCENE_HEIGHT
	invoke	Image_construct, offset img_ground, 584, 0, 336, 112
	invoke	Image_construct, offset img_txtFlappyBird, 702, 182, 178, 48
	invoke	Image_construct, offset img_btRate, 930, 2, 62, 36
	invoke	Image_construct, offset img_btMenu, 924, 52, 80, 28
	invoke	Image_construct, offset img_btOk, 924, 84, 80, 28
	invoke	Image_construct, offset img_txtGameOver, 790, 118, 192, 42
	invoke	Image_construct, offset img_txtGetReady, 590, 118, 184, 50
	invoke	Image_construct, offset img_help, 584, 182, 114, 98
	invoke	Image_construct, offset img_btShare, 584, 284, 80, 28
	invoke	Image_construct, offset img_btContinue, 668, 284, 26, 28
	invoke	Image_construct, offset img_btPlay, 708, 236, 104, 58
	invoke	Image_construct, offset img_btRank, 828, 236, 104, 58
	invoke	Image_construct, offset img_nightBackground, 292, 0, SCENE_WIDTH, SCENE_HEIGHT
	invoke	Image_construct, offset img_scorePane, 6, 518, 226, 114
	invoke	Image_construct, offset img_sliverMedal, 242, 516, 44, 44
	invoke	Image_construct, offset img_goldenMedal, 242, 564, 44, 44
	invoke	Image_construct, offset img_copperMedal, 224, 954, 44, 44
	invoke	Image_construct, offset img_btPause, 242, 612, 26, 28
	invoke	Image_construct, offset img_orangePipeUp, 0, 646, 52, 320
	invoke	Image_construct, offset img_orangePipeDown, 56, 646, 52, 320
	invoke	Image_construct, offset img_greenPipeUp, 112, 646, 52, 320
	invoke	Image_construct, offset img_greenPipeDown, 168, 646, 52, 320
	mov		ebx, offset img_blueBird
	mov		edx, sizeof Image
	invoke	Image_construct, ebx, 174, 982, 34, 24
	add		ebx, edx
	invoke	Image_construct, ebx, 230, 658, 34, 24
	add		ebx, edx
	invoke	Image_construct, ebx, 230, 710, 34, 24
	mov		ebx, offset img_redBird
	invoke	Image_construct, ebx, 230, 762, 34, 24
	add		ebx, edx
	invoke	Image_construct, ebx, 230, 814, 34, 24
	add		ebx, edx
	invoke	Image_construct, ebx, 230, 866, 34, 24
	mov		ebx, offset img_yellowBird
	invoke	Image_construct, ebx, 6, 982, 34, 24
	add		ebx, edx
	invoke	Image_construct, ebx, 62, 982, 34, 24
	add		ebx, edx
	invoke	Image_construct, ebx, 118, 982, 34, 24
	invoke	Image_construct, offset img_new, 224, 1002, 32, 14
	mov		ebx, offset img_largeNum
	invoke	Image_construct, ebx, 992, 120, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 272, 910, 16, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 584, 320, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 612, 320, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 640, 320, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 668, 320, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 584, 368, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 612, 368, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 640, 368, 24, 36
	add		ebx, edx
	invoke	Image_construct, ebx, 668, 368, 24, 36
	mov		ebx, offset img_middleNum
	invoke	Image_construct, ebx, 274, 612, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 278, 954, 10, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 274, 978, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 262, 1002, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 1004, 0, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 1004, 24, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 1010, 52, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 1010, 84, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 586, 484, 14, 20
	add		ebx, edx
	invoke	Image_construct, ebx, 622, 412, 14, 20
	mov		ebx, offset img_smallNum
	invoke	Image_construct, ebx, 276, 646, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 282, 664, 6, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 698, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 716, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 750, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 768, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 802, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 820, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 854, 12, 14
	add		ebx, edx
	invoke	Image_construct, ebx, 276, 872, 12, 14
	invoke	Image_construct, offset img_txtCopyright, 886, 184, 134, 10
	invoke	Image_construct, offset img_black, 584, 412, 32, 32
	invoke	Image_construct, offset img_white, 584, 448, 32, 32

	invoke	Spirit_construct, offset sp_bird, offset img_yellowBird, 0, 0
	invoke	AnimationManager_allocAnimation
	mov		sp_bird.ani, eax
	invoke	Spirit_construct, offset sp_sky, offset img_dayBackground, 144, 256
	invoke	Spirit_construct, offset sp_ground, offset img_ground, 144, 456
	invoke	Spirit_construct, offset sp_txtGetReady, offset img_txtGetReady, 137, 145
	invoke	Spirit_construct, offset sp_txtGameOver, offset img_txtGameOver, 0, 0
	invoke	AnimationManager_allocAnimation
	mov		sp_txtGameOver.ani, eax
	invoke	Spirit_construct, offset sp_txtCopyright, offset img_txtCopyright, 134, 430
	invoke	Spirit_construct, offset sp_txtFlappyBird, offset img_txtFlappyBird, 139, 144
 	invoke	Spirit_construct, offset sp_largeScore, offset img_largeNum, 144, 80
	invoke	Spirit_construct, offset sp_helpInfo, offset img_help, 137, 249
	invoke	Spirit_construct, offset sp_btPlay, offset img_btPlay, 72, 371
	invoke	Spirit_construct, offset sp_btRank, offset img_btRank, 212, 371
	invoke	Spirit_construct, offset sp_btRate, offset img_btRate, 144, 285
	invoke	Spirit_construct, offset sp_scoreBoard, offset img_scorePane, 0, 0
	invoke	AnimationManager_allocAnimation
	mov		sp_scoreBoard.ani, eax
	invoke	Spirit_construct, offset sp_blackFade, offset img_black, 0, 0
	invoke	AnimationManager_allocAnimation
	mov		sp_blackFade.ani, eax
	invoke	Spirit_construct, offset sp_whiteFade, offset img_white, 0, 0
	invoke	AnimationManager_allocAnimation
	mov		sp_whiteFade.ani, eax
	mov		ecx, 0
	mov		eax, offset sp_upPipes
	mov		ebx, offset sp_downPipes
	.while	ecx < 4
		inc		ecx
		invoke	Spirit_construct, eax, offset img_greenPipeUp, 0, 0
		invoke	Spirit_construct, ebx, offset img_greenPipeDown, 0, 0
		add		eax, sizeof Spirit
		add		ebx, sizeof Spirit
	.endw
	
	invoke	ImageManager_randomSkyAndBird

	mov		eax, TRUE
	ret
ImageManager_initAll	endp

ImageManager_drawFadeCover	proc public uses eax	img : ImagePtr, alpha : DWORD
	local	@blend : BLENDFUNCTION

	mov		@blend.AlphaFormat, 0
	mov		@blend.BlendOp, AC_SRC_OVER
	mov		@blend.BlendFlags, 0
	mov		al, BYTE ptr alpha
	mov		@blend.SourceConstantAlpha, al
	invoke	AlphaBlend, g_imgMgr.dstHdc, 0, 0, 288, 512, g_imgMgr.imgHdc, \
			img + Image.x, img + Image.y, img + Image._width, img + Image.height, addr @blend
	ret
ImageManager_drawFadeCover	endp

ImageManager_drawNumber		proc public uses eax ebx ecx edx	num : DWORD, _cx : SDWORD, cy : SDWORD, _size : DWORD, _align : DWORD
	local	@digitNum : DWORD
	local	@grap : DWORD
	local	@halfHeight : DWORD
	local	@halfWidth : DWORD
	local	@digit : DWORD
	local	@startX : SDWORD
	local	@startY : SDWORD

	invoke	_getDigitNum, num
	mov		@digitNum, eax
	assume	ebx : ptr Image
	mov		ebx, NULL
	mov		@grap, 2

	.if		_size == NumberSize_large
		mov		ebx, offset img_largeNum
		mov		@grap, 0
	.elseif	_size == NumberSize_middle
		mov		ebx, offset img_middleNum
	.elseif	_size == NumberSize_small
		mov		ebx, offset img_smallNum
	.endif
	; 计算绘制位置
	mov		eax, [ebx].height
	sar		eax, 1
	mov		@halfHeight, eax
	mov		@halfWidth, 0
	mov		ecx, 0
	mov		edx, ebx
	.while	ecx < @digitNum
		invoke	_getDigit, num, ecx
		mov		@digit, eax
		push	edx
		push	ecx
		mov		edx, 0
		mov		ecx, sizeof Image
		mul		ecx
		pop		ecx
		pop		edx
		add		eax, edx
		mov		ebx, eax
		mov		eax, @halfWidth
		add		eax, [ebx]._width
		add		eax, @grap
		.if		_size != NumberSize_large && @digit == 1
			mov		ebx, edx
			add		ebx, sizeof Image
			sub		eax, [ebx]._width
			add		ebx, sizeof Image
			add		eax, [ebx]._width
		.endif
		mov		@halfWidth, eax
		inc		ecx
	.endw
	mov		eax, @halfWidth
	sar		eax, 1
	sub		eax, @grap
	mov		@halfWidth, eax
	mov		eax, cy
	sub		eax, @halfHeight
	mov		@startY, eax
	.if		_align == NumberAlign_left
		mov		eax, @halfWidth
		sal		eax, 1
		add		eax, _cx
	.elseif	_align == NumberAlign_center
		mov		eax, @halfWidth
		add		eax, _cx
	.elseif _align == NumberAlign_right
		mov		eax, _cx
	.endif
	mov		@startX, eax
	; 绘制数字
	mov		ecx, 0
	.while	ecx < @digitNum
		invoke	_getDigit, num, ecx
		mov		@digit, eax
		push	edx
		push	ecx
		mov		edx, 0
		mov		ecx, sizeof Image
		mul		ecx
		pop		ecx
		pop		edx
		add		eax, edx
		mov		ebx, eax
		mov		eax, @startX
		.if		_size != NumberSize_large && ecx > 0
			sub		eax, @grap
		.endif
		mov		@startX, eax
		invoke	TransparentBlt, g_imgMgr.dstHdc, @startX, @startY, [ebx]._width, [ebx].height, \
				g_imgMgr.imgHdc, [ebx].x, [ebx].y, [ebx]._width, [ebx].height, TRANSPRENT_COLOR
		.if		_size != NumberSize_large && @digit == 1
			mov		eax, @startX
			mov		ebx, edx
			add		ebx, sizeof Image
			add		eax, [ebx]._width
			add		ebx, sizeof Image
			sub		eax, [ebx]._width
			mov		@startX, eax
		.endif
	.endw
	assume	ebx : nothing
	ret
ImageManager_drawNumber		endp
	end