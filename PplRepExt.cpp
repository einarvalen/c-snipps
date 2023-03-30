/*******************************************************************************
 *
 *  PplRepExt.cpp - Search Extension for PowerPlay
 *
 *	March 3rd 1999 Einar Valen IC-Bergen
 *
 ******************************************************************************/

#include "stdafx.h"
#include <winbase.h>
#include "resource.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <fstream.h>


// Definitions
#define OK 1
enum eNewOptions		{ eTransformer, eAutoDesign, ePowerPlay };
enum ePowerPlayModel	{ eNew, eExisting };


// Global variables
int  giNewOptions;					// For Dialog box radio buttons
int  giPowerPlayModel;				// For Dialog box radio buttons
HINSTANCE hInstance;				// For Windows
char gszDataFile[BUFSIZ]			= "C:\\TEMP\\PplData.txt";	// Name of the data file.
char gszModelFile[BUFSIZ]			= "C:\\TEMP\\PplData.mdl";	// Model file name
char gszPowerPlayModel[40]			= "New";		// Indicates if user intends to crete a new model
char gszColDelimiter[10]			= ",";			// Column delimiter to use.
char gszDecimalSeparator[10]		= ".";			// Decmal separator to use.
char gszPowerPlayModelOptions[15]	= "AutoDesign"; //"PowerPlay" // "Transformer";	// How does the user wish to build the new model
int	 giDataFD   = -1;				// File descriptor for the data file.
long glRowCount = 0;				// Track the number of rows processed.


// Function prototypes

// This function returns the extension name to Actuate which the user sees.
// Note:  The implementation could be changed to get the name from the
//		  envrironment allowing for OEM customization.
const char* FAR PASCAL PplRepExtGetName();	// Invoked from Actuate
// This function returns the parameter string to Actuate for serialization to the definition file.
// Currently, there are two parameters delimited with an asterisk.
const char* FAR PASCAL PplRepExtGetParameters();	// Invoked from Actuate
// This function is used by Actuate to pass the de-serialized parameters to the extension.
void FAR PASCAL PplRepExtSetParameters( const char* szParms );	// Invoked from Actuate
// This function displays a dialog to allow the user to input the parameters.
BOOL FAR PASCAL PplRepExtInputParameters();	// Invoked from Actuate
// This function returns the column delimeter which Actuate will use when constructing the rows.
const char* FAR PASCAL PplRepExtGetDelimiter();	// Invoked from Actuate
// This function indicates to Actuate if a row of column names should be sent before the first data row.
BOOL FAR PASCAL PplRepExtIncludeHeader();	// Invoked from Actuate
// This function is called when Actuate begins the data extraction. This implementation creates the csv data file and sets the FD.
BOOL FAR PASCAL PplRepExtOpen();	// Invoked from Actuate
// This function is called as each row is constructed during the data extraction.
// This implementation writes the row to the csv data file.
BOOL FAR PASCAL PplRepExtPutRow( long rowIdx, const char* row );	// Invoked from Actuate
// This function is called when the data extraction is complete.
// This implementation closes the csv data file and then executes
// FileQuery with the query design specified in the parameters. 
BOOL FAR PASCAL PplRepExtClose( long clsState );	// Invoked from Actuate
// Retreieves parameters serially from buffer 
static BOOL GetParam( const char *&p, char *szValue, int iValueLen);
// Create and fill new MDL File
void FillModelFile( void);
// Creates MDL file name identical to datafile but with .mdl extension
char *CreateModelFileName( char *szModelFile, int iModelFileLen, char *szDataFile);
// Extract file-name without extension from path
char *CreateModelName( char *szPath, char *szModelName, int iModelNameLen);
// Creates MDC file name identical to MDL file name but without extension
char *CreateMDCFileName( char *szMDCFileName, int iMDCFileNameLen, char *szModelFile);
// Convert user options into interger values
void GetOptions( int *iPowerPlayModel, int *iNewOptions);
// Reports last error from Windows 
void WinError( char *sz);	// For Dialog box
// Executes command line and may (or may not) wait for process to finish
BOOL ExecCmd( char *szCmdLine, bool bWait);
// Centers dialog on screen
void vCenterWindow( HWND hDlg);	// For Dialog box
// Windows dialog box event handeler - Help display
static BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);	// For Dialog box
// Common File Open dialog
char *FileOpenDialog( HWND hDlg, char *szFile, size_t iFileLen, bool bModelFile);	// For Dialog box
// Fetch texts from DialogBox (hDlg) Edit Controls 
 void getControlData( HWND hDlg, int iCtrl, char *szBuf, size_t iBufLen);	// For Dialog box
// Sets texts on DialogBox (hDlg) Edit Controls 
void setControlData( HWND hDlg, int iCtrl, char *szBuf);	// For Dialog box
// Adjust dialog acording to current checked options
void AjustDialogForCheckedOption( HWND hDlg);	// For Dialog box
// Windows dialog box event handeler
static BOOL CALLBACK  DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);	// For Dialog box


// This function returns the extension name to Actuate which the user sees.
// Note:  The implementation could be changed to get the name from the
//		  envrironment allowing for OEM customization.
const char* FAR PASCAL PplRepExtGetName()
	{
	return ("PowerPlay 5.x/6.x (32-bit)");
	}


// This function returns the parameter string to Actuate for serialization to the definition file.
// Currently, there are two parameters delimited with an asterisk.
const char* FAR PASCAL PplRepExtGetParameters()
	{
	static CString parmStr;
	parmStr =  gszDataFile;					parmStr += "*";	// Name of the data file.
	parmStr += gszModelFile;				parmStr += "*";	// Mane of the MDL file
	parmStr += gszColDelimiter;				parmStr += "*";	// Column delimiter to use.
	parmStr += gszDecimalSeparator;			parmStr += "*";	// Column delimiter to use.
	parmStr += gszPowerPlayModel;			parmStr += "*";	// Indicates if user intends to crete a new model
	parmStr += gszPowerPlayModelOptions;	parmStr += "*";	// How does the user wish to build the new model
	return ( (const char*)parmStr);
	}


static BOOL GetParam( const char *&p, char *szValue, int iValueLen)
	{
	if (!*p)
		return (FALSE);
	memset( szValue, 0, iValueLen);
	for (int i = 0; *p  &&  *p != '*' && i < iValueLen; p++, i++)
		szValue[i] = *p;
	szValue[i] = '\0';
	if (*p == '*')
		p++;
	return (TRUE);
	}


// This function is used by Actuate to pass the de-serialized parameters to the extension.
void FAR PASCAL PplRepExtSetParameters( const char* szParms )
	{
	if (szParms == NULL || *szParms == 0 )			
		return;
	if (GetParam( szParms, gszDataFile, sizeof( gszDataFile) ) )					
		if (GetParam( szParms, gszModelFile, sizeof( gszModelFile) ) )					
			if (GetParam( szParms, gszColDelimiter, sizeof( gszColDelimiter) ) )			
				if (GetParam( szParms, gszDecimalSeparator, sizeof( gszDecimalSeparator) ) )	
					if (GetParam( szParms, gszPowerPlayModel, sizeof( gszPowerPlayModel) ) )					
						GetParam( szParms, gszPowerPlayModelOptions, sizeof( gszPowerPlayModelOptions) );
	}


// This function displays a dialog to allow the user to input the parameters.
BOOL FAR PASCAL PplRepExtInputParameters()
	{
	// Set up user display
	hInstance = (HINSTANCE)GetModuleHandle( "PplRepExt");
	if ( 0 > DialogBox( (HINSTANCE)hInstance, MAKEINTRESOURCE( IDD_OPTIONS_DIALOG), (HWND)GetForegroundWindow(), (DLGPROC)DlgProc) )
		WinError( "Error invoking dialog IDD_OPTIONS_DIALOG");
	return (TRUE);
	}


// This function returns the column delimeter which Actuate will use when constructing the rows.
const char* FAR PASCAL PplRepExtGetDelimiter()
	{
	return gszColDelimiter;
	}


// This function indicates to Actuate if a row of column names should be sent before the first data row.
BOOL FAR PASCAL PplRepExtIncludeHeader()
	{
	return (TRUE);
	}


// This function is called when Actuate begins the data extraction. This implementation creates the csv data file and sets the FD.
BOOL FAR PASCAL PplRepExtOpen()
	{
	// Open Datafile
	if (sizeof( gszDataFile) == 0)
		return (FALSE);
	if (0 > (giDataFD = open( gszDataFile, O_CREAT|O_TRUNC|O_RDWR, S_IREAD|S_IWRITE ) ) )
		return FALSE;
	glRowCount = 0;
	return TRUE;
	}


// This function is called as each row is constructed during the data extraction.
// This implementation writes the row to the csv data file.
BOOL FAR PASCAL PplRepExtPutRow( long rowIdx, const char* row )
	{
	if (giDataFD < 0)
		return FALSE;
	int rowLen = strlen( row);
	if (write( giDataFD, row, rowLen) < rowLen)
		return FALSE;
	if (write( giDataFD, "\n", 1 ) < 1)
		return FALSE;
	glRowCount++;
	return TRUE;
}


// Executes command line and waits for process to finish
BOOL ExecCmd( char *szCmdLine, bool bWait)
	{
	PROCESS_INFORMATION tProcInfo; 
	STARTUPINFO  tStartInfo;
	ZeroMemory( &tStartInfo, sizeof( tStartInfo) );
	tStartInfo.cb = sizeof( tStartInfo);
	if ( !CreateProcess( NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &tStartInfo, &tProcInfo) )
		return (FALSE);
	if (bWait)
		{
		if (WaitForSingleObject( tProcInfo.hProcess, INFINITE) == WAIT_FAILED)
			return (FALSE);
		}
	BOOL bRet =  CloseHandle(tProcInfo.hProcess); 
    bRet &= CloseHandle( tProcInfo.hThread);
	return (bRet); 
	}


// This function is called when the data extraction is complete.
// This implementation closes the csv data file and then executes
// FileQuery with the query design specified in the parameters. 
BOOL FAR PASCAL PplRepExtClose( long clsState )
	{
	// Convert user options into integer values
	int iPowerPlayModel, iNewOptions;
	GetOptions( &iPowerPlayModel, &iNewOptions);
	// Close Files
	if (giDataFD > 0) 
		close( giDataFD );  
	giDataFD  = -1;	
	// Create and fill new MDL File
	if (iPowerPlayModel == eNew)
		FillModelFile();
	// Execute transformer (and PowerPlay)
	if (glRowCount > 0 && clsState != 2 )
		{
		char szCmd[BUFSIZ], sz[BUFSIZ];
		// Build command line to run transformer
		GetPrivateProfileString( "Cognos Locations", "PowerPlay Transformer", "", sz, sizeof( sz), "COGNOS.INI");
		strcpy( szCmd, "\"");	 // Command line may need to be hyphenated due to spaces in file names
		strcat( szCmd, sz);
		strcat( szCmd, "\\trnsfrmr.exe\" ");
		if (iPowerPlayModel == eExisting 
				|| (iPowerPlayModel == eNew && iNewOptions == ePowerPlay) )
			strcat( szCmd, "-n -s ");
		strcat( szCmd, "\"");	 // File Name may need to be hyphenated due to spaces in file names
		strcat( szCmd, gszModelFile);
		strcat( szCmd, "\"");	 // File Name may need to be hyphenated due to spaces in file names
		if (iPowerPlayModel == eExisting 
				|| (iPowerPlayModel == eNew && iNewOptions == ePowerPlay) )
			{
			// Execute transformer and wait for it to finish
			if (!ExecCmd( szCmd, TRUE) ) 
				WinError( szCmd);
			// Build command line to run PowerPlay
			if (GetPrivateProfileString( "Cognos Locations", "PowerPlay 6.0", "", sz, sizeof( sz), "COGNOS.INI") == 0)	// PowerPlay 6,0
				GetPrivateProfileString( "Cognos Locations", "PowerPlay",     "", sz, sizeof( sz), "COGNOS.INI");		// PowerPlay 5,0
			strcpy( szCmd, "\"");	 // Command line may need to be hyphenated due to spaces in file names
			strcat( szCmd, sz);
			strcat( szCmd, "\\pwrplay.exe\" ");
			char szMDCFileName[BUFSIZ];
			CreateMDCFileName( szMDCFileName, sizeof( szMDCFileName), gszModelFile);	
			strcat( szCmd, "\"");	 // File Name may need to be hyphenated due to spaces in file names
			strcat( szCmd, szMDCFileName);
			strcat( szCmd, ".mdc");
			strcat( szCmd, "\"");	 // File Name may need to be hyphenated due to spaces in file names
			// Execute PowerPlay
			if (!ExecCmd( szCmd, FALSE) )
				WinError( szCmd);
			}
		else
			{
			// Execute transformer 
			if (!ExecCmd( szCmd, FALSE) ) 
				WinError( szCmd);
			}
		}
	return TRUE;
	}


// Create and fill new MDL File
void FillModelFile( void) 
	{
	// Create MDL filename based on data file name 
	CreateModelFileName( gszModelFile, sizeof( gszModelFile), gszDataFile);	
	char szsModelName[BUFSIZ];
	// Extract model name from MDL file name
	CreateModelName( gszModelFile, szsModelName, sizeof( szsModelName) );
	// Create MDCFile name
	char szMDCFileName[BUFSIZ];
	CreateMDCFileName( szMDCFileName, sizeof( szMDCFileName), gszModelFile);	
	// Fill MDL file
	ofstream ofs( gszModelFile);
	ofs << "NewModel \"" << szsModelName << "\""<< endl;
	ofs << "QueryMake \"" << szsModelName << "\" Separator \"" << gszColDelimiter << "\" SourceType" << endl;
	ofs << "FlatFile_ColNames CharacterSet Ansi DecimalSep \"" << gszDecimalSeparator << "\"" << endl;
	ofs << "Source \"" << gszDataFile << "\" CreateColumns \"" << szsModelName  << "\""<< endl;
	int iPowerPlayModel, iNewOptions;
	GetOptions( &iPowerPlayModel, &iNewOptions);
	if (iNewOptions != eTransformer)
		{
		ofs << "AutoDesign" << endl;
		if (iNewOptions != eAutoDesign)
			{
			ofs << "CubeMake \"" << szsModelName << "\""<< " MDCFile \"" << szMDCFileName << "\""<< endl;
			ofs << "Createfiles"<< endl;	
			ofs << "ModelEnsureCompleteness"<< endl; 
			}
		}
	ofs.close();
	}


// Creates MDL file name identical to datafile but with .mdl extension
char *CreateModelFileName( char *szModelFile, int iModelFileLen, char *szDataFile)
	{
	for (int i = 0; i < iModelFileLen && szDataFile[i]; i++)
		{
		szModelFile[i] = szDataFile[i];	// Copy from DataFile name
		if (szModelFile[i] == '.')		// Find extension
			{
			szModelFile[i] = '\0';		// Truncate string
			break;
			}
		}
	strcat( szModelFile, ".mdl");	// Append new file extension
	return (szModelFile);
	}


// Extract file-name without extension from path
char *CreateModelName( char *szPath, char *szModelName, int iModelNameLen)
	{
	char *pszFileName = szPath;
	// Find start of filename in path
	for (int i = strlen( szPath); i > 0; i--)
		if (szPath[i] == '\\')		
			{
			pszFileName = &szPath[i+1];			// Point to filename
			break;
			}
	// Copy filename without extension
	memset( szModelName, 0, iModelNameLen);		// Initialize ModelName
	for (i = 0; pszFileName[i] && i < iModelNameLen;  i++)
		{
		szModelName[i] = pszFileName[i];	
		if (szModelName[i] == '.')				// Skip file extension
			{
			szModelName[i] = '\0';		
			break;
			}
		}
	return (szModelName);		
	}


// Creates MDC file name identical to MDL file name but without extension
char *CreateMDCFileName( char *szMDCFileName, int iMDCFileNameLen, char *szModelFile)
	{
	for (int i = 0; i < iMDCFileNameLen && szModelFile[i]; i++)
		{
		szMDCFileName[i] = szModelFile[i];	// Copy from MDL file name
		if (szMDCFileName[i] == '.')		// Find extension
			{
			szMDCFileName[i] = '\0';		// Truncate string
			break;
			}
		}
	return (szMDCFileName);
	}


// Convert user options into interger values
void GetOptions( int *iPowerPlayModel, int *iNewOptions)
	{
	if (strcmp( gszPowerPlayModel, "New") == 0)
		{
		*iPowerPlayModel = eNew;
		if (strcmp( gszPowerPlayModelOptions, "Transformer") == 0)
			*iNewOptions = eTransformer;
		else if (strcmp( gszPowerPlayModelOptions, "AutoDesign") == 0)
			*iNewOptions = eAutoDesign;
		else
			*iNewOptions = ePowerPlay;
		}
	else
		*iPowerPlayModel = eExisting;

	}

// Windows User dialog Functions

// Reports last error from Windows 
void WinError( char *sz)
	{
	LPVOID lpMsgBuf;
	// Get error messge
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
			GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, sz, MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );	// Free the buffer lpMsgBuf
	}


// Centers dialog on screen
void vCenterWindow( HWND hDlg)
	{
	RECT rc, rcDTop;
	int NewX, NewY, CenterX, CenterY;
	HWND hDTopWnd;
	hDTopWnd = GetDesktopWindow();
	GetWindowRect(hDTopWnd, &rcDTop);	// desktop rectangle size
	GetWindowRect(hDlg, &rc);			// dialog box rectangle size
	// Calculate window position in center
	CenterX = (rcDTop.left + rcDTop.right) / 2;
	CenterY = (rcDTop.top + rcDTop.bottom) / 2;
	NewX = CenterX - ( (rc.right - rc.left) / 2);
	NewY = CenterY - ( (rc.bottom - rc.top) / 2);
	// Give it a new location
	SetWindowPos( hDlg, NULL, NewX, NewY, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}


// Callback windows event loop handeler procedure for the help display
static BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
char *FileOpenDialog( HWND hDlg, char *szFile, size_t iFileLen, bool bModelFile)
	{
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof( OPENFILENAME) );
	ofn.lStructSize = sizeof( OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = iFileLen;
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (bModelFile)
		{
		ofn.lpstrFilter = "Model files (*.mdl; *.pyh; *.pyg)\0*.MDL;*.PYH;*.PYG\0\0";
		ofn.lpstrTitle = "Select Model File";
		}
	else
		{
		ofn.lpstrFilter = "Data files (*.txt; *.asc; *.csv)\0*.TXT;*.ASC;*.CSV\0All files (*.*)\0*.*\0\0";
		ofn.lpstrTitle = "Select Data File";
		}
	if (GetOpenFileName( &ofn) )
		return (szFile);
	else
		return (NULL);
	}

// Fetch texts from DialogBox (hDlg) Edit Controls 
 void getControlData( HWND hDlg, int iCtrl, char *szBuf, size_t iBufLen)
	{
	// Data from Edit box
	if (iCtrl == IDC_EDIT_ModelFile ||  iCtrl == IDC_EDIT_ColumnDelimiter || iCtrl == IDC_EDIT_DecimalSeparator || iCtrl == IDC_EDIT_DataFile)
		{
		HWND hwndCtrl = GetDlgItem( hDlg, iCtrl);
		size_t iLen = 1 + (int)SendMessage( hwndCtrl, WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
		SendMessage( hwndCtrl, WM_GETTEXT, (WPARAM)(iLen > iBufLen) ? iBufLen : iLen, (LPARAM)(LPSTR)szBuf);
		}
	// Get check state for 	new og existing model
	else if (iCtrl == IDC_RADIO_NewModel || iCtrl == IDC_RADIO_ExistingModel)
		{
		if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO_NewModel, BM_GETCHECK, 0, 0) )		// Is New Model checked
			strcpy( szBuf, "New");
		else
			strcpy( szBuf, "Existing");
		}
	// Get check state for new options
	else if (iCtrl == IDC_RADIO_Transformer || iCtrl == IDC_RADIO_AutoDesign || iCtrl == IDC_RADIO_PowerPlay)
		{
		if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO_Transformer, BM_GETCHECK, 0, 0) )		// Is the Tansformer option checked ?
			strcpy( szBuf, "Transformer");
		else if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_GETCHECK, 0, 0) )   	// Is the PowerPlay option checked ?
			strcpy( szBuf, "PowerPlay");
		else
			strcpy( szBuf, "AutoDesign");
		}
	}


// Sets texts on DialogBox (hDlg) Edit Controls 
void setControlData( HWND hDlg, int iCtrl, char *szBuf)
	{
	// Display Edit box
	if (iCtrl == IDC_EDIT_ModelFile ||  iCtrl == IDC_EDIT_ColumnDelimiter || iCtrl == IDC_EDIT_DecimalSeparator || iCtrl == IDC_EDIT_DataFile)
		SendDlgItemMessage( hDlg, iCtrl, (WORD)WM_SETTEXT,	(WPARAM)0, (LPARAM)(LPSTR)szBuf);
	// Set correct check state for checkboxes
	else if (iCtrl == IDC_RADIO_NewModel || iCtrl == IDC_RADIO_ExistingModel)
		{
		if (strcmp( szBuf, "New") == 0)
			{
			giPowerPlayModel = eNew;
			SendDlgItemMessage( hDlg, IDC_RADIO_NewModel, BM_SETCHECK, TRUE, 0);		// New Model checked
			SendDlgItemMessage( hDlg, IDC_RADIO_ExistingModel, BM_SETCHECK, FALSE, 0);	// Existing Model unchecked
			}
		else			 
			{
			giPowerPlayModel = eExisting;
			SendDlgItemMessage( hDlg, IDC_RADIO_NewModel, BM_SETCHECK, FALSE, 0);		// New Model unchecked
			SendDlgItemMessage( hDlg, IDC_RADIO_ExistingModel, BM_SETCHECK, TRUE, 0);	// Existing Model checked
			}
        }
	else if (iCtrl == IDC_RADIO_Transformer || iCtrl == IDC_RADIO_AutoDesign || iCtrl == IDC_RADIO_PowerPlay)
		{
		// Set all valid options unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO_Transformer, BM_SETCHECK, FALSE, 0);	// Open Transformer	unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO_AutoDesign, BM_SETCHECK, FALSE, 0);		// Open Transformer and Use AutoDesign unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_SETCHECK, FALSE, 0);		// Open PowerPlay unchecked
		// Set correct check state for current option
		if (strcmp( szBuf, "Transformer") == 0)
			{
			giNewOptions = eTransformer;
			SendDlgItemMessage( hDlg, IDC_RADIO_Transformer, BM_SETCHECK, TRUE, 0);	// Open Transformer checked
			}
		else if (strcmp( szBuf, "PowerPlay") == 0)
			{
			giNewOptions = ePowerPlay;
			SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_SETCHECK, TRUE, 0);	// Open PowerPlay checked
			}
		else
			{
			giNewOptions = eAutoDesign;
			SendDlgItemMessage( hDlg, IDC_RADIO_AutoDesign, BM_SETCHECK, TRUE, 0);	// Open Transformer and Use AutoDesign checked
			}
		}
	}


// Adjust dialog acording to current checked options
void AjustDialogForCheckedOption( HWND hDlg)
	{
	// Set correct check state for current new-model-option
	if (giNewOptions == eTransformer)
		{
		SendDlgItemMessage( hDlg, IDC_RADIO_Transformer, BM_SETCHECK, TRUE, 0);			// Open Transformer checked
		SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_SETCHECK, FALSE, 0);			// Open PowerPlay unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO_AutoDesign, BM_SETCHECK, FALSE, 0);			// Open Transformer and Use AutoDesign unchecked
		}
	//else if ( (int) SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_GETCHECK, 0, 0) ) // Open PowerPlay checked ?
	else if (giNewOptions == ePowerPlay)
		{
		SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_SETCHECK, TRUE, 0);			// Open PowerPlay checked
		SendDlgItemMessage( hDlg, IDC_RADIO_Transformer, BM_SETCHECK, FALSE, 0);		// Open Transformer unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO_AutoDesign, BM_SETCHECK, FALSE, 0);			// Open Transformer and Use AutoDesign unchecked
		}
	else  	// Open Transformer and Use AutoDesign checked
		{
		SendDlgItemMessage( hDlg, IDC_RADIO_AutoDesign, BM_SETCHECK, TRUE, 0);			// Open Autodesign checked
		SendDlgItemMessage( hDlg, IDC_RADIO_Transformer, BM_SETCHECK, FALSE, 0);		// Open Transformer unchecked
		SendDlgItemMessage( hDlg, IDC_RADIO_PowerPlay, BM_SETCHECK, FALSE, 0);			// Open PowerPlay unchecked
		}
	// Set correct check state for current PowerPlay-model-option
	if (giPowerPlayModel == eNew)	// New Model checked
		{
		SendDlgItemMessage( hDlg, IDC_RADIO_NewModel, BM_SETCHECK, TRUE, 0);	// New Model checked
		// Enable valid options
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_Transformer), TRUE);			// Enable option Open Transformer	
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_AutoDesign), TRUE);			// Enable option Open Transformer and Use AutoDesign 
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_PowerPlay), TRUE);			// Enable option Open PowerPlay 
		// Disable invalid options
		EnableWindow( GetDlgItem( hDlg, IDC_EDIT_ModelFile), FALSE);			// Disable edit box for MDL file name
		EnableWindow( GetDlgItem( hDlg, IDC_BUTTON_BROWSE_ModelFile), FALSE);	// Disable browse button for MDL file name
		// Turn off alternative radio button
		SendDlgItemMessage( hDlg, IDC_RADIO_ExistingModel, BM_SETCHECK, FALSE, 0);	// Existing Model unchecked
		}
	else	// Existing Model checked
		{
		SendDlgItemMessage( hDlg, IDC_RADIO_ExistingModel, BM_SETCHECK, TRUE, 0);	// Existing Model checked
		// Enable valid options
		EnableWindow( GetDlgItem( hDlg, IDC_EDIT_ModelFile), TRUE);				// Enable edit box for MDL file name
		EnableWindow( GetDlgItem( hDlg, IDC_BUTTON_BROWSE_ModelFile), TRUE);	// Disable browse button for MDL file name
		// Disable invalid options
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_Transformer),  FALSE);		// Disable option Open Transformer	
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_AutoDesign), FALSE);			// Disable option Open Transformer and Use AutoDesign 
		EnableWindow( GetDlgItem( hDlg, IDC_RADIO_PowerPlay), FALSE);			// Disable option Open PowerPlay 
		// Turn off alternative radio button
		SendDlgItemMessage( hDlg, IDC_RADIO_NewModel, BM_SETCHECK, FALSE, 0);	// New Model unchecked
		}
	UpdateWindow( hDlg);	// Refresh dialog
	}


// Windows dialog box event handeler
static BOOL CALLBACK  DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG :
			{
			// Center dialog on screen
			vCenterWindow( hDlg);
			// Enable default button - OK
			EnableWindow( GetDlgItem( hDlg, IDOK), TRUE);	
			SetFocus( (HWND)GetDlgItem( hDlg, IDOK) );	
			// Set controls according to present setting
			setControlData( hDlg, IDC_EDIT_DataFile, gszDataFile);					// Name of the data file.
			setControlData( hDlg, IDC_EDIT_ModelFile, gszModelFile);				// Name of the MDL file
			setControlData( hDlg, IDC_EDIT_ColumnDelimiter, gszColDelimiter);		// Column delimiter to use.
			setControlData( hDlg, IDC_EDIT_DecimalSeparator, gszDecimalSeparator);	// Decimal separator to use.
			setControlData( hDlg, IDC_RADIO_NewModel, gszPowerPlayModel);			// Indicates if user intends to crete a new model
			setControlData( hDlg, IDC_RADIO_Transformer, gszPowerPlayModelOptions);	// How does the user wish to build the new model
			AjustDialogForCheckedOption( hDlg);
			return (TRUE);									
			}
		case WM_COMMAND:	// Dialog control message
			{
			char sz[BUFSIZ];
			switch (wParam)
				{
				case IDOK:	// OK
					{
					char szD[BUFSIZ], szS[BUFSIZ];
					// Get control data, verify and commit
					getControlData( hDlg, IDC_EDIT_ColumnDelimiter, szD, sizeof( szD) );	 // Column delimiter to use.
					getControlData( hDlg, IDC_EDIT_DecimalSeparator,szS, sizeof( szS) );	// Decimal separator to use.
					// Delimiter and Decimal separator must be one byte
					if ( !(strlen(szS) == 1 && strlen(szD) == 1) )
						{
						MessageBox( hDlg, (LPCTSTR)"Delimiter and decimal separator can only be one character long", "Error - Invalid data", MB_OK | MB_ICONINFORMATION );
						return (TRUE);
						}
					// Commit changes
					strcpy( gszColDelimiter,	 szD);	// Column delimiter to use.
					strcpy( gszDecimalSeparator, szS);	// Decimal separator to use.
					getControlData( hDlg, IDC_EDIT_DecimalSeparator,gszDecimalSeparator,		sizeof( gszDecimalSeparator) );		// Decimal separator to use
					getControlData( hDlg, IDC_EDIT_DataFile, 		gszDataFile, 				sizeof( gszDataFile) );				// Name of the data file.
					getControlData( hDlg, IDC_EDIT_ModelFile,		gszModelFile, 				sizeof( gszModelFile) );			// Mane of the MDL file
					getControlData( hDlg, IDC_RADIO_Transformer, 	gszPowerPlayModelOptions,	sizeof( gszPowerPlayModelOptions) );// How does the user wish to build the new model
					getControlData( hDlg, IDC_RADIO_NewModel, 		gszPowerPlayModel, 			sizeof( gszPowerPlayModel) );		// Indicates if user intends to crete a new model
					EndDialog( hDlg, 0);	// End program dialog 
					return (TRUE);
					}
				case IDCANCEL:	// Cancel
					{
					// End program dialog 
					EndDialog( hDlg, 0); 
					return (TRUE);
					}
				case IDC_BUTTON_Help:	// Help
					{
					// Display help dialog
					if ( 0 > DialogBox( (HINSTANCE)hInstance, MAKEINTRESOURCE( IDD_ABOUT_DIALOG), (HWND)NULL, (DLGPROC)AboutDlgProc) )
						WinError( "Error invoking dialog IDD_ABOUT_DIALOG");
					return (TRUE);
					}
				case IDC_RADIO_ExistingModel:	// Existing Model
				case IDC_RADIO_NewModel:		// New Model",
				case IDC_RADIO_Transformer:		// Open Transformer
				case IDC_RADIO_AutoDesign:		// Open Transformer and Use AutoDesign
				case IDC_RADIO_PowerPlay:		// Open PowerPlay
					{
					switch (wParam)
						{
						case IDC_RADIO_NewModel:		giPowerPlayModel = eNew;		break;
						case IDC_RADIO_ExistingModel:	giPowerPlayModel = eExisting;	break;
						case IDC_RADIO_Transformer:		giNewOptions = eTransformer;	break;
						case IDC_RADIO_AutoDesign:		giNewOptions = eAutoDesign;		break;
						case IDC_RADIO_PowerPlay:		giNewOptions = ePowerPlay;		break;
						}
					// Set check state and turn off alternative radio buttons
					AjustDialogForCheckedOption( hDlg);
					return (TRUE);
					}
				case IDC_BUTTON_BROWSE_ModelFile:	// Browse for MDL file
					{
					// Run File Open Dialog
					if (FileOpenDialog( hDlg, strcpy( sz, gszModelFile), sizeof( sz), TRUE) != NULL)
						setControlData( hDlg, IDC_EDIT_ModelFile, sz);	// Name of the MDL file
					return (TRUE);
					}
				case IDC_BUTTON_BROWSE_DataFile:	// Browse for datafile
					{
					// Run File Open Dialog
					if (FileOpenDialog( hDlg, strcpy( sz, gszDataFile), sizeof( sz), FALSE ) != NULL)
						setControlData( hDlg, IDC_EDIT_DataFile, sz);	// Name of the data file.
					return (TRUE);
					}
				}
			}
		}
	return (FALSE);		// Message not handled, return control to windows
	}

	
// End of file.
