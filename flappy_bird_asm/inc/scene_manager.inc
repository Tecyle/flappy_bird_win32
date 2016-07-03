IFNDEF	_SCENE_MANAGER_INC_
_SCENE_MANAGER_INC_	equ	<1>

include		image_manager.inc
;// ������Ϸ��������
;// ��Ϸһ��ֻ������ 4 ��������
;// 1. ���˵�
;// 2. ׼����ʼ����
;// 3. ��Ϸ����
;// 4. ��Ϸ��������
SceneType_mainMenu	equ 1				;///< ���˵�
SceneType_prepare	equ 2				;///< ׼����ʼ����
SceneType_playing	equ 3				;///< ��Ϸ����
SceneType_gameOver	equ 4				;///< ��Ϸ��������
SceneType			typedef DWORD

;// ������Ϸ����
;// ��Ϸ����ָ����һ����������Ҫ������Щ����
;// ��������һ������(Spirit)������
Scene				struct
	spirits			SpiritTypePtr	?
	spiritCount		DWORD			?
Scene				ends
ScenePtr			typedef ptr Scene
;// ���������࣬������Ϸ�ĳ������л��������� main ������Ψһ�ӿ�
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

	;��ʼ����������������������Ϸ��ʼ֮ǰִ��
	
	;hdc : ���ڵĻ�ͼ�����

	;Ϊ�˱��⶯̬�ڴ��ʹ�ã�������ʹ�õ�����ȫ�Ǿ�̬���飬
	;����û������ CleanUp �Ĺ��ܡ�
SceneManager_init			proto stdcall hInstance:HINSTANCE, hWnd:HWND, hdc:HDC 
SceneManager_destruct		proto stdcall

	;������Ϸ����

	;�����ǰ���ÿ�� 60 ֡���ٶȽ��еģ�Ҳ����˵��һ���ڵ���
	;�ú�����ǧ�Σ��ú���Ҳֻ������һ����ֻ���� 60 �Ρ�
	;return ���� true �������ε��û�����ͼ�Σ����� false 
	;���������ǿյ��á�
SceneManager_render			proto stdcall

	;������ͼ�Ĵ�С

	;��ͼ�Ĵ�СҲ���������û������Ļ�ͼ����Ĵ�С������������
	;�����ġ�
SceneManager_setViewSize	proto stdcall  _width:SDWORD,height:SDWORD

	;��Ӧ��Ϸ�ĵ����¼�

	;����Ϸ�û�������ֻ�е����¼���ͨ�������������Ӧ��ͬ����
	;�µĵ����¼���
SceneManager_onClick        proto stdcall  x:SDWORD, y:SDWORD

ENDIF