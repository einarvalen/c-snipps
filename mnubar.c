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


void MNUBAR_begin( MNUBAR *bar)
  {
  assert( bar);  assert( &(bar->first) );  assert( &(bar->last) );
  LL_begin( &(bar->first), &(bar->last) );
  bar->node = &(bar->first);   bar->row = bar->col = 0;   bar->count = 1;
  }


void MNUBAR_end( MNUBAR *bar)
  {
  struct ll *np;
  struct cbar *m;
  assert( bar);  assert( &(bar->first) );  assert( &(bar->last) );
  for (np = &(bar->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    MNU_end( m->mnu);
    free( m->mnu);
    }
  LL_end( &(bar->first) );
  }

BOOL MNUBAR_new( MNUBAR *bar, BYTE *text, short id, short link_to_id)
  {
  struct ll *np;
  struct cbar *new;
  assert( bar);  assert( &(bar->first) );  assert( &(bar->last) );
  if (link_to_id == 0)  /* It is a bar entry */
    {
    if ( (new = (struct cbar *)calloc( sizeof( struct cbar), 1) ) == NULL)
      return (FAIL);
    if ( (bar->node = LL_ins( bar->node, new) ) == NULL)
      return (FAIL);
    new->text = text;  new->id = id;
    if ( (new->mnu = (MNU *)calloc( sizeof( MNU), 1) ) == NULL)
      return (FAIL);
    MNU_begin( new->mnu);
    new->col = bar->count;
    bar->count += strlen( text) + 3;
    }
  else  /* It is a curtain entry */
    {
    for (np = &(bar->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
      {
      new = np->data;
      if (link_to_id == new->id)
        break;
      }
    return (MNU_new( new->mnu, text, id) );
    }
  return (OK);
  }


void MNUBAR_display( MNUBAR *bar)
  {
  struct ll *np;
  struct cbar *m;
  W_clear( &(bar->w), color_choise);
  for (np = &(bar->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    W_puts( &(bar->w), 0, m->col, strlen( m->text), m->text);
    }
  }


void MNUBAR_mount( MNUBAR *bar, short row, short col, short len)
  {
  struct ll *np;
  assert( bar);
  bar->row = row;   bar->col = col;
  W_init( &(bar->w), bar->row, bar->col, 1, len);
  np = &(bar->first);
  bar->node = LL_GET_NEXT( np);
  MNUBAR_display( bar);
  }


short MNUBAR_pulldown( MNUBAR *bar)
  {
  short chr, ret, j;
  BOOL goon;
  struct ll *np, *np1;
  struct cbar *m;
  assert( bar);
  W_cursor_off();
  for (goon = TRUE, np = bar->node;  goon; )
    {
    m = np->data;
    if (W_focus( &(bar->w), 0, m->col, strlen( m->text) ) )
      MNUBAR_display( bar);
    W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_cursor);
    chr = W_get();
    switch (chr)
      {
      case ARROW_START:
        np = &(bar->first);
        if ( (np1 = LL_GET_NEXT( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_choise);
          }
        break;
      case ARROW_END:
        np = &(bar->last);
        if ( (np1 = LL_GET_PREV( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_choise);
          }
        break;
      case ESC:
        goon = ret = FALSE;   /* No selection made */
        break;
      case ARROW_LEFT:
        if ( (np1 = LL_GET_PREV( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_choise);
          }
        break;
      case ARROW_RIGHT:
        if ( (np1 = LL_GET_NEXT( np) ) != NULL)
          {
          np = np1;
          W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_choise);
          }
        break;
      default:
        W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_choise);
        for (np1 = &(bar->first);  ( (np1 = LL_GET_NEXT( np1) ) != NULL); )
          {
          m = np1->data;
          for (j=0; m->text[j]; j++)
            if (toupper( chr) == m->text[j])
              {
              np = np1;
              if (W_focus( &(bar->w), 0, m->col, strlen( m->text) ) )
                MNUBAR_display( bar);
              W_putsa( &(bar->w), 0, m->col, strlen( m->text), m->text, color_cursor);
              ret = MNU_popup( m->mnu, (bar->row + 1), (bar->col + m->col + bar->w.coljust), &chr);
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
      case  ARROW_DOWN:
      case '\r':
      case '\n':
        m = np->data;
        ret = MNU_popup( m->mnu, (bar->row + 1), (bar->col + m->col + bar->w.coljust), &chr);
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
  bar->node = np;
  MNUBAR_display( bar);
  W_cursor_on();
  return (ret);
  }


/** TESTCODE **

#include <stdio.h>
  MNUBAR bar;

void main( void)
  {
  short i, c;
  W_begin();
  COLOR_begin();
  MNUBAR_begin( &bar);
  if ( !MNUBAR_new( &bar, " File   ", 1, 0) ) return;
  if ( !MNUBAR_new( &bar, " Search ",   2,  0) ) return;
  if ( !MNUBAR_new( &bar, " Edit   ",   3,  0) ) return;
  if ( !MNUBAR_new( &bar, " eXit   ",   4,  0) ) return;
  if ( !MNUBAR_new( &bar, " Insert ",   11, 1) ) return;
  if ( !MNUBAR_new( &bar, " Delete ",   12, 1) ) return;
  if ( !MNUBAR_new( &bar, " Update ",   13, 1) ) return;
  if ( !MNUBAR_new( &bar, " First  ",   21, 2) ) return;
  if ( !MNUBAR_new( &bar, " Next   ",   22, 2) ) return;
  if ( !MNUBAR_new( &bar, " Prev   ",   23, 2) ) return;
  if ( !MNUBAR_new( &bar, " Last   ",   23, 2) ) return;
  if ( !MNUBAR_new( &bar, " Upshift  ", 31, 3) ) return;
  if ( !MNUBAR_new( &bar, " Downshift", 32, 3) ) return;
  if ( !MNUBAR_new( &bar, " Quit ",     43, 4) ) return;
  MNUBAR_mount( &bar, 10, 10, 40);
  i = MNUBAR_pulldown( &bar);
  printf( "\n\n\nID = %d\n\n\n", i, c);
  getch();
  MNUBAR_mount( &bar, 12, 12, 25);
  i = MNUBAR_pulldown( &bar);
  printf( "\n\n\nID = %d\n\n\n", i, c);
  getch();
  MNUBAR_end( &bar);
  COLOR_end();
  W_end();
  }
**/
