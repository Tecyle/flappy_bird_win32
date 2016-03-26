#include "stdafx.h"
#include "image_manager.h"

ImageManager g_imgMgr;

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

	HWND hWnd = CreateWindow(L"FlappyBird", L"Flappy Bird", WS_OVERLAPPED | WS_SYSMENU, 
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"��������ʧ�ܣ�", L"Flappy Bird", MB_ICONERROR);
		return -1;
	}

	// ��ʼ����ͼ����
	HDC hdc = GetWindowDC(hWnd);
	ImageManager_construct(&g_imgMgr, hInstance);
	ImageManager_loadScenes(&g_imgMgr);
	ImageManager_initAllSpirits(&g_imgMgr, hdc);

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

		ImageManager_drawScene(&g_imgMgr, hdc);
	}

	return (int)msg.wParam;
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
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
