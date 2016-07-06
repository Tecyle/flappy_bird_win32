include		stdafx.inc
include		sound_manager.inc
include		winmm.inc

includelib	winmm.lib

;/////////////////////////////////////////////////////////
; 定义声音管理器类型以及其唯一实例
SoundManager	struct
	hInstance	DWORD	?
SoundManager	ends

	.data
g_soundManager	SoundManager<0>

soundNames		dd	IDR_SFX_DIE, IDR_SFX_HIT, IDR_SFX_POINT, IDR_SFX_SWOOSHING, IDR_SFX_WING

	.code
;/////////////////////////////////////////////////////////
; SoundManager_init : 初始化声音管理器
;/////////////////////////////////////////////////////////
SoundManager_init		proc public uses eax hInstance : DWORD
	mov		eax, hInstance
	mov		g_soundManager.hInstance, eax
	ret
SoundManager_init	endp

SoundManager_playSound	proc public uses eax edx sound : DWORD
	mov		edx, 0
	mov		eax, sizeof DWORD
	mul		sound
	add		eax, offset soundNames
	mov		eax, DWORD ptr [eax]
	invoke	PlaySound, eax, g_soundManager.hInstance, SND_RESOURCE or SND_ASYNC
	ret
SoundManager_playSound	endp

	end