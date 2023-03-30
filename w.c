#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <string.h>
#include "w.h"
#include "mth.h"
#include "keys.h"
#include "vdo.h"
#include "keyb.h"

static W *current_w;
static BOOL w_begin_is_set;

void PASCAL W_begin( void)
  {
  static BOOL w_begin_is_set;
  if (w_begin_is_set)
    return;
  VDO_begin();
  KEYB_begin();
  w_begin_is_set = TRUE;
  }

void PASCAL W_end( void)
  {
  KEYB_end();
  VDO_end();
  w_begin_is_set = FALSE;
  }

void PASCAL W_set_current_w( W *w)
  {
  assert( w);
  current_w = w;
  VDO_set_window_boundaries( w->row, w->col, w->nrow, w->ncol);
  }

W PASCAL  *W_init( W *w, short row, short col, short nrow, short ncol)
  {
  assert( w);
  if (col == W_CENTER)
    col = W_maxcol( NULL) / 2 - (ncol / 2) - 1;  /* centering window */
  if (row == W_CENTER)
    row = W_maxrow( NULL) / 2 - (nrow / 2) - 1;  /* centering window */
  w->row  = MAX( 0, MIN( row, (VDO_maxrow() - nrow) ) );
  w->col  = MAX( 0, MIN( col, (VDO_maxcol() - ncol) ) );
  w->nrow = MIN( nrow, VDO_maxrow() );
  w->ncol = MIN( ncol, VDO_maxcol() );
  w->save_c = w->save_a = NULL;
  w->rowjust = w->coljust = w->srow = w->scol = 0;
  W_set_current_w( w);
  return (w);
  }

BOOL PASCAL  W_save( W *w)
  {
  assert( w);
  if ( (w->save_c = (BYTE *) malloc( sizeof( BYTE) * (w->nrow*w->ncol) ) ) == NULL)
    return (FAIL);;
  if ( (w->save_a = (BYTE *) malloc( sizeof( BYTE) * (w->nrow*w->ncol) ) ) == NULL)
    return (FAIL);
  VDO_save( w->row, w->col, w->nrow, w->ncol, w->save_c, w->save_a);
  return (OK);
  }

void PASCAL  W_restore( W *w)
  {
  assert( w);  assert( w->save_c); assert( w->save_a);
  if (w != current_w)
    W_set_current_w( w);
  VDO_restore( w->row, w->col, w->nrow, w->ncol, w->save_c, w->save_a);
  free( w->save_c);
  free( w->save_a);
  }

void PASCAL W_clear( W *w, short attr)
  {
  assert( w);
  if (w != current_w)
    W_set_current_w( w);
  w->attr = attr;
  VDO_clear( w->row, w->col, w->nrow, w->ncol, attr);
  }

void PASCAL W_puts( W *w, short row, short col, short maxlen, BYTE *str)
  {
  assert( w);
  if ( (row + w->rowjust) < 0  ||  (row + w->rowjust) > w->nrow)
    return;
  if (w != current_w)
    W_set_current_w( w);
  VDO_puts( (w->row + row + w->rowjust), (w->col + col + w->coljust), maxlen, str);
  }

void PASCAL W_putsa( W *w, short row, short col, short maxlen, BYTE *str, short attr)
  {
  assert( w);
  if (w != current_w)
    W_set_current_w( w);
  VDO_putsa( (w->row + row + w->rowjust), (w->col + col + w->coljust), maxlen, str, attr);
  }

BOOL PASCAL W_scroll( W *w)
  {
  short nrow, ncol;
  assert( w);
  if (w->srow == w->rowjust && w->scol == w->coljust)
    return (FALSE);
  if (w != current_w)
    W_set_current_w( w);
  nrow = w->rowjust - w->srow;  ncol = w->coljust - w->scol;
  w->srow = w->rowjust;   w->scol = w->coljust;
  if (nrow < 0)
    VDO_scroll_up( w->row, w->col, w->nrow, w->ncol, (-nrow), w->attr);
  else if (nrow > 0)
    VDO_scroll_down( w->row, w->col, w->nrow, w->ncol, nrow, w->attr);
  if (ncol < 0)
    VDO_scroll_left( w->row, w->col, w->nrow, w->ncol, (-ncol), w->attr);
  else if (ncol > 0)
    VDO_scroll_right( w->row, w->col, w->nrow, w->ncol, ncol, w->attr);
  return (TRUE);
  }


BOOL PASCAL W_focus( W *w, short row, short col, short n)
  {
  short rj, cj;
  assert( w);
  rj = w->rowjust;   cj = w->coljust;
  while ( (cj + col) <= 0)                          /** TO THE LEFT OF WINDOW **/
    cj++;
  while ( (cj + col + n) >= w->ncol)  /** TO THE RIGTH OF WINDOW **/
    cj--;
  while ( (rj + row) <= 0)                          /** ABOVE WINDOW **/
    rj++;
  while ( (rj + row) >= w->nrow)      /** BELOW WINDOW **/
    rj--;
  w->coljust = cj;   w->rowjust = rj;
  return (W_scroll( w) );
  }

BOOL PASCAL W_ajust( W *w, short row, short col, short n)
  {
  short rj, cj;
  assert( w);
  rj = w->rowjust;   cj = w->coljust;
  while ( (col + cj + n) <= 0)                          /** TO THE LEFT OF WINDOW **/
    cj++;
  while ( (col + cj) > (w->ncol) )  /** TO THE RIGTH OF WINDOW **/
    cj--;
  while ( (row + rj) < 0)                          /** ABOVE WINDOW **/
    rj++;
  while ( (row + rj) >= (w->nrow) )      /** BELOW WINDOW **/
    rj--;
  w->coljust = cj;   w->rowjust = rj;
  return (W_scroll( w) );
  }

WRET PASCAL w__gets( W *w, short row, short col, BYTE *str, short len)
  {
  register int i;
  short c, _row, _col, _maxlen, _maxdisplay, n;
  BYTE *_sp;
  assert( w);
  n = (short)strlen( str);
  _row = (w->row + (MAX(row,0) ) );
  _col = (w->col + (MAX(col,0) ) );
  _sp = ( IF (col>=0) THEN (str) ELSE ( &(str[ MAX( (-col), 0) ]) ) );
  _maxlen = len - (n - strlen( _sp) );
  _maxdisplay = MIN( _maxlen, (w->ncol - MAX(col, 0) ) );
  while (TRUE)
    {
    c = KEYB_gets( _row, _col, _sp, _maxlen, _maxdisplay);
    switch (c)
      {
      case '\b':
        if (col<0)
          {
          for (i=(-col); str[i]; i++)
            str[i-1] = str[i];
          str[i-1] = '\0';
          w->coljust++;
          return (W_REFRESH);
          }
        else
          break;
      case ARROW_LEFT:
        if (strlen( str) == strlen( _sp) )   
          break;                            /* return (c); ? */
        else
          {
          w->coljust++;
          return (W_REFRESH);
          }
      case ARROW_RIGHT:                      
        if (_maxlen == _maxdisplay)
          break;
        else                                /* return (c); ? */
          {
          w->coljust--;
          return (W_REFRESH);
          }
      case ARROW_END:
        w->coljust -= ( n - _maxdisplay);
        return (W_REFRESH);
      case ARROW_START:
        w->coljust = 0; /*+=(len - _maxlen);*/
        return (W_REFRESH);
      case '\n':
      case '\r':
        w->coljust = 0; /*+= (len - _maxlen);*/
        return ( (short)'\r');
      default: return (c);
      }
    }
  }

WRET PASCAL W_gets( W *w, short row, short col, BYTE *str, short len)
  {
  short n, ret;
  assert( w);
  n = (short)strlen( str);
  if (w != current_w)
    W_set_current_w( w);
  if (W_ajust( w, row, col, n) )
    return (W_REFRESH);
  ret = w__gets( w, (row + w->rowjust), (col + w->coljust), str, len);
  W_scroll( w);
  return (ret);
  }


void  W_printf( W *w, short row, short col, char *fmt, ...)
  {
  BYTE  s[BUFSIZ];
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  W_puts( w, row, col, strlen( s), s);
  }

void PASCAL  W_box( W *w, short row, short col, short nrow, short ncol, short border)    /* Draws box with specified border */
  {
  assert( w);
  if (w != current_w)
    W_set_current_w( w);
  VDO_box( (w->row + row + w->rowjust), (w->col + col + w->coljust),  nrow,  ncol,  border);
  }

void PASCAL  W_vert_line( W *w, short toprow, short topcol, short len, short border)    /* Draws vertical line  */
  {
  assert( w);
  if (w != current_w)
    W_set_current_w( w);
  VDO_vert_line(  (w->row + toprow + w->rowjust), (w->col + topcol + w->coljust),  len, border);
  }

void PASCAL  W_hor_line( W *w, short leftrow, short leftcol, short len, short border)    /* Draws vertical line  */
  {
  assert( w);
  if (w != current_w)
    W_set_current_w( w);
  VDO_hor_line(  (w->row + leftrow + w->rowjust), (w->col + leftcol + w->coljust),  len, border);
  }

void PASCAL  W_cursor_on(  void)  {  VDO_cursor_type( ON);   }
void PASCAL  W_cursor_off( void)  {  VDO_cursor_type( OFF);  }
short PASCAL W_maxrow( W *w)  { return (IF (w == NULL) THEN (VDO_maxrow() ) ELSE (w->nrow) );  }
short PASCAL W_maxcol( W *w)  { return (IF (w == NULL) THEN (VDO_maxcol() ) ELSE (w->ncol) );  }
short PASCAL W_minrow( W *w)  { return (IF (w == NULL) THEN (0) ELSE (w->row) );  }
short PASCAL W_mincol( W *w)  { return (IF (w == NULL) THEN (0) ELSE (w->col) );  }
WRET  PASCAL W_get()  {  return ( KEYB_get() );  }



/** TESTCODE **
W w, w1, w2;
BYTE s[21][21];

void show( void)
  {
  register short j;
  w2.coljust = w1.coljust;
  if (W_scroll( &w2) )
    W_puts( &w2, 0, 0, 16, "Test form with scrollabel heading.");
  W_box( &w1, 0, 0, 10, 4, 2);
  W_vert_line( &w1, 2, 2, 5, 4);
  for (j=0; s[j] && j<=20; j++)
    W_puts( &w1, j, 5, 16, s[j]);
  }


void main( void)
  {
  short go_on, i, j;
  WRET c;
  struct frm { short row; short col; BYTE s[11];};
  struct frm f[21];
  W_begin();
  W_init( &w, 5, 5, 15, 20);
  W_save( &w);
  W_clear( &w, 0x70);
  W_box( &w, 0, 0, (w.nrow), (w.ncol), W_SINGLE_LINE_ALL_SIDES);
  W_init( &w1, (w.row+2), (w.col+2), (w.nrow-3), (w.ncol-4) );
  W_init( &w2, (w.row+1), (w.col+2), (1), (w.ncol-4) );
  for (i=0; i<10; i++)
    W_printf( &w1, i, 0, "Line %d", i);
  (void) W_get();
  W_clear( &w1, 0x70);
  W_clear( &w2, 0x70);
  i = 5;
  W_putsa( &w1, 0, 0, 16, "---TEST---", 7);
  (void) W_get();
  W_clear( &w1, 0x70);
  for (i=0; i<20; i++)
    strcpy( s[i], "1234567890qwerty");
  strcpy( s[i],"\0");
  show();
  for (go_on=TRUE,i=0; go_on;)
    {
    c = W_gets( &w1, i, 5, s[i], 20);
    switch( c)
      {
      case ESC:                go_on=FALSE;   break;
      case W_REFRESH:    show();              break;
      case ARROW_UP:      if (i>0)   i--;     break;
      case ARROW_DOWN:    if (i<20)  i++;     break;
      case ARROW_LEFT:    if (i>0)   i--;     break;
      case ARROW_RIGHT:   if (i<20)  i++;     break;
      case '\r':  if (i<20) i++; show();      break;
      default: break;
      }
    }
  W_clear( &w1, 0x70);
  for (i=j=0; i<20; i++)
    {
    strcpy( f[i].s, "1234567890");
    j+=5;
    f[i].row = i;
    f[i].col = j;
    W_puts( &w1, f[i].row, f[i].col, 10, f[i].s);
    }
  for (go_on=TRUE,i=0; go_on;)
    {
    if (W_focus( &w1, f[i].row, f[i].col, 10) )
      {
      for (j=0; j<20; j++)
        W_puts( &w1, f[j].row, f[j].col, 10, f[j].s);
      }
    W_putsa( &w1, f[i].row, f[i].col, 10, f[i].s, 7);
    c = W_get();
    W_putsa( &w1, f[i].row, f[i].col, 10, f[i].s, 0x70 );
    switch( c)
      {
      case ESC:                go_on=FALSE;   break;
      case ARROW_UP:      if (i>0)   i--;     break;
      case ARROW_DOWN:    if (i<19)  i++;     break;
      case ARROW_LEFT:    if (i>0)   i--;     break;
      case ARROW_RIGHT:   if (i<19)  i++;     break;
      default: break;
      }
    }
  W_restore( &w);
  W_end();
  }
**/
