#include "stdafx.h"
#include "image_manager.h"
#include "animation_manager.h"
#include <math.h>
#pragma comment(lib, "msimg32.lib")			// AlphaBlend 函数需要用到这个库

#define TRANSPRENT_COLOR RGB(186, 0, 255)	// 定义透明色，紫色

// 全局变量
Image img_dayBackground;
Image img_nightBackground;
Image img_ground;
Image img_txtGetReady;
Image img_txtGameOver;
Image img_txtFlappyBird;
Image img_txtCopyright;
Image img_smallNum[10];
Image img_middleNum[10];
Image img_largeNum[10];
Image img_scorePane;
Image img_sliverMedal;
Image img_goldenMedal;
Image img_copperMedal;
Image img_help;
Image img_orangePipeUp;
Image img_orangePipeDown;
Image img_greenPipeUp;
Image img_greenPipeDown;
Image img_yellowBird[3];
Image img_redBird[3];
Image img_blueBird[3];
Image img_btPlay;
Image img_btRank;
Image img_btRate;
Image img_btContinue;
Image img_btPause;
Image img_btShare;
Image img_btMenu;
Image img_btOk;
Image img_new;
Image img_black;
Image img_white;

Spirit sp_ground;
Spirit sp_bird;
Spirit sp_sky;
Spirit sp_upPipes[4];
Spirit sp_downPipes[4];
Spirit sp_txtGetReady;
Spirit sp_txtGameOver;
Spirit sp_txtCopyright;
Spirit sp_txtFlappyBird;
Spirit sp_largeScore;
Spirit sp_helpInfo;
Spirit sp_btPlay;
Spirit sp_btRank;
Spirit sp_btRate;
Spirit sp_scoreBoard;
Spirit sp_balckFade;
Spirit sp_whiteFade;

ImageManager g_imgMgr;

Spirit* g_spirits[] = {
	&sp_ground,			///< SpiritType_ground
	&sp_sky,			///< SpiritType_sky
	&sp_bird,			///< SpiritType_bird
	sp_upPipes,			///< SpiritType_upPipes
	sp_downPipes,		///< SpiritType_downPipes
	&sp_txtGetReady,	///< SpiritType_txtGetReady
	&sp_txtGameOver,	///< SpiritType_txtGameOver
	&sp_txtCopyright,	///< SpiritType_txtCopyRight
	&sp_txtFlappyBird,	///< SpiritType_txtFlappyBird
	&sp_largeScore,		///< SpiritType_largeScore
	&sp_helpInfo,		///< SpiritType_helpInfo
	&sp_btPlay,			///< SpiritType_btPlay
	&sp_btRank,			///< SpiritType_btRank
	&sp_btRate,			///< SpiritType_btRate
	&sp_scoreBoard,		///< SpiritType_scoreBoard
	&sp_balckFade,		///< SpiritType_blackFade
	&sp_whiteFade		///< SpiritType_whiteFade
};

//////////////////////////////////////////////////////////////////////////
// 全局函数方法
static void _rotateHdc(HDC hdc, double angle, int cx, int cy)
{
	SetGraphicsMode(hdc, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = (float)cos(angle);
	xform.eM12 = (float)sin(angle);
	xform.eM21 = (float)-sin(angle);
	xform.eM22 = (float)cos(angle);
	xform.eDx = (float)(cx - cos(angle) * cx + sin(angle) * cy);
	xform.eDy = (float)(cy - cos(angle) * cy - sin(angle) * cx);
	SetWorldTransform(hdc, &xform);
}

static void _restoreHdc(HDC hdc)
{
	XFORM xform;
	xform.eM11 = (float)1.0;
	xform.eM12 = (float)0;
	xform.eM21 = (float)0;
	xform.eM22 = (float)1.0;
	xform.eDx = (float)0;
	xform.eDy = (float)0;

	SetWorldTransform(hdc, &xform);
	SetGraphicsMode(hdc, GM_COMPATIBLE);
}

size_t _getDigitNum(size_t num)
{
	size_t digitNum = 1;
	while ((num = num / 10) != 0)
	{
		digitNum++;
	}
	return digitNum;
}

size_t _pow(size_t num, size_t power)
{
	size_t res = 1;
	for (size_t i = 0; i < power; ++i)
	{
		res *= num;
	}
	return res;
}

size_t _getDigit(size_t num, size_t index)
{
	size_t sn = _pow(10, index);
	sn = num / sn;
	sn = sn % 10;
	return sn;
}

//////////////////////////////////////////////////////////////////////////
// Image 对象相关方法
inline void Image_construct(Image* o, int x, int y, int width, int height)
{
	o->x = x;
	o->y = y;
	o->width = width;
	o->height = height;
}

//////////////////////////////////////////////////////////////////////////
// Spirit 对象相关方法
void Spirit_construct(Spirit* o, Image* img, int cx, int cy)
{
	o->image = img;
	o->cx = cx;
	o->cy = cy;
	o->ox = cx;
	o->oy = cy;
	o->halfWidth = img->width / 2;
	o->halfHeight = img->height / 2;
	o->visiable = true;
}

void Spirit_draw(Spirit* o)
{
	HDC srcHdc = g_imgMgr.imgHdc;
	HDC dstHdc = g_imgMgr.dstHdc;
	Image* imgSrc = o->image;
	if (!o->visiable)
		return;
	TransparentBlt(dstHdc, o->cx - o->halfWidth, o->cy - o->halfHeight, imgSrc->width, imgSrc->height,
		srcHdc, imgSrc->x, imgSrc->y, imgSrc->width, imgSrc->height, TRANSPRENT_COLOR);
}

void Spirit_drawBird(Spirit* o)
{
	HDC srcHdc = g_imgMgr.imgHdc;
	HDC dstHdc = g_imgMgr.dstHdc;
	if (!o->visiable)
		return;

	// 小鸟的扇动翅膀动画
	Image* imgSrc = &o->image[FrameAnimation_getFrameIndex(o->ani)];
	// 旋转小鸟
	_rotateHdc(dstHdc, o->angle, o->cx, o->cy);
	TransparentBlt(dstHdc, o->cx - o->halfWidth, o->cy - o->halfHeight, imgSrc->width, imgSrc->height,
		srcHdc, imgSrc->x, imgSrc->y, imgSrc->width, imgSrc->height, TRANSPRENT_COLOR);
	_restoreHdc(dstHdc);
}

void Spirit_drawPipes(Spirit* o)
{
	HDC srcHdc = g_imgMgr.imgHdc;
	HDC dstHdc = g_imgMgr.dstHdc;
	
	for (int i = 0; i < 4; ++i)
	{
		Spirit* sp = &o[i];
		Image* imgSrc = sp->image;
		if (!o->visiable)
			continue;
		TransparentBlt(dstHdc, o->cx - o->halfWidth, o->cy - o->halfHeight, imgSrc->width, imgSrc->height,
			srcHdc, imgSrc->x, imgSrc->y, imgSrc->width, imgSrc->height, TRANSPRENT_COLOR);
	}
}

void Spirit_drawLargeScore(Spirit* o)
{
	HDC srcHdc = g_imgMgr.imgHdc;
	HDC dstHdc = g_imgMgr.dstHdc;
	if (!o->visiable)
		return;

	size_t score = ScoreManager_getCurrentScore();
	ImageManager_drawNumber(score, o->cx, o->cy, NumberSize_large, NumberAlign_center);
}

void Spirit_drawScoreBorad(Spirit* o)
{
	HDC srcHdc = g_imgMgr.imgHdc;
	HDC dstHdc = g_imgMgr.dstHdc;
	if (!o->visiable)
		return;

	// 先绘制底板
	Image* imgSrc = &img_scorePane;
	TransparentBlt(dstHdc, o->cx - o->halfWidth, o->cy - o->halfHeight, imgSrc->width, imgSrc->height,
		srcHdc, imgSrc->x, imgSrc->y, imgSrc->width, imgSrc->height, TRANSPRENT_COLOR);
	// 再绘制当前得分
	ImageManager_drawNumber(ScoreManager_getCurrentScore(),
		o->cx + 90, o->cy - 15,
		NumberSize_middle, NumberAlign_right);
	// 再绘制最高得分
	ImageManager_drawNumber(ScoreManager_getHighScore(),
		o->cx + 90, o->cy + 28,
		NumberSize_middle, NumberAlign_right);
	// 再决定绘制什么奖牌
	Image* imgMedal[] = { &img_goldenMedal, &img_sliverMedal, &img_copperMedal, NULL };
	imgSrc = imgMedal[(int)ScoreManager_getMedalType()];
	if (imgSrc != NULL)
	{
		TransparentBlt(dstHdc, o->cx - 88, o->cy - 14, imgSrc->width, imgSrc->height,
			srcHdc, imgSrc->x, imgSrc->y, imgSrc->width, imgSrc->height, TRANSPRENT_COLOR);
	}
}

void Spirit_drawFade(Spirit* o)
{
	HDC srcHdc = g_imgMgr.imgHdc;
	HDC dstHdc = g_imgMgr.dstHdc;
	Image* imgSrc = o->image;
	if (!o->visiable)
		return;

	ImageManager_drawFadeCover(imgSrc, FadeAnimation_getAlpha(o->ani));
}

//////////////////////////////////////////////////////////////////////////
// Button 对象相关方法


//////////////////////////////////////////////////////////////////////////
// ImageManager 对象相关方法
void ImageManager_construct(HINSTANCE hInstance, HDC dstHdc)
{
	ImageManager* o = &g_imgMgr;
	o->hInstance = hInstance;
	o->dstHdc = dstHdc;
	o->imgHdc = NULL;
	o->imgScene = NULL;
}

void ImageManager_destruct()
{
	ImageManager* o = &g_imgMgr;
}

void ImageManager_drawSpirit(SpiritType spiritType)
{
	// 绘制的精灵一共分为以下几种不同的类型
	// 1. 小鸟
	// 小鸟不一样的地方在于它需要旋转
	// 2. 淡入淡出效果
	// 淡入淡出效果不一样的地方在于它需要绘制半透明效果
	// 3. 管道
	// 管道是因为其包含上下管道且循环出现
	// 4. 得分面板
	// 得分面板是因为有很多东西组合在上面
	// 5. 得分
	// 得分是因为其要绘制数字
	// 3. 其他精灵
	// 剩下的直接绘制即可
	ImageManager* o = &g_imgMgr;
	Spirit* spirit = g_spirits[(int)spiritType];
	switch (spiritType)
	{
	case SpiritType_bird:
		Spirit_drawBird(spirit);
		break;
	case SpiritType_upPipes:
	case SpiritType_downPipes:
		Spirit_drawPipes(spirit);
		break;
	case SpiritType_largeScore:
		Spirit_drawLargeScore(spirit);
		break;
	case SpiritType_scoreBoard:
		Spirit_drawScoreBorad(spirit);
		break;
	case SpiritType_blackFade:
	case SpiritType_whiteFade:
		Spirit_drawFade(spirit);
		break;
	default:
		Spirit_draw(spirit);
		break;
	}
}

Spirit* ImageManager_getSpirit(SpiritType spirit)
{
	return g_spirits[(int)spirit];
}

void ImageManager_randomSkyAndBird()
{
	Image* pBird[] = { img_blueBird, img_yellowBird, img_redBird };
	DWORD rnd = GetTickCount();
	sp_sky.image = rnd % 2 ? &img_dayBackground : &img_nightBackground;
	sp_bird.image = pBird[rnd % 3];
}

bool ImageManager_initAll(HDC hdc)
{
	ImageManager* o = &g_imgMgr;
	o->imgScene = LoadBitmap(o->hInstance, MAKEINTRESOURCE(IDB_SCENE));
	if (!o->imgScene)
		return false;
	return true;

	o->imgHdc = CreateCompatibleDC(hdc);
	SelectObject(o->imgHdc, o->imgScene);

	Image_construct(&img_dayBackground, 0, 0, SCENE_WIDTH, SCENE_HEIGHT);
	Image_construct(&img_ground, 584, 0, 336, 112);
	Image_construct(&img_txtFlappyBird, 702, 182, 178, 48);
	Image_construct(&img_btRate, 930, 2, 62, 36);
	Image_construct(&img_btMenu, 924, 52, 80, 28);
	Image_construct(&img_btOk, 924, 84, 80, 28);
	Image_construct(&img_txtGameOver, 790, 118, 192, 42);
	Image_construct(&img_txtGetReady, 590, 118, 184, 50);
	Image_construct(&img_help, 584, 182, 114, 98);
	Image_construct(&img_btShare, 584, 284, 80, 28);
	Image_construct(&img_btContinue, 668, 284, 26, 28);
	Image_construct(&img_btPlay, 708, 236, 104, 58);
	Image_construct(&img_btRank, 828, 236, 104, 58);
	Image_construct(&img_nightBackground, 292, 0, SCENE_WIDTH, SCENE_HEIGHT);
	Image_construct(&img_scorePane, 6, 518, 226, 114);
	Image_construct(&img_sliverMedal, 242, 516, 44, 44);
	Image_construct(&img_goldenMedal, 242, 564, 44, 44);
	Image_construct(&img_copperMedal, 224, 954, 44, 44);
	Image_construct(&img_btPause, 242, 612, 26, 28);
	Image_construct(&img_orangePipeUp, 0, 646, 52, 320);
	Image_construct(&img_orangePipeDown, 56, 646, 52, 320);
	Image_construct(&img_greenPipeUp, 112, 646, 52, 320);
	Image_construct(&img_greenPipeDown, 168, 646, 52, 320);
	Image_construct(&img_blueBird[0], 174, 982, 34, 24);
	Image_construct(&img_blueBird[1], 230, 658, 34, 24);
	Image_construct(&img_blueBird[2], 230, 710, 34, 24);
	Image_construct(&img_redBird[0], 230, 762, 34, 24);
	Image_construct(&img_redBird[1], 230, 814, 34, 24);
	Image_construct(&img_redBird[2], 230, 866, 34, 24);
	Image_construct(&img_yellowBird[0], 6, 982, 34, 24);
	Image_construct(&img_yellowBird[1], 62, 982, 34, 24);
	Image_construct(&img_yellowBird[2], 118, 982, 34, 24);
	Image_construct(&img_new, 224, 1002, 32, 14);
	Image_construct(&img_largeNum[0], 992, 120, 24, 36);
	Image_construct(&img_largeNum[1], 272, 910, 16, 36);
	Image_construct(&img_largeNum[2], 584, 320, 24, 36);
	Image_construct(&img_largeNum[3], 612, 320, 24, 36);
	Image_construct(&img_largeNum[4], 640, 320, 24, 36);
	Image_construct(&img_largeNum[5], 668, 320, 24, 36);
	Image_construct(&img_largeNum[6], 584, 368, 24, 36);
	Image_construct(&img_largeNum[7], 612, 368, 24, 36);
	Image_construct(&img_largeNum[8], 640, 368, 24, 36);
	Image_construct(&img_largeNum[9], 668, 368, 24, 36);
	Image_construct(&img_middleNum[0], 274, 612, 14, 20);
	Image_construct(&img_middleNum[1], 278, 954, 10, 20);
	Image_construct(&img_middleNum[2], 274, 978, 14, 20);
	Image_construct(&img_middleNum[3], 262, 1002, 14, 20);
	Image_construct(&img_middleNum[4], 1004, 0, 14, 20);
	Image_construct(&img_middleNum[5], 1004, 24, 14, 20);
	Image_construct(&img_middleNum[6], 1010, 52, 14, 20);
	Image_construct(&img_middleNum[7], 1010, 84, 14, 20);
	Image_construct(&img_middleNum[8], 586, 484, 14, 20);
	Image_construct(&img_middleNum[9], 622, 412, 14, 20);
	Image_construct(&img_smallNum[0], 276, 646, 12, 14);
	Image_construct(&img_smallNum[1], 282, 664, 6, 14);
	Image_construct(&img_smallNum[2], 276, 698, 12, 14);
	Image_construct(&img_smallNum[3], 276, 716, 12, 14);
	Image_construct(&img_smallNum[4], 276, 750, 12, 14);
	Image_construct(&img_smallNum[5], 276, 768, 12, 14);
	Image_construct(&img_smallNum[6], 276, 802, 12, 14);
	Image_construct(&img_smallNum[7], 276, 820, 12, 14);
	Image_construct(&img_smallNum[8], 276, 854, 12, 14);
	Image_construct(&img_smallNum[9], 276, 872, 12, 14);
	Image_construct(&img_txtCopyright, 886, 184, 134, 10);
	Image_construct(&img_black, 584, 412, 32, 32);
	Image_construct(&img_white, 584, 448, 32, 32);

	Spirit_construct(&sp_bird, NULL, 0, 0);
	Spirit_construct(&sp_sky, NULL, 144, 256);
	Spirit_construct(&sp_ground, &img_ground, 0, 456);
	ImageManager_randomSkyAndBird();

	return true;
}

void ImageManager_drawFadeCover(Image* img, BYTE alpha)
{
	ImageManager* o = &g_imgMgr;
	BLENDFUNCTION blend;
	blend.AlphaFormat = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = alpha;
	AlphaBlend(o->dstHdc, 0, 0, 288, 512, o->imgHdc, img->x, img->y, img->width, img->height, blend);
}

void ImageManager_drawNumber(size_t num, int cx, int cy, NumberSize size, NumberAlign align)
{
	ImageManager* o = &g_imgMgr;
	size_t digitNum = _getDigitNum(num);
	Image* spNum = NULL;
	int grap = 2;
	switch (size)
	{
	case NumberSize_large:
		spNum = img_largeNum;
		grap = 0;
		break;
	case NumberSize_middle:
		spNum = img_middleNum;
		break;
	case NumberSize_small:
		spNum = img_smallNum;
		break;
	}
	// 计算绘制位置
	int halfHeight = spNum->height / 2;
	int halfWidth = 0;
	for (size_t i = 0; i < digitNum; ++i)
	{
		size_t digit = _getDigit(num, i);
		halfWidth += spNum[digit].width + grap;
		if (size != NumberSize_large && digit == 1)
			halfWidth += spNum[2].width - spNum[1].width;
	}
	halfWidth /= 2;
	halfWidth -= grap;
	int startX;
	int startY = cy - halfHeight;
	switch (align)
	{
	case NumberAlign_left:
		startX = cx + 2 * halfWidth;
		break;
	case NumberAlign_center:
		startX = cx + halfWidth;
		break;
	case NumberAlign_right:
		startX = cx;
		break;
	}
	// 绘制数字
	for (size_t i = 0; i < digitNum; ++i)
	{
		size_t digit = _getDigit(num, i);
		Image* spn = &spNum[digit];
		startX -= spn->width;
		startX -= size != NumberSize_large && i > 0 ? grap : 0;
		TransparentBlt(o->dstHdc, startX, startY, spn->width, spn->height,
			o->imgHdc, spn->x, spn->y, spn->width, spn->height, TRANSPRENT_COLOR);
		if (size != NumberSize_large && digit == 1)
			startX -= spNum[2].width - spNum[1].width;
	}
}



bool Spirit_isPointInMe(Image* o, int x, int y, int dx, int dy)
{
	if (x > dx && x < dx + o->width)
		if (y > dy && y < dy + o->height)
			return true;
	return false;
}
