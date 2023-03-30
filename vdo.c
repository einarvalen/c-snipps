#include <dos.h>
/*#include <conio.h>*/
#include "vdo.h"
#include "mth.h"
#include "std.h"

/**  DEFINES  **/

#define BIOS_VID_SET_CRTMODE       0
#define BIOS_VID_GET_CRTMODE       15
#define BIOS_VID_SET_CURSORTYPE    1
#define BIOS_VID_SET_CURSORPOS     2
#define BIOS_VID_GET_CURSORPOS     3

#define VIDEO_INT 0x10

#define MAXDIM(array)  (sizeof(array) / sizeof(array[0]))
#define VDO_MONO_PORT       0xb0000000
#define VDO_COLOR_PORT      0xb8000000
#define VDO_COLOR_MODE      3

/** TYPES **/
struct sBoxType {
  BYTE                cUpperLeft;
  BYTE                cLowerLeft;
  BYTE                cUpperRight;
  BYTE                cLowerRight;
  BYTE                cLeft;
  BYTE                cRight;
  BYTE                cTop;
  BYTE                cBottom;
  };
typedef struct sBoxType BOXTYPE;

/** GLOBAL VARIABLES **/
extern short vdo_exitrow, vdo_exitcol, vdo_exitmode;     /* For saving original setting */
extern short vdo_endscanline;     /* num of scanlines in cursor */
extern short vdo_mode, vdo_row, vdo_col, vdo_maxrow;     /* current setting */
extern short vdo_ul_boundary_row, vdo_ul_boundary_col, vdo_lr_boundary_row, vdo_lr_boundary_col;     /* text window boundaries */
extern BYTE far *vdo_buf;      /* ponter to video memory matrix */


void PASCAL VDO_set_window_boundaries( short row, short col, short nrow, short ncol)
  {
  vdo_ul_boundary_row = MAX( 0, MIN( (MAXROW-1), row) );
  vdo_ul_boundary_col = MAX( 0, MIN( (MAXCOL-1), col) );
  vdo_lr_boundary_row = MAX( 0, MIN( (MAXROW-1), (row+nrow-1) ) );
  vdo_lr_boundary_col = MAX( 0, MIN( (MAXCOL-1), (col+ncol-1) ) );
  }


void  PASCAL VDO_set_max_window_boundaries( void)
  {
  VDO_set_window_boundaries( 0, 0, (MAXROW-1), (MAXCOL-1) );
  }


void PASCAL VDO_putc( short row, short col, BYTE c)
  {
  if (row>=vdo_ul_boundary_row && row<=vdo_lr_boundary_row && col>=vdo_ul_boundary_col && col<=vdo_lr_boundary_col)
  VDO_CHAR( row, col) = c;  /* Assignment to vdo matrix */
  }


void PASCAL VDO_putca( short row, short col, BYTE c, BYTE a)
  {
  if (row>=vdo_ul_boundary_row && row<=vdo_lr_boundary_row && col>=vdo_ul_boundary_col && col<=vdo_lr_boundary_col)
    {
      VDO_CHAR( row, col) = c;  /* Assignment to vdo matrix */
      VDO_ATTR( row, col) = a;  /* Assignment to vdo matrix */
    }
  }


void PASCAL VDO_setmode( short mode) /* Sets videomode */
  {
  auto union REGS r;
  r.h.ah = BIOS_VID_SET_CRTMODE;
  r.h.al = vdo_mode = (BYTE)mode;
  int86( VIDEO_INT, &r, &r);
  }


void PASCAL VDO_begin( void)
  {
  auto union REGS r;
  static BOOL said_and_done = FALSE;
  if (said_and_done)
    return;
  said_and_done = TRUE;
  r.h.ah = BIOS_VID_GET_CURSORPOS;
  r.h.bh = 0;
  int86( VIDEO_INT, &r, &r);
  vdo_row = vdo_exitrow = r.h.dh;
  vdo_col = vdo_exitcol = r.h.dl;
  vdo_endscanline   = r.h.cl;
  r.h.ah = BIOS_VID_GET_CRTMODE;
  int86( VIDEO_INT, &r, &r);
  vdo_mode = vdo_exitmode = r.h.al;
  vdo_maxrow = r.h.ah;
  if ( vdo_exitmode != MONOCROME )
    {
    vdo_buf = (BYTE far *) VDO_COLOR_PORT;
    if (vdo_exitmode != VDO_COLOR_MODE)
      VDO_setmode( VDO_COLOR_MODE);
    }
  else
    vdo_buf = (BYTE far *) VDO_MONO_PORT;
  VDO_set_max_window_boundaries();
  }


void PASCAL VDO_end( void)
  {
  if ( vdo_exitmode != VDO_getmode() )
    VDO_setmode( vdo_exitmode);
  VDO_cursor_move( vdo_exitrow, vdo_exitcol);
  VDO_cursor_type( ON);
  }


void PASCAL VDO_cursor_move( short row, short col)    /* Positions cursor at location row,col */
  {
  auto union REGS r;
/*  assert( row < vdo_ul_boundary_row);  assert( row > vdo_lr_boundary_row);
  assert( col < vdo_ul_boundary_col);  assert( col > vdo_lr_boundary_col);*/
  r.h.dh = (BYTE)vdo_row = row;  r.h.dl = (BYTE)vdo_col = col;
  r.h.ah = BIOS_VID_SET_CURSORPOS;
  r.h.bh = 0;
  int86( VIDEO_INT, &r, &r);
  }


void PASCAL VDO_cursor_get( short *row, short *col)    /* Gets cursor position */
  {
  *row = vdo_row;
  *col = vdo_col;
  }


void PASCAL VDO_cursor_type( short type)  /* Sets cursor type ON, OFF or BOX */
  {
  auto union REGS r;
  r.h.ah = BIOS_VID_SET_CURSORTYPE;
  if (type == BOX)
    {
    r.h.ch = (char)2;
    r.h.cl = (char)vdo_endscanline;
    }
  else if (type)
    {
    r.h.ch = (char)vdo_endscanline-1;
    r.h.cl = (char)vdo_endscanline;
    }
  else
    {
    r.h.ch = (char)32;
    r.h.cl = (char)32;
    }
  int86( VIDEO_INT, &r, &r);
  }


void PASCAL VDO_save( short row, short col, short nrow, short ncol, BYTE *c, BYTE *a) /* Stores contents of screen sqare from */
  {
  short i,j;
  for (i=0; i<nrow; i++)
    for (j=0; j<ncol; j++)
      {
      c[i*ncol+j] = VDO_CHAR( (row+i), (col+j) );
      a[i*ncol+j] = VDO_ATTR( (row+i), (col+j) );
      }
  }


void PASCAL VDO_restore( short row, short col, short nrow, short ncol, BYTE *c, BYTE *a) /* Restores the screen contents  */
  {
  short i, j;
  for (i=0; i<nrow; i++)
    for (j=0; j<ncol; j++)
      VDO_putca( (row+i), (col+j), c[i*ncol+j], a[i*ncol+j]);
  }

void PASCAL VDO_scroll_left( short row, short col, short nrow, short ncol, short n, short attr) /* Scrolls the screen  */
  {
  short i, j, c, a;
  for (i = 0; i < nrow; i++)
    for (j = 0; j < (ncol - n); j++)
      {
      c = VDO_CHAR( (row + i), (col + j + n) );
      a = VDO_ATTR( (row + i), (col + j + n) );
      VDO_putca( (row + i), (col + j), c, a);
      }
  for (i = 0; i < nrow; i++)
    for (j = ncol - n; j < ncol; j++)
      VDO_putca( (row + i), (col + j), ' ', attr);
  }

void PASCAL VDO_scroll_right( short row, short col, short nrow, short ncol, short n, short attr) /* Scrolls the screen  */
  {
  short i, j, c, a;
  for (i = 0; i < nrow; i++)
    for (j = ncol - 1; j >= n; j--)
      {
      c = VDO_CHAR( (row + i), (col + j - n) );
      a = VDO_ATTR( (row + i), (col + j - n) );
      VDO_putca( (row + i), (col + j), c, a);
      }
  for (i = 0; i < nrow; i++)
    for (j = 0; j < n; j++)
      VDO_putca( (row + i), (col + j), ' ', attr);
  }

void PASCAL VDO_scroll_down( short row, short col, short nrow, short ncol, short n, short attr) /* Scrolls the screen  */
  {
  short i, j, c, a;
  for (i = nrow - 1; i >= n; i--)
    for (j = 0; j < ncol; j++)
      {
      c = VDO_CHAR( (row + i - n), (col + j) );
      a = VDO_ATTR( (row + i - n), (col + j) );
      VDO_putca( (row + i), (col + j), c, a);
      }
  for (i = 0; i < n; i++)
    for (j = 0; j < ncol; j++)
      VDO_putca( (row + i), (col + j), ' ', attr);
  }


void PASCAL VDO_scroll_up( short row, short col, short nrow, short ncol, short n, short attr) /* Scrolls the screen  */
  {
  short i, j, c, a;
  for (i = 0; i < (nrow - n); i++)
    for (j = 0; j < ncol; j++)
      {
      c = VDO_CHAR( (row + i + n), (col + j) );
      a = VDO_ATTR( (row + i + n), (col + j) );
      VDO_putca( (row + i), (col + j), c, a);
      }
  for (i = (nrow - n); i < nrow; i++)
    for (j = 0; j < ncol; j++)
      VDO_putca( (row + i), (col + j), ' ', attr);
  }


static   BOXTYPE       BoxTypes[] =  {
  { 213, 212, 184, 190, 179, 179, 205, 205 }, /* DBL_LINE_TOP_BOTTOM */
  { 214, 211, 183, 189, 186, 186, 196, 196 }, /* DBL_LINE_SIDES      */
  { 201, 200, 187, 188, 186, 186, 205, 205 }, /* DBL_LINE_ALL_SIDES  */
  { 218, 192, 191, 217, 179, 179, 196, 196 }, /* SNGL_LINE_ALL_SIDES */
  { 219, 219, 219, 219, 219, 219, 223, 220 }  /* GRAPHIC BOX     */
  };



void PASCAL VDO_box( short row, short col, short nrow, short ncol, short bordertype)
  {
  short i, r;
  if (bordertype < 0 || bordertype >= MAXDIM(BoxTypes))
    return;
  if (nrow < 2 || ncol < 2)
    return;
  VDO_putc( row, col, (short)BoxTypes[bordertype].cUpperLeft);
  for (i=1; i<ncol; i++)
    VDO_putc( row, (col+i), (short)BoxTypes[bordertype].cTop);
  VDO_putc( row, (col + ncol-1), (short)BoxTypes[bordertype].cUpperRight);
  for( i=row+1, r=row+nrow-1; i<r; i++)
    {
    VDO_putc( i, col, (short)BoxTypes[bordertype].cLeft);
    VDO_putc( i, (col + ncol-1), (short)BoxTypes[bordertype].cRight);
    }
  VDO_putc( (row + nrow-1), col, (short)BoxTypes[bordertype].cLowerLeft);
  for (i=1; i<ncol; i++)
    VDO_putc( (row + nrow-1), (col+i), (short)BoxTypes[bordertype].cBottom);
  VDO_putc( (row + nrow-1), (col+ncol-1), (short)BoxTypes[bordertype].cLowerRight);
  }


void PASCAL VDO_vert_line( short row, short col, short nrow, short bordertype)
  {
  short i, r;
  if (bordertype < 0 || bordertype >= MAXDIM(BoxTypes))
    return;
  for (i=0; i<nrow; i++)
    VDO_putc( (row + i), col, (short)BoxTypes[bordertype].cLeft);
  }


void PASCAL VDO_hor_line( short row, short col, short ncol, short bordertype)
  {
  short i, r;
  if (bordertype < 0 || bordertype >= MAXDIM(BoxTypes))
    return;
  for (i=0; i<ncol; i++)
    VDO_putc( row, (col + i), (short)BoxTypes[bordertype].cTop);
  }


void PASCAL VDO_putsa( short row, short col, short n, BYTE *s, short attr)
  {
  short i;
  for (i=0; s[i] && i<n; i++)
    VDO_putca( row, (col+i), s[i], attr);
  for ( ; (i<n); i++)
    VDO_putca( row, (col+i), ' ', attr);
  }


void PASCAL VDO_puts( short row, short col, short n, BYTE *s)  /* Displays n chars of s at row and col */
  {
  short i;
  for (i=0; i<n && s[i]; i++)
    VDO_putc( row, (col+i), s[i]);
  for ( ; (i<n); i++)
    VDO_putc( row, (col+i), ' ');
  }


void  PASCAL VDO_clear( short row, short col, short nrow, short ncol, short attr)
  {
  short i, j;
  for (i=0; i<nrow; i++)
    for (j=0; j<ncol; j++)
      VDO_putca( (row+i), (col+j), (' '), attr);
  }


/** TESTCODE **

void main( void)
  {
  BYTE a[410], ch[410];
  short i, row, col, r, c;
  BOOL goon;
  VDO_begin();
  VDO_putsa( 5, 20, 50, "----test", 0x70);
  VDO_save( 10, 10, 10, 40, (BYTE *)ch, (BYTE *)a);
  VDO_clear( 10, 10, 10, 40, 0x70);
  VDO_box( 10, 10, 10, 40, 0); getch();
  VDO_box( 10, 10, 10, 40, 1); getch();
  VDO_box( 10, 10, 10, 40, 2); getch();
  VDO_box( 10, 10, 10, 40, 3); getch();
  VDO_box( 1,  1,  20, 70, 4);
  VDO_set_window_boundaries( 10, 10, 10, 40);
  for (r = c = 0, goon = TRUE; goon;)
    {
    VDO_box( 10+r, 10+c, 10, 40, 3);
    VDO_vert_line( 11+r,  11+c,  8, 4);
    for (i=0; i<20; i++)
      VDO_putc( 12+r, (12+i+c), 'T');
    VDO_puts( 13+r, 13+c, 7, "Einar Valen");
    switch ( getch() )
      {
      case  '4': c--; VDO_scroll_left(  10, 10, 10, 40, 1, 0x70); break;
      case  '8': r--; VDO_scroll_up(    10, 10, 10, 40, 1, 0x70); break;
      case  '2': r++; VDO_scroll_down(  10, 10, 10, 40, 1, 0x70); break;
      case  '6': c++; VDO_scroll_right( 10, 10, 10, 40, 1, 0x70); break;
      case  'e': goon = FALSE; break;
      }
    }
  VDO_set_max_window_boundaries();
  VDO_cursor_move( 22, 0);
  printf( "maxrow=%d", VDO_maxrow() );
  VDO_cursor_type( BOX);
  getch();
  VDO_cursor_type( OFF);
  VDO_cursor_move( 23, 0);
  printf( "maxcol=%d", VDO_maxcol() );
  getch();
  VDO_cursor_type( ON);
  VDO_cursor_move( 24, 0);
  VDO_cursor_get( &row, &col);
  printf( "row=%d, col=%d", row, col);
  getch();
  VDO_putc( 10, 10, 'C');
  VDO_restore( 10, 10, 10, 40, (BYTE *)ch, (BYTE *)a);
  VDO_end();
  }
**/
