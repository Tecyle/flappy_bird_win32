#pragma once
#include "image_manager.h"

typedef struct Animation
{
	// 淡入淡出动画参数
	BYTE alphaStart;
	BYTE alphaEnd;
	BYTE nowAlpha;
	int fadeTickCount;
	bool fadeLoop;
	bool fadeEnable;
	// 逐帧动画动画参数
	int frameCount;
	int frameIndex;
	int frameTickCount;
	bool frameLoop;
	bool frameEnable;
	// 移动动画参数
	int sx, sy;
	int ex, ey;
	int nx, ny;
	int transTickCount;
	bool transLoop;
	bool transEnable;
} Animation;

// 淡入淡出动画
void FadeAnimation_init(Animation* o, BYTE from, BYTE to, long duringTime, bool loop);
void FadeAnimation_tick(Animation* o);
BYTE FadeAnimation_getAlpha(Animation* o);
bool FadeAnimation_finished(Animation* o);

// 逐帧动画
void FrameAnimation_init(Animation* o, int frameCount, long duringTime, bool loop);
void FrameAnimation_tick(Animation* o);
int FrameAnimation_getFrameIndex(Animation* o);
bool FrameAnimation_finished(Animation* o);

// 移动动画
void TransAnimation_init(Animation* o, int sx, int sy, int ex, int ey, long duringTime, bool loop);
void TransAnimation_tick(Animation* o);
void TransAnimation_getXY(Animation* o, int* x, int* y);
bool TransAnimation_finished(Animation* o);

typedef struct AnimationManager
{
	Animation ani[32];
	size_t aniCount;
} AnimationManager;

Animation* AnimationManager_allocAnimation();
void AnimationManager_tick();

typedef enum BirdColor
{
	BirdColor_blue,
	BirdColor_red,
	BirdColor_yellow
} BirdColor;

Image* BirdAnimation_step(BirdColor color, bool inc);
void GroundAnimation_step(HDC srcHdc, HDC dstHdc, int fps, bool moving);
