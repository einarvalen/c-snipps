// vinfot.cpp - Test program for vinfo.cpp,
// Author: Einar Valen, Date: 14.2.1994, Tested ok: 14.2.1994
// LoadString() not implemented 14.2.1994

// INCLUDES
#include "vwin.h"
#include "vinfo.h"

// DEFINES
// TYPES
// GLOBAL VARIABLES
// FUNCTIONS

int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	// Main testprocedure
	{
	TvInfo::vError( "Testerror ������", __FILE__, "WinMain", "WinMain");
	TvInfo::iQuestion( "TestQuestion ������");
	TvInfo::vNotice( "Testnotice ������");
	TvInfo::vError( "Testerror ������", __FILE__, "WinMain", "WinMain");
	TvInfo::iQuestion( "TestQuestion ������");
	TvInfo::vNotice( "Testnotice ������");
	return (OK);
	}

// End of File
