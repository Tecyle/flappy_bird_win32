include		stdafx.inc
include		scene_manager.inc
include		animation_manager.inc
include		physic_engine.inc
include		button.inc
include		score_manager.inc
include		sound_manager.inc
include		rank_dialog.inc

	.data
sce_mainMenu			Scene<>
sce_prepare				Scene<>
sce_playing				Scene<>
sce_gameOver			Scene<>

g_sceMgr				SceneManager<>
g_counterFrequency		LARGE_INTEGER<>

sptMainMenu				SpiritType			SpiritType_sky, \
											SpiritType_ground, \
											SpiritType_txtFlappyBird, \
											SpiritType_bird, \
											SpiritType_btRate, \
											SpiritType_btPlay, \
											SpiritType_btRank, \
											SpiritType_txtCopyRight, \
											SpiritType_blackFade

sptPrepare				SpiritType			SpiritType_sky, \
											SpiritType_ground, \
											SpiritType_txtGetReady, \
											SpiritType_helpInfo, \
											SpiritType_bird, \
											SpiritType_largeScore, \
											SpiritType_blackFade

sptPlaying				SpiritType			SpiritType_sky, \
											SpiritType_upPipes, \
											SpiritType_downPipes, \
											SpiritType_ground, \
											SpiritType_largeScore, \
											SpiritType_bird, \
											SpiritType_whiteFade

sptGameOver				SpiritType			SpiritType_sky, \
											SpiritType_upPipes, \
											SpiritType_downPipes, \
											SpiritType_ground, \
											SpiritType_bird, \
											SpiritType_txtGameOver, \
											SpiritType_scoreBoard, \
											SpiritType_btPlay, \
											SpiritType_btRank, \
											SpiritType_whiteFade, \
											SpiritType_blackFade
btMainMenuRate			Button<>
btMainMenuPlay			Button<>
btMainMenuRank			Button<>

btPrepareStart			Button<>

btPlayFly				Button<>

btGameOverReplay		Button<>
btGameOverRank			Button<>

btInMainMenu			ButtonPtr			offset btMainMenuRate, \
											offset btMainMenuPlay, \
											offset btMainMenuRank

btInPrepare				ButtonPtr			offset btPrepareStart

btInPlay				ButtonPtr			offset btPlayFly

btInGameOver			ButtonPtr			offset btGameOverReplay, \
											offset btGameOverRank

lastCount				LARGE_INTEGER		<<0, 0>>
fps_lastCount			DD					0
fps_lastFps				SDWORD				0
fps_strFps				DB					64 dup(0)
fps_strFpsFormat		DB					"FPS: %d", 0

g_scenes				ScenePtr			offset sce_mainMenu, \
											offset sce_prepare, \
											offset sce_playing, \
											offset sce_gameOver

g_spiritTypes			SpiritTypePtr		offset sptMainMenu, \
											offset sptPrepare, \
											offset sptPlaying, \
											offset sptGameOver

g_spiritNum				DWORD				9, 7, 7, 11

_SceneManager_fadeOut	proto stdcall	isBlack : BOOL, during : SDWORD
_SceneManager_fadeIn	proto stdcall	isBlack : BOOL, during : SDWORD

	.code
_Scene_initMainMenu		proc stdcall private uses eax
	; 当切换到主界面时，需要进行下面的初始化
	; 1. 随机选择天空和小鸟
	; 2. 设置小鸟的位置和动画
	; 3. 更新物理引擎设置
	invoke	ImageManager_randomSkyAndBird
	invoke	PhysicEngine_floatingBird
	invoke	PhysicEngine_showPipes, FALSE
	invoke	PhysicEngine_movingPipes, FALSE
	invoke	PhysicEngine_movingGround, TRUE
	ret
_Scene_initMainMenu		endp

_Scene_initPrepare		proc stdcall private uses eax
	invoke	ImageManager_randomSkyAndBird
	invoke	PhysicEngine_prepareBird
	invoke	PhysicEngine_movingPipes, FALSE
	invoke	PhysicEngine_showPipes, FALSE
	invoke	PhysicEngine_movingGround, TRUE
	invoke	ScoreManager_resetCurrentScore
	ret
_Scene_initPrepare		endp

_Scene_initPlaying		proc stdcall private uses eax
	invoke	PhysicEngine_freeBird
	invoke	PhysicEngine_showPipes, TRUE
	invoke	PhysicEngine_movingPipes, TRUE
	ret
_Scene_initPlaying		endp

_Scene_initGameOver		proc stdcall private uses eax
	assume	eax : ptr Spirit
	invoke	ImageManager_getSpirit, SpiritType_txtGameOver
	mov		[eax].visiable, FALSE
	invoke	ImageManager_getSpirit, SpiritType_scoreBoard
	mov		[eax].visiable, FALSE
	invoke	ImageManager_getSpirit, SpiritType_btPlay
	mov		[eax].visiable, FALSE
	invoke	ImageManager_getSpirit, SpiritType_btRank
	mov		[eax].visiable, FALSE
	invoke	PhysicEngine_movingPipes, FALSE
	invoke	PhysicEngine_movingGround, FALSE
	assume	eax : nothing
	ret
_Scene_initGameOver		endp

Scene_draw				proc stdcall public uses eax ebx ecx edx	o : ScenePtr
	assume	ebx : ptr Scene
	mov		ebx, o
	mov		ecx, 0
	mov		edx, [ebx].spirits
	.while	ecx < [ebx].spiritCount
		invoke	ImageManager_drawSpirit, [edx]
		add		edx, sizeof SpiritPtr
		inc		ecx
	.endw
	assume	ebx : nothing
	ret
Scene_draw				endp

Scene_init				proc stdcall public uses eax	scene : SceneType
	mov		eax, scene
	.if		eax == SceneType_mainMenu
		invoke	_Scene_initMainMenu
	.elseif	eax == SceneType_prepare
		invoke	_Scene_initPrepare
	.elseif	eax == SceneType_playing
		invoke	_Scene_initPlaying
	.elseif	eax == SceneType_gameOver
		invoke	_Scene_initGameOver
	.endif
	ret
Scene_init				endp

;////////////////////////////////////////////////////////////////////////
; Button onClick Functions
MainMenuRate_click		proc stdcall public uses eax
	mov		eax, g_sceMgr.showFps
	not		eax
	mov		g_sceMgr.showFps, eax
	ret
MainMenuRate_click		endp

MainMenuPlay_click		proc stdcall public uses eax
	invoke	SoundManager_playSound, SoundType_swooshing
	invoke	_SceneManager_fadeOut, TRUE, 500
	mov		g_sceMgr.currentScene, SceneType_prepare
	invoke	Scene_init, SceneType_prepare
	invoke	_SceneManager_fadeIn, TRUE, 500
	ret
MainMenuPlay_click		endp

MainMenuRank_click		proc stdcall public uses eax
	invoke	RankDialog_doModal, g_sceMgr.hInstance, g_sceMgr.hWnd
	ret
MainMenuRank_click		endp

PlayFly_click			proc stdcall public uses eax
	invoke	SoundManager_playSound, SoundType_wing
	invoke	PhysicEngine_birdFly
	ret
PlayFly_click			endp

PrepareStart_click		proc stdcall public uses eax
	mov		g_sceMgr.currentScene, SceneType_playing
	invoke	Scene_init, SceneType_playing
	invoke	SceneManager_render
	.while	eax == FALSE
		invoke	PlayFly_click
		invoke	SceneManager_render
	.endw
	ret
PrepareStart_click		endp

GameOverReplay_click	proc stdcall public uses eax
	invoke	SoundManager_playSound, SoundType_swooshing
	invoke	_SceneManager_fadeOut, TRUE, 500
	mov		g_sceMgr.currentScene, SceneType_prepare
	invoke	Scene_init, SceneType_prepare
	invoke	_SceneManager_fadeIn, TRUE, 500
	ret
GameOverReplay_click	endp

GameOverRank_click		proc stdcall public uses eax
	invoke	MainMenuRank_click
	ret
GameOverRank_click		endp

;////////////////////////////////////////////////////////////////////////
; SceneManager
_SceneManager_needRedraw	proc stdcall public uses edx
	local	frameInterval : DWORD
	local	nowCount : LARGE_INTEGER
	local	tpCount : LARGE_INTEGER
	
	mov		edx, g_counterFrequency.HighPart
	mov		eax, g_counterFrequency.LowPart
	div		g_sceMgr.fps
	mov		frameInterval, eax
	invoke	QueryPerformanceCounter, addr nowCount
	push	nowCount.HighPart
	pop		tpCount.HighPart
	push	nowCount.LowPart
	pop		tpCount.LowPart
	mov		edx, tpCount.LowPart
	mov		eax, lastCount.LowPart
	sub		edx, eax
	mov		tpCount.LowPart, edx
	.if		CARRY?
		mov		edx, tpCount.HighPart
		dec		edx
		mov		tpCount.HighPart, edx
	.endif
	mov		edx, tpCount.HighPart
	mov		eax, lastCount.HighPart
	sub		edx, eax
	mov		tpCount.HighPart, edx
	mov		eax, tpCount.LowPart
	.if		tpCount.HighPart > 0 || eax > frameInterval
		push	nowCount.HighPart
		pop		lastCount.HighPart
		push	nowCount.LowPart
		pop		lastCount.LowPart
		mov		eax, TRUE
		ret
	.endif
	mov		eax, FALSE
	ret
_SceneManager_needRedraw	endp

_SceneManager_drawScene		proc stdcall private uses eax ecx edx
	mov		eax, sizeof ScenePtr
	mov		edx, 0
	mov		ecx, g_sceMgr.currentScene 
	mul		ecx
	add		eax, offset g_scenes
	invoke	Scene_draw, [eax]
	ret
_SceneManager_drawScene		endp

; 此函数获取的帧速率应该是实时统计的结果
_SceneManager_getFps		proc stdcall private uses ebx ecx edx
	local	@nowCount : DWORD

	invoke	GetTickCount
	mov		@nowCount, eax
	sub		eax, fps_lastCount
	.if		eax > 1000
		mov		ebx, eax
		mov		eax, 1000
		mov		edx, 0
		mov		ecx, g_sceMgr.drawCounter
		mul		ecx 
		mov		edx, 0
		div		ebx
		mov		fps_lastFps, eax
		push	@nowCount
		pop		fps_lastCount
		mov		g_sceMgr.drawCounter, 0
	.endif
	mov		eax, fps_lastFps
	ret
_SceneManager_getFps		endp

_SceneManager_drawFps		proc stdcall private uses eax
	invoke	_SceneManager_getFps
	invoke	wsprintfA, addr fps_strFps, addr fps_strFpsFormat, eax
	invoke	crt_strlen, addr fps_strFps
	invoke	TextOutA, g_sceMgr.bufHdc, 5, 5, addr fps_strFps, eax
	ret
_SceneManager_drawFps		endp

_SceneManager_switchToGameOver	proc stdcall private uses eax ebx edx
	assume	ebx : ptr Spirit
	invoke	_SceneManager_fadeOut, FALSE, 250
	invoke	Scene_init, SceneType_gameOver
	mov		g_sceMgr.currentScene, SceneType_gameOver
	invoke	_SceneManager_fadeIn, FALSE, 250
	invoke	PhysicEngine_isBirdStopped
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	PhysicEngine_isBirdStopped
	.endw
	; 菜单进入动画
	; GameOver
	invoke	ImageManager_getSpirit, SpiritType_txtGameOver
	mov		ebx, eax
	mov		edx, [ebx].ani
	invoke	TransAnimation_init, edx, 144, 110, 144, 150, 200, FALSE
	mov		[ebx].visiable, TRUE
	invoke	TransAnimation_finished, edx
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	TransAnimation_finished, edx
	.endw
	invoke	TransAnimation_init, edx, 144, 150, 144, 140, 200, FALSE
	invoke	TransAnimation_finished, edx
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	TransAnimation_finished, edx
	.endw
	invoke	TransAnimation_init, edx, 144, 140, 144, 150, 200, FALSE
	invoke	TransAnimation_finished, edx
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	TransAnimation_finished, edx
	.endw
	; 计分板
	invoke	ImageManager_getSpirit, SpiritType_scoreBoard
	mov		ebx, eax
	mov		edx, [ebx].ani
	mov		[ebx].visiable, TRUE
	invoke	TransAnimation_init, edx, 144, 571, 144, 270, 500, FALSE
	invoke	SoundManager_playSound, SoundType_swooshing
	invoke	TransAnimation_finished, edx
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	TransAnimation_finished, edx
	.endw
	; 显示按钮
	invoke	ImageManager_getSpirit, SpiritType_btPlay
	mov		ebx, eax
	mov		[ebx].visiable, TRUE
	invoke	ImageManager_getSpirit, SpiritType_btRank
	mov		ebx, eax
	mov		[ebx].visiable, TRUE
	assume	ebx : nothing
	ret
_SceneManager_switchToGameOver	endp

_SceneManager_tick				proc stdcall private
	; 执行游戏逻辑
	invoke	PhysicEngine_tick
	; 更新动画
	invoke	AnimationManager_tick
	; 执行小鸟计分
	.if		g_sceMgr.currentScene == SceneType_playing
		invoke	PhysicEngine_passedPipe
		.if		eax == TRUE
			invoke	ScoreManager_increaseScore
			invoke	SoundManager_playSound, SoundType_point
		.endif
	.endif
	; 执行游戏结束判断，为了防止间接递归调用，通过 onSwitching 进行标记
	invoke	PhysicEngine_isBirdDead
	.if		g_sceMgr.onSwitching == FALSE && g_sceMgr.currentScene == SceneType_playing && eax == TRUE
		mov		g_sceMgr.onSwitching, TRUE
		invoke	SoundManager_playSound, SoundType_hit
		invoke	SoundManager_playSound, SoundType_die
		invoke	_SceneManager_switchToGameOver
		mov		g_sceMgr.onSwitching, FALSE
	.endif
	ret
_SceneManager_tick				endp

_SceneManager_initAllScene	proc stdcall private uses eax ebx ecx edx
	assume	ebx : ptr Scene
	mov		ecx, 0
	.while	ecx < 4
		mov		eax, sizeof ScenePtr
		mov		edx, 0
		mul		ecx
		mov		ebx, offset g_scenes
		add		ebx, eax							; ebx 是指向 g_scenes 的指针
		mov		ebx, ScenePtr ptr [ebx]				; ebx 是指向 g_scenes[i] 的指针

		mov		eax, sizeof SpiritTypePtr
		mov		edx, 0
		mul		ecx
		add		eax, offset g_spiritTypes
		push	[eax]
		pop		[ebx].spirits

		mov		eax, sizeof	DWORD
		mov		edx, 0
		mul		ecx
		add		eax, offset g_spiritNum
		push	[eax]
		pop		[ebx].spiritCount

		inc		ecx
	.endw
	assume	ebx : nothing
	ret
_SceneManager_initAllScene	endp

_SceneManager_checkOnClickButtons	proc stdcall private uses eax ebx ecx	buttons : ptr ButtonPtr, btNum : DWORD, x : SDWORD, y : SDWORD
	mov		ecx, 0
	assume	ebx : ptr Button
	mov		ebx, buttons
	.while	ecx < btNum
		push	ebx
		mov		ebx, ButtonPtr ptr [ebx]
		invoke	Button_isHit, ebx, x, y
		.if		[ebx].enabled != FALSE && eax
			invoke	[ebx].onClick
			ret
		.endif
		pop		ebx
		inc		ecx
		add		ebx, sizeof ButtonPtr
	.endw
	assume	ebx : nothing
	ret
_SceneManager_checkOnClickButtons	endp

_SceneManager_fadeOut		proc stdcall private uses eax ebx ecx	isBlack : BOOL, during : SDWORD
	assume	ebx : ptr Spirit
	.if		isBlack == FALSE
		invoke	ImageManager_getSpirit, SpiritType_whiteFade
	.else
		invoke	ImageManager_getSpirit, SpiritType_blackFade
	.endif
	mov		ebx, eax
	mov		ecx, [ebx].ani

	invoke	FadeAnimation_init, ecx, 0, 255, during, FALSE

	invoke	FadeAnimation_finished, ecx
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	FadeAnimation_finished, ecx
	.endw
	assume	ebx : nothing
	ret
_SceneManager_fadeOut		endp

_SceneManager_fadeIn		proc stdcall private uses eax ebx ecx	isBlack : BOOL, during : SDWORD
	assume	ebx : ptr Spirit
	.if		isBlack == FALSE
		invoke	ImageManager_getSpirit, SpiritType_whiteFade
	.else
		invoke	ImageManager_getSpirit, SpiritType_blackFade
	.endif
	mov		ebx, eax
	mov		ecx, [ebx].ani

	invoke	FadeAnimation_init, ecx, 255, 0, during, FALSE

	invoke	FadeAnimation_finished, ecx
	.while	eax == FALSE
		invoke	SceneManager_render
		invoke	FadeAnimation_finished, ecx
	.endw
	assume	ebx : nothing
	ret
_SceneManager_fadeIn		endp

SceneManager_init			proc stdcall public uses eax	hInstance : HINSTANCE, hWnd : HWND, hdc : HDC
	; 初始化 SceneManager 对象
	mov		g_sceMgr.currentScene, SceneType_mainMenu
	push	hInstance
	pop		g_sceMgr.hInstance
	push	hWnd
	pop		g_sceMgr.hWnd
	push	hdc
	pop		g_sceMgr.scrHdc
	invoke	CreateCompatibleDC, hdc
	mov		g_sceMgr.bufHdc, eax
	invoke	CreateCompatibleBitmap, hdc, SCENE_WIDTH, SCENE_HEIGHT
	mov		g_sceMgr.drawingBoard, eax
	invoke	SelectObject, g_sceMgr.bufHdc, g_sceMgr.drawingBoard
	invoke	SetTextColor, g_sceMgr.bufHdc, 0000ffh
	invoke	SetBkColor, g_sceMgr.bufHdc, 0
	invoke	SetBkMode, g_sceMgr.bufHdc, TRANSPARENT
	mov		g_sceMgr.fps, 60
	mov		g_sceMgr.drawCounter, 0
	mov		g_sceMgr.showFps, TRUE
	mov		g_sceMgr.onSwitching, FALSE
	invoke	QueryPerformanceFrequency, offset g_counterFrequency
	; 初始化游戏需要的各个部件，包括：
	invoke	ScoreManager_init							; 初始化计分管理器
	invoke	AnimationManager_init						; 初始化动画引擎
	invoke	ImageManager_initAll, hInstance, g_sceMgr.bufHdc		; 初始化图像资源管理器
	invoke	PhysicEngine_init							; 初始化物理引擎
	invoke	SoundManager_init, hInstance
	; 初始化场景
	invoke	_SceneManager_initAllScene
	; 初始化按钮
	invoke	Button_construct, offset btMainMenuRate, 144-31, 144+31, 285-18, 285+18, MainMenuRate_click
	invoke	Button_construct, offset btMainMenuPlay, 72-52, 72+52, 371-29, 371+29, MainMenuPlay_click
	invoke	Button_construct, offset btMainMenuRank, 212-52, 212+52, 371-29, 371+29, MainMenuRank_click

	invoke	Button_construct, offset btPrepareStart, 0, SCENE_WIDTH, 0, SCENE_HEIGHT, PrepareStart_click
	
	invoke	Button_construct, offset btPlayFly, 0, SCENE_WIDTH, 0, SCENE_HEIGHT, PlayFly_click
	
	invoke	Button_construct, offset btGameOverReplay, 72-52, 72+52, 371-29, 371+29, GameOverReplay_click
	invoke	Button_construct, offset btGameOverRank, 212-52, 212+52, 371-29, 371+29, GameOverRank_click
	; 初始化需要显示的第一个场景
	invoke	_Scene_initMainMenu
	ret
SceneManager_init			endp

SceneManager_render			proc stdcall public uses eax ebx ecx edx
	; 判断是否需要重绘，如果没有到达绘制时间，则不进行绘制
	; 这样可以保证帧速率稳定
	invoke	_SceneManager_needRedraw
	.if		eax == FALSE
		ret
	.endif

	inc		g_sceMgr.drawCounter

	; 执行游戏逻辑，主要包括更新游戏的物理引擎，从而计算所
	; 有物体的新位置，更新动画引擎，更新所有动画
	invoke	_SceneManager_tick

	; 绘制计算完后的画面
	invoke	_SceneManager_drawScene

	; 绘制帧速率
	.if		g_sceMgr.showFps != FALSE
		invoke	_SceneManager_drawFps
	.endif

	; 翻转缓存图像到前台显示
	invoke	StretchBlt, g_sceMgr.scrHdc, 0, 0, g_sceMgr.windowWidth, g_sceMgr.windowHeight, g_sceMgr.bufHdc, 0, 0, SCENE_WIDTH, SCENE_HEIGHT, SRCCOPY
	mov		eax, TRUE
	ret
SceneManager_render			endp

SceneManager_setViewSize	proc stdcall public uses eax	_width : SDWORD, height : SDWORD
	push	_width
	pop		g_sceMgr.windowWidth
	push	height
	pop		g_sceMgr.windowHeight
	ret
SceneManager_setViewSize	endp

SceneManager_onClick		proc stdcall public uses eax	x : SDWORD, y : SDWORD
	.if		g_sceMgr.currentScene == SceneType_mainMenu
		invoke	_SceneManager_checkOnClickButtons, addr btInMainMenu, 3, x, y
	.elseif	g_sceMgr.currentScene == SceneType_prepare
		invoke	_SceneManager_checkOnClickButtons, addr btInPrepare, 1, x, y
	.elseif	g_sceMgr.currentScene == SceneType_playing
		invoke	_SceneManager_checkOnClickButtons, addr btInPlay, 1, x, y
	.elseif	g_sceMgr.currentScene == SceneType_gameOver
		invoke	_SceneManager_checkOnClickButtons, addr btInGameOver, 2, x, y
	.endif
	ret
SceneManager_onClick		endp

SceneManager_destruct		proc stdcall public uses eax
	invoke	ScoreManager_save
	ret
SceneManager_destruct		endp

	end