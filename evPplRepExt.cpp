/*******************************************************************************
 *
 *  PplRepExt.cpp - Search Extension for PowerPlay
 *
 *	March 3rd 1999 Einar Valen IC-Bergen
 *
 ******************************************************************************/

#include "stdafx.h"
#include "PplRepExt.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <fstream.h>

// Global variables
CString gcsDataFile;				// Name of the data file.
CString	gcsModelFile;				// Mane of the MDL file
CString	gcsNewModel = "New";		// Indicates if user intends to crete a new model
CString	gcsColDelimiter = ";";		// Column delimiter to use.
CString	gcsDecimalSeparator = ".";	// Column delimiter to use.
CString	gcsNewModelOptions = "Transformer";	// How does the user wish to build the new model
int		giDataFD   = -1;			// File descriptor for the data file.
long	glRowCount = 0;				// Track the number of rows processed.

// Function prototypes
BOOL GetParam( const char *&p, CString &value );
CString CreateModelName( CString csPath, CString csModelName);
CString CreateModelFileName( CString csModelFile, CString csDataFile);
void FillModelFile( void);
CString CreateModelFileName( CString csModelFile, CString csDataFile);


// This function returns the extension name to Actuate which the user sees.
// Note:  The implementation could be changed to get the name from the
//		  envrironment allowing for OEM customization.
const char* FAR PASCAL PplRepExtGetName()
	{
	return ("PowerPlay 5.x/6.x");
	}


// This function returns the parameter string to Actuate for serialization to the definition file.
// Currently, there are two parameters delimited with an asterisk.
const char* FAR PASCAL PplRepExtGetParameters()
	{
	static CString parmStr;
	parmStr =  gcsDataFile;			parmStr += "*";	// Name of the data file.
	parmStr += gcsModelFile;		parmStr += "*";	// Mane of the MDL file
	parmStr += gcsColDelimiter;		parmStr += "*";	// Column delimiter to use.
	parmStr += gcsDecimalSeparator; parmStr += "*";	// Column delimiter to use.
	parmStr += gcsNewModel;			parmStr += "*";	// Indicates if user intends to crete a new model
	parmStr += gcsNewModelOptions;	parmStr += "*";	// How does the user wish to build the new model
	MessageBox(  (HWND)NULL, parmStr, "PplRepExtGetParameters()", MB_OK);
	return ( (const char*)parmStr);
	}


static BOOL GetParam( const char *&p, CString &value )
	{
	if (!*p)
		return (FALSE);
	value.Empty();
	for (; *p  &&  *p != '*'; p++)
		value += *p;
	if (*p == '*')
		p++;
	return (TRUE);
	}


// This function is used by Actuate to pass the de-serialized parameters to the extension.
void FAR PASCAL PplRepExtSetParameters( const char* parms )
	{
	MessageBox(  (HWND)NULL, parms, "PplRepExtSetParameters()", MB_OK);
	gcsDataFile.Empty();  		gcsModelFile.Empty();
	gcsColDelimiter.Empty();	gcsDecimalSeparator.Empty();
	gcsNewModel.Empty();			gcsNewModelOptions.Empty();
	if (parms == NULL || *parms == 0 )				return;
	if (!GetParam( parms, gcsDataFile ) )			return;
	if (!GetParam( parms, gcsModelFile ) )			return;
	if (!GetParam( parms, gcsColDelimiter ) )		return;
	if (!GetParam( parms, gcsDecimalSeparator ) )	return;
	if (!GetParam( parms, gcsNewModel ) )			return;
	if (!GetParam( parms, gcsNewModelOptions ) )	return;
	}


// This function displays a dialog to allow the user to input the parameters.
BOOL FAR PASCAL PplRepExtInputParameters()
	{
	gcsDataFile = "C:\\TEMP\\PplRepExt.txt";
	gcsModelFile = "notepad C:\\TEMP\\PplRepExt.txt";
	//gcsColDelimiter = ;	
	//gcsDecimalSeparator;
	//gcsNewModel;			
	//gcsNewModelOptions;
	MessageBox(  (HWND)NULL, gcsModelFile, "PplRepExtInputParameters()", MB_OK);
	return (TRUE);
	}


// This function returns the column delimeter which Actuate will use when constructing the rows.
const char* FAR PASCAL PplRepExtGetDelimiter()
	{
	MessageBox(  (HWND)NULL, gcsColDelimiter, "PplRepExtGetDelimiter()", MB_OK);
	return gcsColDelimiter;
	}


// This function indicates to Actuate if a row of column names should be sent before the first data row.
BOOL FAR PASCAL PplRepExtIncludeHeader()
	{
	MessageBox(  (HWND)NULL, (gcsNewModel ? "TRUE" : "FALSE"), "PplRepExtIncludeHeader()", MB_OK);
	return (TRUE);
	}


// This function is called when Actuate begins the data extraction. This implementation creates the csv data file and sets the FD.
BOOL FAR PASCAL PplRepExtOpen()
	{
	MessageBox( (HWND)NULL, " ", "PplRepExtOpen()", MB_OK);
	// Open Datafile
	if (gcsDataFile.GetLength() == 0)
		return (FALSE);
	if (0 > (giDataFD = open( gcsDataFile, O_CREAT|O_TRUNC|O_RDWR, S_IREAD|S_IWRITE ) ) )
		return FALSE;
	glRowCount = 0;
	return TRUE;
	}


// This function is called when the data extraction is complete.
// This implementation closes the csv data file and then executes
// FileQuery with the query design specified in the parameters. 
BOOL FAR PASCAL PplRepExtClose( long clsState )
	{
	MessageBox(  (HWND)NULL, /*gcsModelFile*/" ", "PplRepExtClose()", MB_OK);
	// Close Files
	if (giDataFD > 0) 
		close( giDataFD );  
	giDataFD  = -1;	
	// Create and fill new MDL File
	if (gcsNewModel == "New")
		FillModelFile();
	// Execute transformer and/or PowerPlay
	if (glRowCount > 0 && clsState != 2 )
		{
		CString csCmd;
		// Run Transformer
		if (gcsNewModelOptions == "Transformer")
			csCmd = "notepad";
		else if (gcsNewModelOptions == "PowerPlay")
			csCmd = "notepad";
		else // if (gcsNewModelOptions == "AutoDesign")
			csCmd = "notepad";
		csCmd += gcsDataFile;
		csCmd += gcsModelFile;
		WinExec( csCmd, SW_SHOW);
		}
	return TRUE;
	}

// This function is called as each row is constructed during the data extraction.
// This implementation writes the row to the csv data file.
BOOL FAR PASCAL PplRepExtPutRow( long rowIdx, const char* row )
	{
	MessageBox(  (HWND)NULL, row, "PplRepExtPutRow()", MB_OK);
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


CString CreateModelFileName( CString csModelFile, CString csDataFile)
	{
	// Changes file extension
	int iIdxStart;
	iIdxStart = csModelFile.ReverseFind( '.');	// Find extension
	iIdxStart++;								// Jump '.'
//	csModelFile.Delete( iIdxStart, 9999);		// Remove file extension
//	csModelFile.Insert( 9999, "mdl");			// Append new file extension
	return (csModelFile);
	}


// Extract file-name without extension from path
CString CreateModelName( CString csPath, CString csModelName)
	{
	int iIdxStart, iIdxEnd;
	if ( (iIdxEnd = csPath.ReverseFind( '.')  ) < 0) // Find extension
		iIdxEnd = csPath.GetLength();
	iIdxStart = csPath.ReverseFind( '\\');			 // Find last back slash
	// Extact what is in between the last back slash and the file extension
	iIdxStart++;
	return (csModelName = csPath.Mid( iIdxStart, iIdxEnd - iIdxStart) );  
	}


// Create and fill new MDL File
void FillModelFile( void) 
	{
	// Create MDL filename based on data file nameif (gcsModelFile.IsEmpty() )
		CreateModelFileName( gcsModelFile, gcsDataFile);	
	CString csModelName;
	// Extract model name from mDL file name
	CreateModelName( gcsModelFile, csModelName);
	// Fill MDL file
	ofstream ofs( gcsModelFile);
	ofs << "NewModel \"" << csModelName << "\""<< endl;
	ofs << "QueryMake \"" << csModelName << "\" Separator \"" << gcsColDelimiter << "\" SourceType" << endl;
	ofs << "FlatFile_ColNames CharacterSet Ansi DecimalSep \"" << gcsDecimalSeparator << "\"" << endl;
	ofs << "Source \"" << gcsDataFile << "\" CreateColumns \"" << csModelName << "\""<< endl;
	if (gcsNewModelOptions == "AutoDesign")
		ofs << "AutoDesign" << endl;
	ofs << "CubeMake \"" << gcsModelFile << "\""<< "MDCFile \"" << csModelName << "\""<< endl;
	ofs << "Createfiles"<< endl;	
	ofs << "ModelEnsureCompleteness"<< endl << ends;
	ofs.close();
	}

// Test function
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
	BOOL bRet =  PplRepExtInputParameters();
	FillModelFile();
	return (0);
	}


// End of file.
