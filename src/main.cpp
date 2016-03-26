#include "stdafx.h"
#include "image_manager.h"

ImageManager g_imgMgr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 就好像命令行程序是从 main 函数开始的一样，Windows 程序是从 WinMain 函数开始的
// 一旦 WinMain 函数退出了，那么 Windows 程序也就结束了
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 创建一个窗口，首先需要填写一些关于这个窗口的样式信息
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	wndClass.style = CS_HREDRAW | CS_VREDRAW;		// 指定当水平或垂直方向长度改变时重绘窗口
	wndClass.lpfnWndProc = WndProc;					// 窗口收到消息时候处理消息的回调函数
	wndClass.hInstance = hInstance;					// 设置当前窗体属于哪个程序
	wndClass.hIcon = NULL;							// 窗口的图标，暂时设置为无图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"FlappyBird";
	wndClass.hIconSm = NULL;
	wndClass.cbSize = sizeof(wndClass);

	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, L"注册窗口类失败！", L"Flappy Bird", MB_ICONERROR);
		return -1;
	}

	HWND hWnd = CreateWindow(L"FlappyBird", L"Flappy Bird", WS_OVERLAPPED | WS_SYSMENU, 
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"创建窗口失败！", L"Flappy Bird", MB_ICONERROR);
		return -1;
	}

	// 初始化绘图对象
	HDC hdc = GetWindowDC(hWnd);
	ImageManager_construct(&g_imgMgr, hInstance);
	ImageManager_loadScenes(&g_imgMgr);
	ImageManager_initAllSpirits(&g_imgMgr, hdc);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// 消息循环
	MSG msg;
	for (;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImageManager_drawScene(&g_imgMgr, hdc);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(NULL, L"是否结束当前游戏？", L"Flappy Bird", MB_ICONQUESTION | MB_YESNO) == IDYES)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
