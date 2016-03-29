#pragma once
#include "image_manager.h"

typedef enum SceneType
{
	SceneType_mainMenu,
	SceneType_prepare,
	SceneType_playing,
	SceneType_gameOver
} SceneType;

typedef struct SceneManager
{
	SceneType sceneType;
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
} SceneManager;

void SceneManager_construct(SceneManager* o, HINSTANCE hInstance, HDC hdc);
void SceneManager_setViewSize(SceneManager* o, int width, int height);
void SceneManager_setFps(SceneManager* o, int fps);
int SceneManager_getFps(SceneManager* o);
void SceneManager_render(SceneManager* o);
void SceneManager_fadeOut(SceneManager* o);
void SceneManager_fadeIn(SceneManager* o);
