/** Debug macros **/

/** #define DEBUG **/
#ifndef DBGH
#define DBGH
#include <stdio.h>
#include <assert.h>

  /* For debugging */
#if defined (DEBUG)
  #define TRACE( s) printf s
  #define DBG( var, frmt) printf( "\nDBG - (var) == frmt", var)
#else
  #define TRACE(s)
  #define DBG( var, frmt)
#endif

/** TEST
main()
  {
  int i;
  i=2;
  DBG(i,%d);
  TRACE(( "\nTRACE()\n"));
  assert(i==1);
  }
**/

#endif

