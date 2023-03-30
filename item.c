#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "keys.h"
#include "keyb.h"
#include "mth.h"
#include "msg.h"
#include "str.h"
#include "tx.h"
#include "ui.h"

#define SCROLLBAR_VERTICAL   "\xb1"
#define SCROLLBAR_HORIZONTAL "\xb1"
#define   SHADOW 0xb0
#define   ARW_UP "\x1e"
#define   ARW_DN "\x1f"
#define   ARW_LT "\x11"
#define   ARW_RT "\x10"
#define UI_ERR_EXIT() exit(-1)

static enum scroll_direction { UP, DOWN, RIGHT, LEFT, TOP };

void UI_status_printf( UI *ui, char *fmt, ...)
  {
  BYTE s[BUFSIZ];
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  if ( ui->is_winit)
    MSG_status_printf( &(ui->w_status), s);
  }

BOOL PASCAL UI_button_new( UI *ui, BYTE *text, short id)
  {
  ui->button_n++;
  return (MNUBUTTON_new( &(ui->button), text, id, (ui->button_n * 2 - 1), 1) );
  }

BOOL PASCAL UI_mnubar_new( UI *ui, BYTE *text, short id, short to_id)
  {
  return (MNUBAR_new( &(ui->bar), text, id, to_id) );
  }

static short PASCAL UI_mnubar_pulldown( UI *ui)
  {
  return (MNUBAR_pulldown( &(ui->bar) ) );
  }

BOOL PASCAL UI_acksel_new( UI *ui, short id, short key)
  {
  return (ACKSEL_new( &(ui->acksel), id, key) );
  }

void PASCAL UI_mail( UI *ui, EVENT mail)
  {
  Q_push( &(ui->event_q), (void *)mail);
  }

BOOL PASCAL UI_begin( UI *ui, BYTE *title, short row, short col, short wcnrow,
    short wcncol, short button_len)
  {
  assert( ui);
  ui->title = title;                 ui->is_winit = FALSE;
  ui->row = row;                     ui->col = col;
  ui->button_len = button_len;       ui->button_n = 0;
  ui->wcnrow = wcnrow;               ui->wcncol = wcncol;
  MNUBUTTON_begin( &(ui->button), ui->button_len);
  MNUBAR_begin( &(ui->bar) );
  ACKSEL_begin( &(ui->acksel) );
  W_init( &(ui->w), ui->row, ui->col, (MAX( ui->wcnrow, ui->button_n)  + 8), (ui->wcncol + ui->button_len + 2 + 3) );
  ui->row = ui->w.row;
  ui->col = ui->w.col;
  ui->wcnrow = ui->w.nrow - 8;
  ui->wcncol = ui->w.ncol - ui->button_len - 2 - 3 - 3;
  W_init( &(ui->w_client), (ui->row + 4), (ui->col + 3), ui->wcnrow, ui->wcncol);
  ui->wcnrow = ui->w_client.nrow;
  ui->wcncol = ui->w_client.ncol;
  W_init( &(ui->w_button), ui->w_client.row, (ui->w_client.col + ui->w_client.ncol + 2), (ui->wcnrow), (ui->button_len + 2) );
  if ( !W_save( &(ui->w) ) )
    return (FAIL);
  Q_begin( &(ui->event_q) );
  if ( !UI_mnubar_new( ui, tx[tx_eXit], UI_EXIT, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_quit], UI_CMD_QUIT, UI_EXIT) ) return (FAIL);
  if ( !UI_button_new( ui, tx[tx_quit], UI_CMD_QUIT) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_PAGE_UP,    K_SHIFT_F5) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_PAGE_DOWN,  K_SHIFT_F6) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_PAGE_LEFT,  K_SHIFT_F7) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_PAGE_RIGHT, K_SHIFT_F8) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_TOP,   CTRL_HOME) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_UP,    K_CTRL_F5) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_DOWN,  K_CTRL_F6) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_LEFT,  K_CTRL_F7) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_SCROLL_RIGHT, K_CTRL_F8) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_MNU,  ESC) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_MNU,  K_F10) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_QUIT, K_F9) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_KEYS, K_CTRL_F1) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_HELP, K_SHIFT_F1) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_OPT, K_F1) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_UPSHIFT,      K_F4) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_DOWNSHIFT,    K_F5) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_CLEAR_LINE,   K_F6) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_CLEAR_TO_EOL, K_F7) ) return (FAIL);
  UI_mail( ui, UI_CMD_PAINT);
  }

void PASCAL UI_end( UI *ui)
  {
  assert( ui);
  ui->is_winit = FALSE;
  MNUBAR_end( &(ui->bar) );
  MNUBUTTON_end( &(ui->button) );
  ACKSEL_end( &(ui->acksel) );
  Q_end( &(ui->event_q) );
  MSG_status_end( &(ui->w_status) );
  W_restore( &(ui->w) );
  }

static void PASCAL UI__paint( UI *ui)
  {
  short nrow, ncol, i;
  BYTE s[BUFSIZ];
  assert( ui);
  if ( !ui->is_winit)
    {
    W_clear( &(ui->w), color_background);
    W_clear( &(ui->w_client), color_info);
    ui->is_winit = TRUE;
    }
  W_box( &(ui->w), 0, 0, ui->w.nrow, ui->w.ncol, W_SINGLE_LINE_ALL_SIDES);
  W_box( &(ui->w), 3, 2, (ui->w_client.nrow + 2), (ui->w_client.ncol + 2), W_SINGLE_LINE_ALL_SIDES);
  W_puts( &(ui->w), 1, ( (ui->w.ncol / 2) - (strlen( ui->title) / 2) ), strlen( ui->title), ui->title);
  ncol = ui->w.ncol - ui->button_len - 4;
  nrow = ui->w.nrow - 4;
  W_putsa( &(ui->w), 3,    1,    1, ARW_UP, color_choise);
  W_putsa( &(ui->w), 3,    ncol, 1, ARW_UP, color_choise);
  W_putsa( &(ui->w), nrow, 1,    1, ARW_DN, color_choise);
  W_putsa( &(ui->w), nrow, ncol, 1, ARW_DN, color_choise);
  STR_set( s, SHADOW, (ui->button_len + 2) );
  for (i = 4, nrow = ui->w.nrow - 4; i < nrow; i++)
    {
    W_puts( &(ui->w), i, 1   , 1, SCROLLBAR_VERTICAL);
    W_puts( &(ui->w), i, ncol, 1, SCROLLBAR_VERTICAL);
    W_puts( &(ui->w), i, (ncol + 1), (ui->button_len + 2), s);
    }
  W_putsa( &(ui->w), ++nrow, 2,      1, ARW_LT, color_choise);
  W_putsa( &(ui->w),   nrow, --ncol, 1, ARW_RT, color_choise);
  for (i = 3; i < ncol; i++)
    W_puts( &(ui->w), nrow, i, 1, SCROLLBAR_HORIZONTAL);
  MNUBUTTON_mount( &(ui->button), &(ui->w_button) );
  MSG_status_begin( &(ui->w), &(ui->w_status) );
  MNUBAR_mount( &(ui->bar), (ui->row + 2), (ui->col + 1), (ui->w.ncol - 2) );
  }

void PASCAL UI_paint( UI *ui)
  {
  if ( !ui->is_winit)
    UI__paint( ui);
  }

void PASCAL UI_button_hilite_on( UI *ui, short id)
  {
  if ( ui->is_winit)
    MNUBUTTON_hilite_on( &(ui->button), &(ui->w_button), id);
  }

void PASCAL UI_button_hilite_off( UI *ui, short id)
  {
  if ( ui->is_winit)
    MNUBUTTON_hilite_off( &(ui->button), &(ui->w_button), id);
  }

BOOL PASCAL UI_button_is_pressed( UI *ui, short chr)
  {
  short c;
  switch (chr)
    {  /* Alt and corresponding letter */
    case  1808: c = 'q'; break;    case  1809: c = 'w'; break;
    case  1810: c = 'e'; break;    case  1811: c = 'r'; break;
    case  1812: c = 't'; break;    case  1813: c = 'y'; break;
    case  1814: c = 'u'; break;    case  1815: c = 'i'; break;
    case  1816: c = 'o'; break;    case  1817: c = 'p'; break;
    case  1822: c = 'a'; break;    case  1823: c = 's'; break;
    case  1824: c = 'd'; break;    case  1825: c = 'f'; break;
    case  1826: c = 'g'; break;    case  1827: c = 'h'; break;
    case  1828: c = 'j'; break;    case  1829: c = 'k'; break;
    case  1830: c = 'l'; break;    case  1836: c = 'z'; break;
    case  1837: c = 'x'; break;    case  1838: c = 'c'; break;
    case  1839: c = 'v'; break;    case  1840: c = 'b'; break;
    case  1841: c = 'n'; break;    case  1842: c = 'm'; break;
    default : c = chr; break;
    }
  return (MNUBUTTON_is_pressed( &(ui->button), c) );
  }

static EVENT PASCAL UI_mouse( UI *ui)
  {
  return (0);
  }

EVENT PASCAL UI_keypress( UI *ui, short key)
  {
  EVENT ret;
  if (key)
    {
    if ( !(ret = ACKSEL_is_pressed( &(ui->acksel), key) ) )
      {
      if ( (ret = ui->bid = UI_button_is_pressed( ui, key) ) )
        UI_button_hilite_on( ui, ui->bid);
      else if (key == W_REFRESH)
        UI_mail( ui, UI_CMD_PAINT);
      }
    }
  else
    ret = UI_mouse( ui);
  if (ret)
    UI_mail( ui, ret);  /* Put event in Que */
  return (0); /* Dummy to force a reevaluation of event */
  }

EVENT PASCAL UI_event( UI *ui, EVENT cmd)
  {
  return ( (EVENT)Q_pop( &(ui->event_q) ) );
  }

static void PASCAL UI_scroll( UI *ui, short direction, short n)
  {
  if ( !ui->is_winit)
    return;
  switch (direction)
    {
    case  UP:   ui->w_client.rowjust += n; break;
    case  DOWN: ui->w_client.rowjust -= n; break;
    case  LEFT: ui->w_client.coljust += n; break;
    case  RIGHT: ui->w_client.coljust -= n; break;
    case  TOP: ui->w_client.coljust = ui->w_client.rowjust = 0; break;
    }
  if (ui->w_client.rowjust > 0)
    ui->w_client.rowjust = 0;
  if (ui->w_client.coljust > 0)
    ui->w_client.coljust = 0;
  W_scroll( &(ui->w_client) );
  }

void UI_help( void)
  {
  static BYTE *keys[] = {
    "SCROLL PAGE UP........SHIFT+F5",
    "SCROLL PAGE DOWN......SHIFT+F6",
    "SCROLL PAGE LEFT......SHIFT+F7",
    "SCROLL PAGE RIGHT.....SHIFT+F8",
    "SCROLL TOP............CTRL+HOME",
    "SCROLL UP.............CTRL+F5",
    "SCROLL DOWN...........CTRL+F6",
    "SCROLL LEFT...........CTRL+F7",
    "SCROLL RIGHT..........CTRL+F8",
    "MENU..................ESC",
    "MENU..................F10",
    "QUIT..................F9",
    "THIS DISPLAY..........CTRL+F1",
    "HELP..................SHIFT+F1",
    "OPTIONS...............F1",
    "UPSHIFT...............F4",
    "DOWNSHIFT.............F5",
    "CLEAR LINE............F6",
    "CLEAR TO END OF LINE..F7",
    "BUTTONS...............ALT+[Capital]",
    NULL
    };
  MSG_info( keys);
  }

BOOL PASCAL UI_dispatch( UI *ui, EVENT cmd)
  {
  EVENT mid;
  switch (cmd)
    {
    default :
      return (FALSE);
    case  UI_CMD_SCROLL_LEFT:       case  UI_CMD_SCROLL_RIGHT:
    case  UI_CMD_SCROLL_UP:         case  UI_CMD_SCROLL_DOWN:
    case  UI_CMD_SCROLL_PAGE_UP:    case  UI_CMD_SCROLL_PAGE_DOWN:
    case  UI_CMD_SCROLL_PAGE_LEFT:  case  UI_CMD_SCROLL_PAGE_RIGHT:
    case  UI_CMD_SCROLL_TOP:
      switch (cmd)
        {
        default : break;
        case  UI_CMD_SCROLL_TOP:
          UI_scroll( ui, TOP, 1); break;
        case  UI_CMD_SCROLL_LEFT:
          UI_scroll( ui, LEFT, 1); break;
        case  UI_CMD_SCROLL_RIGHT:
          UI_scroll( ui, RIGHT, 1); break;
        case  UI_CMD_SCROLL_UP:
          UI_scroll( ui, UP, 1); break;
        case  UI_CMD_SCROLL_DOWN:
          UI_scroll( ui, DOWN, 1); break;
        case  UI_CMD_SCROLL_PAGE_UP:
          UI_scroll( ui, UP, (ui->w_client.nrow) ); break;
        case  UI_CMD_SCROLL_PAGE_DOWN:
          UI_scroll( ui, DOWN, (ui->w_client.nrow) ); break;
        case  UI_CMD_SCROLL_PAGE_LEFT:
          UI_scroll( ui, LEFT, (ui->w_client.ncol) ); break;
        case  UI_CMD_SCROLL_PAGE_RIGHT:
          UI_scroll( ui, RIGHT, (ui->w_client.ncol) ); break;
        }
      UI_mail( ui, UI_CMD_PAINT);
      break;
    case  UI_CMD_MNU:
      if ( (mid =  UI_mnubar_pulldown( ui) ) )
        UI_mail( ui, mid);
      break;
    case  UI_CMD_UPSHIFT:
      KEYB_push( 1); break;  /* ^A */
    case  UI_CMD_DOWNSHIFT:
      KEYB_push( 2); break;  /* ^B */
    case  UI_CMD_CLEAR_LINE:
      KEYB_push( ARROW_START); KEYB_push( 17); KEYB_push( 25); break; /* HOME ^Q^Y */
    case  UI_CMD_CLEAR_TO_EOL:
      KEYB_push( 17); KEYB_push( 25); break; /* ^Q^Y */
    case UI_CMD_KEYS:
      UI_help(); break;
    }
  return (TRUE);
  }

void UI_draw_vline( UI *ui, short row, short col, short nrow)
  {
  if (ui->is_winit)
    W_vert_line( &(ui->w_client), row, col, nrow, W_SINGLE_LINE_ALL_SIDES);
  }

void UI_draw_hline( UI *ui, short row, short col, short ncol)
  {
  if (ui->is_winit)
    W_hor_line( &(ui->w_client), row, col, ncol, W_SINGLE_LINE_ALL_SIDES);
  }

void UI_draw_box( UI *ui, short row, short col, short nrow, short ncol)
  {
  if (ui->is_winit)
    W_box( &(ui->w_client), row, col, nrow, ncol, W_SINGLE_LINE_ALL_SIDES);
  }

void UI_draw_printf( UI *ui, short row, short col, char *fmt, ...)
  {
  BYTE s[BUFSIZ];
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  if (ui->is_winit)
    W_puts( &(ui->w_client), row, col, strlen( s), s);
  }
#ifndef ITEM_H
#define ITEM_H

#include "std.h"
#include "mth.h"
#include "keys.h"
#include "ui.h"

struct citem {
  short  type;  /* FIELD, CLUSTER */
  void  *data;
  short  row, col;
  short  maxlen;
  short  n;
  BOOL   mnu;
  BYTE  *label;
  short  lrow, lcol;
  };

struct item {
  UI *ui;
  struct citem *form;
  struct { short id; short occ; } current;
  struct { short id; short occ; } prev;
  struct { short id; short occ; } hw_mark;
  struct { short id; short occ; } end;  short n;
  }
typedef struct item ITEM;

enum item_type { FIELD = 1, CLUSTER };

enum item_event {
  ITEM_CMD_AT_END = (UI_CMD+UI_CMD_LEAP),
  ITEM_CMD_CURSOR_UP,
  ITEM_CMD_CURSOR_DOWN,
  ITEM_CMD_CURSOR_LEFT,
  ITEM_CMD_CURSOR_RIGHT,
  ITEM_CMD_ACCEPT,
  ITEM_CMD_DISMISS,
  ITEM_CMD_VALIDATE,
  };

/** PROTOTYPES **/
BOOL PASCAL ITEM_begin( ITEM *item, UI *ui, short n);
void PASCAL ITEM_end( ITEM *item);
void PASCAL ITEM_new( ITEM *item, void *data, short maxlen, short n,
    short row, short col, BYTE *label, short lrow, short lcol, BOOL mnu);
BYTE PASCAL *ITEM_fetch( ITEM *item, short id, short occ);
void PASCAL ITEM_init( ITEM *item);
void PASCAL ITEM_paint( ITEM *item);
BOOL PASCAL ITEM_dispatch( ITEM *item, EVENT cmd);
EVENT PASCAL ITEM_event( ITEM *item);
void PASCAL ITEM_goto( ITEM *item, short id, short occ);

#endif

#include <stdlib.h>
/*
#include "item.h"
*/
#define SNAPSHOT( a, b)
static void snapshot( ITEM *item, BYTE *fn)
  {
  short i;
  BYTE **sp;
  printf( "\n%s\n", fn);
  printf( "  0=%s;\n", item->form[0].data);
  printf( "  1=%s;\n", item->form[1].data);
  sp = item->form[2].data;
  for (i=0; i<15; i++)
    printf( "  2:%d=%s;\n", i, sp[i]);
  sp = item->form[3].data;
  for (i=0; i<15; i++)
    printf( "  3:%d=%s;\n", i, sp[i]);
  }

static EVENT PASCAL ITEM_gets( ITEM *item)
  {
  short i, color, row, occ;
  EVENT ret = (EVENT)0;
  BYTE **data, *s;
  assert( item);
  SNAPSHOT( item, "ITEM_gets()");
  i = item->current.id;  occ = item->current.occ;
  color = IF (item->form[i].mnu) THEN (color_choise) ELSE (color_entry);
  if (item->ui->bid)
    UI_button_hilite_off( item->ui, item->ui->bid);
  /*UI_status_printf( item->ui, "\0");*/
  switch (item->form[i].type)
    {
    default : return (ret);
    case  FIELD:
      s = item->form[i].data;
      row = item->form[i].row;
      break;
    case  CLUSTER:
      data = item->form[i].data;
      s = data[occ];
      row = item->form[i].row + occ;
      break;
    }
  W_putsa( &(item->ui->w_client), row, item->form[i].col, item->form[i].maxlen, s, color_cursor);
  if (item->form[i].mnu)
    ret = W_get();
  else
    ret = W_gets( &(item->ui->w_client), row, item->form[i].col, s, item->form[i].maxlen);
  W_putsa( &(item->ui->w_client), row, item->form[i].col, item->form[i].maxlen, s, color);
  return (ret);
  }

static void PASCAL ITEM_mouse( ITEM *item)
  {
  assert( item);
  /* empty, as for now */
  }


static void PASCAL ITEM_accept( ITEM *item)
  {
  assert( item);
  item->prev.id = item->current.id;  item->prev.occ = item->current.occ;
  if (item->current.id > item->hw_mark.id)
    item->hw_mark.id = item->current.id;
  if (item->current.occ > item->hw_mark.occ)
    item->hw_mark.occ = item->current.occ;
  }

static void PASCAL ITEM_dismiss( ITEM *item)
  {
  assert( item);
  item->current.id = item->prev.id;
  item->current.occ = item->prev.occ;
  }

static BOOL PASCAL ITEM_next_is_cluster( ITEM *item, short id)
  {
  assert( item);
  return (id < item->end.id &&  item->form[++id].type == CLUSTER);
  }

static BOOL PASCAL ITEM_prev_is_cluster( ITEM *item, short id)
  {
  assert( item);
  return (id > 0 && item->form[--id].type == CLUSTER);
  }

BOOL PASCAL ITEM_begin( ITEM *item, UI *ui, short n)
  {
  register short i;
  assert( item);
  item->ui = ui;  item->n = n;
  if ( (item->form = (struct citem *)calloc( sizeof( struct citem), n) ) == NULL)
    return (FAIL);
  item->current.id = item->prev.id = item->hw_mark.id = item->current.occ
     = item->prev.occ = item->hw_mark.occ = item->end.occ = 0;
  item->end.id = -1;
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_UP,    ARROW_UP) )    return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_DOWN,  ARROW_DOWN) )  return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_DOWN,  '\r') )        return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_DOWN,  '\n') )        return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_LEFT,  ARROW_LEFT) )  return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_LEFT,  TAB_LEFT) )    return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_RIGHT, ARROW_RIGHT) ) return (FAIL);
  if ( !UI_acksel_new( item->ui, ITEM_CMD_CURSOR_RIGHT, TAB_RIGHT) )   return (FAIL);
  return (OK);
  }

void PASCAL ITEM_end( ITEM *item)
  {
  assert( item);
  free( item->form);
  }

void PASCAL ITEM_new( ITEM *item, void *data, short maxlen, short n, short row,
    short col, BYTE *label, short lrow, short lcol, BOOL mnu)
  {
  short i;
  assert( item);
  item->end.id++;
  i = item->end.id;
  item->form[i].type   = IF (n > 1) THEN (CLUSTER) ELSE (FIELD);
  item->form[i].data   = data;    item->form[i].label = label;
  item->form[i].maxlen = maxlen;  item->form[i].mnu   = mnu;
  item->form[i].row    = row;     item->form[i].col   = col;
  item->form[i].lrow   = lrow;    item->form[i].lcol  = lcol;
  item->form[i].n      = n;       item->end.occ       = n - 1;
  }

BYTE PASCAL *ITEM_fetch( ITEM *item, short id, short occ)
  {
  BYTE **data, *s;
  assert( item);
  id = MIN( MAX( 0, id), item->end.id);
  if (item->form[id].type == CLUSTER)
    {
    data = item->form[id].data;
    occ = MIN( MAX( 0, occ), item->end.occ);
    s = data[occ];
    }
  else
    s = item->form[id].data;
  return (s);
  }

void PASCAL ITEM_init( ITEM *item)
  {
  register short i, j;
  BYTE **data, *s;
  assert( item);
  for (i = 0; i <= item->end.id; i++)
    {
    switch (item->form[i].type)
      {
      default : return;
      case  FIELD:
        s = item->form[i].data;
        s[0] = '\0';
        break;
      case  CLUSTER:
        data = item->form[i].data;
        for (j = 0; j < item->form[i].n; j++)
          data[j][0] = '\0';
        break;
      }
    }
  item->hw_mark.id = item->hw_mark.occ = 0;
  item->current.id = item->current.occ = 0;
  item->prev.id    = item->prev.occ    = 0;
  }

void PASCAL ITEM_paint( ITEM *item)
  {
  register short i, j;
  short color;
  BYTE **data;
  assert( item);
  SNAPSHOT( item, "ITEM_paint()");
  for (i = 0; i <= item->end.id; i++)
    {
    color = IF (item->form[i].mnu) THEN (color_choise) ELSE (color_entry);
    switch (item->form[i].type)
      {
      default : return;
      case  FIELD:
        W_puts( &(item->ui->w_client), item->form[i].lrow, item->form[i].lcol,
            strlen( item->form[i].label), item->form[i].label);
        W_putsa( &(item->ui->w_client), item->form[i].row,  item->form[i].col,
            item->form[i].maxlen, item->form[i].data, color);
        break;
      case  CLUSTER:
        data = item->form[i].data;
        W_puts( &(item->ui->w_client), item->form[i].lrow, item->form[i].lcol,
            strlen( item->form[i].label), item->form[i].label);
        for (j = 0; j < item->form[i].n; j++)
          W_putsa( &(item->ui->w_client), (item->form[i].row + j),
              item->form[i].col, item->form[i].maxlen, data[j], color);
        break;
      }
    }
  }

void PASCAL ITEM_goto( ITEM *item, short id, short occ)
  {
  BOOL at_end = FALSE;
  assert( item);
  SNAPSHOT( item, "ITEM_goto()");
  UI_mail( item->ui, ITEM_CMD_VALIDATE);
  if (occ < 0)
    id--;
  else if (occ > item->end.occ)
    occ = item->end.occ, id++;
  id = MAX( 0, id);
  if (id < item->end.id && occ > 0 && item->form[id].type != CLUSTER)
     id++, occ = 0;
  if (id > item->end.id)
    at_end = TRUE,  id = item->end.id;
/*
  if (occ > item->hw_mark.occ && item->form[id].type == CLUSTER)
    {
    while (item->form[id-1].type == CLUSTER && id > 0)   id--;
    item->hw_mark.occ++;
    occ = item->hw_mark.occ;
    }
    */
  if (item->form[id].type != CLUSTER)
    occ = 0;
  item->current.id = id;  item->current.occ = occ = MAX( 0, occ);
  if (at_end)
    UI_mail( item->ui, ITEM_CMD_AT_END);
  }

BOOL PASCAL ITEM_dispatch( ITEM *item, EVENT cmd)
  {
  short id, occ;
  assert( item);
  SNAPSHOT( item, "ITEM_dispatch()");
  occ = item->current.occ;   id = item->current.id;
  switch (cmd)
    {
    case  ITEM_CMD_CURSOR_UP:    occ--;  break;
    case  ITEM_CMD_CURSOR_DOWN:  occ++;  break;
    case  ITEM_CMD_CURSOR_LEFT:  id--;   break;
    case  ITEM_CMD_CURSOR_RIGHT: id++;   break;
    case  UI_CMD_MOUSE:          ITEM_mouse( item);    break;
    case  ITEM_CMD_ACCEPT:       ITEM_accept( item);   return (TRUE);
    case  ITEM_CMD_DISMISS:      ITEM_dismiss( item);  return (TRUE);
    default :                    return (UI_dispatch( item->ui, cmd) );
    }
  ITEM_goto( item, id, occ);
  return (TRUE);
  }

EVENT PASCAL ITEM_event( ITEM *item)
  {
  EVENT cmd;
  assert( item);
  SNAPSHOT( item, "ITEM_event()");
  if ( (cmd = UI_event( item->ui, 0) ) )
    return (cmd);
  return (UI_keypress( item->ui, ITEM_gets( item) ) );
  }

/** TESTCODE **/

short main( void)
  {
  register int i;
  short id, occ;
  UI ui;
  ITEM item;
  EVENT cmd;
  BOOL goon;
  BYTE name[31], adr[31], mnu[7], *subj[16], tab1[16][31], *res[16], tab2[16][3];
  W_begin();
  COLOR_begin();
  for (i=0; i<15; i++)
    {
    subj[i] = tab1[i];
    res[i] = tab2[i];
    }
  strcpy( mnu, " Text ");
  UI_begin( &ui, "Testbilde", W_CENTER, W_CENTER, 15, 40, 6);
  if ( !ITEM_begin( &item, &ui, 5) ) return (FAIL);
  ITEM_new( &item, name, 30,  1, 0,  10, "Name   :", 0, 1,  FALSE);
  ITEM_new( &item, adr,  30,  1, 1,  10, "Adress :", 1, 1,  FALSE);
  ITEM_new( &item, subj, 30, 15, 4,   1, "Subject",  3, 1,  FALSE);
  ITEM_new( &item, res,  2,  15, 4,  32, "Result",   3, 32, FALSE);
/*
  ITEM_new( &item, mnu,   6,  1, 21, 17, "\0",       0, 0,  TRUE);
*/
  ITEM_init( &item);
  for (goon = TRUE, cmd = occ = id = 0; goon; )
    {
    if ( !(cmd = ITEM_event( &item) ) )
      continue;
    switch (cmd)
      {
      default:
        ITEM_dispatch( &item, cmd);
        break;
      case  UI_CMD_PAINT:
        UI_paint( &ui);
        ITEM_paint( &item);
        break;
      case  UI_CMD_HELP:
        UI_status_printf( item.ui, "Id=%d occ=%d", item.current.id, item.current.occ);
        break;
      case  UI_CMD_OPT:
        snapshot( &item, "SNAPSHOT()");
        break;
      case  UI_CMD_QUIT:
        goon = FALSE;
        break;
      }
    }
  ITEM_end( &item);
  UI_end( &ui);
  COLOR_end();
  W_end();

  }

/**/
