// vwinmisc.cpp - Miscallanous functions to simplify windows programming and
// make windows programs shorter
// Author: Einar Valen, Date: 22.10.1994, Not Tested ok:

// INCLUDES
#include <strstrea.h>
#include <string.h>
#include "vwinmisc.h"
#include "vstring.h"

// DEFINES

// TYPES

// FUNCTION PROTOTYPES

// GLOBAL VARIABLES

// FUNCTIONS

void pascal vPaintIconic( HWND hDlg, HINSTANCE hInstance)
	{
	PAINTSTRUCT ps;
	HDC         PaintDC;
	HICON       hIcon;
	PaintDC = BeginPaint( hDlg, &ps);
	DefWindowProc( hDlg, WM_ICONERASEBKGND, (WPARAM)PaintDC, 0L);
	hIcon = LoadIcon( hInstance, "RASMUS1");
	DrawIcon( PaintDC, 0, 0, hIcon);
	EndPaint( hDlg, &ps);
	}

void vCenterWindow( HWND hDlg)
	{
	RECT rc, rcDTop;
	int NewX, NewY, CenterX, CenterY;
	HWND hDTopWnd;
	hDTopWnd = GetDesktopWindow();
	GetWindowRect(hDTopWnd, &rcDTop);			// desktop rectangle size
	GetWindowRect(hDlg, &rc);		// dialog box rectangle size
	// Calculate window position in center
	CenterX = (rcDTop.left + rcDTop.right) / 2;
	CenterY = (rcDTop.top + rcDTop.bottom) / 2;
	NewX = CenterX - ( (rc.right - rc.left) / 2);
	NewY = CenterY - ( (rc.bottom - rc.top) / 2);
	// Give it a new location
	SetWindowPos( hDlg, NULL, NewX, NewY, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}


// Collect Texts from DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] and store in szBuf sep. by dFS
char *szGetEditCtrlsTexts( HWND hDlg, int iGeCtrl, int iLeCtrl, char *szBuf, size_t iBufLen)
	{
	ostrstream os( szBuf, iBufLen - 1);
	for (int i = iGeCtrl; i <= iLeCtrl; i++)
		{
		char sz[dBUFSIZ + 1];
		HWND hwndCtrl = GetDlgItem( hDlg, i);
		size_t iLen = 1 + (int)SendMessage( hwndCtrl, WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
		SendMessage( hwndCtrl, WM_GETTEXT, (WPARAM)(iLen > dBUFSIZ) ? dBUFSIZ : iLen, (LPARAM)(LPSTR)sz);
		os << sz << dFS;
		}
	os << ends;
	return (vStrJustLeft( vStrTrunc( szBuf) ) );
	}

// Sets texts on DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] stored in szBuf sep. by dFS
void vSetEditCtrlsTexts( HWND hDlg, int iGeCtrl, int iLeCtrl, char *szBuf)
	{
	int i;
	char *pszBuf, *psz;
	for (i = iGeCtrl, pszBuf = szBuf; i <= iLeCtrl; i++)
		{
		// Get first field from buffer
		if ( (psz = strchr( pszBuf, dFS) ) != NULL)
			{
			*psz = '\0';
			psz++;
			}
		// Display current field
		SendDlgItemMessage( hDlg, i, (WORD)WM_SETTEXT,	(WPARAM)0, (LPARAM)(LPSTR)pszBuf);
		// Get ready for next field
		if (psz != NULL)
			pszBuf = psz;
		}
	}

// Collect Texts from DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] and store in szBuf sep. by '","'.
char *szGetListBoxStr( HWND hDlg, UINT uiListCtrl, int iSelIdx, char *szBuf, size_t iBufLen)
	{
	HWND hwndCtrl = GetDlgItem( hDlg, uiListCtrl);
	if (SendMessage( hwndCtrl, LB_GETTEXTLEN, (WPARAM)iSelIdx, (LPARAM)0) >= iBufLen - 1)
		return ("szGetListBoxStr() -> Insuficient space in buffer");
	SendMessage( hwndCtrl, LB_GETTEXT, (WPARAM)iSelIdx, (LPARAM)(LPSTR)szBuf);
	for (size_t i = 0; i < iBufLen; i++)
		if (szBuf[i] == '\t')
			szBuf[i] = dFS;
	return (szBuf);
	}

