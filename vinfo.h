// vinfo.h - Classes for warnings and error messages.
// Author: Einar Valen, Date: 14.2.1994, Tested ok: 14.2.1994
#ifndef VINFO_H
#define VINFO_H

// INCLUDES
#include "vstd.h"

// DEFINES
  /* For debugging */
#if defined (NDEBUG)   
#	define TRACE(s)
#else
#	define TRACE( s) TvInfo::vTraceLog( s, __FILE__, __LINE__)
#endif

// TYPES

class TvInfo
	{
	public:
		static void vError( char *szTxt, char *szSource, char *szClass, char *szFunction);
		static void vError( int iTxtId, char *szSource, char *szClass, char *szFunction);
		static void vErrorLog( char *szMsg, char *szSource, char *szClass, char *szFunction);
		static void vTraceLog( char *szMsg, char *szSource, int iLine);	// Appends messages to a logfile, trace.log.
      static void vNotice( char *szTxt);
		static void vNotice( int iTxtId);
		static int  iQuestion( char *szTxt);
		static int  iQuestion( int iTxtId);
	};

// GLOBAL VARIABLES
// FUNCTIONS

#endif

// End Of File
