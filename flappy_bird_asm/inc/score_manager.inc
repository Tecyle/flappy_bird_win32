IFNDEF	_SCORE_MANAGER_INC_
_SCORE_MANAGER_INC_	equ	<1>

MedalType_golden	equ	0
MedalType_sliver	equ	1
MedalType_copper	equ	2
MedalType_none		equ	3
MedalType		typedef DWORD

ScoreManager_init					proto stdcall
ScoreManager_save					proto stdcall
ScoreManager_resetCurrentScore		proto stdcall
ScoreManager_clear					proto stdcall
ScoreManager_getCurrentScore		proto stdcall
ScoreManager_getCurrentName			proto stdcall
ScoreManager_setCurrentName	 		proto stdcall	_name : DWORD
ScoreManager_getHighScore			proto stdcall
ScoreManager_getMedalType			proto stdcall
ScoreManager_increaseScore			proto stdcall

ScoreManager_getNameByRank			proto stdcall	rank : SDWORD
ScoreManager_getScoreByRank			proto stdcall	rank : SDWORD

ENDIF