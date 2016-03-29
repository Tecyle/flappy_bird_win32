#include "stdafx.h"
#include "scene_manager.h"

static SceneManager g_sceneMgr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �ͺ��������г����Ǵ� main ������ʼ��һ����Windows �����Ǵ� WinMain ������ʼ��
// һ�� WinMain �����˳��ˣ���ô Windows ����Ҳ�ͽ�����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// ����һ�����ڣ�������Ҫ��дһЩ����������ڵ���ʽ��Ϣ
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	wndClass.style = CS_HREDRAW | CS_VREDRAW;		// ָ����ˮƽ��ֱ���򳤶ȸı�ʱ�ػ洰��
	wndClass.lpfnWndProc = WndProc;					// �����յ���Ϣʱ������Ϣ�Ļص�����
	wndClass.hInstance = hInstance;					// ���õ�ǰ���������ĸ�����
	wndClass.hIcon = NULL;							// ���ڵ�ͼ�꣬��ʱ����Ϊ��ͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"FlappyBird";
	wndClass.hIconSm = NULL;
	wndClass.cbSize = sizeof(wndClass);

	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(NULL, L"ע�ᴰ����ʧ�ܣ�", L"Flappy Bird", MB_ICONERROR);
		return -1;
	}

	int hCaption = GetSystemMetrics(SM_CYCAPTION);
	int hBorder = GetSystemMetrics(SM_CYSIZEFRAME);
	int wBorder = GetSystemMetrics(SM_CXSIZEFRAME);
	int hScreen = GetSystemMetrics(SM_CYSCREEN);
	int wScreen = GetSystemMetrics(SM_CXSCREEN);

	int wWindow = 3 * (SCENE_WIDTH + 2 * wBorder) / 2;
	int hWindow = 3 * (SCENE_HEIGHT + hCaption + 2 * hBorder) / 2;
	int xWindow = (wScreen - wWindow) / 2;
	int yWindow = (hScreen - hWindow) / 2;

	HWND hWnd = CreateWindow(L"FlappyBird", L"Flappy Bird", WS_OVERLAPPED | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX, 
		xWindow, yWindow, wWindow, hWindow, 
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"��������ʧ�ܣ�", L"Flappy Bird", MB_ICONERROR);
		return -1;
	}

	// ��ʼ����ͼ����
	HDC hdc = GetDC(hWnd);
	RECT windowRect;
	GetClientRect(hWnd, &windowRect);
	SceneManager_construct(&g_sceneMgr, hInstance, hdc);
	SceneManager_setViewSize(&g_sceneMgr, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��
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

		SceneManager_render(&g_sceneMgr);
	}

	return (int)msg.wParam;
}

void ClearMessageQueue()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(NULL, L"�Ƿ������ǰ��Ϸ��", L"Flappy Bird", MB_ICONQUESTION | MB_YESNO) == IDYES)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		RECT windowRect;
		GetClientRect(hWnd, &windowRect);
		SceneManager_setViewSize(&g_sceneMgr, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
		SceneManager_render(&g_sceneMgr);
	}
		break;
	case WM_LBUTTONUP:
	{
		RECT windowRect;
		GetClientRect(hWnd, &windowRect);
		int rx = (int)((double)LOWORD(lParam) * (double)SCENE_WIDTH / (double)(windowRect.right - windowRect.left));
		int ry = (int)((double)HIWORD(lParam) * (double)SCENE_HEIGHT / (double)(windowRect.bottom - windowRect.top));
		SceneManager_onClick(&g_sceneMgr, rx, ry);
		ClearMessageQueue();
	}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
