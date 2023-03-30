#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "std.h"
#include "keys.h"
#include "mth.h"
#include "w.h"
#include "ll.h"
#include "mnu.h"

void MNU_begin( MNU *mnu)
  {
  assert( mnu);  assert( &(mnu->first) );  assert( &(mnu->last) );
  LL_begin( &(mnu->first), &(mnu->last) );
  mnu->node = &(mnu->first);   mnu->nrow = mnu->ncol = 0;
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
    }
  W_cursor_off();
  for (goon = TRUE, np = mnu->node, i = idx;  goon; )
    {
    m = np->data;
    W_putsa( &w1, i, 0, textlen, m->text, color_cursor);
    chr = W_get();
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


/** TESTCODE **

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
**/

