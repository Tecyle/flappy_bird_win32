#include "stdafx.h"
#include "animation_manager.h"

Spirit* BirdAnimation_step(BirdColor color, bool inc)
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

void GroundAnimation_step(HDC srcHdc, HDC dstHdc, int fps)
{
	int interval = 1;
	static int step = 0;
	static int x = 0;
	static int y = SCENE_HEIGHT - sp_ground.height;
	
	if (step % interval == 0)
		x = (x + 1) % (sp_ground.width - SCENE_WIDTH);
	step = step++ % interval;
	BitBlt(dstHdc, 0, y, SCENE_WIDTH, sp_ground.height, srcHdc, sp_ground.x + x, sp_ground.y, SRCCOPY);
}
