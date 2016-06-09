#include "stdafx.h"
#include "rename_dialog.h"
#include "score_manager.h"

static char newName[64];

void _RenameDialog_initDialog(HWND hWnd)
{
	strcpy(newName, ScoreManager_getCurrentName());
	SetDlgItemTextA(hWnd, IDC_PLAYER_NAME, newName);
}

INT_PTR CALLBACK reNameDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		_RenameDialog_initDialog(hWnd);
		break;
	case WM_CLOSE:
		EndDialog(hWnd, IDCANCEL);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			GetDlgItemTextA(hWnd, IDC_PLAYER_NAME, newName, 64);
		case IDCANCEL:
			EndDialog(hWnd, wParam);
			break;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}
	return TRUE;
}

const char* RenameDialog_getName(HINSTANCE hInstance, HWND parent)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_RENAME), parent, reNameDlgProc);
	return newName;
}
