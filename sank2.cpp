/*
sank2.cpp - 	Loads data from hand held terminal (PDT 3100) into VMS.
				After registration and transfer, data sits in the file C:\PDTPAL\DATA\PDTPAL.TXT.
				The foramt is :
				Leverandørnummer;Varenummer;;Antall;Passord (Kjøkken)
					176;1304109;;2.00;93361
					176;1304119;;1.00;93361
					176;1304164;;1.00;93361
				Data from C:\PDTPAL\DATA\PDTPAL.TXT is loaded into the temporary table req_sank2_hlp 
				in the VMS-database

				Program arguments: requisition_id DSN server DSN db_username db_password
				Run example: sank2 8860 U318.be.statoil.no U318 VMSODBC VMSODBC

	Author: Einar Valen, Date: 23.6.1998, Last tested ok: 7.8.1998
				Development tools used: Microsoft Development studio 97 and Visual C++ 5.0 on Windows NT 4.0
*/


// INCLUDES
#include <windows.h>
#include <direct.h>
#include <process.h>
#include "SQL.H"
#include "ODBCINST.H"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strstrea.h>
#include "resource.h"


// DEFINES
#define FAIL 0
#define OK (!FAIL)                  

// TYPES
class TvSank2
	{
	private:
		BOOL	bError_;			// Error indicator
		FILE 	*fhLog_;			// File handle til logfile
		HENV 	henv_;				// ODBC environment handle
		HDBC 	hdbc_;				// ODBC Connection handle
		HSTMT	hstmt_;				// ODBC statement handle
		HWND	hDlg_;				// Handle to user dialog
		char 	*szInFileName_, 	// Name of input file
				*szLogFileName_,	// Name of log file
				*szReq_,			// Requisition_id
				*szServer_, 		// Vms-Database server
				*szDSN_, 			// Vms-Database DSN
				*szPassword_,		// Vms-Database password
				*szUserName_,		// Vms-Database username
				*szPath_,			// Directory path to the program files
				*szVendorId_,		// Work field
				*szItemId_,			// Work field
				*szQty_;			// Work field
	public:
		TvSank2( void);							// Constructor
		~TvSank2();								// Destructor
		BOOL Init( char *szCmdLine, HWND hDlg);	// Constructor
		void Pdtpal( void);
		void Run( void);						// Main logic
	protected:
		void CloseVmsConnection( void);			// Commit transaction and close database connection
		BOOL ConnectToVMS( void);				// Opens database connection
		char *MakeInsertCmd( char *szBuffer, char *szSqlInsert, size_t iBufferSize);	// Splits up input record and constructs insert statement
		void Error( char *szMsg, char *szMore);	// Error reporting function
		void Log( char *szMsg);					// Log reporting function
		BOOL ReportVmsError( HENV henv, HDBC hdbc, HSTMT hstmt, char *szUserMsg);		// Reports SQL-errors via TvSank2::Error() after rolling back the transaction
		BOOL StrGetField( int iFieldNo, char *szToBuffer, int iToBufferLen, char *szFromBuffer, char cSep);
		void Display( char *psz);				// Adds line to listbox
	};

// FUNCTION PROTOTYPES

// GLOBAL VARIABLES
static LPSTR glpCmdLine = NULL;
// FUNCTIONS

// Constructor 	- Performs initial steps
TvSank2::TvSank2( void)
	{
	// Initialize variables
	fhLog_ = NULL;
	bError_ = FALSE;
//	try		// Does not work like this in Microsoft C++
		{ 
		// Allocate memory
		szInFileName_ 	= new char[BUFSIZ];
	    szLogFileName_ 	= new char[BUFSIZ];
	    szReq_ 			= new char[BUFSIZ];
	    szServer_ 		= new char[BUFSIZ];
	    szDSN_ 			= new char[BUFSIZ];
		szPath_			= new char[BUFSIZ];
		szPassword_		= new char[BUFSIZ];
		szUserName_		= new char[BUFSIZ];
		szVendorId_		= new char[BUFSIZ];
		szItemId_		= new char[BUFSIZ];
		szQty_			= new char[BUFSIZ];
		}
//	catch (xalloc)
		if (szInFileName_ == NULL || szLogFileName_ == NULL 
				|| szServer_ == NULL || szDSN_ == NULL || szPath_ == NULL || szPassword_ == NULL 
				|| szUserName_ == NULL || szVendorId_ == NULL || szItemId_ == NULL 
				|| szQty_ == NULL || szReq_ == NULL)
		{  
		// Enter this block only if xalloc is thrown.
		Error( "Class constructor could not allocate memory:", "Class TvSank2");
		exit( -1);
		}
	// Initialize variables
	fhLog_ = NULL;  henv_ = NULL;  hdbc_ = NULL;  hstmt_ = NULL;
	}

// Destructor  - frees allocated resources
TvSank2::~TvSank2()
	{
	// Free memory
	delete szInFileName_;		    delete szLogFileName_;			
	delete szServer_;			    delete szReq_;					
	delete szDSN_;				    delete szPassword_;				
	delete szUserName_;				delete szPath_;				
	}

// Constructor 	- Performs initial steps
BOOL TvSank2::Init( char *szCmdLine, HWND hDlg)
	{
	// Initialize variables
	hDlg_ = hDlg;		// Handle to user dialog
	if (StrGetField( 1, szReq_, BUFSIZ, szCmdLine, ' ') )
		if (StrGetField( 2, szServer_, BUFSIZ, szCmdLine, ' ') )
			if (StrGetField( 3, szDSN_, BUFSIZ, szCmdLine, ' ') )
				if (StrGetField( 4, szUserName_, BUFSIZ, szCmdLine, ' ') )
					if (StrGetField( 5, szPassword_, BUFSIZ, szCmdLine, ' ') )
						{
						char sz[BUFSIZ];
						strcpy( szPath_,        "C:\\"); 
						strcpy( szLogFileName_, szPath_);
						strcat( szLogFileName_, "sank2.log");
						strcpy( szInFileName_,  "C:\\PDTPAL\\DATA\\PDTPAL.TXT");
						ostrstream( sz, sizeof( sz) ) << "Vil overføre data fra " << szInFileName_ << " til rekvisisjon " << 
								szReq_ << " på server " << szServer_ << ends;
						Display( sz);
						return (OK);
						}
	Error( "Følgende programparametre kreves:", "requisition_id server DSN db_username db_password"); 
	return (FAIL); 
	}

// Error reporting function. Prints in dialog box and posts to logfile
void TvSank2::Error( char *szMsg, char *szMore)
	{
	bError_ = TRUE;
	char sz[BUFSIZ];
	ostrstream (sz, sizeof( sz) ) << szMsg << ':' << szMore << ends;				  // Format message
	Display( sz);		Display( "AVBRUTT !!");								  // Show message in user window
	MessageBox( NULL, (LPSTR)sz, (LPSTR)"Sank2 error", MB_OK | MB_ICONSTOP); // Output message in dialogbox
	Log( sz);                                                                // Log message to file
	}

// Log reporting function
void TvSank2::Log( char *szMsg)
	{
	static BOOL bTimestampDone = FALSE;
	struct tm *newtime; 	// Time variable
	time_t aclock;          // Time variable
	FILE 	*fhLog;			// File handle til logfile
	fhLog = NULL;
	if ( (fhLog = fopen( szLogFileName_, "a") ) != NULL)	// Open log file
		{                   
		// Post timestamp to logfile
		if (!bTimestampDone)
			{
			char sz[BUFSIZ];
			bTimestampDone = TRUE;
			time( &aclock );                 // Get time in seconds 
			newtime = localtime( &aclock );  // Convert time to struct tm form  
			char *psz = asctime( newtime );  // Convert to string
			fprintf( fhLog, "\n%s", psz); 	 // Post timestamp and messag to logfile
			ostrstream( sz, sizeof( sz) ) << "Logfile: " << szLogFileName_ << ends;
			Display( sz);
			}
		fprintf( fhLog, "\t%s\n", szMsg); 	 // Post timestamp and messag to logfile
		fclose(fhLog);           			 // Close log file
		}
	}
	
// Reports SQL-errors via TvSank2::Error() after rolling back the transaction
BOOL TvSank2::ReportVmsError( HENV henv, HDBC hdbc, HSTMT hstmt, char *szUserMsg)
	{                        
	char szSqlState[BUFSIZ], szErrorMsg[BUFSIZ];
	SDWORD fNativeError;                                                                                  
	SWORD cbErrorMsg;
	// Get SQL error message from database via ODBC
	SQLError( henv, hdbc, hstmt, (UCHAR FAR *)szSqlState, (SDWORD FAR *)&fNativeError, (UCHAR FAR *)szErrorMsg, (SWORD)sizeof( szErrorMsg), (SWORD FAR *)&cbErrorMsg);
	SQLTransact( henv_, hdbc_, SQL_ROLLBACK);	// Rollback transaction
	Log( szUserMsg);	Display( szUserMsg);
	Error( szErrorMsg, szSqlState);				// Reporting error in dialog box and logfile
	return (FAIL);                              // Allways return FAIL
	}

// Extract a paricular string field from a string buffer
BOOL TvSank2::StrGetField( int iFieldNo, char *szToBuffer, int iToBufferLen, char *szFromBuffer, char cSep)
	{
	int iSepCount, iCharCount;
	szToBuffer[0] = '\0';  // Returnerer tom streng hvis feltet ikke finnes
	// Scan FromBuffer til feltet er funnet
	for (iCharCount = 0, iSepCount = 1; szFromBuffer[iCharCount]; iCharCount++)
		{
		// Har vi funnet feltet ?
		if (iSepCount == iFieldNo)
			{
	        int i;
			// Kopier feltet så langt det er plass
			for (i = 0; (i < iToBufferLen - 1) && szFromBuffer[i + iCharCount] && szFromBuffer[i + iCharCount] != cSep; i++)
				szToBuffer[i] = szFromBuffer[i+iCharCount];
			szToBuffer[i] = '\0';
			return (TRUE);
			}
		else if (szFromBuffer[iCharCount] == cSep)
			iSepCount++;
		}
	return (FALSE);
	}

// Opens database connection
BOOL TvSank2::ConnectToVMS( void)
	{                                 
	RETCODE rc;     
	// Allcocate ODBC-resources
	if (SQLAllocEnv( &henv_) != SQL_SUCCESS)
		return (ReportVmsError( henv_, SQL_NULL_HDBC, SQL_NULL_HSTMT, "Odbc har ressursallokeringsproblemer") );
	rc = SQLAllocConnect( henv_, (HDBC FAR *)&hdbc_) ;
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
		return (ReportVmsError( henv_, SQL_NULL_HDBC, SQL_NULL_HSTMT, "Odbc har ressursallokeringsproblemer") );
	// Connect to database
	rc = SQLConnect( hdbc_, (UCHAR FAR *)szDSN_, SQL_NTS, (UCHAR FAR *)szUserName_, SQL_NTS, (UCHAR FAR *)szPassword_, SQL_NTS);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
		{
		// Asuming the DSN is not configured, so add DSN to registry
		char sz[BUFSIZ];
		memset( sz, 0, sizeof(sz) );
		ostrstream( sz, sizeof(sz) ) << "DSN=" << szDSN_ << "|AsyncSupport=Disabled|Server=" << szServer_ << "|UserId=" << szUserName_ << "||" << ends;
		Log( "Configuring datasource for Oracle73:"); Display( "Configuring datasource Oracle73:");
		Log( sz); Display( sz);
		// Null terminate each datasource argument
		for (int i = 0; sz[i]; i++)
			if (sz[i] == '|')
				sz[i] = '\0';
		if (!SQLConfigDataSource( (HWND)NULL, (UINT)ODBC_ADD_DSN, (LPCSTR)"Oracle73", (LPCSTR)sz) )
			return (ReportVmsError( henv_, hdbc_, SQL_NULL_HSTMT, "Problemer med konfigurering av datasource") );
		// Try once more to connect to database
		rc = SQLConnect( hdbc_, (UCHAR FAR *)szDSN_, SQL_NTS, (UCHAR FAR *)szUserName_, SQL_NTS, (UCHAR FAR *)szPassword_, SQL_NTS);
		if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
			return (ReportVmsError( henv_, hdbc_, SQL_NULL_HSTMT, "Odbc får ikke forbindelse med databaseserver") );
		}
	// Allcocate ODBC-resources for statements
	rc = SQLAllocStmt( hdbc_, &hstmt_);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
		return (ReportVmsError( henv_, hdbc_, SQL_NULL_HSTMT, "Odbc har ressursallokeringsproblemer") );
	return (OK);
	}

// Commit transaction and close database connection
void TvSank2::CloseVmsConnection( void)
	{                       
	SQLFreeStmt( hstmt_, SQL_DROP );	// Free ODBC resources
	SQLDisconnect( hdbc_);				// Close database connection
	SQLFreeConnect( hdbc_);				// Free ODBC resources
	SQLFreeEnv( henv_);     			// Free ODBC resources
	}

// Splits up input record and constructs insert statement
char *TvSank2::MakeInsertCmd( char *szBuffer, char *szSqlInsert, size_t iBufferSize)
	{
	static int iCount = 1;
	// Leverandørnummer;Varenummer;;Antall;Passord (Kjøkken) Feks: 176;1304109;;2.00;93361
	if (StrGetField( 1, szVendorId_, strlen( szBuffer), szBuffer, ';') )
		if (StrGetField( 2, szItemId_, strlen( szBuffer), szBuffer, ';') )
			if (StrGetField( 4, szQty_, strlen( szBuffer), szBuffer, ';') )
		{
		// Assemble insert command
		ostrstream( szSqlInsert, iBufferSize) <<
				"insert into REQ_SANK2_HLP (requisition_id, req_item_no, vendor_id, art_code, req_item_qty) values (" <<
				szReq_ << ',' << iCount++ << ',' << szVendorId_ << ',' << szItemId_ << ',' << szQty_ <<')' << ends;	
		return (szSqlInsert);	// OK
		}
	return (NULL);	// FAIL
	}

void TvSank2::Display( char *psz)
	{
	SendDlgItemMessage( hDlg_, IDC_LIST1, (WORD)LB_ADDSTRING,	(WPARAM)0, (LPARAM)(LPSTR)psz);
	}


// Run program to transfer data from hand held terminal to file
void TvSank2::Pdtpal( void)
	{
	char sz[BUFSIZ]; int iRet;
	if ( (iRet = chdir( "C:\\Pdtpal") ) != 0)
		{ Error( "Klarer ikke å skifte katalog til C:\\Pdtpal", " "); return; }
	// Make communication script file
	FILE *fhScr;
	if ( (fhScr = fopen( "sank2.scr", "w") ) == NULL)  
		{ Error( "Klarer ikke å lage kommunikasjons-skript:", "sank2.scr"); return; }
	fprintf( fhScr, "4\n9"); 	// Script to commuicate with hand held terminal
	fclose(fhScr);				// Close communication script
	// Start communication with hand held terminal
	if ( (iRet = spawnl( _P_WAIT, "C:\\Pdtpal\\Pdtpal.exe", "C:\\Pdtpal\\Pdtpal.exe", "-ssank2.scr", NULL ) ) == -1)
//	if ( (iRet = spawnl( _P_WAIT, "C:\\Pdtpal\\Pdtpal.exe", "C:\\Pdtpal\\Pdtpal.exe", NULL ) ) == -1)
		{ Error( "Klarer ikke å starte program for å kommuneisere med håndterminal. Error fra spawnl():", _itoa( iRet, sz, 10) ); return; }
	}


// Class entry point
void TvSank2::Run( void)
	{
	char szSqlInsert[BUFSIZ], szInBuffer[BUFSIZ], *psz, sz[BUFSIZ];
	if (bError_)
		return;
	RETCODE rc;
	// Open input file
	FILE *fhIn;
	if ( (fhIn = fopen( szInFileName_, "r") ) == NULL)  
		{ Error( "Klarer ikke å åpne input-fil:", szInFileName_); return; }
	// Connect to database
	if (ConnectToVMS() == FAIL)
		return;
	// Load data from input file into database table
	while (fgets( szInBuffer, sizeof( szInBuffer), fhIn) )
		{                                               
		if (strlen( szInBuffer) >= 9)		// Skip blank lines. A valid record should contain at least 9 byte
			{    				
			if ( (psz = MakeInsertCmd( szInBuffer, szSqlInsert, sizeof( szSqlInsert) ) ) == NULL)	// Split up input record and Build insert statement
				{                 
				ostrstream( sz, sizeof (sz) ) << "Klarer ikke å behandle følgende post: " << szInBuffer << '\n' << ends;;
				Log( sz); Display( sz);
				}
			else
				{
				// Execute Insert statement
				rc = SQLExecDirect( hstmt_, (UCHAR FAR *)psz, SQL_NTS);
				if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
					{
					ostrstream( sz, sizeof (sz) ) << "Insert problem: " << psz << '\n' << ends;;
					(void)(ReportVmsError( henv_, hdbc_, hstmt_, sz) );	// Rollback and report error
					break; 
					}
				}
			}
		}
	if (!bError_)
		SQLTransact( henv_, hdbc_, SQL_COMMIT);		// Commit transaction
	CloseVmsConnection();	// Close database connection
	fclose(fhIn);           // Close input file
	if (!bError_)
		Display( "Ferdig");
	}


static BOOL CALLBACK  DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	static TvSank2 *pTvSank2 = NULL;	// Main object
	HANDLE hCursor;
	switch (message)
		{
		case WM_INITDIALOG :
			{
			// Setup horizontal scroll width in listbox
			int iExtent = 256 + (LOWORD( GetDialogBaseUnits() ) * 300);
			SendDlgItemMessage( hDlg, IDC_LIST1, (WORD)LB_SETHORIZONTALEXTENT, (WPARAM)iExtent, (LPARAM)0L);
			pTvSank2 = new TvSank2();						// Allocate Main object
			if ( !pTvSank2->Init( glpCmdLine, hDlg) )		// Initialize main object
				EndDialog( hDlg, 0);						// End program if problems
			return (TRUE);									
			}
		case IDC_PDTPAL :										// User defined message
			{
			pTvSank2->Pdtpal();								// Communicate with bar code terminal
			SendDlgItemMessage( hDlg, IDC_LIST1, (WORD)LB_ADDSTRING, (WPARAM)0, (LPARAM)(LPSTR)"Overfører data ...");
			UpdateWindow( hDlg);
			PostMessage( hDlg, IDC_RUN, (WPARAM)0, (LPARAM)0);		// Send message to this window to start main logic
			return (TRUE);
			}
		case IDC_RUN :										// User defined message
			{
			pTvSank2->Run();								// Perform data transfer
			// Enable new default button
			EnableWindow( GetDlgItem( hDlg, IDC_OK), TRUE);	
			SendMessage( hDlg, WM_NEXTDLGCTL, 0, FALSE);
			SetFocus( (HWND)GetDlgItem( hDlg, IDC_OK) );	
			return (TRUE);
			}
		case WM_COMMAND :									// Dialog control message
			{
			switch (wParam)
				{
				case IDC_OK : 
				case IDC_CANCEL : 
					{
					delete pTvSank2;						// End main logic and delete object
					EndDialog( hDlg, 0);					// End program dialog
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
		glpCmdLine = lpCmdLine;
		DLGPROC lpfnDialog = (DLGPROC)MakeProcInstance( (FARPROC)DlgProc, hInstance);
		DialogBox( hInstance, "IDD_DIALOG1", (HWND)NULL, (DLGPROC)lpfnDialog);
		FreeProcInstance( (FARPROC)lpfnDialog);
		}
	return (0);
	}

// End of file

