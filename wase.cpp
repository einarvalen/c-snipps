 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <windows.h>

 extern "C" 
 {
 
 #ifdef WIN32
 	_declspec(dllexport)
 #endif
 
 int
#ifdef WIN32
WINAPI
#endif
AcWebSecurityAuthenticate( char** httpHeaders, int numberOfHeaders, char* username,	char* password) 
	{
	char szUser[128];
	// GET the username
	DWORD bufSize = 128;
	if (!GetUserName( szUser, &bufSize) )
		return (1);	// FAIL
	else
		{
		// SET the username for Actuase report server
		strcpy( username, szUser);
		// SET the password for Actuase report server
		strcpy( password, _strlwr( szUser) );	
		}	
 	return (0); // OK
	}	//  AcWebSecurityAuthenticate
 
 } // extern