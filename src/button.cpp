#include "stdafx.h"
#include "button.h"

bool Button_isHit(Button * o, int x, int y)
{
	if (x > o->cx - o->btImage->width / 2 && x < o->cx + o->btImage->width / 2)
	{
		if (y > o->cy - o->btImage->height / 2 && y < o->cy + o->btImage->height / 2)
		{
			return true;
		}
	}
	return false;
}

void Button_construct(Button * o, Image * spirit)
{
	o->btImage = spirit;
	o->cx = o->cy = 0;
	o->alpha = 255;
}
