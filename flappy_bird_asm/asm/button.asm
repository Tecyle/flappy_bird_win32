include		stdafx.inc
include		button.inc

Button		struct
	left	DWORD		?
	right	DWORD		?
	top		DWORD		?
	bottom	DWORD		?
	enabled	DWORD		?
	onClick	FuncOnClick	?
Button		ends

	.code
Button_isHit		proc public uses ebx	o, x, y
	mov		ebx, o
	assume	ebx : ptr Button
	.if		[ebx].enabled == FALSE
		mov		eax, FALSE
		ret
	.endif
	mov		eax, x
	.if		eax > [ebx].left && eax < [ebx].right
		mov		eax, y
		.if		eax > [ebx].top && eax < [ebx].bottom
			mov		eax, TRUE
			ret
		.endif
	.endif
	assume	ebx : nothing
	mov		eax, FALSE
	ret
Button_isHit		endp

Button_construct	proc public uses ebx	o, left, right, top, bottom, onClickFunction : FuncOnClick
	mov		ebx, o
	assume	ebx : ptr Button
	push	left
	pop		[ebx].left
	push	right
	pop		[ebx].right
	push	top
	pop		[ebx].top
	push	bottom
	pop		[ebx].bottom
	push	onClickFunction
	pop		[ebx].onClick
	mov		[ebx].enabled, TRUE
	assume	ebx : nothing
	ret
Button_construct	endp
	end