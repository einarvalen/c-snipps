#include <stdio.h>
#include "str.h"
#include "field.h"

BOOL FIELD_amount_store( BYTE *s, double d)
  {
  if (d > 999999999.99 || d < -999999999.99)
    return (FAIL);
  sprintf( s, "%1.2f", d);
  STR_justright( s, FIELD_LEN_AMOUNT);
  return (OK);
  }

BOOL FIELD_amount_check( BYTE *s)
  {
  double d;
  if (sscanf( s, "%lf", &d) == 0)
    return (FAIL);
  return (FIELD_amount_store( s, d) );
  }

BOOL FIELD_ivalue_store( BYTE *s, long l)
  {
  if (l > 999999999 || l < -999999999)
    return (FAIL);
  sprintf( s, "%1ld", l);
  STR_justright( s, FIELD_LEN_IVALUE);
  return (OK);
  }

BOOL FIELD_ivalue_check( BYTE *s)
  {
  long l;
  if (sscanf( s, "%ld", &l) == 0)
    return (FAIL);
  return (FIELD_ivalue_store( s, l) );
  }

/** TESTCODE **
int main()
  {
  BYTE s[100];
  printf( "ok %d\n", FIELD_amount_check( "-999999999.99") );
  printf( "ok %d\n", FIELD_amount_check( " 999999999.99") );
  printf( "!ok %d\n", FIELD_amount_check( "999999999.999") );
  for (;;)
    {
    printf( "Double: ");
    scanf( "%s", s);
    if ( !FIELD_amount_check( s) )
      printf( "\nFAIL\n");
    else
      printf( "%s\n",s);
    }
  }
**/
