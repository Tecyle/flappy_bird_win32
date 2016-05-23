#pragma once
#include "image_manager.h"

typedef enum SceneType
{
	SceneType_mainMenu,
	SceneType_prepare,
	SceneType_playing,
	SceneType_gameOver
} SceneType;

// 定义游戏场景
// 游戏场景指明了一个场景中需要绘制哪些东西
typedef struct Scene 
{
	SpiritType* spirits;
	size_t spiritCount;
} Scene;

typedef struct SceneManager
{
	SceneType currentScene;
	HDC scrHdc;
	HDC bufHdc;
	HBITMAP drawingBoard;
	int fps;

	int windowWidth;
	int windowHeight;

	size_t drawCounter;
	bool showFps;
	bool isFading;
	int fadeAlpha;
	int fadeStep;
	Image* fadeColor;

	size_t nowScore;
	size_t highScore;

	PhysicEngine physicEngine;
} SceneManager;

void SceneManager_initAllScene();
bool SceneManager_render();



void SceneManager_construct(SceneManager* o, HINSTANCE hInstance, HDC hdc);
void SceneManager_setViewSize(SceneManager* o, int width, int height);
void SceneManager_setFps(SceneManager* o, int fps);
int SceneManager_getFps(SceneManager* o);
void SceneManager_onClick(SceneManager* o, int x, int y);
void SceneManager_rotateBird(SceneManager* o, Image* bird);