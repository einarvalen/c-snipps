#ifndef MSG_H
#define MSG_H

/** INCLUDE FILES **/
#include "std.h"
#include "w.h"

#define MSG_status_end( win)

/** PROTOTYPES **/
void MSG_msg( char *fmt, ...);   /* Standard message display with pausing */
void MSG_warn( char *fmt, ...);  /* Standard warning display */
void MSG_err( char *fmt, ...);   /* Standard error display with loging */
BOOL MSG_question( char *fmt, ...);   /* Standard question display */
void MSG_info( BYTE **info); /* Standard info display */
void MSG_prompt( BYTE *label, BYTE *s, short maxlen); /* Standard gets display */
void MSG_status_begin( W *hostwindow, W *statuswindow);
void MSG_status_printf( W *statuswindow, char *fmt, ...);


#endif
