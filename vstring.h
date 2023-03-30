#ifndef VSTRING_H
#define VSTRING_H

#include "vstd.h"


/** PROTOTYPES **/

char  *vStrTrunc( char *s); 
/* Remove trailing whitespace from string s */

char  *vStrJustLeft( char *s);
/* Removes all leading whitespace */

char  *szShiftLeft( char *s, int n);
/* Removes all leading whitespace, similar to vStrJustleft() only limited by n chars */

char  *vStrJustRight( char *s, size_t maxlen); 
/* Shifts str s right within the bounds inic. by maxlen */

char  *vStrZerofill( char *s, size_t maxlen); 
/* Replaces leading blanks by zeroes */

char  *vStrUpshift( char *s); 
/* Change all lowercase chars in str s to uppercase */

char  *vStrDownshift( char *s); 
/* Change all uppercase chars in str s to lowercase */

BOOL   vStrIsMatch( char *s, char *cmp); 
/* Serces for the str cmp whithin the str s regardles of case) */

BOOL   vStrIsxMatch( char *s, char *cmp); 
/** As vStrIsMatch() bt case is siignif.**/

BOOL   vStrIsMember( char *s, char **cmp);
char  *vStrPad( char *s, size_t maxlen);
char  *vStrEncrypt( char *str, char *key);
char  *vStrDecrypt( char *str, char *key);
char  *vStrSet( char *s, char c, size_t len);
void  vStrPaste( char *str, char *paste, size_t pos, size_t maxlen);
char *vStrPicture( char *s, char *pic, char *str);
  /**
  Copies bytes from 'str' and 'pic' into 's' right to left.
  When a '^' char is found in 'pic' a byte from 'str' is copied
  else a byte from 'pic' is copied.
  Ex: pic="$^^,^^^^^^" and str="-1234.56" gives s="$-1,234.56"
  Ex: pic="$^^,^^^.^^" and str="-123456"  gives s="$-1,234.56"
	**/
void vStrPurge( char *szFromString, char cRemoveByte);
	// Removes all occurences of 'cRemoveByte' from SzFromString

char *szStrPaste( char *szWhere, char *szWhat, size_t iMaxLen);

char *szStrCut( char *szFrom, size_t iHowMany, char *szTo, size_t iToLen);

char *szStrSearch( char *szStr, char *szSearchFor);

char *szWordSearch( char *szTarget, char *szSearchFor);

char *szStrReplaceAll( char *szStr, char *szSearchFor, char *szReplaceBy, size_t iStrLen);

BOOL bStrGetField( int iFieldNo, char *szToBuffer, int iToBufferLen, char *szFromBuffer, char cSep);
	// Kopierer karakter felt fra et buffer til et annet, nullterminert, feltskilletegn=cSep,
#endif
