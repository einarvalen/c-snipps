#include "str.h"

BOOL STR_isint( BYTE *s)
  {
  long l;
  STR_trunc( s);
  if (strlen( s) == 0)
    return (OK);
  return (sscanf( s, "%ld", &l) != 0);
  }

/** TESTCODE **
main()
  {
  BYTE s[20];
  strcpy( s, "12345f");
  printf( "%s %s\n", s, (IF (STR_isint( s)) THEN ("TRUE") ELSE ("FALSE") ) );
  strcpy( s, " 123456");
  printf( "%s %s\n", s, (IF (STR_isint( s)) THEN ("TRUE") ELSE ("FALSE") ) );
  strcpy( s, "\0");
  printf( "%s %s\n", s, (IF (STR_isint( s)) THEN ("TRUE") ELSE ("FALSE") ) );
  }
**/

