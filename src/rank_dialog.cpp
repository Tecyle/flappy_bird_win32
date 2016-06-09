#include "stdafx.h"
#include "rank_dialog.h"
#include "score_manager.h"
#include "rename_dialog.h"

static HINSTANCE g_hInstance;

void _RankDialog_onInitDialog(HWND hWnd)
{
	static int idNames[] = { 
		IDC_NAME1, IDC_NAME2, IDC_NAME3,
		IDC_NAME4, IDC_NAME5, IDC_NAME6,
		IDC_NAME7, IDC_NAME8, IDC_NAME9
	};
	static int idScores[] = {
		IDC_SCORE1, IDC_SCORE2, IDC_SCORE3,
		IDC_SCORE4, IDC_SCORE5, IDC_SCORE6,
		IDC_SCORE7, IDC_SCORE8, IDC_SCORE9
	};
	char buffer[64];
	const char* noName = "<无记录>";

	for (int i = 0; i < 9; ++i)
	{
		const char* player = ScoreManager_getNameByRank(i + 1);
		size_t score = ScoreManager_getScoreByRank(i + 1);
		if (player == NULL || *player == 0)
		{	
			player = noName;
			score = 0;
		}
		SetDlgItemTextA(hWnd, idNames[i], player);
		wsprintfA(buffer, "%u", score);
		SetDlgItemTextA(hWnd, idScores[i], buffer);
	}
}

INT_PTR CALLBACK rankDialogFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		_RankDialog_onInitDialog(hWnd);
		break;
	case WM_CLOSE:
		EndDialog(hWnd, NULL);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			EndDialog(hWnd, IDOK);
			break;
		case IDB_RENAME:
			ScoreManager_setCurrentName(RenameDialog_getName(g_hInstance, hWnd));
			break;
		case IDB_CLEAR:
			if (MessageBox(NULL, L"清空记录之后将无法撤销，确定要清空吗？", L"Are you sure?", MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				ScoreManager_clear();
				_RankDialog_onInitDialog(hWnd);
			}
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

int RankDialog_doModal(HINSTANCE hInstance, HWND parent)
{
	g_hInstance = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_RANK), parent, rankDialogFunc);
	return 0;
}
