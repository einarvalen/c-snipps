#ifndef MOUSE_H
#define MOUSE_H

#include "std.h"
#include "w.h"

#define MOUSE_RESET    0  /* Reset the mouse driver                       */
#define MOUSE_ON       1  /* Turn Mouse Cursor on                         */
#define MOUSE_OFF      2  /* Turn Mouse Cursor off                        */
#define MOUSE_STATUS   3  /* Obtain status of the Mouse Driver            */
#define MOUSE_SET      4  /* Set Mouse Cursor position                    */
#define MOUSE_PRESSED  5  /* Get button press information                 */
#define MOUSE_RELEASED 6  /* Get button release  information              */
#define MOUSE_COLMAX   7  /* Set horizontal Mouse Cursor boundaries       */
#define MOUSE_ROWMAX   8  /* Set vertical   Mouse Cursor boundaries       */
#define MOUSE_GIMAGE   9  /* Map new graphics image onto the mouse cursor */
#define MOUSE_TIMAGE   10 /* Map new character onto the mouse cursor      */

#define LEFTBUTTON   1
#define RIGHTBUTTON  2
#define BOTHBUTTONS  (LEFTBUTTON | RIGHTBUTTON)

#define LEFT_STATUS   0
#define RIGHT_STATUS  1

/** PROTOTYPES **;
void MOUSE_begin( void);  /* Initiates the use of this module */
void MOUSE_end( void);    /* Ends the use of this module */
void MOUSE_reset( void);  /* resets mouse cursor */
BOOL MOUSE_button_new( short id, W *w, short row, short col, short nrow, short ncol, short cmd, BOOL repeat_click);
  /*
  Adds new button definition to event queue.
  Additions are always put at the top of the queue.
  if 'w' is NULL, the physical screen is assumed.
  Useful when a temination of search is called for, by making
  the whole screen area a button the lower level buttons are protected.
  */
void MOUSE_button_del( short id);
  /* Removes all buttons with this id from event queue */
void MOUSE_click( void);
  /*
  Searces the queue for suitable button if mouse is clicked
  and pushes specified command into command queue.
  The search is done from the top, and stops when suiteble button is found.
  */

#endif
