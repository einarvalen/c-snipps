// vwinmisc.h - Header file for vwinmisc.cpp
// Miscallanous functions to simplify windows programming and
// make windows programs shorter.
// Author: Einar Valen, Date: 22.10.1994, Not Tested ok:

// INCLUDES
#include "vstd.h"
#include "vwin.h"

// DEFINES

// TYPES

// FUNCTION PROTOTYPES
// Takes care of iconic repaint of window hWnd
void pascal vPaintIconic( HWND hWnd, HINSTANCE hInstance);

// Positions dialog box window, hDlg, at center of screen
void vCenterWindow( HWND hDlg);

// Collect Texts from DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] and store in szBuf sep. by dFS
char *szGetEditCtrlsTexts( HWND hDlg, int iGeCtrl, int iLeCtrl, char *szBuf, size_t iBufLen);

// Sets texts on DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] stored in szBuf sep. by dFS
void vSetEditCtrlsTexts( HWND hDlg, int iGeCtrl, int iLeCtrl, char *szBuf);

// Collect Texts from DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] and store in szBuf sep. by dFS
char *szGetListBoxStr( HWND hDlg, UINT uiListCtrl, int iSelIdx, char *szBuf, size_t iBufLen);

// GLOBAL VARIABLES

// FUNCTIONS

