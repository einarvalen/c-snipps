// vname.cpp - Test rpogram for name.cpp
// Author: Einar Valen, Date: 1.10.1994, Tested ok:

// INCLUDES
#include <strstrea.h>
#include <string.h>
#include <except.h>
//#include "vdlg.h"
#include "vwevents.h"
#include "vwin.h"
#include "vinfo.h"
#include "vwinmisc.h"
#include "vstring.h"

// DEFINES
//#define dTEST_VLISTBOX  // Kommenteres ut ved modultest

// TYPES
class TvListBox
	{
	private:
		HINSTANCE hInstance__;
		HWND hDlg__;
		BOOL bPicked__;
		size_t iBufferSize__;
		char *szBuffer__,  *szCaption__, *szWork__, *szValueList__,  cSep__;
		void vError( char *szMsg, char *szFunction)
			{ TvInfo::vError( szMsg, __FILE__, "TvListBox", szFunction); };
	public:
		BOOL bDlgProcListBox( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		TvListBox( HANDLE hInstance, char *szPicked, int iPickedLen);
		BOOL bRun( HWND hParent, char *szCaption, char *szValueList, char cSep);
	};

// FUNCTION PROTOTYPES
static BOOL CALLBACK _export bDlgProcListBoxCB( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// GLOBAL VARIABLES
static TvListBox *sgptvListBox = NULL;

// FUNCTIONS


BOOL bSelectBox( HINSTANCE hInstance, HWND hParent, char *szCaption, char *szPicked, int iPickedLen, char *szValueList, char cSep)
	{
	TvListBox tvListBox( hInstance, szPicked, iPickedLen);
	return (tvListBox.bRun( hParent, szCaption, szValueList, cSep) );
	}

TvListBox::TvListBox( HANDLE hInstance, char *szPicked, int iPickedLen)
	{
	szCaption__ = "Velg en";
	hInstance__ = hInstance;
	iBufferSize__ = iPickedLen;
	bPicked__ = FALSE;
	szBuffer__ = szPicked;
	try
		{ // TEST FOR EXCEPTIONS.
		szWork__ = new char[iBufferSize__];
		}
	catch (xalloc)
		{  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
		vError( "Could not allocate memory", "TvListBox()");
		exit( -1);
		}
	}

BOOL TvListBox::bRun( HWND hParent, char *szCaption, char *szValueList, char cSep)
	{
	szCaption__ = szCaption;
	szValueList__ = szValueList;
	cSep__ = cSep;
	DLGPROC lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)bDlgProcListBoxCB, hInstance__);
	if (lpfnDialog == NULL)
		vError( "MakeProcInstance has problems", "bRun");
	TvListBox *pTvListBox = sgptvListBox;  // Keep contents of global ref. to this Class
	sgptvListBox = this;	// Store global reference to this object for Windows callback
	if (DialogBox( (HINSTANCE)hInstance__,  (LPCSTR)"vListBox", (HWND)hParent, (DLGPROC)lpfnDialog) == -1)
		vError( "Problems creating dialog box", "bRun");
	sgptvListBox = pTvListBox;  // Restore contents of global ref. to this class
	FreeProcInstance( (FARPROC)lpfnDialog);
	return (bPicked__);
	}


#pragma argsused
static BOOL CALLBACK _export bDlgProcListBoxCB( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	return (sgptvListBox->bDlgProcListBox( hDlg, message, wParam, lParam) );
	}


#pragma argsused
BOOL TvListBox::bDlgProcListBox( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	hDlg__ = hDlg;
	switch (message)
		{
		case WM_INITDIALOG :
			{
			vCenterWindow( hDlg);
			// Caption text
			SendMessage( hDlg, (WORD)WM_SETTEXT, (WPARAM)0, (LPARAM)szCaption__);
			// Fyll listbox
			SendMessage( hDlg, (UINT)WM_COMMAND, (WPARAM)CM_REFRESH, (LPARAM)0);
			return (TRUE);
			}
		case WM_COMMAND :
			{
			switch (wParam)
				{
				case CM_LISTBOX:
					{
					// If dobleclick, proceed to CM_OK
					if (HIWORD( lParam) != LBN_DBLCLK)
						return (OK);
					}
				case IDOK:
				case CM_OK:
					{
					// Finn selection
					DWORD dwIndex = SendDlgItemMessage( hDlg, CM_LISTBOX, LB_GETCURSEL, (WPARAM)0, (LPARAM)0L);
					if (dwIndex != (DWORD)LB_ERR)
						{
						// Sjekk om det er plass i buffer
						if (SendDlgItemMessage( hDlg, CM_LISTBOX, LB_GETTEXTLEN, (WPARAM)dwIndex, (LPARAM)0) < iBufferSize__ - 1)
							{
							SendDlgItemMessage( hDlg, CM_LISTBOX, LB_GETTEXT, (WPARAM)dwIndex, (LPARAM)(LPSTR)szBuffer__);
							bPicked__ = TRUE;
							}
						}
					EndDialog( hDlg, 0);
					return (OK);
					}
				case IDCANCEL:
				case CM_CANCEL:
					{
					EndDialog( hDlg, 0);
					return (OK);
					}
				case CM_REFRESH:
					{
					// Append row to listbox
					for (int i = 1; bStrGetField( i, szWork__, iBufferSize__, szValueList__, cSep__); i++)
						{
						LRESULT lr = SendDlgItemMessage( hDlg__, CM_LISTBOX, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)(LPSTR)szWork__);
						if (lr != LB_ERR && lr != LB_ERRSPACE)
							SendDlgItemMessage( hDlg, CM_LISTBOX, LB_SETCURSEL, (WPARAM)0, (LPARAM)0L);
						}
					return (OK);
					}
				default:
					return (OK);
				}
			}
		}
	return (FALSE);
	}

// *** TESTCODE ***
#if defined (dTEST_VLISTBOX)

#pragma argsused
int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	if (hPrevInstance == (HANDLE)0)
		{
		char szPicked[30];
		strcpy( szPicked, "Rød");
		while (bSelectBox( hInstance, NULL, "Velg farge", szPicked, sizeof( szPicked), "Rød|Blå|Gul|Grønn|Svart|Hvit", '|') )
			TvInfo::vNotice( szPicked);
		TvInfo::vNotice( "Ingenting valgt");
		TvInfo::vNotice( szPicked);
		}
	return (0);
	}

#endif
// End of file

