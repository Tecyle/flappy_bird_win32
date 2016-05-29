#include "stdafx.h"
#include "score_manager.h"

typedef struct ScoreManager 
{
	size_t highScore;
	size_t nowScore;
} ScoreManager;

ScoreManager g_scoreManager;

void ScoreManager_init()
{
	ScoreManager* o = &g_scoreManager;

	o->highScore = 0;
	o->nowScore = 0;
}

size_t ScoreManager_getCurrentScore()
{
	ScoreManager* o = &g_scoreManager;
	return o->nowScore;
}

size_t ScoreManager_getHighScore()
{
	ScoreManager* o = &g_scoreManager;
	return o->highScore;
}

MedalType ScoreManager_getMedalType()
{
	// undefined
	return MedalType_none;
}

size_t ScoreManager_increaseScore()
{
	ScoreManager* o = &g_scoreManager;
	o->nowScore++;
	if (o->nowScore > o->highScore)
		o->highScore = o->nowScore;
	return o->nowScore;
}