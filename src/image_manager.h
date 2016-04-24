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
	SpiritType_ground = 0,			///< ����
	SpiritType_sky = 1,				///< �������
	SpiritType_bird = 2,			///< С��
	SpiritType_upPipes = 3,			///< �ϲ�ܵ���
	SpiritType_downPipes = 4,		///< �²�ܵ���
	SpiritType_txtGetReady = 5,		///< GetReady ����
	SpiritType_txtGameOver = 6,		///< GameOver ����
	SpiritType_txtCopyRight = 7,	///< ��Ȩ������Ϣ
	SpiritType_txtFlappyBird = 8,	///< FlappyBird ����
	SpiritType_largeScore = 9,		///< ��ǰ�÷�����
	SpiritType_helpInfo = 10,		///< ������Ϣ
	SpiritType_btPlay = 11,			///< ��ʼ��Ϸ��ť
	SpiritType_btRank = 12,			///< ������ť
	SpiritType_btRate = 13,			///< Rate ��ť
	SpiritType_scoreBoard = 14,		///< �Ƿ���
	SpiritType_blackFade = 15,		///< ��ɫ��������
	SpiritType_whiteFade = 16		///< ��ɫ��������
} SpiritType;

// ������ԴͼƬ
// ͼƬ�Ǵ��ⲿ���ص�һ���ŵ� bmp λͼ����������Ҫʹ�õ��ڲ���ֻ������ͼƬ��
// ĳ�������������ṹ������������������Ϣ��
typedef struct Image
{
	int x;			///< ͼ���������� x ����
	int y;			///< ͼ���������� y ����
	int width;		///< ͼ���������Ŀ��
	int height;		///< ͼ���������ĸ߶�
} Image;
// ��ʼ�� Image ����
void Image_construct(Image* o, int x, int y, int width, int height, HDC srcHdc);

// ��Ϸ�Ľ�ɫ
// ���˶�����ͼ�����⣬������������Ϸ�е�λ����Ϣ
typedef struct Spirit
{
	Image* image;		///< ����Ļ���ͼ��
	int cx;				///< ��������� x ����
	int cy;				///< ��������� y ����
	double angle;		///< �������ת�Ƕȣ�Ŀǰ��С����Ч
	int halfWidth;		///< ����Ŀ�ȵ�һ��
	int halfHeight;		///< ����ĸ߶ȵ�һ��
	bool visiable;		///< �����Ƿ�ɼ�
	Animation* ani;		///< ���ƾ���Ķ���
} Spirit;

void Spirit_construct(Spirit* o, Image* img, int cx, int cy);
void Spirit_draw(Spirit* o, HDC dstHdc);

// ������Ϸ�еİ�ť
typedef struct Button
{
	Spirit* imgButton;
};

// ͼ����Դ����ģ�飬�����������п����õ���ͼ��
typedef struct ImageManager
{
	HINSTANCE hInstance;	///< Ӧ�ó����ʵ�����
	HDC dstHdc;
	HBITMAP imgScene;		///< ͼƬ��Դ������������ BMP
	HDC imgHdc;				///< ͼ����ƾ��
} ImageManager;

// ��ʼ�� ImageManager
void ImageManager_construct(HINSTANCE hInstance, HDC dstHdc);
// �ͷ� ImageManager ��Դ
void ImageManager_destruct();

// ��ʼ�������õ���ͼ����Դ
bool ImageManager_initAll(HDC hdc);
void ImageManager_randomSkyAndBird();
void ImageManager_drawSpirit(SpiritType spirit);
void ImageManager_drawFadeCover(Image* img, BYTE alpha);
void ImageManager_drawNumber(size_t num, int cx, int cy, NumberSize size, NumberAlign align);


