#pragma once
//////////////////////////////////////////////////////////////////////////
// ImageManager
// 用来管理图片资源的子功能模块，提供图片的绘制功能

// 定义所有的枚举类型
// 绘制的数字字号
typedef enum NumberSize
{
	NumberSize_large,		///< 大号字，用于游戏过程中显示当前分数
	NumberSize_middle,		///< 中号字，用于游戏结束后显示分数
	NumberSize_small		///< 小号字，本程序中没有用到
} NumberSize;

// 定义游戏中数字的对其方式
typedef enum NumberAlign
{
	NumberAlign_left,		///< 左对齐，游戏中没有用到这种对其方式
	NumberAlign_center,		///< 居中对齐
	NumberAlign_right		///< 右对齐
} NumberAlign;

// 定义游戏中的角色代号
typedef enum SpiritType
{
	SpiritType_ground,			///< 地面
	SpiritType_sky,				///< 背景天空
	SpiritType_bird,			///< 小鸟
	SpiritType_upPipe,			///< 上面的管道
	SpiritType_downPipe,		///< 下面的管道
	SpiritType_txtGetReady,		///< GetReady 文字
	SpiritType_txtGameOver,		///< GameOver 文字
	SpiritType_helpInfo,		///< 帮助信息
	SpiritType_btPlay,			///< 开始游戏按钮
	SpiritType_btRank,			///< 排名按钮
	SpiritType_btRate,			///< Rate 按钮
	SpiritType_scoreBoard		///< 记分牌
} SpiritType;

// 定义资源图片
// 图片是从外部加载的一整张的 bmp 位图，而我们需要使用的内部的只是这种图片的
// 某块矩形区域，这个结构体就是用来保存这个信息的
typedef struct Image
{
	HDC imgHdc;		///< 绘图用的句柄
	int x;			///< 图像矩形区域的 x 坐标
	int y;			///< 图像矩形区域的 y 坐标
	int width;		///< 图像矩形区域的宽度
	int height;		///< 图像矩形区域的高度
} Image;
// 初始化 Image 对象
void Image_construct(Image* o, int x, int y, int width, int height);

// 游戏的角色
// 除了定义了图像以外，还定义了在游戏中的位置信息
typedef struct Spirit
{
	Image* image;		///< 精灵的绘制图像
	int cx;				///< 精灵的中心 x 坐标
	int cy;				///< 精灵的中心 y 坐标
	int width;			///< 精灵的宽度
	int height;			///< 精灵的高度
	bool visiable;		///< 精灵是否可见
} Spirit;

// 定义游戏中的按钮
typedef struct Button
{
	Spirit* imgButton;
};

// 图像资源管理模块，用来管理所有可能用到的图像
typedef struct ImageManager
{
	HINSTANCE hInstance;	///< 应用程序的实例句柄
	HBITMAP imgScene;		///< 图片资源句柄，存放整张 BMP
	HDC imgHdc;				///< 图像绘制句柄
} ImageManager;

// 初始化 ImageManager
void ImageManager_construct(HINSTANCE hInstance);
// 释放 ImageManager 资源
void ImageManager_destruct();

// 初始化所有用到的图像资源
void ImageManager_initAllImages(HDC hdc);
// 加载那张全局的大图片
bool ImageManager_loadScenes();
void ImageManager_drawSpiritToHdc(ImageManager* o, Image* sp, HDC hdc, int dx, int dy);
void ImageManager_alphaBlend(ImageManager* o, Image* sp, HDC hdc, int dx, int dy, int width, int height, BYTE alpha);

void ImageManager_drawNumber(ImageManager* o, size_t num, HDC hdcDst, int cx, int cy, DrawNumberSize numberSize, DrawNumberAlign align);

bool Spirit_isPointInMe(Image* o, int x, int y, int dx, int dy);

// 对于本游戏而言，可用的精灵总共就那么一些，所以就全
// 都枚举出来了
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