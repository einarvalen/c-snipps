#include <dos.h>
#include <stdlib.h>
#include "ll.h"
#include "mouse.h"

struct cmouse {
  short id;
  short row, col, nrow, ncol;
  W *w; /* NULL if absolute screen adressing is required */
  short cmd; /* Command to be pushed into queue */
  BOOL repeat_click; /* if click is supposed to left idle for father function */
  };

static struct {
  struct ll first,last, *node;
  BOOL installed;
  BOOL idle_click;
  short idle_row, idle_col;
  } event_q;

static int mouse( int service, int *bx_register, int *cx_register, int *dx_register)
  {
  union REGS inregs, outregs;
  inregs.x.ax = service;
  inregs.x.bx = *bx_register;
  inregs.x.cx = *cx_register;
  inregs.x.dx = *dx_register;
  int86( 51, &inregs, &outregs);
  *bx_register = outregs.x.bx;
  *cx_register = outregs.x.cx;
  *dx_register = outregs.x.dx;
  return (outregs.x.ax);
  }

void MOUSE_begin( void)
  {
  int bx = 0, cx = 0, dx = 0;
  assert( &(event_q.first) );  assert( &(event_q.last) );
  LL_begin( &(event_q.first), &(event_q.last) );
  event_q.node = &(event_q.first);
  if ( (event_q.installed = mouse(MOUSE_RESET, &bx, &cx, &dx) ) == FAIL)
    printf( "Microsoft Mouse Interface Driver is not installed\n");
  else
    mouse( MOUSE_ON, &bx, &cx, &dx);
  event_q.idle_click = FALSE;
  }

void MOUSE_end( void)
  {
  int bx = 0, cx = 0, dx = 0;
  assert( &(event_q.first) );  assert( &(event_q.last) );
  LL_end( &(event_q.first) );
  mouse( MOUSE_OFF, &bx, &cx, &dx);
  }

void MOUSE_reset( void)
  {
  int bx = 0, cx = 0, dx = 0;
  if (!event_q.installed)
    return;
  mouse(MOUSE_RESET, &bx, &cx, &dx);
  mouse( MOUSE_ON, &bx, &cx, &dx);
  }

static short MOUSE_get( short *row, short *col)
  {
  int bx=0, cx=0, dx=0;
  if (event_q.installed && mouse(MOUSE_STATUS, &bx, &cx, &dx) )
    {
    *row = (cx>>3);
    *col = (dx>>3);
    }
  return (bx);
  }

BOOL MOUSE_button_new( short id, W *w, short row, short col, short nrow, short ncol, short cmd, BOOL repeat_click)
  {
  struct cmouse *new;
  assert( &(event_q.first) );  assert( &(event_q.last) );
  if ( !event_q.installed)
    return (OK);
  if ( (new = (struct cmouse *)calloc( sizeof( struct cmouse), 1) ) == NULL)
    return (FAIL);
  new->w = w;        new->id = id;
  new->row = row;    new->col = col;
  new->nrow = nrow;  new->ncol = ncol;
  new->cmd = cmd;    new->repeat_click = repeat_click;
  event_q.node = &event_q.first;
  return ( (event_q.node = LL_ins( event_q.node, new) ) != NULL);
  }

void MOUSE_button_del( short id)
  {
  struct cmouse *button;
  struct ll *np, *np1;
  if ( !event_q.installed)
    return;
  np = &event_q.first;
  for ( np = LL_GET_NEXT( np); np != NULL; )
    {
    button = np->data;
    if (button != NULL && button->id == id)
      {
      free( np->data);
      np = LL_del( np);
      }
    else
      np = LL_GET_NEXT( np);
    }
 }

void MOUSE_click( void)
  {
  W *w;
  struct cmouse *button;
  struct ll *np, *np1;
  short row, col;
  short visible_from_row, visible_from_col, visible_to_row, visible_to_col;
  if ( !event_q.installed)
    return;
  if (event_q.idle_click)
    {
    row = event_q.idle_row;
    col = event_q.idle_col;
    event_q.idle_click = FALSE;
    }
  else if ( !MOUSE_get( &row, &col) )
    return;
  np = &event_q.last;
  for ( np = LL_GET_NEXT( np); np != NULL; )
    {
    button = np->data;
    if (button->w != NULL)
      {
      w = button->w;
      if (row >= w->row && col >= w->col  /* within window boundaries */
          && row <= (w->row + w->nrow) && col <= (w->col + w->ncol) )
        {
        visible_from_row = button->row + w->rowjust;
        visible_from_col = button->col + w->coljust;
        visible_to_row = visible_from_row + button->nrow;
        visible_to_col = visible_from_col + button->ncol;
        if (row >= visible_from_row && col >= visible_from_col /* within window boundaries */
            && row <= visible_to_row && col <= visible_to_col)
          {
          if (button->repeat_click)
            {
            event_q.idle_click = TRUE;
            event_q.idle_row = row;
            event_q.idle_col = col;
            }
          else
            event_q.idle_click = FALSE;
          KEYB_push( button->cmd);
          return;
          }
        }
      else
        if (row >= button->row && col >= button->col /* within window boundaries */
            && row <= (button->row + button->nrow)
            && col <= (button->col + button->ncol))
          {
          if (button->repeat_click)
            {
            event_q.idle_click = TRUE;
            event_q.idle_row = row;
            event_q.idle_col = col;
            }
          else
            event_q.idle_click = FALSE;
          KEYB_push( button->cmd);
          return;
          }
        }
      np = LL_GET_PREV( np);
    }
  }

/** TESTCODE **/
#include <string.h>
#include <ctype.h>
#include "keys.h"
#include "mth.h"
#include "mnu.h"
short MOUSE_id_count = 0;

void MNU_begin( MNU *mnu)
  {
  assert( mnu);  assert( &(mnu->first) );  assert( &(mnu->last) );
  LL_begin( &(mnu->first), &(mnu->last) );
  mnu->node = &(mnu->first);   mnu->nrow = mnu->ncol = 0;
  MOUSE_button_new( ++MOUSE_id_count, NULL, 0, 0, W_maxrow( NULL), W_maxcol( NULL), ESC, TRUE);
  }

void MNU_end( MNU *mnu)
  {
  assert( mnu);  assert( &(mnu->first) );  assert( &(mnu->last) );
  LL_end( &(mnu->first) );
  }

BOOL MNU_new( MNU *mnu, BYTE *text, short id)
  {
  struct cmnu *new;
  assert( mnu);  assert( &(mnu->first) );  assert( &(mnu->last) );
  if ( (new = (struct cmnu *)calloc( sizeof( struct cmnu), 1) ) == NULL)
    return (FAIL);
  new->text = text;  new->id = id;
  return ( (mnu->node = LL_ins( mnu->node, new) ) != NULL);
  }


short MNU_popup( MNU *mnu, short row, short col, short *c)
  {
  short i, j, k, idx, nrow, ncol, chr, textlen, ret;
  BOOL goon;
  W w, w1;
  struct ll *np, *np1;
  struct cmnu *m;
  if (mnu->nrow == 0)  /* first time */
    {
    for (ncol=nrow=0, np = &(mnu->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
      {
      m = np->data;
      ncol = MAX( ncol, strlen( m->text) );   nrow++;
      }
    np = &(mnu->first);
    mnu->node = LL_GET_NEXT( np);
    nrow += 4;   ncol += 4;
    mnu->nrow = nrow;   mnu->ncol = ncol;
    }
  else
    {
    nrow = mnu->nrow;    ncol = mnu->ncol;
    }
  textlen = ncol;
  row = MIN( row, (W_maxrow( NULL) - nrow) );
  col = MIN( col, (W_maxcol( NULL) - ncol) );
  W_init( &w, row, col, nrow, ncol);
  if ( !W_save( &w) )
    return (-1);  /** Out of ram **/
  W_clear( &w, color_choise);
  W_box( &w, 0, 0, nrow, ncol, W_SINGLE_LINE_ALL_SIDES);
  W_init( &w1, (row+2), (col+2), (nrow-4), (ncol-4) );
  for (i=0, np = &(mnu->first); ( (np = LL_GET_NEXT( np) ) != NULL); i++)
    {
    if (np == mnu->node)
      idx = i;
    m = np->data;
    W_puts( &w1, i, 0, textlen, m->text);
    MOUSE_button_new( MOUSE_id_count, &w1, (w1.row+i), (w1.col), (w1.row), (w1.col+textlen), m->id, FALSE);
    }
  W_cursor_off();
  for (goon = TRUE, np = mnu->node, i = idx;  goon; )
    {
    m = np->data;
    W_putsa( &w1, i, 0, textlen, m->text, color_cursor);
@    chr = W_get();
    switch (chr)
      {
      case ARROW_RIGHT:
      case ARROW_LEFT:
      case ARROW_START:
      case ARROW_END:
      case ESC:
        goon = ret = FALSE;   /* No selection made */
        break;
      case ARROW_UP:
        if ( (np1 = LL_GET_PREV( np) ) != NULL)
          {
          np = np1;
          if (i>0)
            {
            W_putsa( &w1, i, 0, textlen, m->text, color_choise);
            i--;
            }
          }
        break;
      case ARROW_DOWN:
        if ( (np1 = LL_GET_NEXT( np) ) != NULL)
          {
          np = np1;
          if (i < (nrow-4) )
            {
            W_putsa( &w1, i, 0, textlen, m->text, color_choise);
            i++;
            }
          }
        break;
      default:
        k = i;
        for (i=0, np1 = &(mnu->first);  ( (np1 = LL_GET_NEXT( np1) ) != NULL); i++)
          {
          m = np1->data;
          for (j=0; m->text[j]; j++)
            if (toupper( chr) == m->text[j])
              {
              ret = m->id;   np = np1;   goon = FALSE;
              goto stoploop;
              }
          }
        i = k;
        stoploop:
        break;
      case '\r':
      case '\n':
        m = np->data;  ret = m->id;  goon = FALSE;
        break;
      }
    }
  mnu->node = np;   *c = chr;
  W_restore( &w);   W_cursor_on();
  return (ret);
  }


#include <stdio.h>

void main( void)
  {
  short i, c;
  MNU mnu;
  W_begin();
  COLOR_begin();
  MNU_begin( &mnu);
  if ( !MNU_new( &mnu, " Insert ", 1) ) return;
  if ( !MNU_new( &mnu, " Delete ", 2) ) return;
  if ( !MNU_new( &mnu, " eXit   ", 3) ) return;
  i = MNU_popup( &mnu, 10, 10, &c);
  printf( "\n\n\nID = %d, C=%d\n\n\n", i, c);
  getch();
  i = MNU_popup( &mnu, 10, 10, &c);
  printf( "\n\n\nID = %d, C=%d\n\n\n", i, c);
  getch();
  MNU_end( &mnu);
  COLOR_end();
  W_end();
  }
/**/
