#ifndef vsqllst_H
#define vsqllst_H

// vsqllst.h - Header file for vsqllst.cpp.
// Author: Einar Valen, Date: 26.2.1994, Tested ok: 26.2.1994

// INCLUDES
#include "vwin.h"
#include "vstd.h"
#include "vsqllst.h"

// DEFINES
#define VLIST_BUFFERSIZE 512
// TYPES
class TvSqlLst 
	{
	private:
		char *szMenu__;
		HINSTANCE hInstance__, hPrevInstance__;
		HWND hDlg__;
		BOOL bSelectionWasMade;
		short iCurrentControl;
		BYTE szBuffer[VLIST_BUFFERSIZE+1];
    long ilItemData_;
		void vFillList( void);
	public:
		TvSqlLst( void)		{};
		BOOL bRun( char *sz, int iLen);
		BOOL bMainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		void vSetInfo( BYTE *szInfo, BOOL bRightWindow = FALSE);
		void vAppendString( char *sz);
		char *pszBuffer( void) { return (szBuffer); };
		void vSetTabStops( int *aiTabStops, short iMaxNumberOfTabStops);
		virtual void vSqlInsertCB( void)  = 0;
		virtual void vSqlCommitCB( void)  = 0;
		virtual void vSqlModifyCB( int iIdx)  = 0;
		virtual void vSqlQueryCB( void) = 0;
		virtual BOOL bSqlFetchCB( char *sz) = 0;
		virtual BOOL bModifyQueryCB( void)   {	return (TRUE); };
		virtual void vInitCB( void) 		{};
		void vError( char *szMsg, char *szFunction);
	};

// GLOBAL VARIABLES
// FUNCTIONS

#endif

// End of file