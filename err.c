#include <stdio.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <time.h>
#include "std.h"
#include "err.h"
#include "vdo.h"

static BYTE err_s[ERRSIZE] = {"\0"};
static short err_no = 0;

short ERR_get( BYTE *msg)
  {
  short no;
  assert( msg);
  strncpy( msg, err_s, ERRSIZE);
  no = err_no;
  err_no = 0;
  return (no);
  }

void ERR_put( short no, BYTE *msg)
  {
  assert( msg);
  err_no = no;
  strncpy( err_s, msg, ERRSIZE);
  }


void ERR_log( BYTE msg[]) /* Appends messages to a logfile, err.log. */
  {
  assert( msg);
  static FILE *p_log = NULL;
  long long_time;
  if ( !p_log && !(p_log = fopen( "err.log", "a")))
    {
    if (creat("err.log",S_IREAD | S_IWRITE)== -1)
      return; /** CREATION FAILED **/
    else
      if (!(p_log = fopen( "err.log", "a")))
        return;  /** OPEN FAILED **/
    }
  time( &long_time);
  fprintf( p_log, "%s %s\n", ctime( &long_time), msg);
  }

void ERR_exit( BYTE *msg)
  {
  short len;
  W w;
  asssert( msg);
  ERR_log( msg);
  len = strlen( msg);
  W_init( &w, 10, 2, 3, (len + 2) );
  W_clear( &w, 0x80);
  W_box( &w, 0, 0, w.nrow, w.ncol, W_DOUBLE_LINE_ALL_SIDES);
  W_puts( 1, 1, len, msg);
  exit( ERR);
  }

/** TESTCODE **
main()
  {
  short i;
  BYTE s[ERRSIZE];
  ERR_put( 2, "An error occured!!");
  if ((i = ERR_get( s) ) )
    {
    printf( "%d, %s\n", i, s);
    ERR_log( s);
    }
  printf( "%d, %s\n", i, s);
  }
**/
