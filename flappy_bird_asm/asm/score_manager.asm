include		stdafx.inc
include		score_manager.inc

ScorePair		struct
	score		DWORD		?
	_name		db			64 dup(?)
ScorePair		ends
ScorePairPtr	typedef ptr ScorePair

ScoreManager	struct
	myScore		ScorePair	<>
	myRank		DWORD		?
	rankScore	ScorePair	10 dup(<>)
ScoreManager	ends
ScoreManagerPtr	typedef ptr ScoreManager

	.const
g_defPlayerName	db				"Player", 0
g_scoreFileName	db				"flappy_bird.scores", 0
	.data
g_scoreManager	ScoreManager	<>

	.code
ScorePair_copy			proc stdcall public uses eax ebx edx	dst : ScorePairPtr, src : ScorePairPtr
	assume	edx : ScorePairPtr
	assume	ebx : ScorePairPtr
	mov		edx, dst
	mov		ebx, src
	push	[ebx].score
	pop		[edx].score
	invoke	crt_strcpy, addr [edx]._name, addr [ebx]._name
	assume	edx : nothing
	assume	ebx : nothing
	ret
ScorePair_copy			endp

ScorePair_swap			proc stdcall public uses eax	o : ScorePairPtr, r : ScorePairPtr
	local	@t : ScorePair
	invoke	ScorePair_copy, addr @t, o
	invoke	ScorePair_copy, o, r
	invoke	ScorePair_copy, r, addr @t
	ret
ScorePair_swap			endp

ScoreManager_init		proc stdcall public uses eax ecx edx
	local	@hFile : HANDLE
	
	invoke	RtlZeroMemory, offset g_scoreManager.rankScore, 10 * (sizeof ScorePair)
	invoke	crt_strcpy, offset g_scoreManager.myScore._name, offset g_defPlayerName
	invoke	ScoreManager_resetCurrentScore

	invoke	CreateFileA, offset g_scoreFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	mov		@hFile, eax
	.if		eax == INVALID_HANDLE_VALUE
		ret
	.endif

	mov		ecx, 0
	.while	ecx < 9
		mov		eax, sizeof ScorePair
		mov		edx, 0
		mul		ecx
		add		eax, offset g_scoreManager.rankScore
		invoke	ReadFile, @hFile, eax, sizeof ScorePair, NULL, NULL
		inc		ecx
	.endw

	invoke		CloseHandle, @hFile
	ret
ScoreManager_init		endp

ScoreManager_save		proc stdcall public uses eax ecx edx
	local	@hFile : HANDLE

	invoke	CreateFileA, offset g_scoreFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
	mov		@hFile, eax
	mov		ecx, 0
	.while	ecx < 9
		mov		eax, sizeof ScorePair
		mov		edx, 0
		mul		ecx
		add		eax, offset g_scoreManager.rankScore
		invoke	WriteFile, @hFile, eax, sizeof ScorePair, NULL, NULL
		inc		ecx
	.endw
	invoke	CloseHandle, @hFile
	ret
ScoreManager_save		endp

ScoreManager_getCurrentScore	proc stdcall public
	mov		eax, g_scoreManager.myScore.score
	ret
ScoreManager_getCurrentScore	endp

ScoreManager_getCurrentName		proc stdcall public
	mov		eax, offset g_scoreManager.myScore._name
	ret
ScoreManager_getCurrentName		endp

ScoreManager_setCurrentName		proc stdcall public uses eax	_name : DWORD
	invoke	crt_strcpy, offset g_scoreManager.myScore._name, _name
	ret
ScoreManager_setCurrentName		endp

ScoreManager_getHighScore		proc stdcall public
	mov		eax, g_scoreManager.rankScore.score
	ret
ScoreManager_getHighScore		endp

ScoreManager_getMedalType		proc stdcall public
	.if		g_scoreManager.myRank == 1
		mov		eax, MedalType_golden
	.elseif	g_scoreManager.myRank == 2
		mov		eax, MedalType_sliver
	.elseif	g_scoreManager.myRank == 3
		mov		eax, MedalType_copper
	.else
		mov		eax, MedalType_none
	.endif
	ret
ScoreManager_getMedalType		endp

ScoreManager_increaseScore		proc stdcall public uses ebx ecx edx
	assume	ebx : ScorePairPtr
	inc		g_scoreManager.myScore.score
	mov		eax, sizeof ScorePair
	mov		edx, 0
	mov		ecx, g_scoreManager.myRank
	dec		ecx
	mul		ecx
	add		eax, offset g_scoreManager.rankScore
	mov		ebx, eax
	inc		[ebx].score

	mov		eax, sizeof ScorePair
	mov		edx, 0
	mov		ecx, g_scoreManager.myRank
	dec		ecx
	dec		ecx
	mul		ecx
	add		eax, offset g_scoreManager.rankScore
	mov		ebx, eax
	mov		eax, g_scoreManager.myScore.score
	.while	g_scoreManager.myRank > 1 && eax > [ebx].score
		mov		eax, sizeof ScorePair
		add		eax, ebx
		invoke	ScorePair_swap, eax, ebx
		dec		g_scoreManager.myRank
		mov		eax, sizeof ScorePair
		sub		ebx, eax
		mov		eax, g_scoreManager.myScore.score
	.endw

	assume	ebx : nothing
	mov		eax, g_scoreManager.myScore.score
	ret
ScoreManager_increaseScore		endp

ScoreManager_resetCurrentScore	proc stdcall public uses eax
	mov		g_scoreManager.myScore.score, 0
	mov		g_scoreManager.myRank, 10
	invoke	ScorePair_copy, (offset g_scoreManager.rankScore) + 9 * (sizeof ScorePair), offset g_scoreManager.myScore
	ret
ScoreManager_resetCurrentScore	endp

ScoreManager_clear				proc stdcall public uses eax
	invoke	RtlZeroMemory, offset g_scoreManager.rankScore, 10 * (sizeof ScorePair)
	ret
ScoreManager_clear				endp

ScoreManager_getNameByRank		proc stdcall public uses ebx edx	rank : SDWORD
	.if		rank < 1 || rank > 9
		mov		eax, NULL
	.else
		mov		eax, sizeof ScorePair
		mov		edx, 0
		mov		ebx, rank
		dec		ebx
		mul		ebx
		add		eax, offset g_scoreManager.rankScore
		mov		ebx, eax
		assume	ebx : ScorePairPtr
		lea		eax, [ebx]._name
		assume	ebx : nothing
	.endif
	ret
ScoreManager_getNameByRank		endp

ScoreManager_getScoreByRank		proc stdcall public uses ebx edx	rank : SDWORD
	.if		rank < 1 || rank > 9
		mov		eax, NULL
	.else
		mov		eax, sizeof ScorePair
		mov		edx, 0
		mov		ebx, rank
		dec		ebx
		mul		ebx
		add		eax, offset g_scoreManager.rankScore
		mov		ebx, eax
		assume	ebx : ScorePairPtr
		mov		eax, [ebx].score
		assume	ebx : nothing
	.endif
	ret
ScoreManager_getScoreByRank		endp

	end