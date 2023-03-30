#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "std.h"
#include "keys.h"
#include "keyb.h"
#include "mth.h"
#include "w.h"
#include "ll.h"
#include "mnu.h"


void RADIO_begin( RADIO *radio)
  {
  assert( radio);  assert( &(radio->first) );  assert( &(radio->last) );
  LL_begin( &(radio->first), &(radio->last) );
  radio->node = &(radio->first);
  }


void RADIO_end( RADIO *radio)
  {
  struct ll *np;
  struct cradio *m;
  assert( radio);  assert( &(radio->first) );  assert( &(radio->last) );
  LL_end( &(radio->first) );
  }

BOOL RADIO_new( RADIO *radio, BYTE *text, short id, short row, short col)
  {
  struct ll *np;
  struct cradio *new;
  assert( radio);  assert( &(radio->first) );  assert( &(radio->last) );
  if ( (new = (struct cradio *)calloc( sizeof( struct cradio), 1) ) == NULL)
    return (FAIL);
  if ( (radio->node = LL_ins( radio->node, new) ) == NULL)
    return (FAIL);
  new->text = text;  new->id = id;   new->row = row;    new->col=col;
  return (OK);
  }


void RADIO_display( RADIO *radio)
  {
  struct ll *np;
  struct cradio *m;
  W_clear( &(radio->w), color_entry);
  for (np = &(radio->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    W_putsa( &(radio->w), m->, m->col, radio->len, m->text, color_choise);
    }
  }


void RADIO_mount( RADIO *radio, short row, short col, short nrow, short ncol)
  {
  assert( radio);
  W_init( &(radio->w), radio->row, radio->col, radio->nrow, radio->ncol);
  RADIO_display( radio);
  np = &(radio->first);
  radio->node = LL_GET_NEXT( np);
  }


short RADIO_buttons( RADIO *radio)
  {
  short chr, ret, j;
  BOOL goon;
  struct ll *np, *np1;
  struct cradio *m;
  assert( radio);
  W_cursor_off();
  for (goon = TRUE, np = radio->node;  goon; )
    {
    m = np->data;
    if (W_focus( &(radio->w), 0, m->col, strlen( m->text) ) )
      RADIO_display( radio);
    W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_cursor);
    chr = W_get();
    switch (chr)
      {
      case ARROW_START:
        np = &(radio->first);
        if ( (np1 = LL_GET_NEXT( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_choise);
          }
        break;
      case ARROW_END:
        np = &(radio->last);
        if ( (np1 = LL_GET_PREV( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_choise);
          }
        break;
      case ESC:
        goon = ret = FALSE;   /* No selection made */
        break;
      case ARROW_LEFT:
      case ARROW_UP:
        if ( (np1 = LL_GET_PREV( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_choise);
          }
        break;
      case ARROW_RIGHT:
      case ARROW_DOWN:
        if ( (np1 = LL_GET_NEXT( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_choise);
          }
        break;
      default:
        W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_choise);
        for (np1 = &(radio->first);  ( (np1 = LL_GET_NEXT( np1) ) != NULL); )
          {
          m = np1->data;
          for (j=0; m->text[j]; j++)
            if (toupper( chr) == m->text[j])
              {
              np = np1;
              if (W_focus( &(radio->w), 0, m->col, radio->len ) )
                RADIO_display( radio);
              W_putsa( &(radio->w), 0, m->col, radio->len, m->text, color_cursor);
              ret = MNU_popup( m->mnu, (radio->row + 1), (radio->col + m->col + radio->w.coljust), &chr);
              if (ret)
                goon = FALSE;
              else if (chr != ESC)
                {
                KEYB_push( chr);
                KEYB_push( CR);
                }
              goto stoploop;
              }
          }
        stoploop:
        break;
      case '\r':
      case '\n':
        m = np->data;
        ret = MNU_popup( m->mnu, (radio->row + 1), (radio->col + m->col + radio->w.coljust), &chr);
        if (ret)
          goon = FALSE;
        else if (chr != ESC)
          {
          KEYB_push( chr);
          KEYB_push( CR);
          }
        break;
      }
    }
  radio->node = np;   *c = chr;
  W_cursor_on();
  return (ret);
  }


/** TESTCODE **

#include <stdio.h>

void main( void)
  {
  short i, c;
  RADIO radio;
  W_begin();
  COLOR_begin();
  RADIO_begin( &radio);
  if ( !RADIO_new( &radio, " File   ", 1, 0) ) return;
  if ( !RADIO_new( &radio, " Search ",   2,  0) ) return;
  if ( !RADIO_new( &radio, " Edit   ",   3,  0) ) return;
  if ( !RADIO_new( &radio, " eXit   ",   4,  0) ) return;
  if ( !RADIO_new( &radio, " Insert ",   11, 1) ) return;
  if ( !RADIO_new( &radio, " Delete ",   12, 1) ) return;
  if ( !RADIO_new( &radio, " Update ",   13, 1) ) return;
  if ( !RADIO_new( &radio, " First  ",   21, 2) ) return;
  if ( !RADIO_new( &radio, " Next   ",   22, 2) ) return;
  if ( !RADIO_new( &radio, " Prev   ",   23, 2) ) return;
  if ( !RADIO_new( &radio, " Last   ",   23, 2) ) return;
  if ( !RADIO_new( &radio, " Upshift  ", 31, 3) ) return;
  if ( !RADIO_new( &radio, " Downshift", 32, 3) ) return;
  if ( !RADIO_new( &radio, " Quit ",     43, 4) ) return;
  RADIO_mount( &radio, 10, 10, 40);
  i = RADIO_pulldown( &radio);
  printf( "\n\n\nID = %d\n\n\n", i, c);
  getch();
  RADIO_mount( &radio, 12, 12, 25);
  i = RADIO_pulldown( &radio);
  printf( "\n\n\nID = %d\n\n\n", i, c);
  getch();
  RADIO_end( &radio);
  COLOR_end();
  W_end();
  }
**/
