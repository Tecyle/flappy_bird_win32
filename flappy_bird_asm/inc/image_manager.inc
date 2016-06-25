;include		animation_manager.inc

NumberSize_large			equ  0		; ����֣�������Ϸ��������ʾ��ǰ����
NumberSize_middle			equ  1		; �к��֣�������Ϸ��������ʾ����
NumberSize_small			equ  2		; С���֣���������û���õ�

NumberAlign_left			equ  0		; ����룬��Ϸ��û���õ����ֶ��䷽ʽ
NumberAlign_center			equ  1		; ���ж���
NumberAlign_right			equ  2		; �Ҷ���

SpiritType_ground			equ  0		; ����
SpiritType_sky				equ  1		; �������
SpiritType_bird				equ  2		; С��
SpiritType_upPipes			equ  3		; �ϲ�ܵ���
SpiritType_downPipes		equ  4		; �²�ܵ���
SpiritType_txtGetReady		equ  5		; GetReady ����
SpiritType_txtGameOver		equ  6		; GameOver ����
SpiritType_txtCopyRight		equ  7		; ��Ȩ������Ϣ
SpiritType_txtFlappyBird	equ  8		; FlappyBird ����
SpiritType_largeScore		equ  9		; ��ǰ�÷�����
SpiritType_helpInfo			equ 10		; ������Ϣ
SpiritType_btPlay			equ 11		; ��ʼ��Ϸ��ť
SpiritType_btRank			equ 12		; ������ť
SpiritType_btRate			equ 13		; Rate ��ť
SpiritType_scoreBoard		equ 14		; �Ƿ���
SpiritType_blackFade		equ 15		; ��ɫ��������
SpiritType_whiteFade		equ 16		; ��ɫ��������

Image			struct
	x			SDWORD			?	; ͼ���������� x ����
	y			SDWORD			?	; ͼ���������� y ����
	_width		SDWORD			?	; ͼ���������Ŀ���
	height		SDWORD			?	; ͼ���������ĸ߶�
Image			ends
ImagePtr		typedef ptr Image

Spirit			struct
	image		ImagePtr		?	; ����Ļ���ͼ��
	_cx			SDWORD			?	; ��������� x ����
	cy			SDWORD			?	; ��������� y ����
	ox			SDWORD			?	; �����ԭʼ���� x ���꣬�������������õ�
	oy			SDWORD			?	; �����ԭʼ���� y ���꣬�������������õ�
	angle		REAL8			?	; �������ת�Ƕȣ�Ŀǰ��С����Ч
	halfWidth	SDWORD			?	; ����Ŀ��ȵ�һ��
	halfHeight	SDWORD			?	; ����ĸ߶ȵ�һ��
	visiable	BOOLEAN			?	; �����Ƿ�ɼ�
	;ani			AnimationPtr	?	; ���ƾ���Ķ���
Spirit			ends
SpiritPtr		typedef	ptr Spirit

ImageManager	struct 
	hInstance	HINSTANCE		?	; Ӧ�ó����ʵ�����
	dstHdc		HDC				?	;
	imgScene	HBITMAP			?	; ͼƬ��Դ������������ BMP
	imgHdc		HDC				?	; ͼ����ƾ��
ImageManager	ends

Spirit_construct				proto stdcall	o : SpiritPtr, img : ImagePtr, _cx : SDWORD, cy : SDWORD

ImageManager_initAll			proto stdcall	hInstance : HINSTANCE, hdc : HDC
ImageManager_randomSkyAndBird	proto stdcall
ImageManager_drawSpirit			proto stdcall	spirit : DWORD
ImageManager_drawFadeCover		proto stdcall	img : ImagePtr, alpha : BYTE
ImageManager_drawNumber			proto stdcall	num : DWORD, _cx : SDWORD, cy : SDWORD, _size : DWORD, _align : DWORD

ImageManager_getSpirit			proto stdcall	spirit : DWORD