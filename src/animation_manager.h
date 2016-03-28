#pragma once
#include "image_manager.h"

typedef enum BirdColor
{
	BirdColor_blue,
	BirdColor_red,
	BirdColor_yellow
} BirdColor;

Spirit* BirdAnimation_step(BirdColor color, bool inc);