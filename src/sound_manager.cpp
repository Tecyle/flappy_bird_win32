#include "stdafx.h"
#include "sound_manager.h"
#include "resource.h"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

typedef struct SoundManager
{
	HINSTANCE hInstance;
} SoundManager;

static SoundManager g_soundManager;

void SoundManager_init(HINSTANCE hInstance)
{
	SoundManager* o = &g_soundManager;
	o->hInstance = hInstance;
}

void SoundManager_playSound(SoundType sound)
{
	SoundManager* o = &g_soundManager;
	static const char* soundNames[] = { MAKEINTRESOURCEA(IDR_SFX_DIE), 
		MAKEINTRESOURCEA(IDR_SFX_HIT), 
		MAKEINTRESOURCEA(IDR_SFX_POINT), 
		MAKEINTRESOURCEA(IDR_SFX_SWOOSHING), 
		MAKEINTRESOURCEA(IDR_SFX_WING)
	};
	
 	PlaySoundA(soundNames[(int)sound], o->hInstance, SND_RESOURCE | SND_ASYNC);
}