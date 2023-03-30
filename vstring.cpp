#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "vstd.h"
#include "vstring.h"

//#define TEST_VSTRING //Uncomment to invoke test
//#define DEBUG

	/* For debugging */
#if defined (DEBUG)
	#define TRACE( s) printf s
#else
	#define TRACE(s)
#endif

// CRC routines

#define POLY 0x8408

unsigned int vStrCrc16( char *data_p, unsigned int length)
  /**
  " CCITT Polynomial Generator "  Calculates a 'checksum' for the data.
  May be used to verify integity or change in a block of data in either
  of two ways:
	1. Calculate the Crc of the data and compare it to a previously
		calculated Crc
	2. Append the calculated Crc to the data and calculate the Crc of the
		data and its Crc. If the new Crc equals CRC_ok, the data is unchanged.
	Has an error detection rate of 99.9984%, worst case.
	 (Ref CUJ Sep 1990)
  **/
	{
	char i;
  unsigned int data, crc = 0xffff;
  if (length == 0 || data_p == NULL)
		return (~crc);
  do
	 {
	 for (i = 0, data = (unsigned int)0xff & *data_p++; i < 8; i++, data >>= 1)
		{
		if ( (crc & 0x0001) ^ (data & 0x0001) )
		  crc = (crc >> 1) ^ POLY;
			else
		  crc >>= 1;
		}
	 } while (--length);
  crc = ~crc;
  data = crc;
	crc = (crc << 8) | (data >> 8 & 0xff);
  return (crc);
	}

// Crypt routines

char *vStrCrypt( char *str, char *key)
  {
  int i, code;
	if (str == NULL || key == NULL)
		return (NULL);
  for (i=code=0; key[i]; i++)
	 code ^= key[i];
  srand( (int)(0xff & code) );
	for (i=0; str[i]; i++)
	 str[i] = str[i] ^ (0xff & rand() );
  return ( str);
  }

char *vStrEncrypt( char *str, char *key)
	{
	return (vStrCrypt( str, key) );
	}

char *vStrDecrypt( char *str, char *key)
	{
	return (vStrCrypt( str, key) );
	}

char  *vStrTrunc( char *s)
  {
	register int i;
	if (s == NULL)
		return (NULL);
	i = strlen( s) - 1;
	while ( i >= 0 && isspace( s[i]) )
		{
		s[i]='\0';
		i--;
		}
	return (s);
	}


char  *szShiftLeft( char *s, int n)
	{
	register int j, i;
	if (s == NULL || n < 1)
		return (NULL);
	for (i = 0; i < n; i++)
		;
	if (i > 0)
		{
		for (j = 0; s[i + j]; j++)
			s[j] = s[i + j];
		}
	s[j] = '\0';
	return (s);
	}


char  *vStrJustLeft( char *s)
	{
	register int j, len;
	if (s == NULL)
		return (NULL);
	for (len=strlen( s); len>0 && s[0]==' '; len--)
	 {
	 for (j=0; j<len; j++)
		s[j]=s[j+1];
	 }
	return (s);
	}


char *vStrUpshift( char *s)
	{
	char *sp;
	if (s == NULL)
		return (NULL);
	for (sp = s; *sp; sp++)
	 switch (*sp)
			{
		case 'æ': *sp = 'Æ';  break;
			case 'ø': *sp = 'Ø';  break;
		case 'å': *sp = 'Å';  break;
			case 'ö': *sp = 'Ö';  break;
		case 'ä': *sp = 'Ä';  break;
		default : *sp = toupper( *sp);  break;
		}
	return (s);
	}

char *vStrDownshift( char *s)
	{
	char *sp;
	if (s == NULL)
		return (NULL);
	for (sp = s; *sp; sp++)
		switch (*sp)
			{
			case 'Æ': *sp = 'æ';  break;
			case 'Ø': *sp = 'ø';  break;
			case 'Å': *sp = 'å';  break;
			case 'Ö': *sp = 'ö';  break;
			case 'Ä': *sp = 'ä';  break;
			default : *sp = tolower( *sp);  break;
			}
	return (s);
	}

BOOL vStrIsxMatch( char *s, char *cmp)
	{
	register size_t i, j;
	size_t slen, cmplen, k;
	if (s == NULL || cmp == NULL)
		return (FAIL);
	slen = strlen( s);
	cmplen = strlen( vStrJustLeft( vStrTrunc( cmp) ) );
	for (i=j=k=0; i<cmplen && j<slen; i++)
		{
		if (cmp[i] == '*')
			{
			k = i;
			while (s[j] != cmp[i+1] && j<slen)
				j++;
			}
		else if (s[j] == cmp[i])
			j++;
		else
			{
			i = k - 1;
			j++;
			}
		}
	return (cmplen == i);
	}

BOOL vStrIsMatch( char *s, char *cmp)
	{
	BOOL ret;
	char *str;
	if (s == NULL || cmp == NULL)
		return (FAIL);
	str = (char *) malloc( sizeof( char) * (strlen( s) + 1) );
	if (str == NULL)
		 {
		 printf( "vStrMatch() - Out of RAM!\n");
		 return (FAIL);
		 }
	vStrUpshift( cmp);
	strcpy( str, s);
	vStrUpshift( str);
	ret = vStrIsxMatch( str, cmp);
	free( str);
	return (ret);
	}

BOOL vStrIsMember( char *s, char **cmp)
	{
	if (s == NULL)
		return (FAIL);
	for (size_t i=0; cmp[i]; i++)
		{
		if (strcmp( s,  cmp[i]) == 0)
			return (OK);
		}
	return (FAIL);
	}

char *vStrPad( char *s, size_t maxlen) /* Pads s with spaces until s[maxlen-1] */
	{
	if (s == NULL)
		return (NULL);
	for (size_t i = strlen( s); i < maxlen; i++)
		s[i] = ' ';
	s[maxlen] = '\0';
	return (s);
	}

char *vStrZerofill( char *s, size_t maxlen)
	{
	register size_t i;
	if (s == NULL)
		return (NULL);
	vStrJustRight( s, maxlen);
	for (i=0; s[i] == SPACE; i++)
		s[i] = '0';
	return (s);
	}

char  *vStrJustRight( char *str, size_t maxlen)
	{
	int i;
	if (str == NULL)
		return (NULL);
	vStrTrunc( str);
	if ( (i = maxlen - strlen( str) ) > 0)
		{
		register int j;
		TRACE(("OK SPACE\n"));
		do
			{
			TRACE(("LOOPING %d\n",i));
			for (j=maxlen-1; j>0; j--)
				str[j] = str[j-1];
			str[0] = ' ';
			}
			while (--i > 0);
		str[maxlen] = '\0';
		}
	TRACE(("RIGHTSHIFT()-END\n"));
	return(str);
	}

char *vStrSet( char *s, char c, size_t len)
	{
	size_t i;
	if (s == NULL)
		return (NULL);
	for (i = 0; i < len; i++)
		s[i] = c;
	return (s);
	}

void vStrPaste( char *str, char *paste, size_t pos, size_t maxlen)
	{
	int j, i;
	if (str == NULL || paste == NULL)
		return;
	for (j = 0, i = pos; i < maxlen && paste[j]; i++, j++)
		str[i] = paste[j];
	}


char *szStrPaste( char *szWhere, char *szWhat, size_t iMaxLen)
	{
	long iRequiredLen;
	if (szWhere == NULL || szWhat == NULL)
		return (NULL);
	iRequiredLen = strlen( szWhat) + strlen( szWhere);
	vStrJustRight( szWhere, (iMaxLen > iRequiredLen ? iRequiredLen : iMaxLen) );
	vStrPaste( szWhere, szWhat, 0, iMaxLen);
	return (szWhere);
	}


char *szStrCut( char *szFrom, size_t iHowMany, char *szTo, size_t iToLen)
	{
	if (szFrom == NULL || szTo == NULL)
		return (NULL);
	strncpy( szTo, szFrom, iHowMany);
	szTo[iHowMany] = '\0';
	szShiftLeft( szFrom, iHowMany);
	return (szTo);
	}


char *szStrSearch( char *szStr, char *szSearchFor)
	{
	int i, j;
	size_t iStrLen, iSearchForLen;
	if (szStr == NULL || szSearchFor == NULL)
		return (NULL);
	iStrLen = strlen( szStr);
	iSearchForLen = strlen( szSearchFor);
	for (i = j = 0; j < iSearchForLen && i < iStrLen; i++)
		{
		// Start over?
		if (szStr[i] != szSearchFor[j])
			j = 0;
		if (szStr[i] == szSearchFor[j])
			j++;
		}
	return( (j == iSearchForLen) ? &szStr[i - j] : NULL);
	}


char *szWordSearch( char *szTarget, char *szSearchFor)
	{
	char *psz, sz[BUFSIZ];
	for (psz = szTarget; sscanf( psz, "%s", sz) == 1; psz += (strlen( sz) +1) )
		if (strcmp( sz, szSearchFor) == 0)
			return (szStrSearch( psz, szSearchFor) );
	return (NULL);
	}


char *szStrReplaceAll( char *szStr, char *szSearchFor, char *szReplaceBy, size_t iStrLen)
	{
	char sz[512];
	if (szStr == NULL || szSearchFor == NULL || szReplaceBy == NULL)
		return (NULL);
	size_t i = strlen( szReplaceBy);
	for (char *psz = szStr;(psz = szStrSearch( psz, szSearchFor) ) != NULL; psz++)
		{
		szStrCut( psz, i, sz, 512);
		szStrPaste( psz, szReplaceBy, iStrLen);
		}
	return (szStr);
	}

char *vStrPicture( char *s, char *pic, char *str)
	/**
	Copies bytes from 'str' and 'pic' into 's' right to left.
	When a '^' char is found in 'pic' a byte from 'str' is copied
	else a byte from 'pic' is copied.
	Ex: pic="$^^,^^^^^^" and str="-1234.56" gives s="$-1,234.56"
	Ex: pic="$^^,^^^.^^" and str="-123456"  gives s="$-1,234.56"
	**/
	{
	register size_t i, j, n;
	if (s == NULL || pic == NULL || str == NULL)
		return (NULL);
	for (j = n = strlen( pic) - 1, i = strlen( str) - 1; i >= 0 && j >= 0; )
		{
		if (pic[j] == '^')
			s[j--] = str[i--];
		else if (pic[j] == ',' && str[i] == '-' && i>=0)
			s[j--] = str[i--];
		else
			{ s[j] = pic[j]; j--; }
		}
	for (i = j; j >= 0 && i >= 0; j--)
		{
		s[j] = ' ';
		if ( !(pic[j] == '^' || pic[j] == ',' || pic[j] == '.') )
			s[i--] = pic[j];
		}
	s[++n] = '\0';
	return (s);
	}

void vStrPurge( char *szFromString, char cRemoveByte)
	// Removes all occurences of 'cRemoveByte' from SzFromString
	{
	size_t i, j;
	char szBuffer[BUFSIZ + 1];
	if (szFromString == NULL)
		return;
	for (i = j = 0; szFromString[i]; i++)
		if (szFromString[i] != cRemoveByte)
			{
			szBuffer[j] = szFromString[i];
			j++;
			}
	szBuffer[j] = '\0';
	strcpy( szFromString, szBuffer);
	}

BOOL bStrGetField( int iFieldNo, char *szToBuffer, int iToBufferLen, char *szFromBuffer, char cSep)
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


/** TESTCODE **/
#ifdef TEST_VSTRING
#include <windows.h>

main()
	{
	char s[BUFSIZ];
	_InitEasyWin();
	printf( "\n\n\nCRC\n\n");
	strcpy( s, "THE QUICK BROWN FOX,1234567890");
	printf( "The Crc of \"%s\" is 0x%x\n", s, vStrCrc16( s, strlen( s) ) );
	printf( "The Crc of \"%s\" is 0x%x\n", s, vStrCrc16( s, strlen( s) ) );
	strcpy( s, "TEH QUICK BROWN FOX,1234567890");
	printf( "The Crc of \"%s\" is 0x%x\n", s, vStrCrc16( s, strlen( s) ) );
	printf( "The Crc returned should be 0xd9e4. Returned is 0x%x\n",vStrCrc16( "T", 1) );

	printf( "\n\n\nCRYPT (Hit CR)\n\n"); getc( stdin);
	strcpy( s, "einar");
	printf( "string=%s\n", s);
	vStrEncrypt( s, "tore");
	printf( "encrypted=%s\n", s);
	vStrDecrypt( s, "tore");
	printf( "decrypted=%s\n", s);

	printf( "\n\n\nTRUNC (Hit CR)\n\n");  getc( stdin);
	char *str, cmp[100];
	str = " er Einar Valen grei?";
	for (;;)
		{
		printf( "(%s) input match string(*=Wildcard, x to stop)\n:->", str);
		scanf( "%s", cmp);
		if (cmp[0] == 'x')
			 break;
		if (vStrIsMatch( str, cmp) )
			printf( "OK");
		else
			printf( "FAIL");
		printf( "%s\n", cmp);
		printf( "vStrTrunc(| 1  |)|%s|\n", vStrTrunc(" 1  ") );
		printf( "vStrTrunc(|   |)|%s|\n", vStrTrunc("   ") );
		printf( "vStrJustLeft(| 1 |)|%s|\n", vStrJustLeft("  1  ") );
		printf( "vStrJustLeft(|   |)|%s|\n", vStrJustLeft("    ") );
		printf( "vStrUpshift(einaræøåö1)|%s|\n", vStrUpshift("einaræøåö1") );
		printf( "vStrDownshift(EINARÆØÅÖ1)|%s|\n", vStrDownshift("EINARÆØÅÖ1") );
		}

	printf( "\n\n\nISMEMBER (Hit CR)\n\n");  getc( stdin);
	char *sp="GER", *ns="SWE", *pcmp[]={"ENG","NOR","GER","FR", NULL};
	printf( "Correct response is 1. Response is %d\n",
			vStrIsMember( sp, pcmp) );
	printf( "Correct response is 0. Response is %d\n",
			vStrIsMember( ns, pcmp) );

	printf( "\n\n\nPAD\n\n"); getc( stdin);
	vStrSet( s, '9', 5);
	printf( "Pad       str=|%s|\n", vStrPad(  s, 10));
	printf( "Justright str=|%s|\n", vStrJustRight( s, 10));
	printf( "Zerofill  str=|%s|\n", vStrZerofill(  s, 10));

	printf( "\n\n\nPASTE (Hit CR)\n\n");  getc( stdin);
	strcpy( s, "0123456789");
	vStrPaste( s, "abcdefg", 3, strlen(s) );
	printf( "(012abcdefg) == %s\n", s);

	printf( "\n\n\nPICTURE (Hit CR)\n\n");  getc( stdin);
	char s1[80];
	printf( "[%s]\n", vStrPicture( s, "$^^,^^^,^^^^^^^", "-1234567.890") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^^^^^", "-123456.789") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^^^^^", "-12345.678") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-1234567") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-123456") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-12345") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-1234") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-123") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-12") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-1") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "-") );
	printf( "[%s]\n", vStrPicture( s, "$^,^^^,^^^.^^^", "") );
	printf( "[%s]\n", vStrPicture( s, "Year: ^^^^  Month: ^^  Day: ^^", "19860529") );
	printf( "[%s]\n", vStrPicture( s, "Date: ^^/^^/^^^^", "29051986") );
	printf( "[%s]\n", vStrPicture( s, "Time: ^^:^^:^^", "133000") );
	sprintf( s1, "%3d%3d%3d%3d", 12, -12, 34, 1);
	printf( "[%s]\n", vStrPicture( s, "^^^^ ^^^^ ^^^^ ^^^^", " JAN FEB MAR APR") );
	printf( "[%s]\n", vStrPicture( s, "^^.^ ^^.^ ^^.^ ^^.^", s1) );
	char szValue[80], szPic[80], szBuffer[100];
	do
		{
		printf( "Input string Value (x to stop): ");
		scanf( "%s", szValue);
//    vStrPurge( szValue, ',');
		printf( "Input display Picture: ");
		scanf( "%s", szPic);
		printf( "\n[%s]\n", vStrPicture( szBuffer, szPic, szValue) );
		} while (szValue[0] != 'x');
	double dValue;
	do
		{
		printf( "Input Float Value (x to stop): ");
		scanf( "%s", szValue);
		if (szValue[0] == 'x')
			break;
		vStrPurge( szValue, ',');
		printf( "After Purge: [%s]\n", szValue);
		sscanf( szValue, "%lf", &dValue);
		sprintf( szValue, "%.3lf", dValue);  // 3 decimals
		strcpy( szPic, "^,^^^,^^^,^^^,^^^^^^^");
		printf( "Display Picture: %s\n", szPic);
		printf( "\n-->[%s]\n", vStrPicture( szBuffer, szPic, szValue) );
		} while (TRUE);

	if (bStrGetField( 2, szValue, sizeof (szValue), "Rød,Blå,Gul", ',') )
		printf( "Felt nr 2 = %s\n", szValue);
	printf( "\n *** THE END ***\n");
	return (1);
	}
#endif
