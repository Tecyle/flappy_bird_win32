#include "stdafx.h"
#include "button.h"

bool Button_isHit(Button* o, int x, int y)
{
	if (x > o->left && x < o->right)
	{
		if (y > o->top && y < o->bottom)
		{
			return true;
		}
	}
	return false;
}

void Button_construct(Button* o, int left, int right, int top, int bottom, FuncOnClick onClickFunction)
{
	o->left = left;
	o->right = right;
	o->top = top;
	o->bottom = bottom;
	o->onClick = onClickFunction;
	o->enabled = true;
}

