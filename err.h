#ifndef ERR_H
#define ERR_H

#include <std.h>

#define ERRSIZE 80

/** PROTOTYPES  **/

short ERR_get( BYTE *msg);
  /*
  Gets and resets error no and msg
  */
void ERR_put( short no, BYTE *msg);
  /*
  Puts errorno and msg
  */
void ERR_log( BYTE msg[]);
  /*
  Appends messages to a logfile, err.log.
  */

#endif
