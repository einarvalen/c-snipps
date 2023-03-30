// vinfo.cpp - Classes for warnings and error messages.
// Author: Einar Valen, Date: 14.2.1994, Tested ok: 14.2.1994

// INCLUDES
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <sys\stat.h>
#include <strstrea.h>
#include "vwin.h"
#include "vinfo.h"

// DEFINES
#define TINFO_BUFSIZE 512;

// TYPES
// GLOBAL VARIABLES
// FUNCTIONS

	//if (!LoadString( hInstance, wId2, (LPSTR)szText, TINFO_BUFSIZE) )
	//	strcpy( szText, "TvInfo -  Error: Canntot find string");

void TvInfo::vTraceLog( char *szMsg, char *szSource, int iLine)	// Appends messages to a logfile, trace.log.
	{
	static FILE *p_log = NULL;
	long long_time;
	char sz[512];
   for (int i = 0; szMsg[i]; i++)
   	if (szMsg[i] == '\n')
      	szMsg[i] = ' ';
	time( &long_time);
	ostrstream( sz, sizeof( sz) )
   	<<"(" << szMsg
      << ") Source:(" << szSource
      << ") Line:(" << iLine
      << ") Time:" << ctime( &long_time)
		<< ends;
	if ( !p_log && !(p_log = fopen( "trace.log", "a") ) )
		{
		creat( "trace.log", S_IREAD | S_IWRITE);
      p_log = fopen( "trace.log", "a");
		}
   if (p_log)
		fprintf( p_log,  sz);
	}

void TvInfo::vErrorLog( char *szMsg, char *szSource, char *szClass, char *szFunction)
	/* Appends messages to a logfile, err.log. */
	{
	static FILE *p_log = NULL;
	long long_time;
	char sz[512];
	ostrstream( sz, sizeof( sz) )
			<<"{ Source = ("<<szSource<<")"<<szClass<< "::"<<szFunction
			<<"() }\n\n"<<szMsg<<ends;
	if ( !p_log && !(p_log = fopen( "err.log", "a") ) )
		{
		if (creat( "vliberr.log", S_IREAD | S_IWRITE) == -1)
			return; /** CREATION FAILED **/
		else if ( !(p_log = fopen( "err.log", "a") ) )
			return;  /** OPEN FAILED **/
		}
	time( &long_time);
	fprintf( p_log, "%s %s\n", ctime( &long_time), sz);
	}

void TvInfo::vError( char *szTxt, char *szSource, char *szClass, char *szFunction)
	{
	vErrorLog( szTxt, szSource, szClass, szFunction);
	MessageBox( NULL, (LPSTR)szTxt, (LPSTR)"Error", MB_OK | MB_ICONSTOP);
	}

void TvInfo::vNotice( char *szTxt)
	{
	MessageBox( NULL, (LPSTR)szTxt, (LPSTR)"Notice", MB_OK | MB_ICONINFORMATION);
	}

int TvInfo::iQuestion( char *szTxt)
	{
	MessageBox( NULL, (LPSTR)szTxt, (LPSTR)"Question", MB_OK | MB_ICONQUESTION);
	return (1);
	}

void TvInfo::vError( int iTxtId, char *szSource, char *szClass, char *szFunction)
	{
	}

void TvInfo::vNotice( int iTxtId)
	{
	}

int TvInfo::iQuestion( int iTxtId)
	{
	return (1);
	}
