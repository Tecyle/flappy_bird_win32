#include "stdafx.h"
#include "image_manager.h"

#pragma comment(lib, "msimg32.lib")

#define TRANSPRENT_COLOR RGB(186, 0, 255)

Spirit sp_dayBackground;
Spirit sp_nightBackground;
Spirit sp_ground;
Spirit sp_txtGetReady;
Spirit sp_txtGameOver;
Spirit sp_txtFlappyBird;
Spirit sp_txtCopyright;
Spirit sp_smallNum[10];
Spirit sp_middleNum[10];
Spirit sp_largeNum[10];
Spirit sp_scorePane;
Spirit sp_sliverMedal;
Spirit sp_goldenMedal;
Spirit sp_copperMedal;
Spirit sp_help;
Spirit sp_orangePipeUp;
Spirit sp_orangePipeDown;
Spirit sp_greenPipeUp;
Spirit sp_greenPipeDown;
Spirit sp_yellowBird[3];
Spirit sp_redBird[3];
Spirit sp_BlueBird[3];
Spirit sp_btPlay;
Spirit sp_btRank;
Spirit sp_btRate;
Spirit sp_btContinue;
Spirit sp_btPause;
Spirit sp_btShare;
Spirit sp_btMenu;
Spirit sp_btOk;
Spirit sp_new;

static void Spirit_construct(Spirit* o, int x, int y, int width, int height);

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
	Spirit_construct(&sp_BlueBird[0], 174, 982, 34, 24);
	Spirit_construct(&sp_BlueBird[1], 230, 658, 34, 24);
	Spirit_construct(&sp_BlueBird[2], 230, 710, 34, 24);
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
}

void Spirit_construct(Spirit* o, int x, int y, int width, int height)
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

void ImageManager_drawSpiritToHdc(ImageManager* o, Spirit* sp, HDC hdc, int dx, int dy)
{
	TransparentBlt(hdc, dx, dy, sp->width, sp->height, o->imgHdc, sp->x, sp->y, sp->width, sp->height, TRANSPRENT_COLOR);
}
