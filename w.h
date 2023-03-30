#ifndef W_H
#define W_H

/** INCLUDE FILES **/
#include <std.h>
#include <color.h>

/** CONSTANTS **/
/** WRET messages **/
#define W_QUIT          -99    /** RETURN INSTRUCTION TO QUIT APLICATION **/
#define W_REFRESH       -1    /** STATUS RETURN FROM W_IO FUNCTIONS **/

/** Box types **/
#define W_DOUBLE_LINE_TOP_BOTTOM   0
#define W_DOUBLE_LINE_SIDES        1
#define W_DOUBLE_LINE_ALL_SIDES    2
#define W_SINGLE_LINE_ALL_SIDES    3
#define W_GRAPHIC_BOX              4
#define W_NO_WIND_BORDER           99

/** Line types **/
#define W_THICK              5
#define W_DOUBLE             3
#define W_SINGLE             4

#define W_CENTER           -101

/** TYPES **/
typedef short WRET;
struct window {
    short row,  col;          /* upper left corner */
    short nrow, ncol;         /* lower right corner */
    short rowjust, coljust;   /* NEGATIVE NUMBER INDICATING VDO ADJUSTMEN */
    short srow, scol;         /* used by W_scroll() to determin scrolling */
    short attr;               /* background attr */
    BYTE *save_c;             /* array for saving overwritten text */
    BYTE *save_a;             /* array for saving overwritten attr */
}
typedef struct window W;

/** PROTOTYPES **/
void  PASCAL W_puts( W *window, short row, short col, short maxlen, BYTE *str);
  /*
  Prints 'str' at window  relative position 'row' and 'col'. The number of
  characters  printed is limited to 'maxlen'. If the number of characters
  in 'str' is less than 'maxlen' then 'maxlen - strlen(str)' spaces are
  appended. The text and background color is  unchanged.
  */
void  PASCAL W_putsa( W *window, short row, short col, short maxlen, BYTE *str, short attr);
  /*
  Simmilar to W_puts(). W_puts() with given 'attr'. (text and background color)
  */
void  W_printf( W *window, short row, short col, char *fmt, ...);
  /*
  Formated output function. (ala printf() ) See W_puts().
  */
W PASCAL *W_init( W *window, short row, short col, short nrow, short ncol);
  /*
  Initializes window struct.
  */
void  PASCAL W_box( W *window, short row, short col, short nrow, short ncol, short type);
  /*
  Draws box with border types specified in 'type'. The box is output at window
  relative position defined by 'row' and 'col' as upper left corner,
  width defined by 'ncol' and depth by 'nrow'.
  */
BOOL  PASCAL W_save( W *window);
  /*
  Saves window background to provide an overlay fcility.
  */
void  PASCAL W_restore( W *window);
  /*
  Restore saved window background.
  */
void  PASCAL W_hor_line( W *window, short leftrow, short leftcol, short len, short border);
  /*
  Draws horizontal line with border types specified in 'type'.
  The line is output at window relative position defined by 'leftrow'
  and 'leftcol' as left end, width defined by 'len'.
  */
void  PASCAL W_vert_line( W *window, short toprow, short topcol, short len, short border);
  /*
  Draws vertical line with border types specified in 'type'.
  The line is output at window relative position defined by 'toprow'
  and 'topcol' as upper end, depth defined by 'len'.
  */
WRET  PASCAL W_gets( W *window, short row, short col, BYTE *str, short len);
  /*
  Gets a string of keyboard input in 'str' limited 'len' number of characters
  The display window of the displayed string is set by 'window'.
  If some or all of the string is located beyond the window  boundaries,
  the string will have to be scrolled within the spcified display
  window. In this case W_REFRESH  value is returned. If the returned value
  is positive, it represents the last key sent from the keyboard.
  */
void  PASCAL W_end( void);
  /*
  Shuts up the use of this module
  */
void  PASCAL W_begin( void);
  /*
  Initiates this module
  */
void  PASCAL W_cursor_on( void);
  /*
  Turns cursor on.
  */
void PASCAL W_cursor_off( void);
  /*
  Turns cursor off
  */
short PASCAL W_maxcol( W *window);
  /*
  Returns maximimum number of columns in window display,
  if the window argument != NULL else values for the physical display
  is returned.
  */
short PASCAL W_maxrow( W *window);
  /*
  Returns maximimum number of lines in window display,
  if the window argument != NULL else values for the physical display
  is returned.
  */
short PASCAL W_mincol( W *window);
  /*
  Returns minimimum number of columns in window display,
  if the window argument != NULL else values for the physical display
  is returned.
  */
short PASCAL W_minrow( W *window);
  /*
  Returns minimimum number of lines in window display,
  if the window argument != NULL else values for the physical display
  is returned.
  */
WRET  PASCAL W_get( void);
  /*
  Returns a key code from keyboard if the keyb_queue is empt.
  Else the returned value is the next key code popped off
  the keyb_queue.
  */
BOOL PASCAL W_scroll( W *w);
  /*
  Scrolls window up/down/right/left. Determined by rowjust, coljust,
  srow and scol. All set by W_focus(), W_ajust() and W_gets().
  Returns true if any scrolling performed and a refresh is called for.
  */
BOOL  PASCAL W_focus(  W *window, short row, short col, short len);
  /*
  Focuses display window on the field located at window relative position
  represented by 'row' and 'col' with the maximum length of 'len'.
  Returns true if redisplay is called for.
  */
BOOL  PASCAL W_adjust( W *window, short row, short col, short len);
  /*
  Internal function; used by W_gets().
  Brings the field located at window relative position represented by
  'row' and 'col' with the maximum length of 'len' into the actual window.
  Returns true if redisplay is called for.
  */
void  PASCAL W_clear(  W *window, short attr);
  /*
  Clears window defined by 'window' to the text and background color to
  the aquired attributes ('attr').
  */
void  PASCAL W_set_current_w( W *window);
  /*
  Sets the current display window.
  */

#endif
