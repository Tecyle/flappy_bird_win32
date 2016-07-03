include		stdafx.inc
include		scene_manager.inc

	.data
szCaption			db	'Flappy Bird', 0
szExitQuestion		db	'是否结束当前游戏？', 0
szWindowClassName	db	'FlappyBird', 0
szRegFailed			db	'注册窗口类失败！', 0
szWindowFailed		db	'创建窗口失败！', 0

	.code
;/////////////////////////////////////////////////////////
; _ClearMessageQueue : 清空消息队列
;                      用来清空消息队列里面所有未读消息，忽略
;                      这些消息
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
; _WndProc : 消息回调函数
;            用来处理窗口收到的各个有用的消息
;/////////////////////////////////////////////////////////
_WndProc	proc private uses ebx hWnd, msg, wParam, lParam
	local	@windowRect : RECT
	local	@rx : SDWORD
	local	@ry : SDWORD

	mov		eax, msg
	; 关闭窗口时，确认退出
	.if		eax == WM_CLOSE
		invoke	MessageBox, NULL, offset szExitQuestion, offset szCaption, MB_ICONQUESTION or MB_YESNO
		.if		eax == IDYES
			invoke	DestroyWindow, hWnd
		.endif
	; 销毁窗口时，退出程序
	.elseif	eax == WM_DESTROY
		invoke	PostQuitMessage, 0
	; 改变窗口大小时，修改画布尺寸
	.elseif	eax == WM_SIZE
		invoke	GetClientRect, hWnd, addr @windowRect
		mov		eax, @windowRect.right
		sub		eax, @windowRect.left
		mov		ebx, @windowRect.bottom
		sub		ebx, @windowRect.top
		invoke	SceneManager_setViewSize, eax, ebx
		invoke	SceneManager_render
	; 鼠标左键单击时，变换窗口坐标为游戏坐标，并传递坐标进行单击事件判断
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
	; 其他情况交由默认的消息处理程序进行处理
	.else
		invoke	DefWindowProc, hWnd, msg, wParam, lParam
		ret
	.endif
	xor		eax, eax
	ret
_WndProc	endp

;/////////////////////////////////////////////////////////
; WinMain : Windows 程序的入口函数
;           整个程序的入口函数，因为是自己定义的入口函数，所以
;           和 CALL 的 WinMain 不一样，4 个参数需要自己通过
;           API 函数获取
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
	; 填充窗口类
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
	; 注册窗口类
	invoke	RegisterClassEx, addr @wndClass
	.if		eax == 0
		invoke	MessageBox, NULL, offset szRegFailed, offset szCaption, MB_ICONERROR
		mov		eax, -1
		ret
	.endif
	; 获取屏幕和窗口尺寸信息
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
	; 创建窗口
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
	; 初始化绘图对象
	invoke	GetDC, @hWnd
	mov		@hdc, eax
	invoke	GetClientRect, @hWnd, addr @windowRect
	invoke	SceneManager_init, @hInstance, @hWnd, @hdc
	mov		eax, @windowRect.right
	sub		eax, @windowRect.left
	mov		ebx, @windowRect.bottom
	sub		ebx, @windowRect.top
	invoke	SceneManager_setViewSize, eax, ebx
	; 显示窗口
	invoke	ShowWindow, @hWnd, SW_SHOW
	invoke	UpdateWindow, @hWnd
	; 消息循环
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
	; 清理并结束程序
	invoke	SceneManager_destruct
	mov		eax, @msg.wParam
	ret
WinMain		endp

;/////////////////////////////////////////////////////////
; 程序真正的开始位置，其实就是调用 WinMain，然后再利用 WinMain
; 的返回值调用 ExitProcess 退出程序。
;/////////////////////////////////////////////////////////
start:
	invoke	WinMain
	invoke	ExitProcess, eax
	end		start