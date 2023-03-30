// vmain.cpp - Classes for main menu
// Author: Einar Valen, Date: 25.2.1994, Tested ok: 25.2.1994

// INCLUDES
#include "vwin.h"
#include "vwevents.h"
#include "vmain.h"
#include "vstd.h"
#include "vinfo.h"

// DEFINES
// TYPES
// GLOBAL VARIABLES
HINSTANCE ghInstance = NULL;
static int sgiSemaphore = 0;
static TvMain *sgptvMain = NULL;

// FUNCTIONS

#pragma argsused
static BOOL CALLBACK _export AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	if (message == WM_COMMAND && wParam == IDOK)
  	{
		EndDialog( hDlg, 0);
		return (OK);
    }
	return (FALSE);
	}

void vAboutDlg( void)
	{
	DLGPROC  lpfnDialog;
	lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)AboutDlgProc, ghInstance);
	DialogBox( ghInstance, "ABOUTDIALOG", NULL, (DLGPROC)lpfnDialog);
	FreeProcInstance( (FARPROC)lpfnDialog);
	}

#pragma argsused
static BOOL CALLBACK _export MainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	return (sgptvMain->bMainDlgProc( hDlg, message, wParam, lParam) );
	}

#pragma argsused
BOOL TvMain::bMainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
  	{
    case WM_INITDIALOG :
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
			SetMenu( hDlg, LoadMenu( ghInstance, (LPSTR)szMenu__) );
			return (TRUE);
      }
    case WM_PAINT :
      {
      PAINTSTRUCT ps;
      HDC         PaintDC;
      HICON       hIcon;
			if (!IsIconic( hDlg) ) // We need to paint the minimized icon ourselves
        return (FALSE);
			PaintDC = BeginPaint( hDlg, &ps);
      DefWindowProc( hDlg, WM_ICONERASEBKGND, (WPARAM)PaintDC, 0L);
			hIcon = LoadIcon( ghInstance, "RASMUS1");
      DrawIcon( PaintDC, 0, 0, hIcon);
      EndPaint( hDlg, &ps);
      return (TRUE);
      }
		case WM_COMMAND :
			switch (wParam)
				{
				case CM_HELP_ABOUT:
					vAboutDlg();
					return (OK);
				case CM_FILE_EXIT:
					EndDialog( hDlg, 0);
					return (OK);
				default:
					vDispatchCB( wParam);
					return (OK);
				}
		}
  return (FALSE);
	}

TvMain::TvMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine)
	{
	ghInstance = hInstance;
	hPrevInstance__ = hPrevInstance;
	lpCmdLine__ = lpCmdLine;
	}

void TvMain::vRun( char *szMnu)
	{
	szMenu__= szMnu;
	if (sgiSemaphore != 0) // Lock mek. for sgptMain
  	{
		TvInfo::vError( "Semaphore violated. Cannot run.", __FILE__, "TvMain", "Run");
		return;
    }
	sgiSemaphore++;
	lpfnDialog__ = (DLGPROC)MakeProcInstance( (FARPROC)MainDlgProc, ghInstance);
	sgptvMain = this;
	DialogBox( ghInstance, "VMAIN_DLG", NULL, (DLGPROC)lpfnDialog__);
	FreeProcInstance( (FARPROC)lpfnDialog__);
	sgiSemaphore--;
	}

// End of file

