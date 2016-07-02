include		stdafx.inc
include		rank_dialog.inc
include		score_manager.inc
include		rename_dialog.inc

	.const
g_idNames			DWORD			IDC_NAME1, IDC_NAME2, IDC_NAME3, \
									IDC_NAME4, IDC_NAME5, IDC_NAME6, \
									IDC_NAME7, IDC_NAME8, IDC_NAME9
g_idScores			DWORD			IDC_SCORE1, IDC_SCORE2, IDC_SCORE3, \
									IDC_SCORE4, IDC_SCORE5, IDC_SCORE6, \
									IDC_SCORE7, IDC_SCORE8, IDC_SCORE9
g_noName			DB				"<无记录>", 0
g_format			DB				"%u", 0
g_clearMsg			DB				"清空记录之后将无法撤销，确定要清空吗？", 0
g_clearTitle		DB				"Are you sure?", 0

	.data
g_hInstance			HINSTANCE		?
g_buff				DB				64 dup(?)

	.code
_RankDialog_onInitDialog			proc stdcall private uses eax ebx ecx edx	hWnd : HWND
	local	@player : DWORD
	local	@score : DWORD

	mov		ecx, 0
	.while	ecx < 9
		inc		ecx
		invoke	ScoreManager_getNameByRank, ecx
		mov		@player, eax
		invoke	ScoreManager_getScoreByRank, ecx
		mov		@score, eax
		.if		@player == NULL || [@player] == NULL
			mov		@player, offset g_noName
			mov		@score, 0
		.endif
		dec		ecx
		mov		eax, sizeof DWORD
		mov		edx, 0
		mul		ecx
		add		eax, offset g_idNames
		invoke	SetDlgItemTextA, hWnd, [eax], @player
		invoke	wsprintfA, offset g_buffer, offset g_format, @score
		mov		eax, sizeof DWORD
		mov		edx, 0
		mul		ecx
		add		eax, offset g_idScores
		invoke	SetDlgItemTextA, hWnd, [eax], offset g_buffer
		inc		ecx
	.endw
	ret
_RankDialog_onInitDialog			endp

rankDialogFunc						proc stdcall private	hWnd : HWND, msg : UINT, wParam : WPARAM, lParam : LPARAM
	.if		msg == WM_INITDIALOG
		invoke	_RankDialog_onInitDialog, hWnd
	.elseif	msg == WM_CLOSE
		invoke	EndDialog, hWnd, NULL
	.elseif	msg == WM_COMMAND
		.if		wParam == IDOK
			invoke	EndDialog, hWnd, IDOK
		.elseif	wParam == IDB_RENAME
			invoke	RenameDialog_getName, g_hInstance, hWnd
			invoke	ScoreManager_setCurrentName, eax
		.elseif	wParam == IDB_CLEAR
			invoke	MessageBoxA, NULL, offset g_clearMsg, offset g_clearTitle, MB_ICONQUESTION or MB_YESNO
			.if		eax == IDYES
				invoke	ScoreManager_clear
				invoke	_RankDialog_onInitDialog, hWnd
			.endif
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
rankDialogFunc						endp


RankDialog_doModal					proc stdcall public	hInstance : HINSTANCE, parent : HWND
	mov		g_hInstance, hInstancce
	invoke	DialogBoxA, hInstance, IDD_RANK, parent, rankDialogFunc
	mov		eax, 0
	ret
RankDialog_doModal					endp
	
	end
