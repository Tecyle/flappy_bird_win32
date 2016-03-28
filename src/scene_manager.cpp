#include "stdafx.h"
#include "scene_manager.h"

typedef enum DayNightMode
{
	DayNightMode_day,
	DayNightMode_night
} DayNightMode;

typedef enum BirdColor
{
	BirdColor_blue,
	BirdColor_red,
	BirdColor_yellow
} BirdColor;

static DayNightMode g_dayNight;
static BirdColor g_birdColor;
static ImageManager g_imgMgr;

static void _SceneManager_randomBackAndBird()
{
	BirdColor bc[] = { BirdColor_blue, BirdColor_red, BirdColor_yellow };
	DWORD rnd = GetTickCount();
	g_dayNight = rnd % 2 ? DayNightMode_day : DayNightMode_night;
	g_birdColor = bc[rnd % 3];
}

static void _SceneManager_drawMainMenu(SceneManager* o);

void SceneManager_construct(SceneManager* o, HINSTANCE hInstance, HDC hdc)
{
	o->sceneType = SceneType_mainMenu;
	o->scrHdc = hdc;
	o->bufHdc = CreateCompatibleDC(hdc);
	o->drawingBoard = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(o->bufHdc, o->drawingBoard);

	ImageManager_construct(&g_imgMgr, hInstance);
	ImageManager_loadScenes(&g_imgMgr);
	ImageManager_initAllSpirits(&g_imgMgr, hdc);

	_SceneManager_randomBackAndBird();
}

void _SceneManager_drawMainMenu(SceneManager* o)
{
	Spirit* background = g_dayNight == DayNightMode_day ? &sp_dayBackground : &sp_nightBackground;
	ImageManager_drawSpiritToHdc(&g_imgMgr, background, o->bufHdc, 0, 0);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_txtFlappyBird, o->bufHdc, 50, 100);
}

void SceneManager_render(SceneManager* o)
{
	switch (o->sceneType)
	{
	case SceneType_mainMenu:
		_SceneManager_drawMainMenu(o);
		break;
	default:
		break;
	}
	// 翻转缓存图像到前台显示
	StretchBlt(o->scrHdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, o->bufHdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SRCCOPY);
}
