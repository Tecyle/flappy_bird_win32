#include "stdafx.h"
#include "scene_manager.h"
#include "animation_manager.h"
#include "physic_engine.h"
#include "button.h"
#include "score_manager.h"
#include <math.h>
#include "sound_manager.h"

//////////////////////////////////////////////////////////////////////////
// 全局变量
Scene sce_mainMenu;
Scene sce_prepare;
Scene sce_playing;
Scene sce_gameOver;

SceneManager g_sceMgr;
LARGE_INTEGER g_counterFrequency;

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
	SpiritType_upPipes,
	SpiritType_downPipes,
	SpiritType_ground,
	SpiritType_largeScore,
	SpiritType_bird,
	SpiritType_whiteFade
};

// GameOverScene 里面需要绘制的内容
SpiritType sptGameOver[11] = {
	SpiritType_sky,
	SpiritType_upPipes,
	SpiritType_downPipes,
	SpiritType_ground,
	SpiritType_bird,
	SpiritType_txtGameOver,
	SpiritType_scoreBoard,
	SpiritType_btPlay,
	SpiritType_btRank,
	SpiritType_whiteFade,
	SpiritType_blackFade
};

// 所有的按钮
Button btMainMenuRate;
Button btMainMenuPlay;
Button btMainMenuRank;

Button btPrepareStart;

Button btPlayFly;

Button btGameOverReplay;
Button btGameOverRank;

// 为了便于管理按钮创建的按钮指针数组
Button* btInMainMenu[3] = {
	&btMainMenuRate,
	&btMainMenuPlay,
	&btMainMenuRank
};

Button* btInPrepare[1] = {
	&btPrepareStart
};

Button* btInPlay[1] = {
	&btPlayFly
};

Button* btInGameOver[2] = {
	&btGameOverReplay,
	&btGameOverRank
};

void _SceneManager_fadeOut(bool isBlack, long during);
void _SceneManager_fadeIn(bool isBlack, long during);

//////////////////////////////////////////////////////////////////////////
// Scene
void _Scene_initMainMenu()
{
	// 当切换到主界面时，需要进行下面的初始化
	// 1. 随机选择天空和小鸟
	// 2. 设置小鸟的位置和动画
	// 3. 更新物理引擎设置
	ImageManager_randomSkyAndBird();
	PhysicEngine_floatingBird();
	PhysicEngine_showPipes(false);
	PhysicEngine_movingPipes(false);
	PhysicEngine_movingGround(true);
}

void _Scene_initPrepare()
{
	ImageManager_randomSkyAndBird();
	PhysicEngine_prepareBird();
	PhysicEngine_movingPipes(false);
	PhysicEngine_showPipes(false);
	PhysicEngine_movingGround(true);
	ScoreManager_resetCurrentScore();
}

void _Scene_initPlaying()
{
	PhysicEngine_freeBird();
	PhysicEngine_showPipes(true);
	PhysicEngine_movingPipes(true);
}

void _Scene_initGameOver()
{
	Spirit* txtGameOver = ImageManager_getSpirit(SpiritType_txtGameOver);
	Spirit* scoreBoard = ImageManager_getSpirit(SpiritType_scoreBoard);
	Spirit* btReplay = ImageManager_getSpirit(SpiritType_btPlay);
	Spirit* btRank = ImageManager_getSpirit(SpiritType_btRank);
	txtGameOver->visiable = false;
	scoreBoard->visiable = false;
	btReplay->visiable = false;
	btRank->visiable = false;
	PhysicEngine_movingPipes(false);
	PhysicEngine_movingGround(false);
}

void Scene_draw(Scene* o)
{
	for (size_t i = 0; i < o->spiritCount; ++i)
	{
		ImageManager_drawSpirit(o->spirits[i]);
	}
}

void Scene_init(SceneType scene)
{
	switch (scene)
	{
	case SceneType_mainMenu:
		_Scene_initMainMenu();
		break;
	case SceneType_prepare:
		_Scene_initPrepare();
		break;
	case SceneType_playing:
		_Scene_initPlaying();
		break;
	case SceneType_gameOver:
		_Scene_initGameOver();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Button onClick Functions
void MainMenuRate_click()
{
	// undefined
}

void MainMenuPlay_click()
{
	SoundManager_playSound(SoundType_swooshing);
	_SceneManager_fadeOut(true, 500);
	g_sceMgr.currentScene = SceneType_prepare;
	Scene_init(SceneType_prepare);
	_SceneManager_fadeIn(true, 500);
}

void MainMenuRank_click()
{
	// undefined
}

void PlayFly_click()
{
	SoundManager_playSound(SoundType_wing);
	PhysicEngine_birdFly();
}

void PrepareStart_click()
{
	g_sceMgr.currentScene = SceneType_playing;
	Scene_init(SceneType_playing);
	PlayFly_click();
}

void GameOverReplay_click()
{
	SoundManager_playSound(SoundType_swooshing);
	_SceneManager_fadeOut(true, 500);
	g_sceMgr.currentScene = SceneType_prepare;
	Scene_init(SceneType_prepare);
	_SceneManager_fadeIn(true, 500);
}

void GameOverRank_click()
{
	// undefined
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

// 此函数获取的帧速率应该是实时统计的结果
int _SceneManager_getFps()
{
	SceneManager* o = &g_sceMgr;
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

void _SceneManager_drawFps()
{
	SceneManager* o = &g_sceMgr;
	char strFps[64] = { "FPS: " };
	_itoa_s(_SceneManager_getFps(), &strFps[5], 58, 10);
	TextOutA(o->bufHdc, 5, 5, strFps, strlen(strFps));
}

void _SceneManager_switchToGameOver()
{
	SceneManager* o = &g_sceMgr;
	_SceneManager_fadeOut(false, 250);
	Scene_init(SceneType_gameOver);
	o->currentScene = SceneType_gameOver;
	_SceneManager_fadeIn(false, 250);
	while (!PhysicEngine_isBirdStopped())
	{
		SceneManager_render();
	}
	// 菜单进入动画
	// GameOver
	Spirit* txtGameOver = ImageManager_getSpirit(SpiritType_txtGameOver);
	Animation* transAnimation = txtGameOver->ani;
	TransAnimation_init(transAnimation, 144, 110, 144, 150, 200, false);
	txtGameOver->visiable = true;
	while (!TransAnimation_finished(transAnimation))
	{
		SceneManager_render();
	}
	TransAnimation_init(transAnimation, 144, 150, 144, 140, 200, false);
	while (!TransAnimation_finished(transAnimation))
	{
		SceneManager_render();
	}
	TransAnimation_init(transAnimation, 144, 140, 144, 150, 200, false);
	while (!TransAnimation_finished(transAnimation))
	{
		SceneManager_render();
	}
	// 计分板
	Spirit* scoreBoard = ImageManager_getSpirit(SpiritType_scoreBoard);
	transAnimation = scoreBoard->ani;
	scoreBoard->visiable = true;
	TransAnimation_init(transAnimation, 144, 571, 144, 270, 500, false);
	SoundManager_playSound(SoundType_swooshing);
	while (!TransAnimation_finished(transAnimation))
	{
		SceneManager_render();
	}
	// 显示按钮
	Spirit* btReplay = ImageManager_getSpirit(SpiritType_btPlay);
	btReplay->visiable = true;
	Spirit* btRank = ImageManager_getSpirit(SpiritType_btRank);
	btRank->visiable = true;
}

void _SceneManager_tick()
{
	SceneManager* o = &g_sceMgr;
	// 执行游戏逻辑
	PhysicEngine_tick();
	// 更新动画
	AnimationManager_tick();
	// 执行小鸟计分
	if (o->currentScene == SceneType_playing)
	{
		if (PhysicEngine_passedPipe())
		{
			ScoreManager_increaseScore();
			SoundManager_playSound(SoundType_point);
		}
	}
	// 执行游戏结束判断，为了防止间接递归调用，通过 onSwitching 进行标记
	if (!o->onSwitching && o->currentScene == SceneType_playing && PhysicEngine_isBirdDead())
	{
		o->onSwitching = true;
		SoundManager_playSound(SoundType_hit);
		SoundManager_playSound(SoundType_die);
		_SceneManager_switchToGameOver();
		o->onSwitching = false;
	}
}

void _SceneManager_initAllScene()
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

void _SceneManager_checkOnClickButtons(Button** buttons, size_t btNum, int x, int y)
{
	for (size_t i = 0; i < btNum; ++i)
	{
		Button* bt = buttons[i];
		if (bt->enabled && Button_isHit(bt, x, y))
		{
			bt->onClick();
			return;
		}
	}
}

void _SceneManager_fadeOut(bool isBlack, long during)
{
	Spirit* fadeCover = ImageManager_getSpirit(isBlack ? SpiritType_blackFade : SpiritType_whiteFade);
	Animation* fadeAnimation = fadeCover->ani;
	
	FadeAnimation_init(fadeAnimation, 0, 255, during, false);

	while (!FadeAnimation_finished(fadeAnimation))
	{
		SceneManager_render();
	}
}

void _SceneManager_fadeIn(bool isBlack, long during)
{
	Spirit* fadeCover = ImageManager_getSpirit(isBlack ? SpiritType_blackFade : SpiritType_whiteFade);
	Animation* fadeAnimation = fadeCover->ani;

	FadeAnimation_init(fadeAnimation, 255, 0, during, false);

	while (!FadeAnimation_finished(fadeAnimation))
	{
		SceneManager_render();
	}
}

void SceneManager_init(HINSTANCE hInstance, HDC hdc)
{
	// 初始化 SceneManager 对象
	SceneManager* o = &g_sceMgr;
	o->currentScene = SceneType_mainMenu;
	o->scrHdc = hdc;
	o->bufHdc = CreateCompatibleDC(hdc);
	o->drawingBoard = CreateCompatibleBitmap(hdc, SCENE_WIDTH, SCENE_HEIGHT);
	SelectObject(o->bufHdc, o->drawingBoard);
	SetTextColor(o->bufHdc, RGB(255, 0, 0));
	SetBkColor(o->bufHdc, RGB(0, 0, 0));
	SetBkMode(o->bufHdc, TRANSPARENT);
	o->fps = 60;
	o->drawCounter = 0;
	o->showFps = true;
	o->isFading = false;
	o->onSwitching = false;
	QueryPerformanceFrequency(&g_counterFrequency);
	// 初始化游戏需要的各个部件，包括：
	ScoreManager_init();							///< 初始化计分管理器
	AnimationManager_init();						///< 初始化动画引擎
	ImageManager_initAll(hInstance, o->bufHdc);		///< 初始化图像资源管理器
	PhysicEngine_init();							///< 初始化物理引擎
	SoundManager_init(hInstance);
	// 初始化场景
	_SceneManager_initAllScene();
	// 初始化按钮
	Spirit* btPlay = ImageManager_getSpirit(SpiritType_btPlay);
	Button_construct(&btMainMenuRate, 0, 0, 0, 0, MainMenuRate_click);
	Button_construct(&btMainMenuPlay, btPlay->cx - btPlay->halfWidth, btPlay->cx + btPlay->halfWidth, btPlay->cy - btPlay->halfHeight, btPlay->cy + btPlay->halfHeight, MainMenuPlay_click);
	Button_construct(&btMainMenuRank, 0, 0, 0, 0, MainMenuRank_click);

	Button_construct(&btPrepareStart, 0, SCENE_WIDTH, 0, SCENE_HEIGHT, PrepareStart_click);
	
	Button_construct(&btPlayFly, 0, SCENE_WIDTH, 0, SCENE_HEIGHT, PlayFly_click);
	
	Button_construct(&btGameOverReplay, btPlay->cx - btPlay->halfWidth, btPlay->cx + btPlay->halfWidth, btPlay->cy - btPlay->halfHeight, btPlay->cy + btPlay->halfHeight, GameOverReplay_click);
	Button_construct(&btGameOverRank, 0, 0, 0, 0, GameOverRank_click);
	// 初始化需要显示的第一个场景
	_Scene_initMainMenu();
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
	// 有物体的新位置，更新动画引擎，更新所有动画
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

void SceneManager_setViewSize(int width, int height)
{
	SceneManager* o = &g_sceMgr;
	o->windowWidth = width;
	o->windowHeight = height;
}

void SceneManager_onClick(int x, int y)
{
	SceneManager* o = &g_sceMgr;
	switch (o->currentScene)
	{
	case SceneType_mainMenu:
		_SceneManager_checkOnClickButtons(btInMainMenu, 3, x, y);
		break;
	case SceneType_prepare:
		_SceneManager_checkOnClickButtons(btInPrepare, 1, x, y);
		break;
	case SceneType_playing:
		_SceneManager_checkOnClickButtons(btInPlay, 1, x, y);
		break;
	case SceneType_gameOver:
		_SceneManager_checkOnClickButtons(btInGameOver, 2, x, y);
		break;
	}
}
