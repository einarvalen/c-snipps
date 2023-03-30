#ifndef VMAIN_H
#define VMAIN_H

// vmain.h - Classes for main menu
// Author: Einar Valen, Date: 25.2.1994, Tested ok: 25.2.1994

// INCLUDES
#include "vwin.h"

// DEFINES

// TYPES

// Main menu class
class TvMain 
	{
	private:
		char *szMenu__;
		HINSTANCE hPrevInstance__;
		LPSTR lpCmdLine__;
		DLGPROC  lpfnDialog__;
	public:
		BOOL bMainDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		TvMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine);
		void vRun( char *szMnu);
		virtual void vDispatchCB( int iCmd) = 0;
	};

// GLOBAL VARIABLES
// FUNCTIONS
void vAboutDlg( void);

#endif

// End of file