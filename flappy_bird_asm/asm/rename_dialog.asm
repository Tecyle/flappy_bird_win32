include		stdafx.inc
include		rename_dialog.inc
include		score_manager.inc

	.data
newName				DB			64 dup(?)

	.code
_RenameDialog_initDialog	proc stdcall private	hWnd : HWND
	invoke	ScoreManager_getCurrentName
	invoke	crt_strcpy, offset newName, eax
	invoke	SetDlgItemTextA, hWnd, IDC_PLAYER_NAME, offset newName
	ret
_RenameDialog_initDialog	endp

reNameDlgProc				proc stdcall private	hWnd : HWND, msg : UINT, wParam : WPARAM, lParam : LPARAM
	mov		eax, lParam
	.if		msg == WM_INITDIALOG
		invoke	_RenameDialog_initDialog, hWnd
	.elseif	msg == WM_CLOSE
		invoke	EndDialog, hWnd, IDCANCEL
	.elseif	msg == WM_COMMAND
		.if		wParam == IDOK
			invoke	GetDlgItemTextA, hWnd, IDC_PLAYER_NAME, offset newName, 64
		.elseif	wParam == IDCANCEL
			invoke	EndDialog, hWnd, wParam
		.else
			mov		eax, FALSE
			ret
		.endif
	.else
		mov		eax, FALSE
		ret
	.endif
	mov		eax, TRUE
	ret
reNameDlgProc				endp

RenameDialog_getName		proc stdcall public	hInstance : HINSTANCE, parent : HWND
	invoke	DialogBoxParamA, hInstance, IDD_RENAME, parent, reNameDlgProc, 0
	mov		eax, offset newName
	ret
RenameDialog_getName		endp
	end
