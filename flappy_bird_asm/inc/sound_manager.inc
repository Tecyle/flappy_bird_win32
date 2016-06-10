;/////////////////////////////////////////////////////////
; sound_manager.inc
; 管理游戏的声音，所有关于游戏声音控制的调用都在这里
; 用法：
; 1. 在游戏初始化的时候调用 SoundManager_init
; 2. 在需要播放声音的地方，调用 SoundManager_playSound 来播
;    放声音
;/////////////////////////////////////////////////////////

;/////////////////////////////////////////////////////////
; 定义游戏中所有用到的声音类型
SoundType_die		equ 0	; 小鸟死亡的声音
SoundType_hit		equ 1	; 小鸟撞击的声音
SoundType_point		equ 2	; 小鸟获得分数的声音
SoundType_swooshing	equ 3	; 切换界面的声音
SoundType_wing		equ 4	; 小鸟向上飞的声音

;/////////////////////////////////////////////////////////
; SoundManager_init
; 初始化声音引擎
SoundManager_init		proto stdcall	hInstance : DWORD

;/////////////////////////////////////////////////////////
; SoundManager_playSound
; 播放指定的声音，比如，想要播放小鸟死亡的声音，应该：
; invoke	SoundManager_playSound, SoundType_die
SoundManager_playSound	proto stdcall	sound : DWORD