#include "stdafx.h"
#include "scene_manager.h"
#include "animation_manager.h"
#include "physic_engine.h"
#include "button.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////
// 全局变量
Scene sce_mainMenu;
Scene sce_prepare;
Scene sce_playing;
Scene sce_gameOver;

SceneManager g_sceMgr;

// MainScene 里面需要绘制的内容
SpiritType sptMainMenu[9] = {
	SpiritType_sky,
	SpiritType_ground,
	SpiritType_txtFlappyBird,
	SpiritType_bird,
	SpiritType_btRate,
	SpiritType_btPlay,
	SpiritType_btRank,
	SpiritType_txtCopyRight,
	SpiritType_blackFade
};

// PrepareScene 里面需要绘制的内容
SpiritType sptPrepare[7] = {
	SpiritType_sky,
	SpiritType_ground,
	SpiritType_txtGetReady,
	SpiritType_helpInfo,
	SpiritType_bird,
	SpiritType_largeScore,
	SpiritType_blackFade
};

// PlayingScene 里面需要绘制的内容
SpiritType sptPlaying[7] = {
	SpiritType_sky,
	SpiritType_ground,
	SpiritType_upPipes,
	SpiritType_downPipes,
	SpiritType_largeScore,
	SpiritType_bird,
	SpiritType_whiteFade
};

// GameOverScene 里面需要绘制的内容
SpiritType sptGameOver[11] = {
	SpiritType_sky,
	SpiritType_ground,
	SpiritType_upPipes,
	SpiritType_downPipes,
	SpiritType_bird,
	SpiritType_txtGameOver,
	SpiritType_scoreBoard,
	SpiritType_btPlay,
	SpiritType_btRank,
	SpiritType_whiteFade,
	SpiritType_blackFade
};

//////////////////////////////////////////////////////////////////////////
// Scene
void Scene_draw(Scene* o)
{
	for (int i = 0; i < o->spiritCount; ++i)
	{
		ImageManager_drawSpirit(o->spirits[i]);
	}
}

//////////////////////////////////////////////////////////////////////////
// SceneManager

bool _SceneManager_needRedraw()
{
	SceneManager* o = &g_sceMgr;
	static LARGE_INTEGER lastCount = { 0, 0 };
	long long frameInterval = g_counterFrequency.QuadPart / o->fps;
	LARGE_INTEGER nowCount;
	QueryPerformanceCounter(&nowCount);
	if (nowCount.QuadPart - lastCount.QuadPart > frameInterval)
	{
		lastCount = nowCount;
		return true;
	}
	return false;
}

void _SceneManager_drawScene()
{
	SceneManager* o = &g_sceMgr;
	Scene* scene[] = { &sce_mainMenu, &sce_prepare, &sce_playing, &sce_gameOver };
	Scene_draw(scene[(int)o->currentScene]);
}

void _SceneManager_drawFps(SceneManager* o)
{
	char strFps[64] = { "FPS: " };
	itoa(SceneManager_getFps(o), &strFps[5], 10);
	TextOutA(o->bufHdc, 5, 5, strFps, strlen(strFps));
}

void _SceneManager_tick()
{

}

void SceneManager_initAllScene()
{
	Scene* scenes[4] = { &sce_mainMenu, &sce_prepare, &sce_playing, &sce_gameOver };
	SpiritType* spiritTypes[4] = { sptMainMenu, sptPrepare, sptPlaying, sptGameOver };
	size_t spiritNum[4] = { 9, 7, 7, 11 };

	for (int i = 0; i < 4; ++i)
	{
		scenes[i]->spirits = spiritTypes[i];
		scenes[i]->spiritCount = spiritNum[i];
	}
}

bool SceneManager_render()
{
	SceneManager* o = &g_sceMgr;

	// 判断是否需要重绘，如果没有到达绘制时间，则不进行绘制
	// 这样可以保证帧速率稳定
	if (!_SceneManager_needRedraw())
		return false;

	o->drawCounter++;

	// 执行游戏逻辑，主要包括更新游戏的物理引擎，从而计算所
	// 有物体的新位置
	_SceneManager_tick();

	// 绘制计算完后的画面
	_SceneManager_drawScene();

	// 绘制帧速率
	if (o->showFps)
		_SceneManager_drawFps();

	// 翻转缓存图像到前台显示
	StretchBlt(o->scrHdc, 0, 0, o->windowWidth, o->windowHeight, o->bufHdc, 0, 0, SCENE_WIDTH, SCENE_HEIGHT, SRCCOPY);
	return true;
}







typedef enum DayNightMode
{
	DayNightMode_day,
	DayNightMode_night
} DayNightMode;

static DayNightMode g_dayNight;
static BirdColor g_birdColor;
static ImageManager g_imgMgr;
static Button btGameOver;
static Button btReplay;
static Button btRank;
static Button btRankBoard;

static LARGE_INTEGER g_counterFrequency;

static void _SceneManager_randomBackAndBird()
{
	BirdColor bc[] = { BirdColor_blue, BirdColor_red, BirdColor_yellow };
	DWORD rnd = GetTickCount();
	g_dayNight = rnd % 2 ? DayNightMode_day : DayNightMode_night;
	g_birdColor = bc[rnd % 3];
}

static void _SceneManager_drawMainMenu(SceneManager* o);

static bool _SceneManager_needReDraw(SceneManager* o)
{
	static LARGE_INTEGER lastCount = { 0, 0 };
	long long frameInterval = g_counterFrequency.QuadPart / o->fps;
	LARGE_INTEGER nowCount;
	QueryPerformanceCounter(&nowCount);
	if (nowCount.QuadPart - lastCount.QuadPart > frameInterval)
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
	o->fps = 60;

	ImageManager_construct(&g_imgMgr, hInstance);
	ImageManager_loadScenes(&g_imgMgr);
	ImageManager_initAllSpirits(&g_imgMgr, hdc);

	_SceneManager_randomBackAndBird();

	o->drawCounter = 0;
	o->showFps = true;
	o->isFading = false;
	QueryPerformanceFrequency(&g_counterFrequency);

	Button_construct(&btGameOver, &sp_txtGameOver);
	Button_construct(&btRankBoard, &sp_scorePane);
	Button_construct(&btReplay, &sp_btPlay);
	Button_construct(&btRank, &sp_btRank);

	PhysicEngine_setBirdPos(PhysicEngine_pixelToRealCoord(130), PhysicEngine_pixelToRealCoord(210));
}

static void _SceneManager_fadeOut(SceneManager* o, bool isBlack, int speed)
{
	o->isFading = true;
	o->fadeAlpha = 0;
	o->fadeStep = 255 / o->fps * speed;
	o->fadeColor = isBlack ? &sp_black : &sp_white;
	while (o->fadeAlpha != 255)
	{
		SceneManager_render(o);
	}
}

static void _SceneManager_fadeIn(SceneManager* o, bool isBlack, int speed)
{
	o->isFading = true;
	o->fadeAlpha = 255;
	o->fadeStep = -255 / o->fps * speed;
	o->fadeColor = isBlack ? &sp_black : &sp_white;
	while (o->fadeAlpha != 0)
	{
		SceneManager_render(o);
	}
}

void _SceneManager_drawBird(SceneManager* o)
{
	static int framePassed = 0;
	Image* bird = NULL;
	int interval = o->fps / 8;
	if (pe_bird.isDead)
		bird = BirdAnimation_step(g_birdColor, false);
	else
		bird = BirdAnimation_step(g_birdColor, framePassed % interval == 0);

	SceneManager_rotateBird(o, bird);
	framePassed = framePassed++ % interval;
}

void _SceneManager_changeToGameOver(SceneManager* o)
{
	// 初始化按钮
	btGameOver.alpha = 0;
	btRankBoard.alpha = 0;
	btReplay.alpha = 0;
	btRank.alpha = 0;
	// 首先播放小鸟被撞的特效
	_SceneManager_fadeOut(o, false, 5);
	_SceneManager_fadeIn(o, false, 5);
	o->isFading = false;
	// 等待小鸟落地
	while (!pe_bird.isStopped)
		SceneManager_render(o);
	// 显示 Game Over
	btGameOver.cx = 144;
	btGameOver.cy = 110;
	btGameOver.alpha = 255;
	while (btGameOver.cy < 150)
	{
		if (SceneManager_render(o))
		{
			btGameOver.cy += 10;
		}
	}
	while (btGameOver.cy > 140)
	{
		if (SceneManager_render(o))
		{
			btGameOver.cy -= 2;
		}
	}
	while (btGameOver.cy < 150)
	{
		if (SceneManager_render(o))
		{
			btGameOver.cy += 2;
		}
	}
	// 绘制排分榜
	btRankBoard.alpha = 255;
	btRankBoard.cx = 144;
	btRankBoard.cy = SCENE_HEIGHT + btRankBoard.btImage->height / 2 + 2;
	while (btRankBoard.cy > 270)
	{
		if (SceneManager_render(o))
		{
			btRankBoard.cy -= 20;
		}
	}
	// 最后显示按钮
	btReplay.alpha = 255;
	btReplay.cx = 144 - btReplay.btImage->width / 2 - 10;
	btReplay.cy = 371;
	btRank.alpha = 255;
	btRank.cx = 144 + btReplay.btImage->width / 2 + 10;
	btRank.cy = 371;
}

void _SceneManager_tick(SceneManager* o)
{
	static int lastCount = 0;
	if (lastCount++ % o->fps / 20 == 0)
	{
		lastCount = 1;
		switch (o->sceneType)
		{
		case SceneType_mainMenu:
		case SceneType_prepare:
			PhysicEngine_float(1);
			break;
		case SceneType_playing:
			PhysicEngine_tick(1);
			o->nowScore += PhysicEngine_passedPipe() ? 1 : 0;
			if (pe_bird.isDead)
			{
				o->sceneType = SceneType_gameOver;
				o->highScore = o->highScore > o->nowScore ? o->highScore : o->nowScore;
				_SceneManager_changeToGameOver(o);
			}
			break;
		case SceneType_gameOver:
			PhysicEngine_dieTick(1);
			break;
		default:
			break;
		}
	}
}

void _SceneManager_drawMainMenu(SceneManager* o)
{
	Image* background = g_dayNight == DayNightMode_day ? &sp_dayBackground : &sp_nightBackground;
	ImageManager_drawSpiritToHdc(&g_imgMgr, background, o->bufHdc, 0, 0);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_txtFlappyBird, o->bufHdc, 50, 120);
	_SceneManager_drawBird(o);
	GroundAnimation_step(g_imgMgr.imgHdc, o->bufHdc, o->fps, true);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_txtCopyright, o->bufHdc, 70, 425);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_btPlay, o->bufHdc, 20, 342);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_btRank, o->bufHdc, 160, 342);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_btRate, o->bufHdc, 115, 260);
}

void _SceneManager_drawPrepare(SceneManager* o)
{
	Image* background = g_dayNight == DayNightMode_day ? &sp_dayBackground : &sp_nightBackground;
	ImageManager_drawSpiritToHdc(&g_imgMgr, background, o->bufHdc, 0, 0);
	ImageManager_drawNumber(&g_imgMgr, 0, o->bufHdc, 144, 80, DrawNumberSize_large, DrawNumberAlign_center);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_txtGetReady, o->bufHdc, 45, 120);
	ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_help, o->bufHdc, 80, 200);
	_SceneManager_drawBird(o);
	GroundAnimation_step(g_imgMgr.imgHdc, o->bufHdc, o->fps, true);
}

void _SceneManager_drawPlaying(SceneManager* o)
{
	Image* background = g_dayNight == DayNightMode_day ? &sp_dayBackground : &sp_nightBackground;
	ImageManager_drawSpiritToHdc(&g_imgMgr, background, o->bufHdc, 0, 0);
	// 绘制管道
	for (size_t i = 0; i < sizeof(pe_pipes) / sizeof(PEPipeObject); ++i)
	{
		PEPipeObject* pipe = &pe_pipes[i];
		ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_greenPipeUp, o->bufHdc,
			PhysicEngine_realToPixelCoord(pipe->cx) - sp_greenPipeUp.width / 2, 
			PhysicEngine_realToPixelCoord(pipe->cyUp) - sp_greenPipeUp.height);
		ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_greenPipeDown, o->bufHdc,
			PhysicEngine_realToPixelCoord(pipe->cx) - sp_greenPipeDown.width / 2,
			PhysicEngine_realToPixelCoord(pipe->cyDown));
	}
	ImageManager_drawNumber(&g_imgMgr, o->nowScore, o->bufHdc, 144, 80, DrawNumberSize_large, DrawNumberAlign_center);
	_SceneManager_drawBird(o);
	GroundAnimation_step(g_imgMgr.imgHdc, o->bufHdc, o->fps, true);
}

void _SceneManager_drawGameOver(SceneManager* o)
{
	Image* background = g_dayNight == DayNightMode_day ? &sp_dayBackground : &sp_nightBackground;
	ImageManager_drawSpiritToHdc(&g_imgMgr, background, o->bufHdc, 0, 0);
	// 绘制管道
	for (size_t i = 0; i < sizeof(pe_pipes) / sizeof(PEPipeObject); ++i)
	{
		PEPipeObject* pipe = &pe_pipes[i];
		ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_greenPipeUp, o->bufHdc,
			PhysicEngine_realToPixelCoord(pipe->cx) - sp_greenPipeUp.width / 2,
			PhysicEngine_realToPixelCoord(pipe->cyUp) - sp_greenPipeUp.height);
		ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_greenPipeDown, o->bufHdc,
			PhysicEngine_realToPixelCoord(pipe->cx) - sp_greenPipeDown.width / 2,
			PhysicEngine_realToPixelCoord(pipe->cyDown));
	}
	if (btGameOver.alpha == 0)
	{
		ImageManager_drawNumber(&g_imgMgr, o->nowScore, o->bufHdc, 144, 80, DrawNumberSize_large, DrawNumberAlign_center);
	}
	else
	{
		ImageManager_drawSpiritToHdc(&g_imgMgr, btGameOver.btImage, o->bufHdc,
			btGameOver.cx - btGameOver.btImage->width / 2,
			btGameOver.cy - btGameOver.btImage->height / 2);
	}
	GroundAnimation_step(g_imgMgr.imgHdc, o->bufHdc, o->fps, false);
	if (btRankBoard.alpha > 0)
	{
		ImageManager_drawSpiritToHdc(&g_imgMgr, btRankBoard.btImage, o->bufHdc,
			btRankBoard.cx - btRankBoard.btImage->width / 2,
			btRankBoard.cy - btRankBoard.btImage->height / 2);
		ImageManager_drawNumber(&g_imgMgr, o->nowScore, o->bufHdc,
			btRankBoard.cx + 90, btRankBoard.cy - 15,
			DrawNumberSize_middle, DrawNumberAlign_right);
		ImageManager_drawNumber(&g_imgMgr, o->highScore, o->bufHdc,
			btRankBoard.cx + 90, btRankBoard.cy + 28,
			DrawNumberSize_middle, DrawNumberAlign_right);
		ImageManager_drawSpiritToHdc(&g_imgMgr, &sp_goldenMedal, o->bufHdc,
			btRankBoard.cx - 88, btRankBoard.cy - 14);
	}
	_SceneManager_drawBird(o);
	if (btReplay.alpha != 0)
	{
		ImageManager_drawSpiritToHdc(&g_imgMgr, btReplay.btImage, o->bufHdc,
			btReplay.cx - btReplay.btImage->width / 2,
			btReplay.cy - btReplay.btImage->height / 2);
		ImageManager_drawSpiritToHdc(&g_imgMgr, btRank.btImage, o->bufHdc,
			btRank.cx - btRank.btImage->width / 2,
			btRank.cy - btRank.btImage->height / 2);
	}
}

bool SceneManager_render(SceneManager* o)
{
	if (!_SceneManager_needReDraw(o))
		return false;

	o->drawCounter++;

	_SceneManager_tick(o);

	switch (o->sceneType)
	{
	case SceneType_mainMenu:
		_SceneManager_drawMainMenu(o);
		break;
	case SceneType_prepare:
		_SceneManager_drawPrepare(o);
		break;
	case SceneType_playing:
		_SceneManager_drawPlaying(o);
		break;
	case SceneType_gameOver:
		_SceneManager_drawGameOver(o);
		break;
	default:
		break;
	}

	// 绘制帧速率
	if (o->showFps)
		_SceneManager_drawFps(o);

	// 处理渐变动画
	if (o->isFading)
	{
		o->fadeAlpha += o->fadeStep;
		if (o->fadeAlpha < 0)
			o->fadeAlpha = 0;
		if (o->fadeAlpha > 255)
			o->fadeAlpha = 255;
		ImageManager_alphaBlend(&g_imgMgr, o->fadeColor, o->bufHdc, 0, 0, SCENE_WIDTH, SCENE_HEIGHT, o->fadeAlpha);
	}

	// 翻转缓存图像到前台显示
	StretchBlt(o->scrHdc, 0, 0, o->windowWidth, o->windowHeight, o->bufHdc, 0, 0, SCENE_WIDTH, SCENE_HEIGHT, SRCCOPY);
	return true;
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

static void _MainMenu_onClick(SceneManager* o, int x, int y)
{
	if (Spirit_isPointInMe(&sp_btPlay, x, y, 20, 342))
	{
		_SceneManager_fadeOut(o, true, 2);
		o->sceneType = SceneType_prepare;
		_SceneManager_randomBackAndBird();
		PhysicEngine_setBirdPos(PhysicEngine_pixelToRealCoord(80), PhysicEngine_pixelToRealCoord(220));
		_SceneManager_fadeIn(o, true, 2);
		o->isFading = false;
	}
}

static void _Prepare_onClick(SceneManager* o, int x, int y)
{
	PhysicEngine_reset();
	o->sceneType = SceneType_playing;
	PhysicEngine_BirdFly();
}

static void _Playing_onClick(SceneManager* o, int x, int y)
{
	PhysicEngine_BirdFly();
}

static void _GameOver_onClick(SceneManager* o, int x, int y)
{
	if (btReplay.alpha == 0)
		return;

	if (Button_isHit(&btReplay, x, y))
	{
		_SceneManager_fadeOut(o, true, 2);
		o->sceneType = SceneType_prepare;
		o->nowScore = 0;
		_SceneManager_randomBackAndBird();
		PhysicEngine_setBirdPos(PhysicEngine_pixelToRealCoord(80), PhysicEngine_pixelToRealCoord(220));
		_SceneManager_fadeIn(o, true, 2);
		o->isFading = false;
	}
}

void SceneManager_onClick(SceneManager* o, int x, int y)
{
	switch (o->sceneType)
	{
	case SceneType_mainMenu:
		_MainMenu_onClick(o, x, y);
		break;
	case SceneType_prepare:
		_Prepare_onClick(o, x, y);
		break;
	case SceneType_playing:
		_Playing_onClick(o, x, y);
		break;
	case SceneType_gameOver:
		_GameOver_onClick(o, x, y);
		break;
	default:
		break;
	}
}

void SceneManager_rotateBird(SceneManager* o, Image* bird)
{
	_rotateHdc(o->bufHdc, pe_bird.angle, 
		PhysicEngine_realToPixelCoord(pe_bird.cx), 
		PhysicEngine_realToPixelCoord(pe_bird.cy));
	ImageManager_drawSpiritToHdc(&g_imgMgr, bird, o->bufHdc,
		PhysicEngine_realToPixelCoord(pe_bird.cx - pe_bird.width),
		PhysicEngine_realToPixelCoord(pe_bird.cy - pe_bird.height));
	_restoreHdc(o->bufHdc);
}
