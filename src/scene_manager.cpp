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

static void _SceneManager_drawFps(SceneManager* o)
{
	char strFps[64] = { "FPS: " };
	itoa(SceneManager_getFps(o), &strFps[5], 10);
	TextOutA(o->bufHdc, 5, 5, strFps, strlen(strFps));
}

static bool _SceneManager_needReDraw(SceneManager* o)
{
	static DWORD lastCount = 0;
	DWORD frameInterval = 1000 / o->fps;
	DWORD nowCount = GetTickCount();
	if (nowCount - lastCount > frameInterval)
	{
		lastCount = nowCount;
		return true;
	}
	return false;
}

void SceneManager_construct(SceneManager* o, HINSTANCE hInstance, HDC hdc)
{
	o->sceneType = SceneType_mainMenu;
	o->scrHdc = hdc;
	o->bufHdc = CreateCompatibleDC(hdc);
	o->drawingBoard = CreateCompatibleBitmap(hdc, SCENE_WIDTH, SCENE_HEIGHT);
	SelectObject(o->bufHdc, o->drawingBoard);
	SetTextColor(o->bufHdc, RGB(255, 0, 0));
	SetBkColor(o->bufHdc, RGB(0, 0, 0));
	SetBkMode(o->bufHdc, TRANSPARENT);
	o->fps = 100;

	ImageManager_construct(&g_imgMgr, hInstance);
	ImageManager_loadScenes(&g_imgMgr);
	ImageManager_initAllSpirits(&g_imgMgr, hdc);

	_SceneManager_randomBackAndBird();

	o->drawCounter = 0;
	o->showFps = true;
}

void _SceneManager_drawMainMenu(SceneManager* o)
{
	Spirit* background = g_dayNight == DayNightMode_day ? &sp_dayBackground : &sp_nightBackground;
	ImageManager_drawSpiritToHdc(&g_imgMgr, background, o->bufHdc, 0, 0);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_txtFlappyBird, o->bufHdc, 50, 100);
}

void SceneManager_render(SceneManager* o)
{
	if (!_SceneManager_needReDraw(o))
		return;

	switch (o->sceneType)
	{
	case SceneType_mainMenu:
		_SceneManager_drawMainMenu(o);
		break;
	default:
		break;
	}

	// 绘制帧速率
	if (o->showFps)
		_SceneManager_drawFps(o);

	// 翻转缓存图像到前台显示
	StretchBlt(o->scrHdc, 0, 0, o->windowWidth, o->windowHeight, o->bufHdc, 0, 0, SCENE_WIDTH, SCENE_HEIGHT, SRCCOPY);
	o->drawCounter++;
}

void SceneManager_setViewSize(SceneManager* o, int width, int height)
{
	o->windowWidth = width;
	o->windowHeight = height;
}

void SceneManager_setFps(SceneManager* o, int fps)
{
	if (fps < 10 || fps > 120)
		fps = 60;

	o->fps = fps;
}

// 此函数获取的帧速率应该是实时统计的结果
int SceneManager_getFps(SceneManager* o)
{
	static DWORD lastCount = GetTickCount();
	static int lastFps = 0;
	DWORD nowCount = GetTickCount();
	if (nowCount - lastCount > 1000)
	{
		lastFps = o->drawCounter * 1000 / (nowCount - lastCount);
		lastCount = nowCount;
		o->drawCounter = 0;
	}
	return lastFps;
}
