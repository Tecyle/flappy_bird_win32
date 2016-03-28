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
