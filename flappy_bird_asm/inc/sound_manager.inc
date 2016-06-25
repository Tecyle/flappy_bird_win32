;/////////////////////////////////////////////////////////
; sound_manager.inc
; ������Ϸ�����������й�����Ϸ�������Ƶĵ��ö�������
; �÷���
; 1. ����Ϸ��ʼ����ʱ����� SoundManager_init
; 2. ����Ҫ���������ĵط������� SoundManager_playSound ����
;    ������
;/////////////////////////////////////////////////////////

;/////////////////////////////////////////////////////////
; ������Ϸ�������õ�����������
SoundType_die		equ 0	; С������������
SoundType_hit		equ 1	; С��ײ��������
SoundType_point		equ 2	; С���÷���������
SoundType_swooshing	equ 3	; �л����������
SoundType_wing		equ 4	; С�����Ϸɵ�����

;/////////////////////////////////////////////////////////
; SoundManager_init
; ��ʼ����������
SoundManager_init		proto stdcall	hInstance : DWORD

;/////////////////////////////////////////////////////////
; SoundManager_playSound
; ����ָ�������������磬��Ҫ����С��������������Ӧ�ã�
; invoke	SoundManager_playSound, SoundType_die
SoundManager_playSound	proto stdcall	sound : DWORD