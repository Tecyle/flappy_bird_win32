#pragma once
#include "image_manager.h"

typedef struct Button 
{
	Spirit* btImage;
	int cx;
	int cy;
	int alpha;
} Button;

bool Button_isHit(Button* o, int x, int y);
void Button_construct(Button* o, Spirit* spirit);