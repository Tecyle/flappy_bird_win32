include		stdafx.inc
include		button.inc

	.code
Button_isHit		proc public uses ebx	o : ButtonPtr, x : SDWORD, y : SDWORD
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

Button_construct	proc public uses ebx	o : ButtonPtr, left : SDWORD, right : SDWORD, top : SDWORD, bottom : SDWORD, onClickFunction : FuncOnClick
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