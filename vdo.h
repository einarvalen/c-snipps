#ifndef VDO_H
#define VDO_H

/**  INCLUDE FILES  **/
#include <std.h>

/**  DEFINES  **/
#define  MONOCROME       7
#define  COLOR           3
#define  BOX             -1
#define  VDO_ISMONO() (vdo_mode == MONOCROME)


/** GLOBAL VARIABLES **/
short vdo_exitrow, vdo_exitcol, vdo_exitmode;     /* For saving original setting */
short vdo_endscanline;     /* num of scanlines in cursor */
short vdo_mode, vdo_row, vdo_col, vdo_maxrow;     /* current setting */
short vdo_ul_boundary_row, vdo_ul_boundary_col, vdo_lr_boundary_row, vdo_lr_boundary_col;     /* text window boundaries */
BYTE far *vdo_buf;      /* ponter to video memory matrix */


/** MACROS **/
#define MAXCOL 80     /*(vdo_maxcol + 1)*/
#define MAXROW 25
#define VDO_CHAR( row, col)  vdo_buf[ ( (row * MAXCOL * 2) + (col * 2) ) ]    /* Video map matrix */
#define VDO_ATTR( row, col)  vdo_buf[ ( (row * MAXCOL * 2) + (col * 2) ) + 1]  /* Video map matrix */
#define VDO_getmode()   (vdo_mode)
#define VDO_maxcol()    (MAXCOL)
#define VDO_maxrow()    (MAXROW)


/** FUNCTION PROTOTYPES **/
void  PASCAL VDO_begin( void);                   /* Intializes program for 'VDO' functtions */
void  PASCAL VDO_end( void);
/*short PASCAL VDO_getmode( void); */               /* Returns current videomode */
void  PASCAL VDO_setmode( short mode);               /* Sets videomode */
void  PASCAL VDO_cursor_type( short type);                /* Sets cursor CUR_ON, CUR_OFF, CUR_BOX */
void  PASCAL VDO_cursor_get( short *row, short *col); /* Gets cursor position */
void  PASCAL VDO_cursor_move( short row, short col);        /* Positions cursor at location row,col */
void  PASCAL VDO_putc( short row, short col, BYTE c);     /* Prints char on screen at row, col */
void  PASCAL VDO_putca( short row, short col, BYTE c, BYTE a);   /* Prints char and attr on screen at row, col */
void  PASCAL VDO_clear( short row, short col, short nrow, short ncol, short attr);
void  PASCAL VDO_puts( short row, short col, short n, BYTE *s);
void  PASCAL VDO_putsa( short row, short col, short maxlen, BYTE *s, short attr);
void  PASCAL VDO_save( short row, short col, short nrow, short ncol, BYTE *c, BYTE *a);      /* stores a rectangle of the screen */
void  PASCAL VDO_restore( short row, short col, short nrow, short ncol, BYTE *c, BYTE *a);   /* Restores a rectangle to the screen */
void  PASCAL VDO_box( short row, short col, short nrow, short ncol, short border);    /* Draws box with specified bordere and attr */
void PASCAL VDO_hor_line( short row, short col, short ncol, short bordertype);
void PASCAL VDO_vert_line( short row, short col, short nrow, short bordertype);
void  PASCAL VDO_set_max_window_boundaries( void);    /* Limits display operation to physical screen */
void  PASCAL VDO_set_window_boundaries( short row, short col, short nrow, short ncol);  /* Limits display operation to sertain area on screen */
void PASCAL VDO_scroll_left( short row, short col, short nrow, short ncol, short n, short attr);
  /*
  Scrolls the screen n cols left.
  Attr is used for resetting off scrolled screen area.
  */
void PASCAL VDO_scroll_right( short row, short col, short nrow, short ncol, short n, short attr);
  /*
  Scrolls the screen n cols right.
  Attr is used for resetting off scrolled screen area.
  */
void PASCAL VDO_scroll_up( short row, short col, short nrow, short ncol, short n, short attr);
  /*
  Scrolls the screen n rows up.
  Attr is used for resetting off scrolled screen area.
  */
void PASCAL VDO_scroll_down( short row, short col, short nrow, short ncol, short n, short attr);
  /*
  Scrolls the screen n rows down.
  Attr is used for resetting off scrolled screen area.
  */

#endif
