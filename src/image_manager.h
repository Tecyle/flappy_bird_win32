#pragma once

// 定义绘图的单元 ： 精灵
// 由于加载进来的图像是一张大图像，而图像上的某些区域
// 才是我们想要绘制的对象，精灵类就是用来标识各个物体
typedef struct Spirit
{
	int x;
	int y;
	int width;
	int height;
} Spirit;

// 动画
// 动画其实就是由一系列的精灵图像组成的数组，这些图像
// 按照一定的时间间隔反复切换就行成了动画
typedef struct Animation
{
	size_t frameNum;
	Spirit* frames;
	size_t interval;
} Animation;

// 对于本游戏而言，可用的精灵总共就那么一些，所以就全
// 都枚举出来了
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
extern Spirit sp_dliverMedal;
extern Spirit sp_goldenMedal;
extern Spirit sp_help;
extern Spirit sp_orangePipeUp;
extern Spirit sp_orangePipeDown;
extern Spirit sp_greenPipeUp;
extern Spirit sp_greenPipeDown;
extern Spirit sp_yellowBird[3];
extern Spirit sp_redBird[3];
extern Spirit sp_BlueBird[3];
extern Spirit sp_btPlay;
extern Spirit sp_btRank;
extern Spirit sp_btRate;
extern Spirit sp_btContinue;
extern Spirit sp_btPause;
extern Spirit sp_btShare;
extern Spirit sp_btMenu;
extern Spirit sp_btOk;