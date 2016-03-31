#pragma once

// �����ͼ�ĵ�Ԫ �� ����
// ���ڼ��ؽ�����ͼ����һ�Ŵ�ͼ�񣬶�ͼ���ϵ�ĳЩ����
// ����������Ҫ���ƵĶ��󣬾��������������ʶ��������
typedef struct Spirit
{
	int x;
	int y;
	int width;
	int height;
} Spirit;

// ����
// ������ʵ������һϵ�еľ���ͼ����ɵ����飬��Щͼ��
// ����һ����ʱ���������л����г��˶���
typedef struct Animation
{
	size_t frameNum;
	Spirit* frames;
	size_t interval;
} Animation;

typedef struct ImageManager
{
	HINSTANCE hInstance;
	HBITMAP imgScene;
	HDC imgHdc;
} ImageManager;

void ImageManager_construct(ImageManager* o, HINSTANCE hInstance);

void ImageManager_initAllSpirits(ImageManager* o, HDC hdc);
bool ImageManager_loadScenes(ImageManager* o);
void ImageManager_drawSpiritToHdc(ImageManager* o, Spirit* sp, HDC hdc, int dx, int dy);
void ImageManager_alphaBlend(ImageManager* o, Spirit* sp, HDC hdc, int dx, int dy, int width, int height, BYTE alpha);
typedef enum DrawNumberSize
{
	DrawNumberSize_large,
	DrawNumberSize_middle,
	DrawNumberSize_small
} DrawNumberSize;
void ImageManager_drawNumber(ImageManager* o, size_t num, HDC hdcDst, int cx, int cy, DrawNumberSize numberSize);

bool Spirit_isPointInMe(Spirit* o, int x, int y, int dx, int dy);

// ���ڱ���Ϸ���ԣ����õľ����ܹ�����ôһЩ�����Ծ�ȫ
// ��ö�ٳ�����
extern Spirit sp_dayBackground;
extern Spirit sp_nightBackground;
extern Spirit sp_ground;
extern Spirit sp_txtGetReady;
extern Spirit sp_txtGameOver;
extern Spirit sp_txtFlappyBird;
extern Spirit sp_txtCopyright;
extern Spirit sp_smallNum[10];
extern Spirit sp_middleNum[10];
extern Spirit sp_largeNum[10];
extern Spirit sp_scorePane;
extern Spirit sp_sliverMedal;
extern Spirit sp_goldenMedal;
extern Spirit sp_copperMedal;
extern Spirit sp_help;
extern Spirit sp_orangePipeUp;
extern Spirit sp_orangePipeDown;
extern Spirit sp_greenPipeUp;
extern Spirit sp_greenPipeDown;
extern Spirit sp_yellowBird[3];
extern Spirit sp_redBird[3];
extern Spirit sp_blueBird[3];
extern Spirit sp_btPlay;
extern Spirit sp_btRank;
extern Spirit sp_btRate;
extern Spirit sp_btContinue;
extern Spirit sp_btPause;
extern Spirit sp_btShare;
extern Spirit sp_btMenu;
extern Spirit sp_btOk;
extern Spirit sp_new;
extern Spirit sp_black;
extern Spirit sp_white;