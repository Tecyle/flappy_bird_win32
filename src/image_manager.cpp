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
Spirit sp_dliverMedal;
Spirit sp_goldenMedal;
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

static void Spirit_construct(Spirit* o, int x, int y, int width, int height);
static void Spirit_drawToHdc(Spirit* o, HDC srcHdc, HDC dstHdc, int dx, int dy);

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

	Spirit_construct(&sp_dayBackground, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Spirit_construct(Spirit* o, int x, int y, int width, int height)
{
	o->x = x;
	o->y = y;
	o->width = width;
	o->height = height;
}

void ImageManager_drawScene(ImageManager* o, HDC hdc)
{
	Spirit_drawToHdc(&sp_dayBackground, o->imgHdc, hdc, 0, 0);
}

void Spirit_drawToHdc(Spirit* o, HDC srcHdc, HDC dstHdc, int dx, int dy)
{
	TransparentBlt(dstHdc, dx, dy, o->width, o->height, srcHdc, o->x, o->y, o->width, o->height, TRANSPRENT_COLOR);
}

void ImageManager_construct(ImageManager* o, HINSTANCE hInstance)
{
	o->hInstance = hInstance;
}
