#include <str.h>
#include <string.h>

BOOL STR_ismember( BYTE *s, BYTE **cmp)
  {
  register i;
  for (i=0; cmp[i]; i++)
    {
    if (!strcmp( s,  cmp[i]) )
      return (OK);
    }
  return (FAIL);
  }
/** TESTCODE **
main()
  {
  static BYTE *s="GER", *ns="SWE", *cmp[]={"ENG","NOR","GER","FR"};
  printf( "%d\n", STR_ismember( s, cmp) );
  printf( "%d\n", STR_ismember( ns, cmp) );
  printf( "Correct response is 1 0\n");
  }
**/
