#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "std.h"
#include "str.h"


BYTE  *STR_trunc( BYTE *s)
  {
  register short i;
  assert( s);
  for (i=strlen( s)-1; i>=0 && s[i]<=' '; i--)
    s[i]='\0';
  return (s);
  }


BYTE  *STR_justleft( BYTE *s)
  {
  register short i, j;
  short len;
  assert( s);
  for (len=strlen( s); len>0 && s[0]==' '; len--)
    {
    for (j=0; j<len; j++)
     s[j]=s[j+1];
    }
  return (s);
  }

BYTE *STR_upshift( BYTE *s)
  {
  register short i;
  assert( s);
  for (i=0; s[i]; i++)
    s[i] = toupper( s[i]);
  return (s);
  }

BYTE *STR_downshift( BYTE *s)
  {
  register short i;
  assert( s);
  for (i=0; s[i]; i++)
    s[i] = tolower( s[i]);
  return (s);
  }

BOOL STR_isxmatch( BYTE *s, BYTE *cmp)
  {
  register short i, j;
  short slen, cmplen, k;
  assert( s); assert( cmp);
  slen = strlen( s);
  cmplen = strlen( STR_justleft( STR_trunc( cmp) ) );
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

BOOL STR_ismatch( BYTE *s, BYTE *cmp)
  {
  BOOL ret;
  BYTE *str;
  assert( s); assert( cmp);
  str = (BYTE *) malloc( sizeof( BYTE) * (strlen( s) + 1) );
  if (str == NULL)
     {
     printf( "STR_match() - Out of RAM!\n");
     return (FAIL);
     }
  STR_upshift( cmp);
  strcpy( str, s);
  STR_upshift( str);
  ret = STR_isxmatch( str, cmp);
  free( str);
  return (ret);
  }



/** TESTCODE **
main()
  {
  BYTE *str;
  BYTE *cmp;
  str = " er Einar Valen grei?";
  for (;;)
    {
    printf( "(%s) input match str:", str);
    scanf( "%s", cmp);
    if (cmp[0] == 'x')
       return;
    if (STR_ismatch( str, cmp) )
      printf( "OK");
    else
      printf( "FAIL");
    printf( "%s\n", cmp);
    printf( "STR_trunc(| 1  |)|%s|\n", STR_trunc(" 1  ") );
    printf( "STR_trunc(|   |)|%s|\n", STR_trunc("   ") );
    printf( "STR_justleft(| 1 |)|%s|\n", STR_justleft("  1  ") );
    printf( "STR_justleft(|   |)|%s|\n", STR_justleft("    ") );
    printf( "STR_upshift(einar)|%s|\n", STR_upshift("einar") );
    }
  }
**/

