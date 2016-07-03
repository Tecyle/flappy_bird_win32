IFNDEF	_IMAGE_MANAGER_INC_
_IMAGE_MANAGER_INC_	equ <1>

include		animation_manager.inc

NumberSize_large			equ  0		; 大号字，用于游戏过程中显示当前分数
NumberSize_middle			equ  1		; 中号字，用于游戏结束后显示分数
NumberSize_small			equ  2		; 小号字，本程序中没有用到

NumberAlign_left			equ  0		; 左对齐，游戏中没有用到这种对其方式
NumberAlign_center			equ  1		; 居中对齐
NumberAlign_right			equ  2		; 右对齐

SpiritType_ground			equ  0		; 地面
SpiritType_sky				equ  1		; 背景天空
SpiritType_bird				equ  2		; 小鸟
SpiritType_upPipes			equ  3		; 上层管道组
SpiritType_downPipes		equ  4		; 下层管道组
SpiritType_txtGetReady		equ  5		; GetReady 文字
SpiritType_txtGameOver		equ  6		; GameOver 文字
SpiritType_txtCopyRight		equ  7		; 版权文字信息
SpiritType_txtFlappyBird	equ  8		; FlappyBird 文字
SpiritType_largeScore		equ  9		; 当前得分数字
SpiritType_helpInfo			equ 10		; 帮助信息
SpiritType_btPlay			equ 11		; 开始游戏按钮
SpiritType_btRank			equ 12		; 排名按钮
SpiritType_btRate			equ 13		; Rate 按钮
SpiritType_scoreBoard		equ 14		; 记分牌
SpiritType_blackFade		equ 15		; 黑色渐变遮罩
SpiritType_whiteFade		equ 16		; 白色渐变遮罩

Image			struct
	x			SDWORD			?	; 图像矩形区域的 x 坐标
	y			SDWORD			?	; 图像矩形区域的 y 坐标
	_width		SDWORD			?	; 图像矩形区域的宽度
	height		SDWORD			?	; 图像矩形区域的高度
Image			ends
ImagePtr		typedef ptr Image

Spirit			struct
	image		ImagePtr		?	; 精灵的绘制图像
	_cx			SDWORD			?	; 精灵的中心 x 坐标
	cy			SDWORD			?	; 精灵的中心 y 坐标
	ox			SDWORD			?	; 精灵的原始中心 x 坐标，在物理引擎中用到
	oy			SDWORD			?	; 精灵的原始中心 y 坐标，在屋里引擎中用到
	angle		REAL8			?	; 精灵的旋转角度，目前仅小鸟有效
	halfWidth	SDWORD			?	; 精灵的宽度的一半
	halfHeight	SDWORD			?	; 精灵的高度的一半
	visiable	BOOLEAN			?	; 精灵是否可见
	;ani			AnimationPtr	?	; 控制精灵的动画
Spirit			ends
SpiritPtr		typedef	ptr Spirit

ImageManager	struct 
	hInstance	HINSTANCE		?	; 应用程序的实例句柄
	dstHdc		HDC				?	;
	imgScene	HBITMAP			?	; 图片资源句柄，存放整张 BMP
	imgHdc		HDC				?	; 图像绘制句柄
ImageManager	ends

Spirit_construct				proto stdcall	o : SpiritPtr, img : ImagePtr, _cx : SDWORD, cy : SDWORD

ImageManager_initAll			proto stdcall	hInstance : HINSTANCE, hdc : HDC
ImageManager_randomSkyAndBird	proto stdcall
ImageManager_drawSpirit			proto stdcall	spirit : DWORD
ImageManager_drawFadeCover		proto stdcall	img : ImagePtr, alpha : BYTE
ImageManager_drawNumber			proto stdcall	num : DWORD, _cx : SDWORD, cy : SDWORD, _size : DWORD, _align : DWORD

ImageManager_getSpirit			proto stdcall	spirit : DWORD

ENDIF