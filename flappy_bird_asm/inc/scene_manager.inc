IFNDEF	_SCENE_MANAGER_INC_
_SCENE_MANAGER_INC_	equ	<1>

include		image_manager.inc
;// 定义游戏场景类型
;// 游戏一共只有以下 4 个场景：
;// 1. 主菜单
;// 2. 准备开始界面
;// 3. 游戏界面
;// 4. 游戏结束界面
SceneType_mainMenu	equ 0				;///< 主菜单
SceneType_prepare	equ 1				;///< 准备开始界面
SceneType_playing	equ 2				;///< 游戏界面
SceneType_gameOver	equ 3				;///< 游戏结束界面
SceneType			typedef DWORD
SceneTypePtr		typedef ptr SceneType

;// 定义游戏场景
;// 游戏场景指明了一个场景中需要绘制哪些东西
;// 本质上是一个精灵(Spirit)的数组
Scene				struct
	spirits			SpiritTypePtr	?
	spiritCount		DWORD			?
Scene				ends
ScenePtr			typedef ptr Scene
;// 场景控制类，控制游戏的场景及切换，是面向 main 函数的唯一接口
SceneManager		struct
	currentScene	SceneTypePtr	?
	scrHdc			HDC				?
	bufHdc			HDC				?
	drawingBoard	HBITMAP			?
	fps				SDWORD			?

	windowWidth		SDWORD			?
	windowHeight	SDWORD			?

	onSwitching		BOOL			?

	drawCounter		DWORD			?
	showFps			BOOL			?
	isFading		BOOL			?
	fadeAlpha		SDWORD			?
	fadeStep		SDWORD			?
	fadeColor		ImagePtr		?
	hInstance		HINSTANCE		?
	hWnd			HWND			?
SceneManager		ends
SceneManagerPtr		typedef ptr SceneManager

	;初始化场景管理器，必须在游戏开始之前执行
	
	;hdc : 窗口的绘图句柄。

	;为了避免动态内存的使用，本程序使用的内容全是静态数组，
	;所以没有类似 CleanUp 的功能。
SceneManager_init			proto stdcall hInstance:HINSTANCE, hWnd:HWND, hdc:HDC 
SceneManager_destruct		proto stdcall

	;绘制游戏画面

	;绘制是按照每秒 60 帧的速度进行的，也就是说，一秒内调用
	;该函数上千次，该函数也只会在这一秒内只绘制 60 次。
	;return 返回 true 代表本次调用绘制了图形，返回 false 
	;代表本次是空调用。
SceneManager_render			proto stdcall

	;设置视图的大小

	;视图的大小也就是最终用户看到的绘图区域的大小，可以是任意
	;比例的。
SceneManager_setViewSize	proto stdcall  _width:SDWORD,height:SDWORD

	;响应游戏的单击事件

	;本游戏用户的输入只有单击事件，通过这个函数来响应不同场景
	;下的单击事件。
SceneManager_onClick        proto stdcall  x:SDWORD, y:SDWORD

ENDIF