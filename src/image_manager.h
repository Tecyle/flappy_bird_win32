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
	int halfWidth;		///< ����Ŀ�ȵ�һ��
	int halfHeight;		///< ����ĸ߶ȵ�һ��
	bool visiable;		///< �����Ƿ�ɼ�
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
void ImageManager_drawFadeCover(bool isBlack, BYTE alpha);
void ImageManager_drawNumber(size_t num, int cx, int cy, NumberSize size, NumberAlign align);


