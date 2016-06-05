#include "stdafx.h"
#include "score_manager.h"

typedef struct ScorePair
{
	size_t score;
	char name[64];
} ScorePair;

typedef struct ScoreManager 
{
	ScorePair myScore;
	size_t myRank;
	ScorePair rankScore[10];
} ScoreManager;

ScoreManager g_scoreManager;

void ScorePair_copy(ScorePair* dst, const ScorePair* src)
{
	dst->score = src->score;
	strcpy_s(dst->name, src->name);
}

void ScorePair_swap(ScorePair* o, ScorePair* r)
{
	ScorePair t;
	ScorePair_copy(&t, o);
	ScorePair_copy(o, r);
	ScorePair_copy(r, &t);
}

void ScoreManager_init()
{
	ScoreManager* o = &g_scoreManager;

	memset(o->rankScore, 0, sizeof(o->rankScore));
	strcpy_s(o->myScore.name, "Player");
	ScoreManager_resetCurrentScore();

	HANDLE hFile = CreateFileA("flappy_bird.scores", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	for (size_t i = 0; i < 9; ++i)
	{
		ReadFile(hFile, &o->rankScore[i], sizeof(ScorePair), NULL, NULL);
	}

	CloseHandle(hFile);
}

void ScoreManager_save()
{
	ScoreManager* o = &g_scoreManager;
	HANDLE hFile = CreateFileA("flappy_bird.scores", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	for (size_t i = 0; i < 9; ++i)
	{
		WriteFile(hFile, &o->rankScore[i], sizeof(ScorePair), NULL, NULL);
	}
	CloseHandle(hFile);
}

size_t ScoreManager_getCurrentScore()
{
	ScoreManager* o = &g_scoreManager;
	return o->myScore.score;
}

const char* ScoreManager_getCurrentName()
{
	ScoreManager* o = &g_scoreManager;
	return o->myScore.name;
}

void ScoreManager_setCurrentName(const char * name)
{
	ScoreManager* o = &g_scoreManager;
	strcpy(o->myScore.name, name);
}

size_t ScoreManager_getHighScore()
{
	ScoreManager* o = &g_scoreManager;
	return o->rankScore[0].score;
}

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