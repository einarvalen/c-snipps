#include <stdio.h>
#include <ctype.h>
#include <std.h>

main()
  {
  short  i;
  BYTE s[BUFSIZ];
  while ( gets( s) )
    {
    for (i=0; s[i]; i++)
      {
      if (s[i] == '_' && isupper( s[i+1]) && islower( s[i+2]) )
        s[i+1] = tolower( s[i+1] );
      }
    puts( s);
    }
  }

