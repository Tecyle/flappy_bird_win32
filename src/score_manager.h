#pragma once

typedef enum MedalType
{
	MedalType_golden = 0,
	MedalType_sliver = 1,
	MedalType_copper = 2,
	MedalType_none = 3
} MedalType;

void ScoreManager_init();
void ScoreManager_save();
void ScoreManager_resetCurrentScore();
void ScoreManager_clear();
size_t ScoreManager_getCurrentScore();
const char* ScoreManager_getCurrentName();
void ScoreManager_setCurrentName(const char* name);
size_t ScoreManager_getHighScore();
MedalType ScoreManager_getMedalType();
size_t ScoreManager_increaseScore();

const char* ScoreManager_getNameByRank(int rank);
size_t ScoreManager_getScoreByRank(int rank);