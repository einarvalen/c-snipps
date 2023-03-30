#include <std.h>
#include <str.h>


BYTE *STR_pad( BYTE *s, short maxlen) /* Pads s with spaces until s[maxlen-1] */
  {
  register short i;
  assert(s);
  for (i=strlen( s); i<maxlen; i++)
    s[i] = ' ';
  s[maxlen] = '\0';
  return (s);
  }


BYTE *STR_zerofill( BYTE *s, short maxlen)
  {
  register short i;
  assert(s);
  STR_justright( s, maxlen);
  for (i=0; s[i] == SPACE; i++)
    s[i] = '0';
  return (s);
  }


BYTE  *STR_justright( BYTE *str, short maxlen)
  {
  int i;
  STR_trunc( str);
  if ((i = maxlen - strlen( str) ) > 0)
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
    str[maxlen] = NULL;
    }
  TRACE(("RIGHTSHIFT()-END\n"));
  return(str);
  }

BYTE *STR_set( BYTE *s, BYTE c, short len)
  {
  short i;
  for (i = 0; i < len; i++)
    s[i] = c;
  return (s);
  }


/** TESTCODE **
main()
  {
  BYTE str[11];
  strcpy( str, "1234");
  printf( "Pad       str=|%s|\n", STR_pad(  str, 10));
  printf( "Justright str=|%s|\n", STR_justright( str, 10));
  printf( "Zerofill  str=|%s|\n", STR_zerofill(  str, 10));
  }
**/

