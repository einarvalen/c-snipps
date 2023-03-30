//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USERC("PplRepExt.rc");
//---------------------------------------------------------------------------
#include <windows.h>
#include <direct.h>
#include <process.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strstrea.h>
#include <commdlg.h>
#include <fstream.h>
#include <ctype.h>
#include "resource.h"

#define OK 1
#define FAIL 0


char gszNewModelTmp[BUFSIZ], gszNewModelOptionsTmp[BUFSIZ];
char gszDataFile[BUFSIZ] = "C:\\TEMP\\einar.txt";
char gszModelFile[BUFSIZ] = "";
char gszColDelimiter[BUFSIZ] = ";";
char gszDecimalSeparator[BUFSIZ] = ",";
char gszNewModel[BUFSIZ] = "New";
char gszNewModelOptions[BUFSIZ] = "AutoDesign";
HANDLE hInstance;

// Centers dialog on screen
void vCenterWindow( HWND hDlg)
	{
	RECT rc, rcDTop;
	int NewX, NewY, CenterX, CenterY;
	HWND hDTopWnd;
	hDTopWnd = GetDesktopWindow();
	GetWindowRect(hDTopWnd, &rcDTop);			// desktop rectangle size
	GetWindowRect(hDlg, &rc);		// dialog box rectangle size
	// Calculate window position in center
	CenterX = (rcDTop.left + rcDTop.right) / 2;
	CenterY = (rcDTop.top + rcDTop.bottom) / 2;
	NewX = CenterX - ( (rc.right - rc.left) / 2);
	NewY = CenterY - ( (rc.bottom - rc.top) / 2);
	// Give it a new location
	SetWindowPos( hDlg, NULL, NewX, NewY, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

// Callback windows event loop handeler procedure for the help display
static BOOL CALLBACK _export AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	if (message == WM_INITDIALOG)
		vCenterWindow( hDlg);
	else if (message == WM_COMMAND && (wParam == IDOK || wParam == IDCANCEL) )
		{
		EndDialog( hDlg, 0);
		return (OK);
		}
	return (FALSE);
	}

// Common File Open dialog
char *szFileOpenDialog( HWND hDlg, char *szFile, size_t iFileLen)
	{
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof( OPENFILENAME) );
	ofn.lStructSize = sizeof( OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = iFileLen;
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileName( &ofn) )
		return (szFile);
	else
		return (NULL);
	}

// Collect Texts from DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] and store in szBuf sep. by dFS
 void getControlData( HWND hDlg, int iCtrl, char *szBuf, size_t iBufLen)
	{
	// Data from Edit box
	if (iCtrl == IDC_EDIT1 ||  iCtrl == IDC_EDIT2 || iCtrl == IDC_EDIT3 || iCtrl == IDC_EDIT4)
		{
		HWND hwndCtrl = GetDlgItem( hDlg, iCtrl);
		size_t iLen = 1 + (int)SendMessage( hwndCtrl, WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
		SendMessage( hwndCtrl, WM_GETTEXT, (WPARAM)(iLen > iBufLen) ? iBufLen : iLen, (LPARAM)(LPSTR)szBuf);
		}
	// Get check state for 	new og existing model
	else if (iCtrl == IDC_RADIO1 || iCtrl == IDC_RADIO5)
		{
		if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO1, BM_GETCHECK, 0, 0) )		// Is New Model checked
			strcpy( szBuf, "New");
		else
			strcpy( szBuf, "Existing");
		}
	// Get check state for new options
	else if (iCtrl == IDC_RADIO2 || iCtrl == IDC_RADIO3 || iCtrl == IDC_RADIO4)
		{
		if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO2, BM_GETCHECK, 0, 0) )			// Is the Tansformer option checked ?
			strcpy( szBuf, "Transformer");
		else if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO4, BM_GETCHECK, 0, 0) )   	// Is the PowerPlay option checked ?
			strcpy( szBuf, "PowerPlay");
		else
			strcpy( szBuf, "AutoDesign");
		}
	}

// Sets texts on DialogBox (hDlg) Edit Controls [iGeCtrl, iLeCtrl] stored in szBuf sep. by dFS
void setControlData( HWND hDlg, int iCtrl, char *szBuf)
	{
	// Display Edit box
	if (iCtrl == IDC_EDIT1 ||  iCtrl == IDC_EDIT2 || iCtrl == IDC_EDIT3 || iCtrl == IDC_EDIT4)
		SendDlgItemMessage( hDlg, iCtrl, (WORD)WM_SETTEXT,	(WPARAM)0, (LPARAM)(LPSTR)szBuf);
	// Set correct check state for checkboxes
	else if (iCtrl == IDC_RADIO1 || iCtrl == IDC_RADIO5)
		{
		if (strcmp( szBuf, "New") == 0)
			{
			SendDlgItemMessage( hDlg, IDC_RADIO1, BM_SETCHECK, TRUE, 0);		// New Model checked
			SendDlgItemMessage( hDlg, IDC_RADIO5, BM_SETCHECK, FALSE, 0);	// Existing Model unchecked
			}
		else			 
			{
			SendDlgItemMessage( hDlg, IDC_RADIO1, BM_SETCHECK, FALSE, 0);	// New Model unchecked
			SendDlgItemMessage( hDlg, IDC_RADIO5, BM_SETCHECK, TRUE, 0);		// Existing Model checked
			}
        }
	else if (iCtrl == IDC_RADIO2 || iCtrl == IDC_RADIO3 || iCtrl == IDC_RADIO4)
		{
		// Set all valid options unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO2, BM_SETCHECK, FALSE, 0);	// Open Transformer	unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO3, BM_SETCHECK, FALSE, 0);	// Open Transformer and Use AutoDesign unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO4, BM_SETCHECK, FALSE, 0);	// Open PowerPlay unchecked
		// Set correct check state for current option
		if (strcmp( szBuf, "Transformer") == 0)
			SendDlgItemMessage( hDlg, IDC_RADIO2, BM_SETCHECK, TRUE, 0);	// Open Transformer checked
		else if (strcmp( szBuf, "PowerPlay") == 0)
			SendDlgItemMessage( hDlg, IDC_RADIO4, BM_SETCHECK, TRUE, 0);	// Open PowerPlay checked
		else
			SendDlgItemMessage( hDlg, IDC_RADIO3, BM_SETCHECK, TRUE, 0);	// Open Transformer and Use AutoDesign checked
		}
	}


// Adjust dialog acording to current checked options
void AjustDialogForCheckedOption( HWND hDlg)
	{
	if ((int) SendDlgItemMessage( hDlg, IDC_RADIO1, BM_GETCHECK, 0, 0) )	// New Model checked
		{
		// Enable valid options
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO2), TRUE);		// Enable option Open Transformer	
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO3), TRUE);		// Enable option Open Transformer and Use AutoDesign 
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO4), TRUE);		// Enable option Open PowerPlay 
		// Disable invalid options
		EnableWindow( GetDlgItem( hDlg, IDC_EDIT1), FALSE);	// Disable edit box for MDL file name
		// Turn off alternative radio button
		SendDlgItemMessage( hDlg, IDC_RADIO5, BM_SETCHECK, FALSE, 0);	// Existing Model unchecked
		// Set correct check state for current option
		if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO2, BM_GETCHECK, 0, 0) )	// Open Transformer checked ?
			{
			SendDlgItemMessage( hDlg, IDC_RADIO4, BM_SETCHECK, FALSE, 0);	// Open PowerPlay unchecked
			SendDlgItemMessage( hDlg, IDC_RADIO3, BM_SETCHECK, FALSE, 0);	// Open Transformer and Use AutoDesign unchecked
			}
		else if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO2, BM_GETCHECK, 0, 0) ) 	// Open PowerPlay checked ?
			{
			SendDlgItemMessage( hDlg, IDC_RADIO2, BM_SETCHECK, FALSE, 0);	// Open Transformer unchecked
			SendDlgItemMessage( hDlg, IDC_RADIO3, BM_SETCHECK, FALSE, 0);	// Open Transformer and Use AutoDesign unchecked
			}
		else  	// Open Transformer and Use AutoDesign checked
			{
			SendDlgItemMessage( hDlg, IDC_RADIO2, BM_SETCHECK, FALSE, 0);	// Open Transformer unchecked
			SendDlgItemMessage( hDlg, IDC_RADIO4, BM_SETCHECK, FALSE, 0);	// Open PowerPlay unchecked
			}
		}
	else	// Existing Model checked
		{
		// Enable valid options
		EnableWindow( GetDlgItem( hDlg, IDC_EDIT1), TRUE);		// Enable edit box for MDL file name
		// Disable invalid options
		EnableWindow( GetDlgItem( hDlg, IDC_EDIT1),  FALSE);	// Disable option Open Transformer	
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO3), FALSE);	// Disable option Open Transformer and Use AutoDesign 
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO4), FALSE);	// Disable option Open PowerPlay 
		// Turn off alternative radio button
		SendDlgItemMessage( hDlg, IDC_RADIO5, BM_SETCHECK, TRUE, 0);		// New Model unchecked
		}
	UpdateWindow( hDlg);	// Refresh dialog
	}


static BOOL CALLBACK  DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG :
			{
			// Enable default button - OK
			EnableWindow( GetDlgItem( hDlg, IDC_OK), TRUE);	
			SendMessage( hDlg, WM_NEXTDLGCTL, 0, FALSE);
			SetFocus( (HWND)GetDlgItem( hDlg, IDC_OK) );	
			// Set controls according to present setting
			setControlData( hDlg, IDC_EDIT4, gszDataFile);			// Name of the data file.
			setControlData( hDlg, IDC_EDIT1, gszModelFile);			// Name of the MDL file
			setControlData( hDlg, IDC_EDIT2, gszColDelimiter);		// Column delimiter to use.
			setControlData( hDlg, IDC_EDIT3, gszDecimalSeparator);	// Decimal separator to use.
			setControlData( hDlg, IDC_RADIO1, gszNewModel);			// Indicates if user intends to crete a new model
			setControlData( hDlg, IDC_RADIO2, gszNewModelOptions);	// How does the user wish to build the new model
			AjustDialogForCheckedOption( hDlg);
			return (TRUE);									
			}
		case WM_COMMAND:	// Dialog control message
			{
			char sz[BUFSIZ];
			switch (wParam)
				{
				case IDC_OK:	// OK
					{
					// Commit changes
					getControlData( hDlg, IDC_EDIT4, 	gszDataFile, 		 sizeof( gszDataFile) );		 // Name of the data file.
					getControlData( hDlg, IDC_EDIT1,	gszModelFile, 		 sizeof( gszModelFile) );		 // Mane of the MDL file
					getControlData( hDlg, IDC_RADIO2, 	gszNewModelOptions,  sizeof( gszNewModelOptions) );	 // How does the user wish to build the new model
					getControlData( hDlg, IDC_EDIT2, 	gszColDelimiter, 	 sizeof( gszColDelimiter) );	 // Column delimiter to use.
					getControlData( hDlg, IDC_EDIT3, 	gszDecimalSeparator, sizeof( gszDecimalSeparator) ); // Decimal separator to use.
					getControlData( hDlg, IDC_RADIO1, 	gszNewModel, 		 sizeof( gszNewModel) );		 // Indicates if user intends to crete a new model
					// End program dialog 
					EndDialog( hDlg, 0);
					return (TRUE);
					}
				case IDC_CANCEL:	// Cancel
					{
					// End program dialog 
					EndDialog( hDlg, 0); 
					return (TRUE);
					}
				case IDC_BUTTON3:	// Help
					{
					// Display help dialog
					DLGPROC  lpfnDialog;
					lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)AboutDlgProc, hInstance);
					DialogBox( hInstance, "ABOUTDIALOG", NULL, (DLGPROC)lpfnDialog);
					FreeProcInstance( (FARPROC)lpfnDialog);
					return (TRUE);
					}
				case IDC_RADIO1:	// New Model",
				case IDC_RADIO5:	// Existing Model
				case IDC_RADIO2:	// Open Transformer
				case IDC_RADIO3:	// Open Transformer and Use AutoDesign
				case IDC_RADIO4:	// Open PowerPlay
					{
					// Set indicator and turn off alternative radio buttons
					AjustDialogForCheckedOption( hDlg);
					return (TRUE);
					}
				case IDC_BUTTON1:	// Browse for MDL file
					{
					// Run File Open Dialog
					if (szFileOpenDialog( hDlg, strcpy( sz, "*.mdl"), sizeof( sz) ) != NULL)
						setControlData( hDlg, IDC_EDIT1, sz);	// Name of the MDL file
					return (TRUE);
					}
				case IDC_BUTTON2:	// Browse for datafile
					{
					// Run File Open Dialog
					if (szFileOpenDialog( hDlg, strcpy( sz, "*.txt"), sizeof( sz) ) != NULL)
						setControlData( hDlg, IDC_EDIT4, sz);	// Name of the data file.
					return (TRUE);
					}
				}
			}
		}
	return (FALSE);		// Message not handled, return control to windows
	}

// Main program  -  Loads data from hand held terminal (PDT 3100) into VMS
int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	if (hPrevInstance == (HANDLE)0)
		{
		// Set up user display
		DLGPROC lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)DlgProc, hInstance);
		DialogBox( hInstance, "IDD_DIALOG1", (HWND)NULL, (DLGPROC)lpfnDialog);
		FreeProcInstance( (FARPROC)lpfnDialog);
		}
	return (0);
	}

/*

		case WM_INITDIALOG :
			{
			// Setup horizontal scroll width in listbox
			int iExtent = 256 + (LOWORD( GetDialogBaseUnits() ) * 300);
			SendDlgItemMessage( hDlg, IDC_LIST1, (WORD)LB_SETHORIZONTALEXTENT, (WPARAM)iExtent, (LPARAM)0L);
			return (TRUE);									
			}
		case IDC_PDTPAL :										// User defined message
			{
			SendDlgItemMessage( hDlg, IDC_LIST1, (WORD)LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"Overfører data ...");
			UpdateWindow( hDlg);
			PostMessage( hDlg, IDC_RUN, (WPARAM)0, (LPARAM)0);		// Send message to this window to start main logic
			return (TRUE);
			}
		case IDC_RUN :										// User defined message
			{
			// Enable new default button
			EnableWindow( GetDlgItem( hDlg, IDC_OK), TRUE);	
			SendMessage( hDlg, WM_NEXTDLGCTL, 0, FALSE);
			SetFocus( (HWND)GetDlgItem( hDlg, IDC_OK) );	
			return (TRUE);
			}
				case IDC_START : 
					{
					SendDlgItemMessage( hDlg, IDC_LIST1, (WORD)LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"Starter program for å kommuneisere med håndterminal ...");
					EnableWindow( GetDlgItem( hDlg, IDC_CANCEL), FALSE);	// Disable button
					EnableWindow( GetDlgItem( hDlg, IDC_OK),     FALSE);	// Disable button
					EnableWindow( GetDlgItem( hDlg, IDC_START),  FALSE);	// Disable button
					// Load hour-glass cursor.
					hCursor = LoadCursor( NULL, IDC_WAIT);
					SetCursor( hCursor);
					UpdateWindow( hDlg);
					PostMessage( hDlg, IDC_PDTPAL, (WPARAM)0, (LPARAM)0);		// Send message to this window to start comunication with bar code terminal
					return (TRUE);
					}


*/
// End of file
