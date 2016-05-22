#include "stdafx.h"
#include "animation_manager.h"

//////////////////////////////////////////////////////////////////////////
// 全局变量
AnimationManager g_aniMgr;

//////////////////////////////////////////////////////////////////////////
// FadeAnimation
void FadeAnimation_init(Animation* o, BYTE from, BYTE to, long duringTime, bool loop)
{
	o->fadeEnable = true;
	o->alphaStart = from;
	o->alphaEnd = to;
	o->fadeLoop = loop;
	o->nowAlpha = from;
	int tickCount = duringTime * 60 / 1000;
	o->fadeStep = abs(to - from) / tickCount;
}

void FadeAnimation_tick(Animation* o)
{
	if (!o->fadeEnable)
		return;

	if (o->nowAlpha == o->alphaEnd)
	{
		if (o->fadeLoop)
			o->nowAlpha = o->alphaStart;
		else
			o->fadeEnable = false;
		return;
	}

	if (o->alphaEnd > o->alphaStart)
	{
		o->nowAlpha += o->fadeStep;
		if (o->nowAlpha > o->alphaEnd)
			o->nowAlpha = o->alphaEnd;
	}
	else
	{
		o->nowAlpha -= o->fadeStep;
		if (o->nowAlpha < o->alphaEnd)
			o->nowAlpha = o->alphaEnd;
	}
}

BYTE FadeAnimation_getAlpha(Animation* o)
{
	return (BYTE)o->nowAlpha;
}

bool FadeAnimation_finished(Animation* o)
{
	if (!o->fadeLoop && o->nowAlpha == o->alphaEnd)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
// FrameAniamtion
void FrameAnimation_init(Animation* o, int frameCount, long duringTime, bool loop)
{
	o->frameCount = frameCount;
	o->frameIndex = 0;
	o->frameEnable = true;
	o->frameLoop = loop;
	int tickCount = duringTime * 60 / 1000;
	o->frameStep = tickCount / frameCount;
	o->frameTickCount = 0;
}

void FrameAnimation_tick(Animation* o)
{
	if (!o->frameEnable)
		return;

	o->frameTickCount++;
	if (o->frameTickCount < o->frameStep)
	{
		return;
	}
	else
	{
		o->frameTickCount = 0;
	}

	if (o->frameIndex == o->frameCount - 1)
	{
		if (o->frameLoop)
			o->frameIndex = 0;
		else
			o->frameEnable = false;
		return;
	}

	o->frameIndex++;
}

int FrameAnimation_getFrameIndex(Animation* o)
{
	return o->frameIndex;
}

bool FrameAnimation_finished(Animation* o)
{
	if (!o->frameLoop && o->frameIndex == o->frameCount - 1)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
// TransAnimation
void TransAnimation_init(Animation* o, int sx, int sy, int ex, int ey, long duringTime, bool loop)
{
	o->transEnable = true;
	o->sx = sx;
	o->sy = sy;
	o->ex = ex;
	o->ey = ey;
	o->nx = sx;
	o->ny = sy;
	o->transLoop = loop;
	int tickCount = duringTime * 60 / 1000;
	o->transXStep = (ex - sx) / tickCount;
	o->transYStep = (ey - sy) / tickCount;
}

void TransAnimation_tick(Animation* o)
{
	if (!o->transEnable)
		return;

	if (o->nx == o->ex && o->ny == o->ey)
	{
		if (o->transLoop)
		{
			o->nx = o->sx;
			o->ny = o->sy;
		}
		else
		{
			o->transEnable = false;
		}
		return;
	}

	o->nx += o->transXStep;
	o->ny += o->transYStep;
}

void TransAnimation_getXY(Animation* o, int* x, int* y)
{
	*x = o->nx;
	*y = o->ny;
}

bool TransAnimation_finished(Animation* o)
{
	if (!o->transLoop && o->nx == o->ex && o->ny == o->ey)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
// AnimationManager
void _AnimationManager_initAnimation(Animation* ani)
{
	ani->fadeEnable = false;
	ani->frameEnable = false;
	ani->transEnable = false;
}

void AnimationManager_init()
{
	AnimationManager* o = &g_aniMgr;
	o->aniCount = 0;
}

Animation* AnimationManager_allocAnimation()
{
	AnimationManager* o = &g_aniMgr;
	if (o->aniCount == 32)
		return NULL;

	Animation* ani = &o->ani[o->aniCount++];
	_AnimationManager_initAnimation(ani);
	return ani;
}

void AnimationManager_tick()
{
	AnimationManager* o = &g_aniMgr;
	for (int i = 0; i < o->aniCount; ++i)
	{
		Animation* ani;
		FadeAnimation_tick(ani);
		FrameAnimation_tick(ani);
		TransAnimation_tick(ani);
	}
}




Image* BirdAnimation_step(BirdColor color, bool inc)
{
	static const int g_baStepIndex[] = { 0, 1, 2, 1 };
	static size_t step = 0;
	if (inc)
		step = step++ % 4;
	switch (color)
	{
	case BirdColor_blue:
		return &sp_blueBird[g_baStepIndex[step]];
	case BirdColor_red:
		return &sp_redBird[g_baStepIndex[step]];
	case BirdColor_yellow:
		return &sp_yellowBird[g_baStepIndex[step]];
	}
	return NULL;
}

void GroundAnimation_step(HDC srcHdc, HDC dstHdc, int fps, bool moving)
{
	int interval = 1;
	static int step = 0;
	static int x = 0;
	static int y = SCENE_HEIGHT - sp_ground.height;
	
	if (moving)
	{
		if (step % interval == 0)
			x = (x + 1) % (sp_ground.width - SCENE_WIDTH);
		step = step++ % interval;
	}
	BitBlt(dstHdc, 0, y, SCENE_WIDTH, sp_ground.height, srcHdc, sp_ground.x + x, sp_ground.y, SRCCOPY);
}
