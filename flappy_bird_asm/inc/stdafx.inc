;/////////////////////////////////////////////////////////
; stdafx.inc
; ������ VC �� stdafx.h�������������� asm �ļ��Ĺ������ò���
; ��ͨ������������Ŀ������ asm �ļ���һ�ж������ǣ�
; include stdafx.inc
;/////////////////////////////////////////////////////////
	.387
	.model	flat, stdcall
	option casemap:none

include		windows.inc
include		user32.inc
include		gdi32.inc
include		kernel32.inc
include		msvcrt.inc
;include		resource.inc

includelib	user32.lib
includelib	gdi32.lib
includelib	kernel32.lib
includelib	msvcrt.lib

SCENE_WIDTH		equ	288
SCENE_HEIGHT	equ	512