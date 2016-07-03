include		stdafx.inc
include		scene_manager.inc

	.data
szCaption			db	'Flappy Bird', 0
szExitQuestion		db	'�Ƿ������ǰ��Ϸ��', 0
szWindowClassName	db	'FlappyBird', 0
szRegFailed			db	'ע�ᴰ����ʧ�ܣ�', 0
szWindowFailed		db	'��������ʧ�ܣ�', 0

	.code
;/////////////////////////////////////////////////////////
; _ClearMessageQueue : �����Ϣ����
;                      ���������Ϣ������������δ����Ϣ������
;                      ��Щ��Ϣ
;/////////////////////////////////////////////////////////
_ClearMessageQueue	proc private
	local	@msg : MSG

	mov		eax, 1
	.while	eax != 0
		invoke	PeekMessage, addr @msg, NULL, 0, 0, PM_REMOVE
	.endw
	ret
_ClearMessageQueue	endp

;/////////////////////////////////////////////////////////
; _WndProc : ��Ϣ�ص�����
;            �����������յ��ĸ������õ���Ϣ
;/////////////////////////////////////////////////////////
_WndProc	proc private uses ebx hWnd, msg, wParam, lParam
	local	@windowRect : RECT
	local	@rx : SDWORD
	local	@ry : SDWORD

	mov		eax, msg
	; �رմ���ʱ��ȷ���˳�
	.if		eax == WM_CLOSE
		invoke	MessageBox, NULL, offset szExitQuestion, offset szCaption, MB_ICONQUESTION or MB_YESNO
		.if		eax == IDYES
			invoke	DestroyWindow, hWnd
		.endif
	; ���ٴ���ʱ���˳�����
	.elseif	eax == WM_DESTROY
		invoke	PostQuitMessage, 0
	; �ı䴰�ڴ�Сʱ���޸Ļ����ߴ�
	.elseif	eax == WM_SIZE
		invoke	GetClientRect, hWnd, addr @windowRect
		mov		eax, @windowRect.right
		sub		eax, @windowRect.left
		mov		ebx, @windowRect.bottom
		sub		ebx, @windowRect.top
		invoke	SceneManager_setViewSize, eax, ebx
		invoke	SceneManager_render
	; ����������ʱ���任��������Ϊ��Ϸ���꣬������������е����¼��ж�
	.elseif	eax == WM_LBUTTONDOWN
		invoke	GetClientRect, hWnd, addr @windowRect
		mov		@rx, SCENE_WIDTH
		fild	@rx
		mov		eax, lParam
		and		eax, 0ffffH
		mov		@rx, eax
		fimul	@rx
		mov		eax, @windowRect.right
		sub		eax, @windowRect.left
		mov		@rx, eax
		fidiv	@rx
		fistp	@rx
		mov		@ry, SCENE_HEIGHT
		fild	@ry
		mov		eax, lParam
		shr		eax, 16
		mov		@ry, eax
		fimul	@ry
		mov		eax, @windowRect.bottom
		sub		eax, @windowRect.top
		mov		@ry, eax
		fidiv	@ry
		fistp	@ry
		invoke	SceneManager_onClick, @rx, @ry
		invoke	_ClearMessageQueue
	; �����������Ĭ�ϵ���Ϣ���������д���
	.else
		invoke	DefWindowProc, hWnd, msg, wParam, lParam
		ret
	.endif
	xor		eax, eax
	ret
_WndProc	endp

;/////////////////////////////////////////////////////////
; WinMain : Windows �������ں���
;           �����������ں�������Ϊ���Լ��������ں���������
;           �� CALL �� WinMain ��һ����4 ��������Ҫ�Լ�ͨ��
;           API ������ȡ
;/////////////////////////////////////////////////////////
WinMain		proc uses ebx
	local	@wndClass : WNDCLASSEX
	local	@hInstance : HINSTANCE
	local	@hWnd : HWND
	local	@hdc : HDC
	local	@msg : MSG
	local	@hCaption : DWORD
	local	@hBorder : DWORD
	local	@wBorder : DWORD
	local	@hScreen : DWORD
	local	@wScreen : DWORD
	local	@wWindow : DWORD
	local	@hWindow : DWORD
	local	@xWindow : DWORD
	local	@yWindow : DWORD
	local	@windowRect : RECT

	invoke	GetModuleHandle, NULL
	mov		@hInstance, eax
	; ��䴰����
	invoke	RtlZeroMemory, addr @wndClass, sizeof @wndClass
	mov		@wndClass.hInstance, eax
	mov		@wndClass.style, CS_HREDRAW or CS_VREDRAW
	mov		@wndClass.lpfnWndProc, offset _WndProc
	mov		@wndClass.hIcon, NULL
	mov		@wndClass.hbrBackground, COLOR_WINDOW + 1
	mov		@wndClass.lpszMenuName, NULL
	mov		@wndClass.lpszClassName, offset szWindowClassName
	mov		@wndClass.hIconSm, NULL
	mov		@wndClass.cbSize, sizeof @wndClass
	invoke	LoadCursor, NULL, IDC_ARROW
	mov		@wndClass.hCursor, eax
	; ע�ᴰ����
	invoke	RegisterClassEx, addr @wndClass
	.if		eax == 0
		invoke	MessageBox, NULL, offset szRegFailed, offset szCaption, MB_ICONERROR
		mov		eax, -1
		ret
	.endif
	; ��ȡ��Ļ�ʹ��ڳߴ���Ϣ
	invoke	GetSystemMetrics, SM_CYCAPTION
	mov		@hCaption, eax
	invoke	GetSystemMetrics, SM_CYSIZEFRAME
	mov		@hBorder, eax
	invoke	GetSystemMetrics, SM_CXSIZEFRAME
	mov		@wBorder, eax
	invoke	GetSystemMetrics, SM_CYSCREEN
	mov		@hScreen, eax
	invoke	GetSystemMetrics, SM_CXSCREEN
	mov		@wScreen, eax
	mov		eax, @wBorder
	sal		eax, 1
	add		eax, SCENE_WIDTH
	mov		@wWindow, eax
	mov		eax, @hBorder
	sal		eax, 1
	add		eax, SCENE_HEIGHT
	mov		@hWindow, eax
	mov		eax, @wScreen
	sub		eax, @wWindow
	sar		eax, 1
	mov		@xWindow, eax
	mov		eax, @hScreen
	sub		eax, @hWindow
	sar		eax, 1
	mov		@yWindow, eax
	; ��������
	invoke	CreateWindowEx, 0, offset szWindowClassName, offset szCaption, \
			WS_OVERLAPPED or WS_SYSMENU or WS_SIZEBOX or WS_MINIMIZEBOX, \
			@xWindow, @yWindow, @wWindow, @hWindow, \
			NULL, NULL, @hInstance, NULL
	mov		@hWnd, eax
	.if		eax == NULL
		invoke	MessageBox, NULL, offset szWindowFailed, offset szCaption, MB_ICONERROR
		mov		eax, -1
		ret
	.endif
	; ��ʼ����ͼ����
	invoke	GetDC, @hWnd
	mov		@hdc, eax
	invoke	GetClientRect, @hWnd, addr @windowRect
	invoke	SceneManager_init, @hInstance, @hWnd, @hdc
	mov		eax, @windowRect.right
	sub		eax, @windowRect.left
	mov		ebx, @windowRect.bottom
	sub		ebx, @windowRect.top
	invoke	SceneManager_setViewSize, eax, ebx
	; ��ʾ����
	invoke	ShowWindow, @hWnd, SW_SHOW
	invoke	UpdateWindow, @hWnd
	; ��Ϣѭ��
	.while	TRUE
		invoke	PeekMessage, addr @msg, NULL, 0, 0, PM_REMOVE
		.if		eax != 0
			.if		@msg.message == WM_QUIT
				.break
			.endif
			invoke	TranslateMessage, addr @msg
			invoke	DispatchMessage, addr @msg
			.continue
		.endif
		invoke	SceneManager_render
	.endw
	; ������������
	invoke	SceneManager_destruct
	mov		eax, @msg.wParam
	ret
WinMain		endp

;/////////////////////////////////////////////////////////
; ���������Ŀ�ʼλ�ã���ʵ���ǵ��� WinMain��Ȼ�������� WinMain
; �ķ���ֵ���� ExitProcess �˳�����
;/////////////////////////////////////////////////////////
start:
	invoke	WinMain
	invoke	ExitProcess, eax
	end		start