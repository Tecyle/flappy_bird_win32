#pragma once
#include "image_manager.h"

typedef void (*FuncOnClick)();

typedef struct Button 
{
	int left;
	int right;
	int top;
	int bottom;
	BOOL enabled;

	FuncOnClick onClick;
} Button;

void Button_construct(Button* o, int left, int right, int top, int bottom, FuncOnClick onClickFunction);
bool Button_isHit(Button* o, int x, int y);
