
#ifndef KEYB_H
#define KEYB_H

/** INCLUDE FILES **/
#include <std.h>


/** CONSTANTS **/


/** PROTOTYPES **/
void KEYB_begin( void);
  /*
  Initiates this module
  */
short KEYB_pop( void);
  /*
  Pop next key off keyb_queue
  */
short KEYB_get( void);
  /*
  Returns a key code from keyboard if the keyb_queue is empt.
  Else the returned value is the next key code popped off
  the keyb_queue.
  */
void  KEYB_push( short key);
  /*
  Appends key code to keyb_queue
  */
void KEYB_end( void);
  /*
  Shuts up the use of this module
  */
short KEYB_gets( short row, short col, BYTE *str, short maxlen, short maxdisplay);
  /*
  Gets a string of keyboard input in str limited maxlen number of characters
  The disoplay window of the displayed string is set by row, col and
  maxdisplay (max number of chars displayed). If maxlen exeeds maxdisplay,
  the string will have to be scrolled within the spcified display
  window.
  Returns last key code returned from KEYB_get() that KEYB_gets()
  cannot service.
  */


#endif
