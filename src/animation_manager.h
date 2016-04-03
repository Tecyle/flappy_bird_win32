#pragma once
#include "image_manager.h"

typedef enum BirdColor
{
	BirdColor_blue,
	BirdColor_red,
	BirdColor_yellow
} BirdColor;

Image* BirdAnimation_step(BirdColor color, bool inc);
void GroundAnimation_step(HDC srcHdc, HDC dstHdc, int fps, bool moving);
