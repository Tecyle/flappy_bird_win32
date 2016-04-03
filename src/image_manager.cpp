#include "stdafx.h"
#include "image_manager.h"
#pragma comment(lib, "msimg32.lib")			// AlphaBlend 函数需要用到这个库

#define TRANSPRENT_COLOR RGB(186, 0, 255)	// 定义透明色，紫色

// 全局变量
Image sp_dayBackground;
Image sp_nightBackground;
Image sp_ground;
Image sp_txtGetReady;
Image sp_txtGameOver;
Image sp_txtFlappyBird;
Image sp_txtCopyright;
Image sp_smallNum[10];
Image sp_middleNum[10];
Image sp_largeNum[10];
Image sp_scorePane;
Image sp_sliverMedal;
Image sp_goldenMedal;
Image sp_copperMedal;
Image sp_help;
Image sp_orangePipeUp;
Image sp_orangePipeDown;
Image sp_greenPipeUp;
Image sp_greenPipeDown;
Image sp_yellowBird[3];
Image sp_redBird[3];
Image sp_blueBird[3];
Image sp_btPlay;
Image sp_btRank;
Image sp_btRate;
Image sp_btContinue;
Image sp_btPause;
Image sp_btShare;
Image sp_btMenu;
Image sp_btOk;
Image sp_new;
Image sp_black;
Image sp_white;

//////////////////////////////////////////////////////////////////////////
// Image 对象相关方法

inline void Image_construct(Image* o, int x, int y, int width, int height)
{
	o->x = x;
	o->y = y;
	o->width = width;
	o->height = height;
}

static void Spirit_construct(Image* o, int x, int y, int width, int height);

bool ImageManager_loadScenes(ImageManager* o)
{
	o->imgScene = LoadBitmap(o->hInstance, MAKEINTRESOURCE(IDB_SCENE));
	if (!o->imgScene)
		return false;
	return true;
}

void ImageManager_initAllSpirits(ImageManager* o, HDC hdc)
{
	o->imgHdc = CreateCompatibleDC(hdc);
	SelectObject(o->imgHdc, o->imgScene);

	Spirit_construct(&sp_dayBackground, 0, 0, SCENE_WIDTH, SCENE_HEIGHT);
	Spirit_construct(&sp_ground, 584, 0, 336, 112);
	Spirit_construct(&sp_txtFlappyBird, 702, 182, 178, 48);
	Spirit_construct(&sp_btRate, 930, 2, 62, 36);
	Spirit_construct(&sp_btMenu, 924, 52, 80, 28);
	Spirit_construct(&sp_btOk, 924, 84, 80, 28);
	Spirit_construct(&sp_txtGameOver, 790, 118, 192, 42);
	Spirit_construct(&sp_txtGetReady, 590, 118, 184, 50);
	Spirit_construct(&sp_help, 584, 182, 114, 98);
	Spirit_construct(&sp_btShare, 584, 284, 80, 28);
	Spirit_construct(&sp_btContinue, 668, 284, 26, 28);
	Spirit_construct(&sp_btPlay, 708, 236, 104, 58);
	Spirit_construct(&sp_btRank, 828, 236, 104, 58);
	Spirit_construct(&sp_nightBackground, 292, 0, SCENE_WIDTH, SCENE_HEIGHT);
	Spirit_construct(&sp_scorePane, 6, 518, 226, 114);
	Spirit_construct(&sp_sliverMedal, 242, 516, 44, 44);
	Spirit_construct(&sp_goldenMedal, 242, 564, 44, 44);
	Spirit_construct(&sp_copperMedal, 224, 954, 44, 44);
	Spirit_construct(&sp_btPause, 242, 612, 26, 28);
	Spirit_construct(&sp_orangePipeUp, 0, 646, 52, 320);
	Spirit_construct(&sp_orangePipeDown, 56, 646, 52, 320);
	Spirit_construct(&sp_greenPipeUp, 112, 646, 52, 320);
	Spirit_construct(&sp_greenPipeDown, 168, 646, 52, 320);
	Spirit_construct(&sp_blueBird[0], 174, 982, 34, 24);
	Spirit_construct(&sp_blueBird[1], 230, 658, 34, 24);
	Spirit_construct(&sp_blueBird[2], 230, 710, 34, 24);
	Spirit_construct(&sp_redBird[0], 230, 762, 34, 24);
	Spirit_construct(&sp_redBird[1], 230, 814, 34, 24);
	Spirit_construct(&sp_redBird[2], 230, 866, 34, 24);
	Spirit_construct(&sp_yellowBird[0], 6, 982, 34, 24);
	Spirit_construct(&sp_yellowBird[1], 62, 982, 34, 24);
	Spirit_construct(&sp_yellowBird[2], 118, 982, 34, 24);
	Spirit_construct(&sp_new, 224, 1002, 32, 14);
	Spirit_construct(&sp_largeNum[0], 992, 120, 24, 36);
	Spirit_construct(&sp_largeNum[1], 272, 910, 16, 36);
	Spirit_construct(&sp_largeNum[2], 584, 320, 24, 36);
	Spirit_construct(&sp_largeNum[3], 612, 320, 24, 36);
	Spirit_construct(&sp_largeNum[4], 640, 320, 24, 36);
	Spirit_construct(&sp_largeNum[5], 668, 320, 24, 36);
	Spirit_construct(&sp_largeNum[6], 584, 368, 24, 36);
	Spirit_construct(&sp_largeNum[7], 612, 368, 24, 36);
	Spirit_construct(&sp_largeNum[8], 640, 368, 24, 36);
	Spirit_construct(&sp_largeNum[9], 668, 368, 24, 36);
	Spirit_construct(&sp_middleNum[0], 274, 612, 14, 20);
	Spirit_construct(&sp_middleNum[1], 278, 954, 10, 20);
	Spirit_construct(&sp_middleNum[2], 274, 978, 14, 20);
	Spirit_construct(&sp_middleNum[3], 262, 1002, 14, 20);
	Spirit_construct(&sp_middleNum[4], 1004, 0, 14, 20);
	Spirit_construct(&sp_middleNum[5], 1004, 24, 14, 20);
	Spirit_construct(&sp_middleNum[6], 1010, 52, 14, 20);
	Spirit_construct(&sp_middleNum[7], 1010, 84, 14, 20);
	Spirit_construct(&sp_middleNum[8], 586, 484, 14, 20);
	Spirit_construct(&sp_middleNum[9], 622, 412, 14, 20);
	Spirit_construct(&sp_smallNum[0], 276, 646, 12, 14);
	Spirit_construct(&sp_smallNum[1], 282, 664, 6, 14);
	Spirit_construct(&sp_smallNum[2], 276, 698, 12, 14);
	Spirit_construct(&sp_smallNum[3], 276, 716, 12, 14);
	Spirit_construct(&sp_smallNum[4], 276, 750, 12, 14);
	Spirit_construct(&sp_smallNum[5], 276, 768, 12, 14);
	Spirit_construct(&sp_smallNum[6], 276, 802, 12, 14);
	Spirit_construct(&sp_smallNum[7], 276, 820, 12, 14);
	Spirit_construct(&sp_smallNum[8], 276, 854, 12, 14);
	Spirit_construct(&sp_smallNum[9], 276, 872, 12, 14);
	Spirit_construct(&sp_txtCopyright, 886, 184, 134, 10);
	Spirit_construct(&sp_black, 584, 412, 32, 32);
	Spirit_construct(&sp_white, 584, 448, 32, 32);
}

void Spirit_construct(Image* o, int x, int y, int width, int height)
{
	o->x = x;
	o->y = y;
	o->width = width;
	o->height = height;
}

void ImageManager_construct(ImageManager* o, HINSTANCE hInstance)
{
	o->hInstance = hInstance;
}

void ImageManager_drawSpiritToHdc(ImageManager* o, Image* sp, HDC hdc, int dx, int dy)
{
	TransparentBlt(hdc, dx, dy, sp->width, sp->height, o->imgHdc, sp->x, sp->y, sp->width, sp->height, TRANSPRENT_COLOR);
}

void ImageManager_alphaBlend(ImageManager* o, Image* sp, HDC hdc, int dx, int dy, int width, int height, BYTE alpha)
{
	BLENDFUNCTION blend;
	blend.AlphaFormat = 0;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = alpha;
	AlphaBlend(hdc, dx, dy, width, height, o->imgHdc, sp->x, sp->y, sp->width, sp->height, blend);
}

bool Spirit_isPointInMe(Image* o, int x, int y, int dx, int dy)
{
	if (x > dx && x < dx + o->width)
		if (y > dy && y < dy + o->height)
			return true;
	return false;
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

void ImageManager_drawNumber(ImageManager* o, size_t num, HDC hdcDst, int cx, int cy, DrawNumberSize numberSize, DrawNumberAlign align)
{
	size_t digitNum = _getDigitNum(num);
	Image* spNum = NULL;
	switch (numberSize)
	{
	case DrawNumberSize_large:
		spNum = sp_largeNum;
		break;
	case DrawNumberSize_middle:
		spNum = sp_middleNum;
		break;
	case DrawNumberSize_small:
		spNum = sp_smallNum;
		break;
	}
	// 计算绘制位置
	int grap = numberSize == DrawNumberSize_large ? 0 : 2;
	int halfHeight = spNum->height / 2;
	int halfWidth = 0;
	for (size_t i = 0; i < digitNum; ++i)
	{
		halfWidth += spNum[_getDigit(num, i)].width + grap;
		if (numberSize != DrawNumberSize_large && _getDigit(num, i) == 1)
			halfWidth += spNum[2].width - spNum[1].width;
	}
	halfWidth /= 2;
	halfWidth -= grap;
	int startX;
	int startY = cy - halfHeight;
	switch (align)
	{
	case DrawNumberAlign_left:
		startX = cx + 2 * halfWidth;
		break;
	case DrawNumberAlign_center:
		startX = cx + halfWidth;
		break;
	case DrawNumberAlign_right:
		startX = cx;
		break;
	}
	// 绘制数字
	for (size_t i = 0; i < digitNum; ++i)
	{
		Image* spn = &spNum[_getDigit(num, i)];
		startX -=spn->width;
		startX -= numberSize != DrawNumberSize_large && i > 0 ? grap : 0;
		ImageManager_drawSpiritToHdc(o, spn, hdcDst, startX, startY);
		if (numberSize != DrawNumberSize_large && _getDigit(num, i) == 1)
			startX -= spNum[2].width - spNum[1].width;
	}
}