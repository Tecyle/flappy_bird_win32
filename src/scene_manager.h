#pragma once
#include "image_manager.h"

// ������Ϸ��������
// ��Ϸһ��ֻ������ 4 ��������
// 1. ���˵�
// 2. ׼����ʼ����
// 3. ��Ϸ����
// 4. ��Ϸ��������
typedef enum SceneType
{
	SceneType_mainMenu,		///< ���˵�
	SceneType_prepare,		///< ׼����ʼ����
	SceneType_playing,		///< ��Ϸ����
	SceneType_gameOver		///< ��Ϸ��������
} SceneType;

// ������Ϸ����
// ��Ϸ����ָ����һ����������Ҫ������Щ����
// ��������һ������(Spirit)������
typedef struct Scene 
{
	SpiritType* spirits;
	size_t spiritCount;
} Scene;

// ���������࣬������Ϸ�ĳ������л��������� main ������Ψһ�ӿ�
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
	��ʼ����������������������Ϸ��ʼ֮ǰִ��
	
	hdc : ���ڵĻ�ͼ�����

	Ϊ�˱��⶯̬�ڴ��ʹ�ã�������ʹ�õ�����ȫ�Ǿ�̬���飬
	����û������ CleanUp �Ĺ��ܡ�
*/
void SceneManager_init(HINSTANCE hInstance, HWND hWnd, HDC hdc);

void SceneManager_destruct();

/*
	������Ϸ����

	�����ǰ���ÿ�� 60 ֡���ٶȽ��еģ�Ҳ����˵��һ���ڵ���
	�ú�����ǧ�Σ��ú���Ҳֻ������һ����ֻ���� 60 �Ρ�
	return ���� true �����ε��û�����ͼ�Σ����� false 
	�������ǿյ��á�
*/
bool SceneManager_render();

/*
	������ͼ�Ĵ�С

	��ͼ�Ĵ�СҲ���������û������Ļ�ͼ����Ĵ�С������������
	�����ġ�
*/
void SceneManager_setViewSize(int width, int height);

/*
	��Ӧ��Ϸ�ĵ����¼�

	����Ϸ�û�������ֻ�е����¼���ͨ�������������Ӧ��ͬ����
	�µĵ����¼���
*/
void SceneManager_onClick(int x, int y);
