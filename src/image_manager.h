#pragma once
//////////////////////////////////////////////////////////////////////////
// ImageManager
// ��������ͼƬ��Դ���ӹ���ģ�飬�ṩͼƬ�Ļ��ƹ���

// �������е�ö������
// ���Ƶ������ֺ�
typedef enum NumberSize
{
	NumberSize_large,		///< ����֣�������Ϸ��������ʾ��ǰ����
	NumberSize_middle,		///< �к��֣�������Ϸ��������ʾ����
	NumberSize_small		///< С���֣���������û���õ�
} NumberSize;

// ������Ϸ�����ֵĶ��䷽ʽ
typedef enum NumberAlign
{
	NumberAlign_left,		///< ����룬��Ϸ��û���õ����ֶ��䷽ʽ
	NumberAlign_center,		///< ���ж���
	NumberAlign_right		///< �Ҷ���
} NumberAlign;

// ������Ϸ�еĽ�ɫ����
typedef enum SpiritType
{
	SpiritType_ground,			///< ����
	SpiritType_sky,				///< �������
	SpiritType_bird,			///< С��
	SpiritType_upPipe,			///< ����Ĺܵ�
	SpiritType_downPipe,		///< ����Ĺܵ�
	SpiritType_txtGetReady,		///< GetReady ����
	SpiritType_txtGameOver,		///< GameOver ����
	SpiritType_helpInfo,		///< ������Ϣ
	SpiritType_btPlay,			///< ��ʼ��Ϸ��ť
	SpiritType_btRank,			///< ������ť
	SpiritType_btRate,			///< Rate ��ť
	SpiritType_scoreBoard		///< �Ƿ���
} SpiritType;

// ������ԴͼƬ
// ͼƬ�Ǵ��ⲿ���ص�һ���ŵ� bmp λͼ����������Ҫʹ�õ��ڲ���ֻ������ͼƬ��
// ĳ�������������ṹ������������������Ϣ��
typedef struct Image
{
	HDC imgHdc;		///< ��ͼ�õľ��
	int x;			///< ͼ���������� x ����
	int y;			///< ͼ���������� y ����
	int width;		///< ͼ���������Ŀ��
	int height;		///< ͼ���������ĸ߶�
} Image;
// ��ʼ�� Image ����
void Image_construct(Image* o, int x, int y, int width, int height);

// ��Ϸ�Ľ�ɫ
// ���˶�����ͼ�����⣬������������Ϸ�е�λ����Ϣ
typedef struct Spirit
{
	Image* image;		///< ����Ļ���ͼ��
	int cx;				///< ��������� x ����
	int cy;				///< ��������� y ����
	int width;			///< ����Ŀ��
	int height;			///< ����ĸ߶�
	bool visiable;		///< �����Ƿ�ɼ�
} Spirit;

// ������Ϸ�еİ�ť
typedef struct Button
{
	Spirit* imgButton;
};

// ͼ����Դ����ģ�飬�����������п����õ���ͼ��
typedef struct ImageManager
{
	HINSTANCE hInstance;	///< Ӧ�ó����ʵ�����
	HBITMAP imgScene;		///< ͼƬ��Դ������������ BMP
	HDC imgHdc;				///< ͼ����ƾ��
} ImageManager;

// ��ʼ�� ImageManager
void ImageManager_construct(HINSTANCE hInstance);
// �ͷ� ImageManager ��Դ
void ImageManager_destruct();

// ��ʼ�������õ���ͼ����Դ
void ImageManager_initAllImages(HDC hdc);
// ��������ȫ�ֵĴ�ͼƬ
bool ImageManager_loadScenes();
void ImageManager_drawSpiritToHdc(ImageManager* o, Image* sp, HDC hdc, int dx, int dy);
void ImageManager_alphaBlend(ImageManager* o, Image* sp, HDC hdc, int dx, int dy, int width, int height, BYTE alpha);

void ImageManager_drawNumber(ImageManager* o, size_t num, HDC hdcDst, int cx, int cy, DrawNumberSize numberSize, DrawNumberAlign align);

bool Spirit_isPointInMe(Image* o, int x, int y, int dx, int dy);

// ���ڱ���Ϸ���ԣ����õľ����ܹ�����ôһЩ�����Ծ�ȫ
// ��ö�ٳ�����
extern Image sp_dayBackground;
extern Image sp_nightBackground;
extern Image sp_ground;
extern Image sp_txtGetReady;
extern Image sp_txtGameOver;
extern Image sp_txtFlappyBird;
extern Image sp_txtCopyright;
extern Image sp_smallNum[10];
extern Image sp_middleNum[10];
extern Image sp_largeNum[10];
extern Image sp_scorePane;
extern Image sp_sliverMedal;
extern Image sp_goldenMedal;
extern Image sp_copperMedal;
extern Image sp_help;
extern Image sp_orangePipeUp;
extern Image sp_orangePipeDown;
extern Image sp_greenPipeUp;
extern Image sp_greenPipeDown;
extern Image sp_yellowBird[3];
extern Image sp_redBird[3];
extern Image sp_blueBird[3];
extern Image sp_btPlay;
extern Image sp_btRank;
extern Image sp_btRate;
extern Image sp_btContinue;
extern Image sp_btPause;
extern Image sp_btShare;
extern Image sp_btMenu;
extern Image sp_btOk;
extern Image sp_new;
extern Image sp_black;
extern Image sp_white;