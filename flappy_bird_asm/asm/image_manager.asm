include		stdafx.inc
include		image_manager.inc
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
	;mov		[ebx].ani, NULL
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
	;assume	ecx : ptr Animation
	mov		ebx, o
	.if		[ebx].visiable == FALSE
		ret
	.endif
	;mov		ecx, [ebx].ani
	;.if		ecx != NULL && [ecx].transEnable
		;invoke	TransAnimation_getXY, ecx, offset [ebx]._cx, offset [ebx].cy
	;.endif
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

Spirit_drawBird		proc public uses ebx ecx	o : SpiritPtr
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
	add		ecx, @frameIndex * (sizeof Image)
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
	invoke	_restoreHdc, dstHdc
	assume	ebx : nothing
	assume	ecx : nothing
	ret
Spirit_drawBird		endp

Spirit_drawPipes	proc public uses ebx ecx edx	oword : SpiritPtr
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
	local	srcHdc : HDC
	local	dstHdc : HDC

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
	invoke	TransAnimation_getXY, [ebx].ani, offset [ebx]._cx, offset [ebx].cy
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
	mov		edx, sizeof Image
	mul		edx, eax
	mov		ecx, imgMedal
	add		ecx, edx
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

	end
