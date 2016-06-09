;/////////////////////////////////////////////////////////
; stdafx.inc
; 类似于 VC 的 stdafx.h，用来包含各个 asm 文件的公共引用部分
; 和通用声明，本项目的所有 asm 文件第一行都必须是：
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