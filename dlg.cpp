// vname.cpp - Test rpogram for name.cpp
// Author: Einar Valen, Date: 1.10.1994, Tested ok:

// INCLUDES
#include <strstrea.h>
#include <string.h>
#include <stdio.h>
#include <except.h>
#include "vwin.h"
#include "vwevents.h"
#include "vinfo.h"
#include "vstd.h"
#include "vsqlsel.h"
#include "vwinmisc.h"

extern "C" {
#include "qelib.h"	/* function prototypes */
}
// DEFINES

// TYPES

// Name class
class TvName
	{
	private:
		WPARAM wCurrentCtrl__;
		HINSTANCE hInstance__;
		size_t iBufferSize__;
		char *szBuffer__;
		char *szUniqueRowId__;
		char *szWhere__;
		char *szOrderBy__;
		char *szPicked__;
		char *szTable__;
		char *szColumns__;
		int  hDbc__;		/* Handle to a database connection */
		int  hStmt;		/* Handle to a SQL Statement */
		char szConnect[256];   /* Connect string */
		char szSql[2048];	/* SQL Statement */
	public:
		BOOL bDlgProcMntName( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL bDlgProcLstName( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		TvName( HANDLE hInstance);
		char *szList( HWND hDlg, char *szWhere, char  *szOrderBy);
		void vMaintain( HWND hDlg, char *szUniqueRowId);
		void vError( char *szMsg, char *szFunction)
			{
			TvInfo::vError( szMsg, __FILE__, "TvName", szFunction);
			};
		void vbSqlConnect( void);
		void vbSqlDisconnect( void);
		void vSqlConnect( void);
		void vSqlDisconnect( void);
		void vSqlInsert( char *szTable, char *szColumns, char *szValues);
		void vSqlModify( char *sz);
		void vSqlDelete( void);
		char *szSqlSelectUnique( char *szBuffer, size_t iBufferSize, char *szWhere);
		char *szSqlSelect( char *szBuffer, size_t iBufferSize, char *szTable, char *szColumns, char *szWhere, char *szOrderBy);
		void vHelp( int iCtrl);
	};

// FUNCTION PROTOTYPES

// GLOBAL VARIABLES
static TvName *sgptvName = NULL;

// FUNCTIONS

#pragma argsused
static BOOL CALLBACK _export DlgProcLstName( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	return (sgptvName->bDlgProcLstName( hDlg, message, wParam, lParam) );
	}

void TvName::vSqlInsert( char *szTable, char *szColumns, char *szValues)
	{
	char szSql[dBUFSIZ];
	int hStmt = 0;		/* Handle to a SQL Statement */
	// Set up Select statement
	ostrstream( szSql, sizeof( szSql) )
			<< "insert into " << szTable << " (" << szColumns << ") values ("
			<< szValues << ");" << ends;
	// Start Transaction
	qeBeginTran( hDbc__);
	// Execute the Insert statement
	if ( (hStmt = qeExecSQL( hDbc__, szSql) ) == 0)
		vError( qeErrMsg(), "szSqlInsert()");
	else if (qeEndSQL( hStmt) )
		vError( qeErrMsg(), "szSqlInsert()");
	// End of Transaction
	qeCommit( hDbc__);
	}

void TvName::vSqlModify( char *szBuf)
	{
	vError( szBuf, "vSqlModify()");
	}

void TvName::vSqlDelete( void)
	{
	vError( "vSqlDelete()", "vSqlDelete()");
	}


void TvName::vSqlConnect( void)
	{
	hDbc__ = 0;		/* Handle to a database connection */
	if ( (hDbc__ = qeConnect( "DSN=QEDBF") ) == 0)
		vError( qeErrMsg(), "TvSqlConnect");
	qeSetSelectOptions( hDbc__, qeFETCH_FORWARD_DIR + qeLOG_IF_NEEDED);
	}


void TvName::vSqlDisconnect( void)
	{
	if (qeDisconnect( hDbc__) != qeSUCCESS)
		vError( qeErrMsg(), "~TvSqlLstTst");
	}


char *TvName::szSqlSelectUnique( char *szBuffer, size_t iBufferSize, char *szWhere)
	{
	char szSql[dBUFSIZ];
	ostrstream( szSql, sizeof( szSql) ) << "select * from emp where (" << szWhere << ");" << ends;
	vError( szSql, "szSqlSelect()");
	return (szBuffer);
	}


char *TvName::szSqlSelect( char *szBuffer, size_t iBufferSize, char *szTable, char *szColumns, char *szWhere, char *szOrderBy)
	{
	char szSql[dBUFSIZ];
	int hStmt = 0;		/* Handle to a SQL Statement */
	// Set up Select statement
	ostrstream osSql( szSql, sizeof( szSql) );
	osSql << "select " << szColumns << " from " << szTable;
	if (szWhere && strlen( szWhere) > 0)
		osSql << " where (" << szWhere	<< ")";
	if (szOrderBy && strlen( szOrderBy) > 0)
		osSql << " order by " << szOrderBy;
	osSql << " ;" << ends;
	ostrstream os( szBuffer, iBufferSize - 1);
	// Start Transaction
	qeBeginTran( hDbc__);

	if (strcmp( szWhere, "columns") == 0)
		hStmt = qeColumns( hDbc__, szOrderBy);	// Show columns
	else	if (strcmp( szWhere, "sources") == 0)
		hStmt = qeSources( qeSRC_AVAIL_LOGON);	// Show sources
	else	if (strcmp( szWhere, "databases") == 0)
		hStmt = qeDatabases( hDbc__);	// Show databases
	else	if (strcmp( szWhere, "tables") == 0)
		hStmt = qeTables( hDbc__, szOrderBy, "%", "%", 0x1f);	// Show tables
	else if (strcmp( szWhere, "sql") == 0)
		hStmt = qeExecSQL( hDbc__, szOrderBy);	// Execute raw sql
	else
		hStmt = qeExecSQL( hDbc__, szSql); // Execute the Select statement
	if (hStmt == 0)
		vError( qeErrMsg(), "szSqlSelect()");
	else
		{
		// Fetch the rows from the select result set
		int iColCount = qeNumCols( hStmt);
		char szColSep[2]; szColSep[0] = dFS; szColSep[1] = '\0';
		for (;;)
			{
			char *psz;
			// Do the Fecht until end of result set
			if (qeFetchNext( (long)hStmt) != qeSUCCESS)
				break;
			// Format all Collumns to ascii and append to buffer
			psz = qeValMultiChar( hStmt, 1, iColCount, "", "", szColSep);
			os << psz << dRS;
			// Quit loop if buffer is too full
			if ( iBufferSize <= (strlen( psz) + os.pcount() + 1) )
				break;
			}
		os << ends;
		// End Select - Fetch sequence
		if (qeEndSQL( hStmt) )
			vError( qeErrMsg(), "szSqlSelect()");
		}
	// End of Transaction
	qeCommit( hDbc__);
	return (szBuffer);
	}

void TvName::vHelp( int iCtrl)
	{
	char sz[dBUFSIZ];
	ostrstream( sz, sizeof( sz) ) << "!!! Help !!!!\n\nControl=" << iCtrl << ends;
	vError( sz, "vHelp()");
	}


#pragma argsused
BOOL TvName::bDlgProcLstName( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG :
			{
			vCenterWindow( hDlg);
			SetMenu( hDlg, LoadMenu( hInstance__, (LPSTR)"vMnuLstName") );

			// Setup tabstops and horizontal scroll width in listbox
			int iExtent = dRECSIZ + (LOWORD( GetDialogBaseUnits() ) * 300);
			SendDlgItemMessage( hDlg, CM_LISTBOX, (WORD)LB_SETHORIZONTALEXTENT, (WPARAM)iExtent, (LPARAM)0L);
			SendMessage( hDlg, (UINT)WM_COMMAND, (WPARAM)CM_FETCH, (LPARAM)0);
			return (TRUE);
			}
		case WM_PAINT :
			{
			if (!IsIconic( hDlg) ) // We need to paint the minimized icon ourselves
				return (FALSE);
			vPaintIconic( hDlg, hInstance__);
			return (TRUE);
			}
		case WM_COMMAND :
			{
			switch (wParam)
				{
				case CM_INSERT:
					{
					vMaintain( hDlg, NULL);
					return (OK);
					}
				case CM_MODIFY:
					{
					DWORD dwIndex = SendDlgItemMessage( hDlg, CM_LISTBOX, LB_GETCURSEL, (WPARAM)0, (LPARAM)0L);
					if (dwIndex == (DWORD)LB_ERR)
						return (OK);
					vMaintain( hDlg, szGetListBoxStr( hDlg, CM_LISTBOX, (int)dwIndex, szPicked__, dBUFSIZ -1) );
					return (OK);
					}
				case CM_SELECT:
					{
					if( !bDlgBoxSqlSelect( hInstance__, hDlg, szTable__, 99, szColumns__, dBUFSIZ-1, szWhere__, dBUFSIZ-1, szOrderBy__, dBUFSIZ-1) )
						return (OK);
					}
				case CM_FETCH:
					{
					char *psz, *pszBuf;
					pszBuf = szSqlSelect( szBuffer__, iBufferSize__, szTable__, szColumns__, szWhere__, szOrderBy__);
					SendDlgItemMessage( hDlg, CM_LISTBOX, (WORD)LB_RESETCONTENT, (WPARAM)0, (LPARAM)0L);
					for (; (psz = strchr( pszBuf, dRS) ) != NULL; )
						{
						*psz = '\0';  psz++;
						for (size_t i = 0; pszBuf[i]; i++)
							if (pszBuf[i] == dFS)
								pszBuf[i] = '\t';
						SendDlgItemMessage( hDlg, CM_LISTBOX, (WORD)LB_INSERTSTRING,	(WPARAM)-1, (LPARAM)(LPSTR)pszBuf);
						pszBuf = psz;
						}
					SendDlgItemMessage( hDlg, CM_LISTBOX, (WORD)LB_SETCURSEL, (WPARAM)0, (LPARAM)0L);
					return (OK);
					}
				case IDHELP:
					{
					vHelp( wParam);
					return (OK);
					}
				case CM_LISTBOX:
					{
					if (HIWORD( lParam) != LBN_DBLCLK)
						return (OK);
					}
				case IDOK:
					{
					DWORD dwIndex = SendDlgItemMessage( hDlg, CM_LISTBOX, LB_GETCURSEL, (WPARAM)0, (LPARAM)0L);
					if (dwIndex != (DWORD)LB_ERR)
						(void)szGetListBoxStr( hDlg, CM_LISTBOX, (int)dwIndex, szPicked__, dBUFSIZ -1);
					EndDialog( hDlg, 0);
					return (OK);
					}
				case IDCANCEL:
					{
					szPicked__[0] = '\0';
					EndDialog( hDlg, 0);
					return (OK);
					}
				default:
					return (OK);
				}
			}
		}
	return (FALSE);
	}


TvName::TvName( HANDLE hInstance)
	{
	hInstance__ = hInstance;
	iBufferSize__ = 0xffff;
	try
		{ // TEST FOR EXCEPTIONS.
		szBuffer__ = new char[iBufferSize__];
		szWhere__ = new char[dBUFSIZ];
		szOrderBy__ = new char[dBUFSIZ];
		szTable__ = new char[100];
		szColumns__ = new char[dBUFSIZ];
		szPicked__ = new char[dBUFSIZ];
		}
	catch (xalloc)
		{  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
		vError( "Could not allocate memory", "TvName()");
		exit( -1);
		}
	szPicked__[0] = '\0';
	szWhere__[0] = '\0';
	strcpy( szTable__, "addr");
	strcpy( szColumns__, "*");
	szOrderBy__[0] = '\0';
	}


char *TvName::szList( HWND hDlg, char *szWhere, char  *szOrderBy)
	{
	if (szWhere != NULL)
		strcpy( szWhere__, szWhere);
	if (szOrderBy != NULL)
		strcpy( szOrderBy__, szOrderBy);
	szPicked__[0] = '\0';	// Initialize buffer for selection from Picklist
	DLGPROC lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)DlgProcLstName, hInstance__);
	sgptvName = this;	// Store global reference to this object for Windows callback
	DialogBox( hInstance__, "vDlgLstName", hDlg, (DLGPROC)lpfnDialog);
	FreeProcInstance( (FARPROC)lpfnDialog);
	return (strlen( szPicked__) > 0 ? szPicked__ : NULL);
	}

#pragma argsused
static BOOL CALLBACK _export DlgProcMntName( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	return (sgptvName->bDlgProcMntName( hDlg, message, wParam, lParam) );
	}


#pragma argsused
BOOL TvName::bDlgProcMntName( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG :
			{
			vCenterWindow( hDlg);
			SetMenu( hDlg, LoadMenu( hInstance__, (LPSTR)"vMnuMntName") );
			wCurrentCtrl__ = 1001;
			if (szUniqueRowId__ != NULL)
				vSetEditCtrlsTexts( hDlg, 1001, 1004, szUniqueRowId__);
			return (TRUE);
			}
		case WM_PAINT :
			{
			if (!IsIconic( hDlg) ) // Need to paint the minimized icon ourselves
				return (FALSE);
			vPaintIconic( hDlg, hInstance__);
			return (TRUE);
			}
		case WM_COMMAND :
			{
			char szBuf[dBUFSIZ + 1];
			switch (wParam)
				{
				case CM_EDITUNDO:
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_UNDO, (WPARAM)0, (LPARAM)0);
					return (OK);
				case CM_EDITCUT:
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_CUT, (WPARAM)0, (LPARAM)0);
					return (OK);
				case CM_EDITCOPY:
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_COPY, (WPARAM)0, (LPARAM)0);
					return (OK);
				case CM_EDITPASTE:
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_PASTE, (WPARAM)0, (LPARAM)0);
					return (OK);
				case CM_INSERT:
					{
					// Count number of significant Columns
					size_t i, iCols;
					for (i = iCols = 0; szColumns__[i]; i++)
						if (szColumns__[i] == ',')
						 iCols++;
					// Retrieve Field values from Dialog Box
					(void)szGetEditCtrlsTexts( hDlg, 1001, 1001 + iCols, szBuf, sizeof( szBuf) );
					// Remove trailing field sep. (dFS)
					size_t t = strlen( szBuf) - 1;
					szBuf[t > 0 ? t : 0] = '\0';
					// Convert dFS-separated Value-list into ","-separated Value-list
					char sz[dBUFSIZ+1], *psz;
					for (i = 0, psz = &sz[0]; szBuf[i]; i++)
						if (szBuf[i] == dFS)
							{ *(psz++) = '\"'; *(psz++) = ','; *(psz++) = '\"'; }
						else
							*(psz++) = szBuf[i];
					*(psz++) = '\0';
					vSqlInsert( szTable__, szColumns__, sz);
					return (OK);
					}
				case IDOK:
				case CM_MODIFY:
					(void)szGetEditCtrlsTexts( hDlg, 1001, 1004, szBuf, sizeof( szBuf) );
					vSqlModify( szBuf);
				case IDCANCEL:
					EndDialog( hDlg, 0);
					return (OK);
				default:
					if (wParam <= 1004 && wParam >= 1001)
						wCurrentCtrl__ = wParam;
					else if (wParam <= 3004 && wParam >= 3001)
						vHelp( wParam - 2000);
					return (OK);
				}
			}
		}
	return (FALSE);
	}


void TvName::vMaintain( HWND hDlg, char *szUniqueRowId)
	{
	szUniqueRowId__ = szUniqueRowId;
	sgptvName = this;
	DLGPROC lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)DlgProcMntName, hInstance__);
	DialogBox( hInstance__, "vDlgMntName", hDlg, (DLGPROC)lpfnDialog);
	FreeProcInstance( (FARPROC)lpfnDialog);
	}


// Main program
#pragma argsused
int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	char *psz;
	if (hPrevInstance == (HANDLE)0)
		{
		if ( qeLibInit() != qeSUCCESS)
			TvInfo::vError( qeErrMsg(), __FILE__, " ", "WinMain");
		else
			{
			TvName tvName( hInstance);
			tvName.vSqlConnect();
			if ( (psz = tvName.szList( NULL, "FIRST_NAME like \'J\%\'", "FIRST_NAME, LAST_NAME") ) != NULL)
				TvInfo::vNotice( psz);
			tvName.vSqlDisconnect();
			qeLibTerm();
			}
		}
	return (0);
	}

// End of file