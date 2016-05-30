#pragma once

typedef enum SoundType
{
	SoundType_die = 0,
	SoundType_hit = 1,
	SoundType_point = 2,
	SoundType_swooshing = 3,
	SoundType_wing = 4
} SoundType;

void SoundManager_init(HINSTANCE hInstance);
void SoundManager_playSound(SoundType sound);