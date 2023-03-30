// vsqllstt.cpp - Test rpogram for main.cpp
// Author: Einar Valen, Date: 6.1.1994, Tested ok: DD.MM.YYYY.

// INCLUDES
#include <stdio.h>
#include <string.h>
#include <strstream.h>
#include "vmain.h"
#include "vinfo.h"
#include "vstd.h"
#include "vsqllst.h"
#include "qelib.h"	/* function prototypes */

// DEFINES
// TYPES

class TvSqlLstTst : public TvSqlLst
	{
  private:
  	int	hDbc;		/* Handle to a database connection */
		int	hStmt;		/* Handle to a SQL Statement */
		char szConnect[256];   /* Connect string */
		char szSql[2048];	/* SQL Statement */
		char szLastName [11];
		float fSalary;
		long lLastNameLen;
		long lSalaryLen;
	public:
		int iCount;
		BOOL format( char *sz);
		TvSqlLstTst();
		~TvSqlLstTst();
		BOOL Dsply( BYTE *s);
		virtual void vSqlInsertCB( void);
		virtual void vSqlModifyCB( int iIdx);
		virtual void vSqlQueryCB( void);
		virtual BOOL bSqlFetchCB( char *sz);
		virtual void vSqlCommitCB( void);
		virtual void vModifyQueryCB( void);
		virtual void vInitCB( void);
	};

class TaMain : public TvMain
	{
	public:
		TaMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine)
				: TvMain( hInstance, hPrevInstance, lpCmdLine)
			{};
		void vError( char *szMsg, char *szFunction)
			{
			TvInfo::vError( szMsg, __FILE__, "vSqlLst Test", szFunction);
			};
		virtual void vDispatchCB( int iCmd);
	};

#pragma argsused
void TaMain::vDispatchCB( int iCmd)
	{
  char sz[512];
	TvSqlLstTst *ptvSqlLstTst;
	if (NULL == (ptvSqlLstTst = new TvSqlLstTst() ) )
		TvInfo::vNotice("void TaMain::vDispatchCB( OUT OF RAM)");
	if (ptvSqlLstTst->bRun( sz, sizeof( sz) ) )
			TvInfo::vNotice( sz);
	}

// FUNCTIONS

TvSqlLstTst::TvSqlLstTst() : TvSqlLst()
	{
	if ( qeLibInit() != qeSUCCESS)
		vError( qeErrMsg(), "qeLib()");
	else
  	{
		hDbc = 0;		/* Handle to a database connection */
		strcpy( szConnect, "DSN=QEDBF");
		if ( (hDbc = qeConnect( szConnect) ) == 0)
			vError( qeErrMsg(), "qeLib()");
    }
	}

TvSqlLstTst::~TvSqlLstTst( void)
	{
	qeEndSQL( hStmt);
	if (qeDisconnect( hDbc) != qeSUCCESS)
		vError( qeErrMsg(), "qeLib()");
	hDbc = 0;
	qeLibTerm();
  }

void TvSqlLstTst::vSqlQueryCB( void)
	{
	lLastNameLen = sizeof( szLastName);
	lSalaryLen = sizeof( fSalary);
	hStmt = 0;		/* Handle to a SQL Statement */
	strcpy( szSql,  "SELECT last_name, salary FROM emp");
	if ( (hStmt = qeExecSQL( hDbc, szSql) ) == 0)
		goto err;
	/* Bind the result columns to program variables. */
	if (qeBindColChar( hStmt, 1, szLastName, &lLastNameLen, "") != qeSUCCESS)
		goto err;
	if (qeBindColFloat( hStmt, 2, &fSalary, &lSalaryLen) != qeSUCCESS)
		goto err;
	return;
	err:
		vError( qeErrMsg(), "qeLib()");
	}

BOOL TvSqlLstTst::bSqlFetchCB( char *sz)
	{
	/* Fetch the records from the select statement. */
	szLastName[0] ='\0';
	fSalary = 0.00;
	int iRes = qeFetchNext( hStmt);
	sprintf( sz, "<[%s]\t[%f]>", szLastName, fSalary);
	return (iRes == qeSUCCESS);
	}

void TvSqlLstTst::vSqlCommitCB( void)
	{
	iCount = 0; Dsply( "vSqlCommitCB()");
	qeEndSQL( hStmt);
	}

void TvSqlLstTst::vSqlInsertCB( void)
	{
	Dsply( "vSqlInsertCB()");
	}

void TvSqlLstTst::vSqlModifyCB( int iIdx)
	{
	Dsply( pszBuffer() );
	}

void TvSqlLstTst::vModifyQueryCB( void)
	{
	Dsply( "vModifyQueryCB()");
	}

void TvSqlLstTst::vInitCB( void)
	{
	// Setup tabstops and horizontal scroll width in listbox
//	int i;
//	int aiTabStops[10]; //tab-stop array
//	i = 0;
//	aiTabStops[i] = lLastNameLen + 2;
//	i++;
//	aiTabStops[i] = aiTabStops[i-1] + lSalaryLen + 2; i++;
//	vSetTabStops( aiTabStops, i);
	}

BOOL TvSqlLstTst::format( char  *sz)
	{
	sprintf( sz, "<%d\t[%d]\t(%d)\t{%d}\t{%d}\t{%d}\t{%d}>",
	iCount, iCount, iCount, iCount, iCount, iCount, iCount);
//	vAppendString( sz);
	return (TRUE);
	};

BOOL TvSqlLstTst::Dsply( BYTE *s)
	{
	MessageBox( NULL, (LPSTR)s, (LPSTR)"Test", MB_OK | MB_ICONSTOP);
	vSetInfo( s);
	vSetInfo( s, FALSE);
	return (TRUE);
	}

// Main program
#pragma argsused
int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	TaMain taMain( hInstance, hPrevInstance, lpCmdLine);
	taMain.vRun( "vmain");
	return (OK);
	}

// End of file
