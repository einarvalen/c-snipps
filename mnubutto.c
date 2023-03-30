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

void MNUBUTTON_begin( MNUBUTTON *button, short len)
  {
  assert( button);  assert( &(button->first) );  assert( &(button->last) );
  LL_begin( &(button->first), &(button->last) );
  button->node = &(button->first);   button->len = len;
  }

void MNUBUTTON_end( MNUBUTTON *button)
  {
  assert( button);  assert( &(button->first) );  assert( &(button->last) );
  LL_end( &(button->first) );
  }

BOOL MNUBUTTON_new( MNUBUTTON *button, BYTE *text, short id, short row, short col)
  {
  struct cbutton *new;
  assert( button);  assert( &(button->first) );  assert( &(button->last) );  assert( text);
  if ( (new = (struct cbutton *)calloc( sizeof( struct cbutton), 1) ) == NULL)
    return (FAIL);
  new->text = text;  new->id = id;   new->row = row;    new->col = col;
  return ( (button->node = LL_ins( button->node, new) ) != NULL);
  }

void MNUBUTTON_mount( MNUBUTTON *button, W *w)
  {
  struct ll *np;
  struct cbutton *m;
  assert( button);  assert( &(button->first) );  assert( &(button->last) );  assert( w);
  for (np = &(button->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    W_putsa( w, m->row, m->col, button->len, m->text, color_choise);
    }
  }

short MNUBUTTON_is_pressed( MNUBUTTON *button, short chr)
  {
  short j;
  struct ll *np;
  struct cbutton *m;
  assert( button);  assert( &(button->first) );  assert( &(button->last) );
  for (np = &(button->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    for (j=0; m->text[j]; j++)
      if (toupper( chr) == m->text[j])
        return (m->id);
    }
  return (0);
  }

void MNUBUTTON_hilite( MNUBUTTON *button, W *w, short id, short attr)
  {
  struct ll *np;
  struct cbutton *m;
  assert( button);  assert( &(button->first) );  assert( &(button->last) );  assert( w);
  for (np = &(button->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    if (m->id == id)
      W_putsa( w, m->row, m->col, button->len, m->text, attr);
    }
  }

void MNUBUTTON_hilite_on(  MNUBUTTON *button, W *w, short id)  {  MNUBUTTON_hilite( button, w, id, color_cursor);  }
void MNUBUTTON_hilite_off( MNUBUTTON *button, W *w, short id)  {  MNUBUTTON_hilite( button, w, id, color_choise);  }

/** TESTCODE **
static BYTE *tx_yes = " Fine ";
static BYTE *tx_no  = " Shit ";

int main( void)
  {
  BYTE s[BUFSIZ];
  W w;
  short len, row, col, chr, i;
  BOOL goon, ret;
  MNUBUTTON button;
  W_begin();
  COLOR_begin();
  strcpy( s, " How are you going mate ? ");
  W_init( &w, 1, 1, 7, (strlen( s) + 4) );
  if ( !W_save( &w) )
    return (-1);
  W_clear( &w, color_info);
  W_box( &w, 0, 0, w.nrow, w.ncol, W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w, 2, 2, strlen( s), s );
  len = MAX( strlen( tx_yes), strlen( tx_no) );
  col = (w.col + w.ncol) / 2 - len - 1;
  MNUBUTTON_begin( &button, len);
  if ( !MNUBUTTON_new( &button, tx_yes, 1, 4, col) )              return (-1);
  if ( !MNUBUTTON_new( &button, tx_no,  2, 4, (col + len + 2) ) ) return (-1);
  MNUBUTTON_mount( &button, &w);
  W_cursor_off();
  for (i = 1, goon = TRUE; goon; )
    {
    MNUBUTTON_hilite_on( &button, &w, i);
    chr = W_get();
    MNUBUTTON_hilite_off( &button, &w, i);
    if ( (ret = MNUBUTTON_is_pressed( &button, chr) ) )
      {
      ret--;
      goon = FALSE;
      }
    else
      {
      switch (chr)
        {
        case  ARROW_LEFT:  if (i > 1)  i--;   break;
        case  ARROW_RIGHT: if (i < 2)  i++;   break;
        case '\r':
        case '\n': goon = FALSE;  ret =  --i; break;
        default: break;
        }
      }
    }
  W_cursor_on();
  W_restore( &w);
  MNUBUTTON_end( &button);
  COLOR_end();
  W_end();
  printf( "\n\n\n\ret=%d", (ret ^ 1) );
  return (ret ^ 1);
  }

**/
