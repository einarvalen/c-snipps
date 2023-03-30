// $Revision: 2 $
// $Archive: /gemini-maintenence/Client/srchext/PplRepExt/PplRepExt.h $

#ifndef __PplRepExt_H__
#define __PplRepExt_H__

/*******************************************************************************
 *
 *  PplRepExt.h -	Sample Search Extension for file output. 
 *
 *	5/27/98   Copyright Actuate Software Corporation
 *
 ******************************************************************************/
#define DllExport				extern __declspec( dllexport )
#define TRUE	1
#define FALSE	0
typedef int	BOOL;
/////////////////////////////////////////////////////////////////////////////

DllExport const char*	FAR PASCAL PplRepExtGetName();
DllExport const char*	FAR PASCAL PplRepExtGetParameters();
DllExport void			FAR PASCAL PplRepExtSetParameters( const char* parms );
DllExport BOOL			FAR PASCAL PplRepExtInputParameters();
DllExport const char*	FAR PASCAL PplRepExtGetDelimiter();
DllExport BOOL			FAR PASCAL PplRepExtIncludeHeader();
DllExport BOOL			FAR PASCAL PplRepExtOpen();
DllExport BOOL			FAR PASCAL PplRepExtClose( long clsState );
DllExport BOOL			FAR PASCAL PplRepExtPutRow( long rowIdx, const char* row );

#endif /* __PplRepExt_H__ */
