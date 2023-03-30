// phwinfix.cpp - Konverteringsprogram for PowerHouse Windows repositories
// Author: Einar Valen, Date: 20.1.1995, Tested ok: 16.2.95

// INCLUDES
#include <commdlg.h>
#include <strstrea.h>
#include <fstream.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <except.h>
#include "vwin.h"
#include "vwevents.h"
#include "vinfo.h"
#include "vstd.h"
#include "vwinmisc.h"
#include "vstring.h"


// DEFINES
#define vEDIT 1000
#define vTEXT 2000
#define vHELP 3000
#define vFIRST 1
#define vLAST 9
#define szFixFeileNorskeTegn(sz) sz
#define dJUSTERING_FOR_KORTE_FELT (3)  //(1 * 5.4); 30.1.96
#define dMED_ACCELERATORER  // Kobles ut dersom kortkommandoer skal utelukkes fra menyene
#ifndef NPO
#define dLEGG_TIL_ENDRE_MENYVALG  // Kobles ut dersom nytt endre-menyvalg ikke skal legges til
#else
//#define dLEGG_TIL_ENDRE_MENYVALG  // Kobles ut dersom endre-menyvalg skal utføres
#endif
//#define dVI_TESTER_HVORDAN_LABLENE_SAA_UT_FOER
#define dKNAPPERAD_SKAL_MED
//#define dKNAPPERADEN_FOERST
#define dKNAPPERADEN_SIST

// TYPES

// TvPhWinFix class
class TvPhWinFix
	{
	private:
		WPARAM wCurrentCtrl__;
		HINSTANCE hInstance__;
		size_t iBufferSize__;
		size_t iFontSize__;
#ifndef NPO
		char *szBuffer__, *szNull__,  *szRpoGglFilnavnHeader__, *szRpoGglFilnavnIn__,
#else
		char 	*szBuffer__, *szNull__,  *szRpoGglFilnavnHeader__, *szRpoGglFilnavnIn__,
#endif
				*szRpoGglFilnavnUt__, *szRpoApFilnavnIn__, *szRpoApFilnavnUt__,
#ifndef NPO
				*szFormSize__, *szFont__, *szFontSize__,  *szBackgroundColor__;
#else
				*szFormSize__, *szFont__, *szEditFont__, *szFontSize__,
				*szButtonFontSize__,  *szBackgroundColor__;
#endif
		enum eStatusOfInputStream  { ePassTru, eHeadingFound }eStatus;
	public:
		BOOL bDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
		TvPhWinFix( HANDLE hInstance);
		~TvPhWinFix();
		void vDialog( HWND hDlg);
		BOOL bRun( void);
		BOOL bFixAP( void);
		BOOL bFixGGL( void);
		BOOL bInsertHeading( FILE *fhUt);
		void vError( char *szMsg, char *szFunction)
			{
			TvInfo::vError( szMsg, __FILE__, "TvPhWinFix", szFunction);
			};
	};


// FUNCTION PROTOTYPES
void vAboutDlg( unsigned int hInstance);
char *szFileSaveAsDialog( HWND hDlg, char *szFile, size_t iFileLen);
char *szFileOpenDialog( HWND hDlg, char *szFile, size_t iFileLen);
BOOL bSelectBox( HINSTANCE hInstance, HWND hParent, char *szCaption, char *szPicked, int iPickedLen, char *szValueList, char cSep);

// GLOBAL VARIABLES

static TvPhWinFix *sgptvPhWinFix = NULL;


// FUNCTIONS

#pragma argsused
static BOOL CALLBACK _export DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	return (sgptvPhWinFix->bDlgProc( hDlg, message, wParam, lParam) );
	}


#pragma argsused
char *szFileOpenDialog( HWND hDlg, char *szFile, size_t iFileLen)
	{
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof( OPENFILENAME) );
	ofn.lStructSize = sizeof( OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = iFileLen;
#ifndef NPO
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
#else
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
#endif
	if (GetOpenFileName( &ofn) )
		return (szFile);
	else
		return (NULL);
	}


#pragma argsused
char *szFileSaveAsDialog( HWND hDlg, char *szFile, size_t iFileLen)
	{
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof( OPENFILENAME) );
	ofn.lStructSize = sizeof( OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = iFileLen;
#ifndef NPO
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
#else
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
#endif
	if (GetSaveFileName( &ofn) )
		return (szFile);
	else
		return (NULL);
	}


#pragma argsused
static BOOL CALLBACK _export AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	if (message == WM_INITDIALOG)
		vCenterWindow( hDlg);
#ifndef NPO
	else if (message == WM_COMMAND && wParam == IDOK)
#else
	else if (message == WM_COMMAND && (wParam == IDOK || wParam == IDCANCEL) )
#endif
		{
		EndDialog( hDlg, 0);
		return (OK);
		}
	return (FALSE);
	}

void vAboutDlg( unsigned int hInstance)
	{
	DLGPROC  lpfnDialog;
	lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)AboutDlgProc, hInstance);
	DialogBox( hInstance, "ABOUTDIALOG", NULL, (DLGPROC)lpfnDialog);
	FreeProcInstance( (FARPROC)lpfnDialog);
	}


#pragma argsused
BOOL TvPhWinFix::bDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG :
			{
			vCenterWindow( hDlg);
			SetMenu( hDlg, LoadMenu( hInstance__, (LPSTR)"vMnuPhWinFix") );
			// Setup tabstops and horizontal scroll width in listbox
			int iExtent = dRECSIZ + (LOWORD( GetDialogBaseUnits() ) * 300);
			SendDlgItemMessage( hDlg, CM_LISTBOX, (WORD)LB_SETHORIZONTALEXTENT, (WPARAM)iExtent, (LPARAM)0L);
			SendMessage( hDlg, (UINT)WM_COMMAND, (WPARAM)CM_FETCH, (LPARAM)0);
			wCurrentCtrl__ = vEDIT+vFIRST;
			vSetEditCtrlsTexts( hDlg, vEDIT+1, vEDIT+1, szRpoGglFilnavnIn__);
			vSetEditCtrlsTexts( hDlg, vEDIT+2, vEDIT+2, szRpoGglFilnavnUt__);
			vSetEditCtrlsTexts( hDlg, vEDIT+3, vEDIT+3, szRpoApFilnavnIn__);
			vSetEditCtrlsTexts( hDlg, vEDIT+4, vEDIT+4, szRpoApFilnavnUt__);
			vSetEditCtrlsTexts( hDlg, vEDIT+5, vEDIT+5, szRpoGglFilnavnHeader__);
			vSetEditCtrlsTexts( hDlg, vEDIT+6, vEDIT+6, szFormSize__);
			vSetEditCtrlsTexts( hDlg, vEDIT+7, vEDIT+7, szFont__);
			vSetEditCtrlsTexts( hDlg, vEDIT+8, vEDIT+8, szFontSize__);
//         DWORD dwIndex = SendDlgItemMessage( hdlg, vEDIT+9, CB_ADDSTRING, 0, (LPARAM)((LPSTR)"None")
			vSetEditCtrlsTexts( hDlg, vEDIT+9, vEDIT+9, szBackgroundColor__);
#ifdef NPO
//			vSetEditCtrlsTexts( hDlg, vEDIT+10, vEDIT+10, szEditFont__);
//			vSetEditCtrlsTexts( hDlg, vEDIT+11, vEDIT+11, szButtonFontSize__);
#endif
			return (TRUE);
			}
		case WM_PAINT :
			{
			if (!IsIconic( hDlg) ) // We need to paint the minimized icon ourselves
				return (FALSE);
			vPaintIconic( hDlg, hInstance__);
			return (TRUE);
			}
		case WM_COMMAND :
			{
			switch (wParam)
				{
				default:
					{
					if (wParam <= vEDIT+vLAST && wParam >= vEDIT+vFIRST)
						wCurrentCtrl__ = wParam;
					else if (wParam <= vHELP+vLAST && wParam >= vHELP+vFIRST)
						{
						char sz[128];
						switch (wParam)
							{
							default:
								break;
							case vHELP+1:
								if (szFileOpenDialog( hDlg, strcpy( sz, "*.ggl"), sizeof( sz) ) != NULL)
									vSetEditCtrlsTexts( hDlg, vEDIT+1, vEDIT+1, strcpy( szRpoGglFilnavnIn__, sz) );
								break;
							case vHELP+2:
								if (szFileSaveAsDialog( hDlg, strcpy( sz, "*.ggl"),sizeof( sz) ) != NULL)
									vSetEditCtrlsTexts( hDlg, vEDIT+2, vEDIT+2, strcpy( szRpoGglFilnavnUt__, sz) );
								break;
							case vHELP+3:
								if (szFileOpenDialog( hDlg, strcpy( sz, "*.ap"),sizeof( sz) ) != NULL)
									vSetEditCtrlsTexts( hDlg, vEDIT+3, vEDIT+3,  strcpy( szRpoApFilnavnIn__, sz) );
								break;
							case vHELP+4:
								if (szFileSaveAsDialog( hDlg, strcpy( sz, "*.ap"),sizeof( sz) ) != NULL)
									vSetEditCtrlsTexts( hDlg, vEDIT+4, vEDIT+4,  strcpy( szRpoApFilnavnUt__, sz) );
								break;
							case vHELP+5:
								if (szFileOpenDialog( hDlg, strcpy( sz, "*.ggl"),sizeof( sz) ) != NULL)
									vSetEditCtrlsTexts( hDlg, vEDIT+5, vEDIT+5,  strcpy( szRpoGglFilnavnHeader__, sz) );
								break;
							case vHELP+6:
								TvInfo::vNotice(
									"Angi tekst erstatning for standard formularstørrelse. \
									F.eks: 500 BY 350.");
								break;
							case vHELP+7:
#ifndef NPO
								TvInfo::vNotice(
									"Angi tekst erstatning for font og punktstørrelse. \
									F.eks: Arial.");
#else
								if (bSelectBox( hInstance__, hDlg, "Velg Font", szFont__, iBufferSize__, "Arial,Courier New,System,Times New Roman,Small Fonts,Terminal", ',') )
									vSetEditCtrlsTexts( hDlg, vEDIT+7, vEDIT+7, szFont__);
/*								TvInfo::vNotice(
									"Angi tekst erstatning for font. \
									F.eks: Arial."); */
#endif
								break;
							case vHELP+8:
#ifndef NPO
								TvInfo::vNotice(
									"Angi tekst erstatning for standard formularstørrelse. \
									F.eks: 10");
#else
								if (bSelectBox( hInstance__, hDlg, "Velg Font for bruk i input felter", szFontSize__, iBufferSize__, "5,6,7,8,9,10,11,12,13,14", ',') )
									vSetEditCtrlsTexts( hDlg, vEDIT+8, vEDIT+8, szFontSize__);
/*								TvInfo::vNotice(
									"Angi tekst erstatning for standard font størrelse. \
									F.eks: 10");*/
#endif
								break;
							case vHELP+9:
								if (bSelectBox( hInstance__, hDlg, "Velg Farge", szBackgroundColor__, iBufferSize__, "None,Black,IvoryBlack,LampBlack,Blue,LightBlue,Cadet,Cobalt,Navy,RoyalBlue,SkyBlue,SteelBlue,TurquoiseBlue,Ultramarine,Green,Chartreuse,CobaltGreen,EmeraldGreen,ForestGreen,DarkGreen,Mint,Olive,SeaGreen,Cyan,Aquamarine,Turquoise,Red,AlizarinCrimson,Brick,Coral,EnglishRed,Maroon,Pink,Beige,Brown,Tan,CadmiumOrange,DarkOrange,Orange,Magenta,BlueViolet,Orchid,Plum,Purple,Violet,Yellow,CadmiumLemon,Gold,White,Ivory,Lavender,VeryLightGrey,LightGrey,Grey,VeryDarkGrey,SlateGrey,WarmGrey", ',') )
									vSetEditCtrlsTexts( hDlg, vEDIT+9, vEDIT+9, szBackgroundColor__);
								break;
#ifdef NPO
/*
							case vHELP+10:
								if (bSelectBox( hInstance__, hDlg, "Velg Font for bruk i input felter", szEditFont__, iBufferSize__, "Arial,Courier New,System,Times New Roman,Small Fonts,Terminal", ',') )
									vSetEditCtrlsTexts( hDlg, vEDIT+10, vEDIT+10, szEditFont__);
								break;
							case vHELP+11:
								if (bSelectBox( hInstance__, hDlg, "Velg Font for bruk i input felter", szButtonFontSize__, iBufferSize__, "5,6,7,8,9,10,11,12,13,14", ',') )
									vSetEditCtrlsTexts( hDlg, vEDIT+11, vEDIT+11, szButtonFontSize__);
								TvInfo::vNotice(
									"Angi tekst erstatning for standard fontstørrelse for bruk i buttons forran ledetekstene. \
									F.eks: 10");
								break;
*/
#endif
							}
						}
					return (OK);
					}
				case CM_EDITUNDO:
					{
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_UNDO, (WPARAM)0, (LPARAM)0);
					return (OK);
					}
				case CM_EDITCUT:
					{
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_CUT, (WPARAM)0, (LPARAM)0);
					return (OK);
					}
				case CM_EDITCOPY:
					{
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_COPY, (WPARAM)0, (LPARAM)0);
					return (OK);
					}
				case CM_EDITPASTE:
					{
					SendMessage( (HWND)GetDlgItem( hDlg, wCurrentCtrl__), WM_PASTE, (WPARAM)0, (LPARAM)0);
					return (OK);
					}
				case CM_FETCH:
					{
					return (OK);
					}
				case CM_HELP_ABOUT:
					{
					vAboutDlg( hInstance__);
					return (OK);
					}
				case IDHELP:
					{
					WinExec( "notepad.exe phwinfix.txt", SW_SHOWNORMAL);
					return (OK);
					}
				case IDOK:
					{
					FILE *fhUt;
					int i, j;
					char szBuf[dBUFSIZ+1], *apsz[vLAST+1];
					HANDLE hCursor;
					// Load hour-glass cursor.
					hCursor = LoadCursor( NULL, IDC_WAIT);
					SetCursor( hCursor);
					(void)szGetEditCtrlsTexts( hDlg, vEDIT+vFIRST, vEDIT+vLAST, szBuf, sizeof( szBuf) );
					// Convert dFS-separated Value-list into ","-separated Value-list
					for (j = i = 0, apsz[0] = &szBuf[0]; szBuf[i]; i++)
						{
						if (szBuf[i] == dFS)
							{
							szBuf[i] = '\0';
							j++;
							if (j < (sizeof( apsz) / sizeof( apsz[0]) ) )
								apsz[j] = &szBuf[i+1];
							}
						}
					strcpy( szRpoGglFilnavnIn__, 		 apsz[i=0]);
					strcpy( szRpoGglFilnavnUt__, 		 apsz[++i]);
					strcpy( szRpoApFilnavnIn__,  		 apsz[++i]);
					strcpy( szRpoApFilnavnUt__,  		 apsz[++i]);
					strcpy( szRpoGglFilnavnHeader__,  apsz[++i]);
					strcpy( szFormSize__,  				 apsz[++i]);
					strcpy( szFont__,						 apsz[++i]);
					strcpy( szFontSize__,				 apsz[++i]);
					strcpy( szBackgroundColor__,		 apsz[++i]);
#ifdef NPO
//					strcpy( szEditFont__,		 		 apsz[++i]);
//					strcpy( szButtonFontSize__,		 apsz[++i]);
#endif
					if ( (fhUt = fopen( "\phwinfix.ini", "w") ) != NULL)  // Ini-file.
						{
						fprintf( fhUt, "%s\n", szRpoGglFilnavnIn__);
						fprintf( fhUt, "%s\n", szRpoGglFilnavnUt__);
						fprintf( fhUt, "%s\n", szRpoApFilnavnIn__);
						fprintf( fhUt, "%s\n", szRpoApFilnavnUt__);
						fprintf( fhUt, "%s\n", szRpoGglFilnavnHeader__);
						fprintf( fhUt, "%s\n", szFormSize__);
						fprintf( fhUt, "%s\n", szFont__);
						fprintf( fhUt, "%s\n", szFontSize__);
						fprintf( fhUt, "%s\n", szBackgroundColor__);
#ifdef NPO
						fprintf( fhUt, "%s\n", szEditFont__);
						fprintf( fhUt, "%s\n", szButtonFontSize__);
#endif
						fclose( fhUt);
						}
					else
						TvInfo::vNotice( "Kan ikke skrive ut .INI filen");
					(void)bRun();
					 SetCursor( hCursor);
					// Reload arrow cursor.
					TvInfo::vNotice(
							"Tilpassing av AP-Fil og GGL-fil er ferdig.\nRepository må lastes på nytt.\nBruk cognos\\convert\\load12.");
					EndDialog( hDlg, 0);
					return (OK);
					}
				case IDCANCEL:
					{
					EndDialog( hDlg, 0);
					return (OK);
					}
				}
			}
		}
	return (FALSE);
	}


TvPhWinFix::TvPhWinFix( HANDLE hInstance)
	{
	hInstance__ = hInstance;
	iBufferSize__ = BUFSIZ + 1;
	try
		{ // TEST FOR EXCEPTIONS.
		szBuffer__ = new char[iBufferSize__];
		szNull__ = new char[iBufferSize__];
		szRpoGglFilnavnIn__ = new char[iBufferSize__];
		szRpoGglFilnavnUt__ = new char[iBufferSize__];
		szRpoApFilnavnIn__ = new char[iBufferSize__];
		szRpoApFilnavnUt__ = new char[iBufferSize__];
		szRpoGglFilnavnHeader__ = new char[iBufferSize__];
		szFormSize__ = new char[iBufferSize__];
		szFont__ = new char[iBufferSize__];
#ifdef NPO
		szEditFont__ = new char[iBufferSize__];
#endif
		szFontSize__ = new char[iBufferSize__];
#ifdef NPO
		szButtonFontSize__ = new char[iBufferSize__];
#endif
		szBackgroundColor__ = new char[iBufferSize__];
		}
	catch (xalloc)
		{  // ENTER THIS BLOCK ONLY IF xalloc IS THROWN.
		vError( "Could not allocate memory", "TvName()");
		exit( -1);
		}
	}

TvPhWinFix::~TvPhWinFix()
	{
	delete szBuffer__;
	delete szNull__;
	delete szRpoGglFilnavnIn__;
	delete szRpoGglFilnavnUt__;
	delete szRpoApFilnavnIn__;
	delete szRpoApFilnavnUt__;
	delete szRpoGglFilnavnHeader__;
	delete szFormSize__;
	delete szFont__;
	delete szFontSize__;
	delete szBackgroundColor__;
#ifdef NPO
	delete szEditFont__;
	delete szButtonFontSize__;
#endif
	}


void TvPhWinFix::vDialog( HWND hDlg)
	{
	FILE *fhIn;
#ifndef NPO
	strcpy( szRpoGglFilnavnIn__, "c:\\cognos\\convert\\default.ggl");
	strcpy( szRpoGglFilnavnUt__, "c:\\cognos\\convert\\phwinfix.ggl");
	strcpy( szRpoApFilnavnIn__, "c:\\cognos\\convert\\default.ap");
	strcpy( szRpoApFilnavnUt__, "c:\\cognos\\convert\\phwinfix.ap");
#else
	strcpy( szRpoGglFilnavnIn__, 		"c:\\cognos\\convert\\default.ggl");
	strcpy( szRpoGglFilnavnUt__, 		"c:\\cognos\\convert\\phwinfix.ggl");
	strcpy( szRpoApFilnavnIn__, 		"c:\\cognos\\convert\\default.ap");
	strcpy( szRpoApFilnavnUt__, 		"c:\\cognos\\convert\\phwinfix.ap");
#endif
	strcpy( szRpoGglFilnavnHeader__, "c:\\cognos\\convert\\header.ggl");
	strcpy( szFormSize__, "500 BY 350");
	strcpy( szFont__, "Arial");
	strcpy( szFontSize__, "10");
	strcpy( szBackgroundColor__, "None");
#ifdef NPO
	strcpy( szEditFont__, "Courier New");
	strcpy( szButtonFontSize__, "6");
#endif
	if ( (fhIn = fopen( "\phwinfix.ini", "r") ) != NULL)  // Ini-file.
		{
		if (vStrTrunc( fgets( szRpoGglFilnavnIn__, iBufferSize__, fhIn) ) )
			if (vStrTrunc( fgets( szRpoGglFilnavnUt__, iBufferSize__, fhIn) ) )
				if (vStrTrunc( fgets( szRpoApFilnavnIn__, iBufferSize__, fhIn) ) )
					if (vStrTrunc( fgets( szRpoApFilnavnUt__, iBufferSize__, fhIn) ) )
						if (vStrTrunc( fgets( szRpoGglFilnavnHeader__, iBufferSize__, fhIn) ) )
							if (vStrTrunc( fgets( szFormSize__, iBufferSize__, fhIn) ) )
								if (vStrTrunc( fgets( szFont__, iBufferSize__, fhIn) ) )
									if (vStrTrunc( fgets( szFontSize__, iBufferSize__, fhIn) ) )
#ifndef NPO
										vStrTrunc( fgets( szBackgroundColor__, iBufferSize__, fhIn) );
#else
										if (vStrTrunc( fgets( szBackgroundColor__, iBufferSize__, fhIn) ) )
											if (vStrTrunc( fgets( szEditFont__, iBufferSize__, fhIn) ) )
												vStrTrunc( fgets( szButtonFontSize__, iBufferSize__, fhIn) );
#endif
		fclose( fhIn);
		}
	DLGPROC lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)DlgProc, hInstance__);
	sgptvPhWinFix = this;	// Store global reference to this object for Windows callback
	DialogBox( hInstance__, "vDlgPhWinFix", hDlg, (DLGPROC)lpfnDialog);
	FreeProcInstance( (FARPROC)lpfnDialog);
	}


BOOL TvPhWinFix::bRun( void)
	{
	iFontSize__ = atoi( szFontSize__);
	if ( !bFixGGL() )
		{ vError( "Modifisering av GGL gikk skitt", "bRun()"); return (FAIL); }
	if ( !bFixAP() )
		{ vError( "Modifisering av AP gikk skitt", "bRun()"); return (FAIL); }
	return (OK);
	}


BOOL TvPhWinFix::bFixGGL( void)
	{
#ifndef NPO
	FILE *fhIn, *fhUt, *fhTmp;
#else
	FILE *fhIn, *fhUt, *fhTmp, *fhCmd;
#endif
	BOOL bSubFormInd = FALSE;
	int iFormCount = 0;
#ifdef NPO
	int iCountLabelButtons = 0;
	char szCurrentFormName[80];
#endif
	if ( (fhIn = fopen( szRpoGglFilnavnIn__, "r") ) == NULL)  // Input file.
		{ vError( "Klarer ikke å åpne GGL In-fil!", "bFixGGL()"); return (FAIL); }
	if ( (fhUt = fopen( szRpoGglFilnavnUt__, "w") ) == NULL)  //Output file.
		{ vError( "Klarer ikke å åpne GGL Ut-fil!", "bFixGGL()"); return (FAIL); }
#ifndef NPO
	if ( (fhTmp = fopen( "phwinfix.tmp", "w") ) == NULL)  //Output temp-file.
#else
	if ( (fhTmp = fopen( "phwinfix.tmp", "w") ) == NULL)  //Output temp-file for commands
#endif
		{ vError( "Klarer ikke å åpne temp Ut-fil!", "bFixGGL()"); return (FAIL); }
#ifdef NPO
	if ( (fhCmd = fopen( "phwinfix.cmd", "w") ) == NULL)  //Output temp-file for commands.
		{ vError( "Klarer ikke å åpne temp (phwinfix.cmd) Ut-fil!", "bFixGGL()"); return (FAIL); }
#endif
	if ( !bInsertHeading( fhUt) )
		return (FAIL);
	while (fgets( szBuffer__, iBufferSize__, fhIn) != NULL)
		{
		// Fjerner trailing blanks og LF samt retter feilkonverterte norske tegn
#if defined (dGAMMAL_DRIT)
		(void)szFixFeileNorskeTegn( vStrTrunc( szBuffer__) );
#endif
#ifndef NPO
		// Justerer standard formularet
#else
		// FORM: Justerer standard formularet
#endif
		if (szStrSearch( szBuffer__, "FORM \"") == &szBuffer__[0]
				&& szStrSearch( szBuffer__, "\" SIZE ") != NULL)
			{
#ifdef NPO
			// Husk Current form
			char *psz1, *psz2;
			psz1 = strchr( szBuffer__, '"');
			if (psz1 == NULL)
				vError( "Filen har ikke korrekt formularnavn (1)", "bFixGGL()");
			psz2 = strchr( psz1, '"');
			if (psz2 == NULL)
				vError( "Filen har ikke korrekt formularnavn (2)", "bFixGGL()");
			psz1++;
			strncpy( szCurrentFormName, psz1, ((long)psz2 - (long)psz1) );
#endif
#if defined (dVI_TESTER_HVORDAN_LABLENE_SAA_UT_FOER)
			bSubFormInd = TRUE;  // Dette er lureri. For enkelt å koble ut label-formateringskode
#else
			bSubFormInd = FALSE;  // Dette er den orginale linjen
#endif
			if (iFormCount > 0)
				{
#if defined (dKNAPPERAD_SKAL_MED) && defined (dKNAPPERADEN_SIST)
				fprintf( fhUt, "SUBFORM \"VMS_Buttons\" WITHIN 2.4,327.6 TO 288.58,291.6\n");
				fprintf( fhUt, "\tROTATION 0,\n");
				fprintf( fhUt, "\tOPTION DEMAND,\n");
				fprintf( fhUt, "\tTYPE SCROLL,\n");
				fprintf( fhUt, "\tOCCURRENCE CLUSTER,\n");
				fprintf( fhUt, "\tOCCURS 1 REPEATS DOWN 1 FAULTS RIGHT,\n");
				fprintf( fhUt, "\tFORM \"VMS_Buttons\"\n");
#endif
				}
			iFormCount++;
#ifdef NPO
			iCountLabelButtons = 0;
#endif
			char *psz = szStrSearch( szBuffer__, "SIZE");
			*psz = '\0';
			// Standard form størrelse
			fprintf( fhUt, "\n%s SIZE %s\n", szBuffer__, szFormSize__);
			fprintf( fhUt, "TYPEFACE \"%s\"\n", szFont__);
			fprintf( fhUt, "TYPESTYLE \"Bold\"\n");
			fprintf( fhUt, "TYPESIZE \"%s\"\n", szFontSize__);
#if defined (dGAMMAL_DRIT)
			fprintf( fhUt, "SUBFORM \"Standard_Buttonbar\" WITHIN 14.25,347 TO 336.25,317\n");
			fprintf( fhUt, "SUBFORM \"VMS_Buttons\" WITHIN 7.6,321.2 TO 295.19,285.2\n");
#endif
#if defined (dKNAPPERAD_SKAL_MED) && defined (dKNAPPERADEN_FOERST)
			fprintf( fhUt, "SUBFORM \"VMS_Buttons\" WITHIN 2.4,327.6 TO 288.58,291.6\n");
			fprintf( fhUt, "\tROTATION 0,\n");
			fprintf( fhUt, "\tOPTION DEMAND,\n");
			fprintf( fhUt, "\tTYPE SCROLL,\n");
			fprintf( fhUt, "\tOCCURRENCE CLUSTER,\n");
			fprintf( fhUt, "\tOCCURS 1 REPEATS DOWN 1 FAULTS RIGHT,\n");
			fprintf( fhUt, "\tFORM \"VMS_Buttons\"\n");
#endif
#if defined (dGAMMAL_DRIT)
			fprintf( fhUt, "SUBFORM \"Heading\" WITHIN 15,312.5 TO 485,290.5\n");
			fprintf( fhUt, "SUBFORM \"VMS_Heading\" WITHIN 9,341 TO 477,316.4\n");
#endif
			fprintf( fhUt, "SUBFORM \"VMS_Heading\" WITHIN 3,346.8 TO 478.8,322.2\n");
			fprintf( fhUt, "\tROTATION 0,\n");
			fprintf( fhUt, "\tOPTION DEMAND,\n");
			fprintf( fhUt, "\tTYPE SCROLL,\n");
			fprintf( fhUt, "\tOCCURRENCE CLUSTER,\n");
			fprintf( fhUt, "\tOCCURS 1 REPEATS DOWN 1 FAULTS RIGHT,\n");
			fprintf( fhUt, "\tFORM \"VMS_Heading\"\n");
			}
#ifndef NPO
		// Fjerner default skjermheading
#else
		// HEADING: Fjerner default skjermheading
#endif
		else if (szStrSearch( szBuffer__, "EDIT \"DEFINE D_SYSTEM\"") != NULL
				|| szStrSearch( szBuffer__, "EDIT \"DEFINE D_SCREENID\"") != NULL
				|| szStrSearch( szBuffer__, "EDIT \"DEFINE D_SCREEN_TITLE\"") != NULL
				|| szStrSearch( szBuffer__, "EDIT \"GBL_CURR_USER OF GLOBALS\"") != NULL
				|| szStrSearch( szBuffer__, "EDIT \"TEMPORARY T_SYSDATE\"") != NULL
				|| szStrSearch( szBuffer__, "EDIT \"TEMPORARY T_MENU_COMMAND\"") != NULL
				|| szStrSearch( szBuffer__, "EDIT \"DEFINE D_TIME\"") != NULL)
			{
			// Hopp over de 15 påfølgende linjene
			for (int i = 0; i < 15; i++)
				if (fgets( szBuffer__, iBufferSize__, fhIn) == NULL)
					{ vError( "Ufulstendig EDIT-felt", "bFixGGL()"); return (FAIL); }
			}
#ifndef NPO
		// Fjerner alle linjer med forekomster av følgende:
#else
		// SLETT: Fjerner alle linjer med forekomster av følgende:
#endif
		else if (szStrSearch( szBuffer__, "TYPEFACE \"System\"") != NULL
				|| szStrSearch( szBuffer__, "TYESTYLE \"Normal\"") != NULL
				|| szStrSearch( szBuffer__, "RECTANGLE WITHIN ") != NULL
				|| szStrSearch( szBuffer__, "TYPECOLOR \"") != NULL
				|| szStrSearch( szBuffer__, "FRAMEFILLCOLOR \"") != NULL
				|| szStrSearch( szBuffer__, "\"BILDE:\"") != NULL
				|| szStrSearch( szBuffer__, "\"Bilde:\"") != NULL
				|| szStrSearch( szBuffer__, "LINE FROM ") != NULL)
			{
			; // Fjerner linjer. Ikke skriv ut noe
			}
#ifndef NPO
		// Endrer utseendet på edit-felter
#else
		// EDIT: Endrer utseendet på edit-felter
#endif
		else if (szStrSearch( szBuffer__, "FRAMELINECOLOR \"Black\"") != NULL)
			{
			fprintf( fhUt, "FRAMELINECOLOR \"Black\"\nFRAMEFILLCOLOR \"None\"\n");
			}
#ifndef NPO
		// Endrer utseendet på buttons og tekst-konstanter
#else
		// BUTTON+TEXT: Endrer utseendet på buttons og tekst-konstanter
#endif
		else if (szStrSearch( szBuffer__, "FRAMELINECOLOR \"None\"") != NULL)
			{
			fprintf( fhUt, "FRAMELINECOLOR \"NONE\"\n");
			}
#ifndef NPO
		// Øker lengden på editfelter
#else
		// TEXT: Setter Font
		else if (szStrSearch( szBuffer__, "TEXT AT ") != NULL)
			{
			// Skift til normal font 
			fprintf( fhUt, "TYPESTYLE \"Normal\"\nTYPESIZE \"%s\"\nTYPEFACE \"%s\"\n", szFontSize__, szFont__);
			// Skriv ut TEXT AT linjen
			fprintf( fhUt, "%s", szBuffer__);
			}
		// EDIT: Øker lengden på editfelter
#endif
		else if (szStrSearch( szBuffer__, "EDIT ") != NULL
				&& szStrSearch( szBuffer__, " WITHIN ") != NULL)
			{
			char szEdit[100], *psz;
			char sz[12], szTo[10], szLokasjon1[20], szLokasjon2[20];
			double x1, x2, y1, y2;
			// Kopier button lokasjon
			strcpy( szEdit, szBuffer__);
			psz = szStrSearch( szEdit, " WITHIN ");
			*psz = '\0';
			psz = szStrSearch( szBuffer__, " WITHIN ");
			sscanf( psz, "%s%s%s%s", sz, szLokasjon1, szTo, szLokasjon2);
			// Skill flyttallene i szLokasjon  med ' ' ikke ','
			int i;
			for (i = 0; szLokasjon1[i]; i++)
				if (szLokasjon1[i] == ',')
					szLokasjon1[i] = ' ';
			for (i = 0; szLokasjon2[i]; i++)
				if (szLokasjon2[i] == ',')
					szLokasjon2[i] = ' ';
			// Converter til float
			sscanf( szLokasjon1, "%lf%lf", &x1, &y1);
			sscanf( szLokasjon2, "%lf%lf", &x2, &y2);
			// Beregn nye koordinater for edit control
			x2 +=  dJUSTERING_FOR_KORTE_FELT;
#ifdef NPO
			// Skift font tilpasset edit-kontroll
			fprintf( fhUt, "TYPESTYLE \"Normal\"\nTYPESIZE \"%s\"\nTYPEFACE \"%s\"\n", szFontSize__, szEditFont__);
			// Skriv ut Edit-linjen
#endif
			fprintf( fhUt, "%s WITHIN %lg,%lg TO %lg,%lg\n", szEdit, x1, y1, x2, y2);
			}
#ifndef NPO
		// Endrer utseende på felt labler
#else
		// BUTTON: Endrer utseende på felt labler
#endif
		else if (szStrSearch( szBuffer__, "BUTTON ") != NULL
				&& szStrSearch( szBuffer__, " WITHIN ") != NULL
				&& !bSubFormInd)
			{
			BOOL bPrinted;
			char szButton[100], *psz;
			char sz[12], szTo[10], szLokasjon1[20], szLokasjon2[20];
			double x1, x2, y1, y2;
			long iCurPos;  // Offset i .ggl filen
			// Husker offsettpekers posisjon i filen
			iCurPos = ftell( fhIn);
			bPrinted = FALSE;
			// Kopier button lokasjon
			strcpy( szButton, szBuffer__);
			psz = szStrSearch( szButton, " WITHIN ");
			*psz = '\0';
			psz = szStrSearch( szBuffer__, " WITHIN ");
			sscanf( psz, "%s%s%s%s", sz, szLokasjon1, szTo, szLokasjon2);
			// Skill flyttallene i szLokasjon  med ' ' ikke ','
			int i;
			for (i = 0; szLokasjon1[i]; i++)
				if (szLokasjon1[i] == ',')
					szLokasjon1[i] = ' ';
			for (i = 0; szLokasjon2[i]; i++)
				if (szLokasjon2[i] == ',')
					szLokasjon2[i] = ' ';
			// Converter til float
			sscanf( szLokasjon1, "%lf%lf", &x1, &y1);
			sscanf( szLokasjon2, "%lf%lf", &x2, &y2);
			// Les neste linje
			if (fgets( szNull__, iBufferSize__, fhIn) != NULL
					&& szStrSearch( szNull__, "STYLE LABELED,") != NULL)
				{
				// Les neste linje
				if (fgets( szNull__, iBufferSize__, fhIn) != NULL
						&& szStrSearch( szNull__, "LABEL ") != NULL)
					{
					char szLabel[89];
					// Kopier Label
					strcpy( szLabel, szStrSearch( szNull__, "\"") );
					(void)vStrTrunc( szLabel);
					szLabel[strlen( szLabel) - 1] = '\0';  // Fjerner ','
#ifndef NPO
					// Skriver ut text- og buttonlabel-linjen
#else
					// Skift til normal font
					fprintf( fhUt, "TYPESTYLE \"Normal\"\nTYPESIZE \"%s\"\nTYPEFACE \"%s\"\n", szFontSize__, szFont__);
					// Skriver ut text-label-linjen
#endif
					fprintf( fhUt, "TEXT AT %lg,%lg %s\n", x1, y1, szLabel);
					// Beregn koordinater for ny button-snuppeditt
#ifndef NPO
					x1 -= 10;     // orginalt x1 -= 6; 		For å flytte button mot venstre: trekk fra mer
					x2 = x1 + 8; // orginalt x2 = x1 + 4; 	For å strekke button mot høyre: 	legg til mer
					y1 -= 1;     // orginalt y1 -= 2; 		For å flytte button nedover:  	trekk fra mer
					y2 = y1 - 7; // orginalt y2 = y1 - 5; 	For å strekke button nedover: 	trekk fra mer
					// Skriver ut text- og buttonlabel-linjen
#else
					x1 -= 12;     // For å flytte button mot venstre: trekk fra mer; 	orginalt x1 -= 6;
					x2 = x1 + 10; // For å strekke button mot høyre: 	legg til mer; 	orginalt x2 = x1 + 4;
					y1 -= 1;      // For å flytte button nedover:  	trekk fra mer; 	orginalt y1 -= 2;
					y2 = y1 - 9;  // For å strekke button nedover: 	trekk fra mer; 	orginalt y2 = y1 - 5;
					// Skift font-størrelse tilpasset trykknapp forran ledetext
					fprintf( fhUt, "TYPESTYLE \"Normal\"\nTYPESIZE \"%s\"\nTYPEFACE \"%s\"\n", szButtonFontSize__, szFont__);
					// Skriver ut buttonlabel-linjen
#endif
					fprintf( fhUt, "%s WITHIN %lg,%lg TO %lg,%lg\n", szButton, x1, y1, x2, y2);
#ifndef NPO
					fprintf( fhUt, "\tSTYLE LABELED,\n");
#else
					++iCountLabelButtons;
					fprintf( fhUt, "\tSTYLE LABELED,\n\tLABEL \"%d\",\n\tIMAGE_FORMAT \"bmp\",\n", iCountLabelButtons);
#endif
					// Hopp over siste linjen - FLAGS 0
					if (fgets( szNull__, iBufferSize__, fhIn) != NULL)
						fprintf( fhUt, szNull__);
					bPrinted = TRUE;
#ifdef NPO
					// Må føre liste med alle kommandoene som skal inn i ap-filen
					fprintf( fhCmd, "%s %d\n", szCurrentFormName, iCountLabelButtons);
#endif
					}
				}
			if ( !bPrinted)
				{
				fprintf( fhUt, szBuffer__);
				// Resetter offsettpeker til korrekt posisjon i filen, slik at prosessering kan fortsette
				fseek( fhIn, iCurPos, SEEK_SET);
				}
			}
#ifndef NPO
		// Subform: Setter font
#else
		// SUBFORM: Setter font
#endif
		else if (szStrSearch( szBuffer__, "PICTURE SIZE ") != NULL )
			{
			fprintf( fhUt, szBuffer__);
			fprintf( fhUt, "TYPEFACE \"%s\"\n", szFont__);
			fprintf( fhUt, "TYPESTYLE \"Bold\"\n");
			fprintf( fhUt, "TYPESIZE \"%s\"\n", szFontSize__);
			}
#ifndef NPO
		// Subform: Etablerer grunnlag for acttion-kommandoer i .ap fil
#else
		// SUBFORM: Etablerer grunnlag for acttion-kommandoer i .ap fil
#endif
		else if (szStrSearch( szBuffer__, "SUBFORM ") != NULL
				&& szStrSearch( szBuffer__, " WITHIN ") != NULL)
			{
#ifdef NPO
			bSubFormInd = TRUE;
#endif
			long iCurPos;  // Offset i .ggl filen
			int i, iOccurrence, iFirstId;
			char *psz;
			// Skriver ut kommando på som den er
			fprintf( fhUt, szBuffer__);  // Skriver ut
			// Husker offsettpekers posisjon i input-filen
			iCurPos = ftell( fhIn);
			// Sjekker at dette er aktuell subform kommando
			psz = szStrSearch( szBuffer__, "SUBFORM ");
			if (szStrSearch( psz, "-00") != NULL)
				{
				// Finn formnavn
				char szFormName[80], *pszNavn;
				pszNavn = szStrSearch( psz, "\"");
				pszNavn++;
				psz = szStrSearch( pszNavn, "-");
				*psz = '\0';
				strcpy( szFormName, pszNavn);
				// Finn Occurrence. Leter 10 linjer fram
				for (iOccurrence = i = 0; (i<10) && (iOccurrence == 0); i++)
					{
					if (fgets( szNull__, iBufferSize__, fhIn) != NULL)
						{
						if ( (psz = szStrSearch( szNull__, "OCCURS ") ) != NULL)
							{
							psz += 7;
							sscanf( psz, "%d", &iOccurrence);
							}
						}
					}
#ifndef NPO
				// Kun clustre med mer enn en forekomst akl behhandles som subforms
				if (iOccurrence > 1)
					bSubFormInd = TRUE;
				// Finn Label. Leter intil 100 linjer fram
				for (iFirstId = i = 0; (i<100) && (iFirstId == 0); i++)
#else
				// Finn Label. Leter intil 1000 linjer fram
				for (iFirstId = i = 0; (i<1000) && (iFirstId == 0); i++)
#endif
					{
					if (fgets( szNull__, iBufferSize__, fhIn) != NULL)
						{
						if ( (psz = szStrSearch( szNull__, "BUTTON \"") ) != NULL)
							{
							psz += 7; // Hopp over ordet BUTTON
							// Erstatt hermetegn (") med blanke
							for (int j = 0; j < 10; j++)
								{
								if (psz[j] == '\"')
									psz[j] = ' ';
								}
							sscanf( psz, "%d", &iFirstId);  // Konverter Label til integer
							}
						}
					}
				// Skriv til  tempfil
				if (iFirstId != 0 && iOccurrence != 0)
#ifndef NPO
					fprintf( fhTmp, "%s %d %d", szFormName, iFirstId, iOccurrence);
#else
					fprintf( fhTmp, "%s %d %d\n", szFormName, iFirstId, iOccurrence);
#endif
				}
			// Resetter offsettpeker til korrekt posisjon i filen, slik at prosessering kan fortsette
			fseek( fhIn, iCurPos, SEEK_SET);
			}
#ifndef NPO
		// Combobox, fra drop_down_list til edit_drop_down_list
#else
		// CHOICE: Combobox, fra drop_down_list til edit_drop_down_list
#endif
		else if (szStrSearch( szBuffer__, "STYLE DROP_DOWN_LIST") != NULL)
			{
			fprintf( fhUt, "\tSTYLE EDIT_DROP_DOWN_LIST,\n");
			}
#ifndef NPO
		// Comboboxer skal ikke være sortert
#else
		// CHOICE: Comboboxer skal ikke være sortert
#endif
		else if (szStrSearch( szBuffer__, "SORT YES,") != NULL)
			{
			fprintf( fhUt, "\tSORT NO,\n");
			}
#ifndef NPO
		// Combobox, Justerer koordinater for Choise-felter
#else
		// CHOICE: Combobox, Justerer koordinater for Choise-felter
#endif
		else if (szStrSearch( szBuffer__, "CHOICE ") != NULL
				&& szStrSearch( szBuffer__, " WITHIN ") != NULL)
			{
			BOOL bValueStatement;
			int i, iAntValueStatements = 0;
			long iCurPos;  // Offset i .ggl filen
			double x1, y1, x2, y2;
			char sz[12], szChoice[BUFSIZ], *psz, szTo[10], szLokasjon1[20], szLokasjon2[20];
			// Finner koordinater
			psz = szStrSearch( szBuffer__, " WITHIN ");
			sscanf( psz, "%s%s%s%s", sz, szLokasjon1, szTo, szLokasjon2);
			// Skill flyttallene i szLokasjon  med ' ' ikke ','
			for (i = 0; szLokasjon1[i]; i++)
				if (szLokasjon1[i] == ',')
					szLokasjon1[i] = ' ';
			for (i = 0; szLokasjon2[i]; i++)
				if (szLokasjon2[i] == ',')
					szLokasjon2[i] = ' ';
			// Converter til float
			sscanf( szLokasjon1, "%lf%lf", &x1, &y1);
			sscanf( szLokasjon2, "%lf%lf", &x2, &y2);
			// Isolerer Choice statement uten koordinater
			strcpy( szChoice, szBuffer__);
			psz = szStrSearch( szChoice, " WITHIN ");
			*psz = '\0';
			// Husker offsettpekers posisjon i filen
			iCurPos = ftell( fhIn);
			// Hopper over de 9 påfølgende linjer
			for (i = 0; i < 9; i++)
				fgets( szNull__, iBufferSize__, fhIn);
			// Teller Value statements
			for (bValueStatement = TRUE, iAntValueStatements = 1; bValueStatement == TRUE; iAntValueStatements++)
				{
#ifndef NPO
				if (fgets( szNull__, iBufferSize__, fhIn) != NULL)
					{
					if (szStrSearch( szNull__, ",VALUE ") == NULL)
#else
				if (fgets( szNull__, iBufferSize__, fhIn) == NULL)
#endif
						bValueStatement = FALSE;
#ifndef NPO
					}
#else
				else if (szStrSearch( szNull__, ",VALUE ") == NULL)
						bValueStatement = FALSE;
#endif
				}
			if (iAntValueStatements < 4)
				iAntValueStatements = 4;
			// Beregner justerte koordinater for combobox
			y2 = y1 - (iFontSize__ * 2.35) - (iFontSize__ + 1) * iAntValueStatements;
			x2 += dJUSTERING_FOR_KORTE_FELT;  // Justerer langeden på feltet
#ifdef NPO
			// Skift font tilpasset edit-kontoll
			fprintf( fhUt, "TYPESTYLE \"Normal\"\nTYPESIZE \"%s\"\nTYPEFACE \"%s\"\n", szFontSize__, szEditFont__);
#endif
			// Skriver ut Choice statementet med justerte koordinater
			fprintf( fhUt, "%s WITHIN %lg,%lg TO %lg,%lg\n", szChoice, x1, y1, x2, y2);
			// Resetter offsettpeker til korrekt posisjon i filen, slik at prosessering kan fortsette
			fseek( fhIn, iCurPos, SEEK_SET);
			}
#ifdef NPO
		// INGENTING
#endif
		else
			{
			fprintf( fhUt, "%s", szBuffer__);
			}
		}
#if defined (dKNAPPERAD_SKAL_MED) && defined (dKNAPPERADEN_SIST)
	fprintf( fhUt, "SUBFORM \"VMS_Buttons\" WITHIN 2.4,327.6 TO 288.58,291.6\n");
	fprintf( fhUt, "\tROTATION 0,\n");
	fprintf( fhUt, "\tOPTION DEMAND,\n");
	fprintf( fhUt, "\tTYPE SCROLL,\n");
	fprintf( fhUt, "\tOCCURRENCE CLUSTER,\n");
	fprintf( fhUt, "\tOCCURS 1 REPEATS DOWN 1 FAULTS RIGHT,\n");
	fprintf( fhUt, "\tFORM \"VMS_Buttons\"\n");
#endif
#ifndef NPO
	fclose( fhIn);  fclose( fhUt);  fclose( fhTmp);
#else
	fclose( fhIn);  fclose( fhUt);  fclose( fhTmp);  fclose( fhCmd);
#endif
	return (OK);
	}

BOOL TvPhWinFix::bFixAP( void)
	{
	int i, j;
#ifndef NPO
	FILE *fhIn, *fhUt, *fhTmp;
#else
	FILE *fhIn, *fhUt, *fhTmp, *fhCmd;
#endif
	char szCurrentForm[BUFSIZ], *psz;
	if ( (fhIn = fopen( szRpoApFilnavnIn__, "r") ) == NULL)  // Input file.
		{ vError( "Klarer ikke å åpne AP In-fil!", "bFixAP()"); return (FAIL); }
	if ( (fhUt = fopen( szRpoApFilnavnUt__, "w") ) == NULL)  //Output file.
		{ vError( "Klarer ikke å åpne AP Ut-fil!", "bFixAP()"); return (FAIL); }
	if ( (fhTmp = fopen( "phwinfix.tmp", "r") ) == NULL)  //Output temp-file.
#ifndef NPO
		{ vError( "Klarer ikke å åpne temp In-fil!", "bFixGGL()"); return (FAIL); }
#else
		{ vError( "Klarer ikke å åpne temp In-fil!", "bFixAp()"); return (FAIL); }
	if ( (fhCmd = fopen( "phwinfix.cmd", "r") ) == NULL)  //Output temp-file for commands
		{ vError( "Klarer ikke å åpne temp (phwinfix.cmd) In-fil!", "bFixAp()"); return (FAIL); }
#endif
	while (fgets( szBuffer__, iBufferSize__, fhIn) )
		{
		// Hvilket skjermbilde/form arbeider vi med nå?
		if ( (psz = szStrSearch( szBuffer__, "SCREEN \"") ) != NULL)
			{
			fprintf( fhUt, "%s", szBuffer__);  // Skriv ut
			psz += 8;	  // Hopp over SCREEN "
			// Erstatt hermetegn med blank
			for (i = 0; psz[i]; i++)
				if (psz[i] == '\"')
					psz[i] = ' ';
			sscanf( psz, "%s", szCurrentForm);
			}
		// Fjern menuitem: endre hvis det finnes (vi legger det til hvis det ikke finnes)
#if defined (dLEGG_TIL_ENDRE_MENYVALG)
		else if (szStrSearch( szBuffer__, "MENUITEM \"Endre") != NULL
				|| (szStrSearch( szBuffer__, "ACTION TYPE ID MARK") != NULL
					&& szStrSearch( szBuffer__, "\"Endre") != NULL)	)
			{
			;  // Har tenkt å legge til kode for dette, så derfor fjernes linjene hvis de allerede finnes
			}
		// Legg til menuitem: endre
		else if (szStrSearch( szBuffer__, "POPUP \"Utfør\"") != NULL
				&& szStrSearch( szBuffer__, "QUICKINDEX") != NULL)
			{
			fprintf( fhUt, "%s", szBuffer__);
#ifndef NPO
// ** EV 10.5.96: Idag skal vi ikke legge til nytt endre-menyvalg
#endif
#if defined (dMED_ACCELERATORER)
#ifndef NPO
//	**		fprintf( fhUt, "MENUITEM \"Endre           F6 \"  QUICKINDEX 1 ACTION \"Endre\"\n");
#else
			fprintf( fhUt, "MENUITEM \"Endre           F6 \"  QUICKINDEX 1 ACTION \"Endre\"\n");
#endif
#else
#ifndef NPO
//	**		fprintf( fhUt, "MENUITEM \"Endre            \"  QUICKINDEX 1 ACTION \"Endre\"\n");
#else
			fprintf( fhUt, "MENUITEM \"Endre            \"  QUICKINDEX 1 ACTION \"Endre\"\n");
#endif
#endif	// dMED_ACCELERATORER
			}
#endif	// dLEGG_TIL_ENDRE_MENYVALG
		// Legg til menuitems
		else if (szStrSearch( szBuffer__, "ENDPOPUP \"Vis\"") != NULL)
			{
#if defined (dMED_ACCELERATORER)
			fprintf( fhUt, "MENUITEM \"Første side     Shift+PageDown\"  QUICKINDEX 1 ACTION \"First Record\"\n");
			fprintf( fhUt, "MENUITEM \"Neste linje     Shift+ArrowUp\"  QUICKINDEX 1 ACTION \"Next Record\"\n");
			fprintf( fhUt, "MENUITEM \"Forrige linje   Shift+ArrowDown\"  QUICKINDEX 1 ACTION \"Previous Record\"\n");
//			fprintf( fhUt, "MENUITEM \"Edit\"  QUICKINDEX 1 ACTION \"Edit\"\n");
#else
			fprintf( fhUt, "MENUITEM \"Første side     \"  QUICKINDEX 1 ACTION \"First Record\"\n");
			fprintf( fhUt, "MENUITEM \"Neste linje     \"  QUICKINDEX 1 ACTION \"Next Record\"\n");
			fprintf( fhUt, "MENUITEM \"Forrige linje   \"  QUICKINDEX 1 ACTION \"Previous Record\"\n");
//			fprintf( fhUt, "MENUITEM \"Edit\"  QUICKINDEX 1 ACTION \"Edit\"\n");
#endif
			fprintf( fhUt, "%s", szBuffer__);
			}
		// Legg på action-kommandoer
		else if (szStrSearch( szBuffer__, "ACTION TYPE ID MARK NO PROMPT NO STARTID 1 ENDID 0 NAME \"1\"") != NULL)
			{
#if defined (dLEGG_TIL_ENDRE_MENYVALG)
#ifndef NPO
// ** EV 10.5.96: Idag skal vi ikke legge til nytt endre-menyvalg
#endif
			// For menuitem: Endre
#ifndef NPO
//	**		fprintf( fhUt, "ACTION TYPE ID MARK NO PROMPT NO STARTID 1 ENDID 50 NAME \"Endre\"\n");
//	**		fprintf( fhUt, "%s", szBuffer__);
#else
			fprintf( fhUt, "ACTION TYPE ID MARK NO PROMPT NO STARTID 1 ENDID 50 NAME \"Endre\"\n");
			fprintf( fhUt, "%s", szBuffer__);
#endif
#endif	// dLEGG_TIL_ENDRE_MENYVALG
			// Legg på action-kommandoer for cluster id-ene
			char szFormName[BUFSIZ];
			int iFirstId, iOccurrence;
			fseek( fhTmp, 0, SEEK_SET);
			for (BOOL bGoOn = TRUE; bGoOn; )
				{
				if (fscanf( fhTmp, "%s%d%d", szFormName, &iFirstId, &iOccurrence) < 3)
					bGoOn = FALSE;
				else if (strcmp( szFormName, szCurrentForm) == 0)
					{
					bGoOn = FALSE;
					for (int i = 1; i < iOccurrence; i++)
						fprintf( fhUt, "ACTION TYPE ID MARK NO PROMPT NO STARTID %d ENDID 0 NAME \"%d\"\n", (iFirstId + i), (iFirstId +i));
					}
				}
#ifdef NPO
			// Legg på action-commandoer for nye id-buttons
			int iId;
			fseek( fhCmd, 0, SEEK_SET);
			for (;;)
				{
				if (fscanf( fhCmd, "%s%d", szFormName, &iId) < 2)
					break;
				else if (strcmp( szFormName, szCurrentForm) == 0)
						fprintf( fhUt, "ACTION TYPE ID MARK NO PROMPT NO STARTID %d ENDID 0 NAME \"%d\"\n", iId, iId);
				}
#endif
			}
		// Menyvalg endre skal kalle designer procedyren V021 ikke V020
		else if (szStrSearch( szBuffer__, "ACTION TYPE DESIGNER V020") != NULL)
			{
			while ( (psz = szStrSearch( szBuffer__, "V020") ) != NULL)
				{
				char szCut[10];
				(void)szStrCut( psz, 4, szCut, sizeof( szCut) );
				(void)szStrPaste( psz, "V021", iBufferSize__);
				}
			fprintf( fhUt, "%s\n",szBuffer__);
			}
		// Standard form størrelse
		else if (szStrSearch( szBuffer__, "DIALOG DEFAULT FORM ") != NULL)
			{
			char sz1[100], sz2[100], sz3[100], szName[100];
			sscanf( szBuffer__, "%s %s %s %s", sz1, sz2, sz3, szName);
			sscanf( szFormSize__, "%s %s %s", sz1, sz2, sz3);
			fprintf( fhUt, "DIALOG DEFAULT FORM %s AT 0, 0, %s00, %s00 FILLCOLOR \"%s\" PATTERN \"Solid\"\n", szName, sz1, sz3, szBackgroundColor__);
			}
		// Endrer separator fra alfanum separator til std. windows
		else if (szStrSearch( szBuffer__, "MENUITEM \"------------------") != NULL)
			fprintf( fhUt, "MENUITEM SEPARATOR\n");
		// Fjerner kommando for alfanum separator
		else if (szStrSearch( szBuffer__, "ACTION TYPE DESIGNER VSEP") != NULL
				&& szStrSearch( szBuffer__, "----------") != NULL)
			{
			;  // Fjerner linjer. Ikke skriv ut noe
			}
#if !defined (dMED_ACCELERATORER)
		// Fjerner display accelerator tast kombinasjoner i menyene
		else if (szStrSearch( szBuffer__, "MENUITEM \"") != NULL
				|| szStrSearch( szBuffer__, "ACTION TYPE DESIGNER") != NULL
				|| szStrSearch( szBuffer__, "ACTION TYPE COMMAND") != NULL)
			{
			char *aszAccelerators[] =
				{
				"Shift+F2","Shift+F7","Shift+F8","Shift+F9","Shift+F12","Shift+F1","Shift+F11",
				"Shift+PageDown","Shift+PageUp","Shift+Tab","Shift+Insert",
				"Ctrl+F2","Ctrl+F9","Ctrl+F12","Ctrl+Delete","Ctrl+Enter","Ctrl+S","Ctrl+U","Ctrl+N","Ctrl+B",
				" F1 "," F2 "," F4 "," F5 "," F6 "," F7 "," F8 "," F9 "," F10 "," F11 "," F12 ","PageDown","PageUp","Delete",
				NULL
				};
			for (i = 0; aszAccelerators[i]; i++)
				{
				while ( (psz = szStrSearch( szBuffer__, aszAccelerators[i]) ) != NULL)
					szStrCut( psz, strlen( aszAccelerators[i]),  szNull__, iBufferSize__);
				}
			fprintf( fhUt, "%s", szBuffer__);
			}
#endif
		// Skriv ut alle andre linjer
		else
			fprintf( fhUt, "%s", szBuffer__);
		}
#ifndef NPO
	fclose( fhIn);  fclose( fhUt);  fclose( fhTmp);
#else
	fclose( fhIn);  fclose( fhUt);  fclose( fhTmp);  fclose( fhCmd);
#endif
	return (OK);
	}


BOOL TvPhWinFix::bInsertHeading( FILE *fhUt)
	{
	// fprintf( fhUt, "\nHEADING_BEGIN\n");
	FILE *fhIn;
	if ( (fhIn = fopen( szRpoGglFilnavnHeader__, "r") ) == NULL)  // Header file.
		{ vError( "Klarer ikke å åpne GGL-header-fil!", "bFixGGL()"); return (FAIL); }
	while (fgets( szBuffer__, iBufferSize__, fhIn) )
		fprintf( fhUt, szFixFeileNorskeTegn( szBuffer__) );
	// fprintf( fhUt, "\nHEADING_END\n");
	return (OK);
	}


// Main program
#pragma argsused
int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	if (hPrevInstance == (HANDLE)0)
		{
		TvPhWinFix tvPhWinFix( hInstance);
		tvPhWinFix.vDialog( NULL);
		}
	return (0);
	}

// End of file

