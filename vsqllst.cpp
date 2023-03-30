// vsqllst.cpp - Classes for main menu
// Author: Einar Valen, Date: 6.3.1994, Tested ok: DD.MM.YYYY.

// INCLUDES
#include <strstream.h>
#include <string.h>
#include "vwin.h"
#include "vwevents.h"
#include "vstd.h"
#include "vinfo.h"
#include "vmain.h"
#include "vsqllst.h"

// DEFINES
#define MAX_TABSTOPS 100
#define MAX_RECORD_LIST 100
// TYPES

// GLOBAL VARIABLES
static int sgiSemaphore = 0;	// Sperre-mekanisme for endring av sgtvSqlst
static TvSqlLst *sgptvSqlLst = NULL;

// FUNCTIONS
#pragma argsused
static BOOL CALLBACK _export sbDefDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	if (message == WM_COMMAND && wParam == IDOK)
  	{
		EndDialog( hDlg, 0);
		return (OK);
    }
	return (FALSE);
	}

#pragma argsused
static BOOL CALLBACK _export MainLstDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	return (sgptvSqlLst->bMainDlgProc( hDlg, message, wParam, lParam) );
	}

BOOL TvSqlLst::bRun( char *sz, int iLen)
	{
	DLGPROC  lpfnDialog;
	if (sgiSemaphore != 0) // Sjekk sperre for endring av sgtvSqlst
  	{
		TvInfo::vError( "Semaphore violated. Cannot run.", __FILE__, "TvMain", "Run");
		return (FAIL);
    }
	sgiSemaphore++; // Sett sperre for endring av sgtvSqlst
	sgptvSqlLst = this;
	if (NULL == (lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)MainLstDlgProc, ghInstance) ) )
		TvInfo::vNotice( "Error running dialog box (1)");
	if (-1 == DialogBox( ghInstance, "VSQLLSTDLG", NULL, (DLGPROC)lpfnDialog) )
		TvInfo::vNotice( "Error running dialog box (2)");
	FreeProcInstance( (FARPROC)lpfnDialog);
	sgiSemaphore--;	// Fjern sperre for endring av sgtvSqlst
  if (bSelectionWasMade)
		strncpy( sz, szBuffer, iLen);
  sz[iLen - 1] = '\0';            
	return (bSelectionWasMade); 
	}

void TvSqlLst::vSetTabStops( int *aiTabStops, short iMaxNumberOfTabStops)
	{
	if (iMaxNumberOfTabStops <= 0)
  	return;
	DWORD wUnits = GetDialogBaseUnits();
	for (int i = 0; i < iMaxNumberOfTabStops-1; i++)
		aiTabStops[i] *= LOWORD( wUnits);
	SendDlgItemMessage( hDlg__, CM_LISTBOX, (WORD)LB_SETTABSTOPS,	(WPARAM)iMaxNumberOfTabStops, (LPARAM)aiTabStops);
	// Setup horizontal scroll width in listbox
	int iExtent = aiTabStops[iMaxNumberOfTabStops - 1] + (LOWORD( wUnits) * 30);
	SendDlgItemMessage( hDlg__, CM_LISTBOX, (WORD)LB_SETHORIZONTALEXTENT,	(WPARAM)iExtent, (LPARAM)0L);
	}

#pragma argsused
BOOL TvSqlLst::bMainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
      hDlg__ = hDlg;
      bSelectionWasMade = FALSE;
			vInitCB();
			vSqlQueryCB();
			vFillList();
			SendDlgItemMessage( hDlg__, CM_LISTBOX, (WORD)LB_SETCURSEL, (WPARAM)0, (LPARAM)0L);
			return (TRUE);
      }
		case WM_COMMAND :
			{
			DWORD dwIndex;
			DLGPROC  lpfnDialog;
			vSetInfo( (BYTE *)" ");
			switch (wParam)
				{
				case CM_LISTBOX:
					if (HIWORD( lParam) != LBN_DBLCLK)
          	break;
				case IDOK:
					DWORD dwIndex;
					if ( (dwIndex = SendDlgItemMessage( hDlg__, CM_LISTBOX, LB_GETCURSEL, 0, 0) ) != LB_ERR)
						{
						bSelectionWasMade = TRUE;
						SendDlgItemMessage( hDlg__, CM_LISTBOX, LB_GETTEXT, (WPARAM)dwIndex, (LPARAM)(LPSTR)szBuffer);
						}
				case IDCANCEL:
					EndDialog( hDlg, 0);
					return (OK);
				case CM_HELP_ABOUT:
					vAboutDlg();
					return (OK);
				case CM_HELP_KEYBOARD:
					lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)sbDefDlgProc, ghInstance);
					DialogBox( ghInstance, "VSQLLISTKEYBOARDHELP", NULL, (DLGPROC)lpfnDialog);
					FreeProcInstance( (FARPROC)lpfnDialog);
					return (OK);
				case IDHELP:
					lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)sbDefDlgProc, ghInstance);
					DialogBox( ghInstance, "VSQLLISTCOMMANDSHELP", NULL, (DLGPROC)lpfnDialog);
					FreeProcInstance( (FARPROC)lpfnDialog);
					return (OK);
				case CM_FILE_MODIFY:
					if ( (dwIndex = SendDlgItemMessage( hDlg__, CM_LISTBOX, LB_GETCURSEL, 0, 0) ) != LB_ERR)
          	{
						SendDlgItemMessage( hDlg__, CM_LISTBOX, LB_GETTEXT, (WPARAM)dwIndex, (LPARAM)(LPSTR)szBuffer);
						vSqlModifyCB( (int)dwIndex);
            }
          return (OK);
				case CM_FILE_ADD:
					vSqlInsertCB();
          return (OK);
				case CM_BROWSE_SEARCH:
					if ( !bModifyQueryCB() )
          	return (OK);
				case CM_FILE_COMMIT:
				case CM_BROWSE_FRST:
					vSqlCommitCB();
					vSqlQueryCB();
				case CM_BROWSE_NEXT:
					SendDlgItemMessage( hDlg__, CM_LISTBOX, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0L);
					vFillList();
					SendDlgItemMessage( hDlg__, CM_LISTBOX, (WORD)LB_SETCURSEL, (WPARAM)0, (LPARAM)0L);
					return (OK);
				}
			return (FALSE);
      }
		}
	return (FALSE);
	}

void TvSqlLst::vFillList( void)
	{
	char sz[VLIST_BUFFERSIZE];
	for (int i = 1; (i < MAX_RECORD_LIST) && bSqlFetchCB( sz); i++)
		vAppendString( sz);
	}

void TvSqlLst::vAppendString( char *sz)
	{
	SendDlgItemMessage( hDlg__, CM_LISTBOX, (WORD)LB_INSERTSTRING,	(WPARAM)-1, (LPARAM)(LPSTR)sz);
	}

void TvSqlLst::vSetInfo( BYTE *szInfo, BOOL bRightWindow)
	{
	if (bRightWindow)
		SetDlgItemText( hDlg__, ID_INFO1, (LPSTR)szInfo);
	else
		SetDlgItemText( hDlg__, ID_INFO2, (LPSTR)szInfo);
	}

void TvSqlLst::vError( char *szMsg, char *szFunction)
	{
	TvInfo::vError( szMsg, __FILE__, "vSqlLst", szFunction);
	}
// End of file

