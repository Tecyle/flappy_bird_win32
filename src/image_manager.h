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
	SpiritType_ground = 0,			///< 地面
	SpiritType_sky = 1,				///< 背景天空
	SpiritType_bird = 2,			///< 小鸟
	SpiritType_upPipes = 3,			///< 上层管道组
	SpiritType_downPipes = 4,		///< 下层管道组
	SpiritType_txtGetReady = 5,		///< GetReady 文字
	SpiritType_txtGameOver = 6,		///< GameOver 文字
	SpiritType_txtCopyRight = 7,	///< 版权文字信息
	SpiritType_txtFlappyBird = 8,	///< FlappyBird 文字
	SpiritType_largeScore = 9,		///< 当前得分数字
	SpiritType_helpInfo = 10,		///< 帮助信息
	SpiritType_btPlay = 11,			///< 开始游戏按钮
	SpiritType_btRank = 12,			///< 排名按钮
	SpiritType_btRate = 13,			///< Rate 按钮
	SpiritType_scoreBoard = 14,		///< 记分牌
	SpiritType_blackFade = 15,		///< 黑色渐变遮罩
	SpiritType_whiteFade = 16		///< 白色渐变遮罩
} SpiritType;

// 定义资源图片
// 图片是从外部加载的一整张的 bmp 位图，而我们需要使用的内部的只是这种图片的
// 某块矩形区域，这个结构体就是用来保存这个信息的
typedef struct Image
{
	int x;			///< 图像矩形区域的 x 坐标
	int y;			///< 图像矩形区域的 y 坐标
	int width;		///< 图像矩形区域的宽度
	int height;		///< 图像矩形区域的高度
} Image;
// 初始化 Image 对象
void Image_construct(Image* o, int x, int y, int width, int height, HDC srcHdc);

// 游戏的角色
// 除了定义了图像以外，还定义了在游戏中的位置信息
typedef struct Spirit
{
	Image* image;		///< 精灵的绘制图像
	int cx;				///< 精灵的中心 x 坐标
	int cy;				///< 精灵的中心 y 坐标
	double angle;		///< 精灵的旋转角度，目前仅小鸟有效
	int halfWidth;		///< 精灵的宽度的一半
	int halfHeight;		///< 精灵的高度的一半
	bool visiable;		///< 精灵是否可见
	Animation* ani;		///< 控制精灵的动画
} Spirit;

void Spirit_construct(Spirit* o, Image* img, int cx, int cy);
void Spirit_draw(Spirit* o, HDC dstHdc);

// 定义游戏中的按钮
typedef struct Button
{
	Spirit* imgButton;
};

// 图像资源管理模块，用来管理所有可能用到的图像
typedef struct ImageManager
{
	HINSTANCE hInstance;	///< 应用程序的实例句柄
	HDC dstHdc;
	HBITMAP imgScene;		///< 图片资源句柄，存放整张 BMP
	HDC imgHdc;				///< 图像绘制句柄
} ImageManager;

// 初始化 ImageManager
void ImageManager_construct(HINSTANCE hInstance, HDC dstHdc);
// 释放 ImageManager 资源
void ImageManager_destruct();

// 初始化所有用到的图像资源
bool ImageManager_initAll(HDC hdc);
void ImageManager_randomSkyAndBird();
void ImageManager_drawSpirit(SpiritType spirit);
void ImageManager_drawFadeCover(Image* img, BYTE alpha);
void ImageManager_drawNumber(size_t num, int cx, int cy, NumberSize size, NumberAlign align);


