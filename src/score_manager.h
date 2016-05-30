#pragma once

typedef enum MedalType
{
	MedalType_golden = 0,
	MedalType_sliver = 1,
	MedalType_copper = 2,
	MedalType_none = 3
} MedalType;

void ScoreManager_init();
void ScoreManager_resetCurrentScore();
size_t ScoreManager_getCurrentScore();
size_t ScoreManager_getHighScore();
MedalType ScoreManager_getMedalType();
size_t ScoreManager_increaseScore();