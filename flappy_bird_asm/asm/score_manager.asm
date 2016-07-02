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
ScorePair_copy			proc stdcall public uses eax ebx	dst : ScorePairPtr, src : ScorePairPtr
	assume	eax : ScorePairPtr
	assume	ebx : ScorePairPtr
	mov		eax, dst
	mov		ebx, src
	push	[ebx].score
	pop		[eax].score
	invoke	crt_strcpy, offset [eax]._name, offset [ebx]._name
	assume	eax : nothing
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
	mov		eax, offset g_scoreManager.myScore.name
	ret
ScoreManager_getCurrentName		endp

ScoreManager_setCurrentName		proc stdcall public uses eax	_name : ptr BYTE
	invoke	crt_strcpy, offset g_scoreManager.myScore._name, _name
	ret
ScoreManager_setCurrentName		endp

ScoreManager_getHighScore		proc stdcall public
	mov		eax, g_scoreManager.rankScore.score
	ret
ScoreManager_getHighScore		endp

MedalType ScoreManager_getMedalType()
{
	ScoreManager* o = &g_scoreManager;
	if (o->myRank == 1)
		return MedalType_golden;
	if (o->myRank == 2)
		return MedalType_sliver;
	if (o->myRank == 3)
		return MedalType_copper;
	return MedalType_none;
}

size_t ScoreManager_increaseScore()
{
	ScoreManager* o = &g_scoreManager;
	o->myScore.score++;
	o->rankScore[o->myRank - 1].score++;
	while (o->myRank > 1 && o->myScore.score > o->rankScore[o->myRank - 2].score)
	{
		ScorePair_swap(&o->rankScore[o->myRank - 1], &o->rankScore[o->myRank - 2]);
		o->myRank--;
	}
	return o->myScore.score;
}

void ScoreManager_resetCurrentScore()
{
	ScoreManager* o = &g_scoreManager;
	o->myScore.score = 0;
	o->myRank = 10;
	ScorePair_copy(&o->rankScore[9], &o->myScore);
}

void ScoreManager_clear()
{
	ScoreManager* o = &g_scoreManager;
	memset(o->rankScore, 0, sizeof(o->rankScore));
}

const char* ScoreManager_getNameByRank(int rank)
{
	ScoreManager* o = &g_scoreManager;
	if (rank < 1 || rank > 9)
		return NULL;
	return o->rankScore[rank - 1].name;
}
size_t ScoreManager_getScoreByRank(int rank)
{
	ScoreManager* o = &g_scoreManager;
	if (rank < 1 || rank > 9)
		return NULL;
	return o->rankScore[rank - 1].score;
}

	end