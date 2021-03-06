#pragma once
#include "image_manager.h"

// 定义游戏场景类型
// 游戏一共只有以下 4 个场景：
// 1. 主菜单
// 2. 准备开始界面
// 3. 游戏界面
// 4. 游戏结束界面
typedef enum SceneType
{
	SceneType_mainMenu,		///< 主菜单
	SceneType_prepare,		///< 准备开始界面
	SceneType_playing,		///< 游戏界面
	SceneType_gameOver		///< 游戏结束界面
} SceneType;

// 定义游戏场景
// 游戏场景指明了一个场景中需要绘制哪些东西
// 本质上是一个精灵(Spirit)的数组
typedef struct Scene 
{
	SpiritType* spirits;
	size_t spiritCount;
} Scene;

// 场景控制类，控制游戏的场景及切换，是面向 main 函数的唯一接口
typedef struct SceneManager
{
	SceneType currentScene;
	HDC scrHdc;
	HDC bufHdc;
	HBITMAP drawingBoard;
	int fps;

	int windowWidth;
	int windowHeight;

	bool onSwitching;

	size_t drawCounter;
	bool showFps;
	bool isFading;
	int fadeAlpha;
	int fadeStep;
	Image* fadeColor;
	HINSTANCE hInstance;
	HWND hWnd;
} SceneManager;

/*
	初始化场景管理器，必须在游戏开始之前执行
	
	hdc : 窗口的绘图句柄。

	为了避免动态内存的使用，本程序使用的内容全是静态数组，
	所以没有类似 CleanUp 的功能。
*/
void SceneManager_init(HINSTANCE hInstance, HWND hWnd, HDC hdc);

void SceneManager_destruct();

/*
	绘制游戏画面

	绘制是按照每秒 60 帧的速度进行的，也就是说，一秒内调用
	该函数上千次，该函数也只会在这一秒内只绘制 60 次。
	return 返回 true 代表本次调用绘制了图形，返回 false 
	代表本次是空调用。
*/
bool SceneManager_render();

/*
	设置视图的大小

	视图的大小也就是最终用户看到的绘图区域的大小，可以是任意
	比例的。
*/
void SceneManager_setViewSize(int width, int height);

/*
	响应游戏的单击事件

	本游戏用户的输入只有单击事件，通过这个函数来响应不同场景
	下的单击事件。
*/
void SceneManager_onClick(int x, int y);
