// vmaint.cpp - Test rpogram for main.cpp
// Author: Einar Valen, Date: 25.2.1994, Tested ok: 25.2.1994

// INCLUDES
#include <strstream.h>
#include "vmain.h"
#include "vinfo.h"
#include "vstd.h"

// DEFINES
// TYPES

class TaMain : public TvMain
	{
	public:
		TaMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine)
				: TvMain( hInstance, hPrevInstance, lpCmdLine)
			{};
		void vError( char *szMsg, char *szFunction)
			{
			TvInfo::vError( szMsg, __FILE__, "TaMain", szFunction);
			};
		virtual void vDispatchCB( int iCmd)
			{
			vError( "Dette er ingen feilmelding egentlig.\nSnarere en kvitering på at funksjonen er startet", "vDispatchCB()");
			};
	};

// FUNCTIONS

// Main program
#pragma argsused
int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	TaMain taMain( hInstance, hPrevInstance, lpCmdLine);
	char szBuff[80];
	ostrstream( szBuff, sizeof( szBuff) )
			<< "\nsizeof( void *):"	 << sizeof( void *)
			<< "\nsizeof( FARPROC):" << sizeof( FARPROC)
			<< ends;
	TvInfo::vNotice( szBuff);
	TvInfo::vNotice( lpCmdLine);
	taMain.vRun( "vmain");
	return (0);
	}

// End of file
