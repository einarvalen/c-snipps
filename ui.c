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
  Q_push( &(ui->event_q), mail);
  }

BOOL PASCAL UI_begin( UI *ui, BYTE *title, short row, short col, short wcnrow, short wcncol, short button_len, short color)
  {
  assert( ui);
  ui->title = title;                 ui->is_winit = FALSE;
  ui->row = row;                     ui->col = col;
  ui->button_len = button_len;       ui->button_n = 0;
  ui->wcnrow = wcnrow;               ui->wcncol = wcncol;
  MNUBUTTON_begin( &(ui->button), ui->button_len);
  MNUBAR_begin( &(ui->bar) );
  ACKSEL_begin( &(ui->acksel) );
  ui->color = color;
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
    W_clear( &(ui->w), ui->color);
    W_clear( &(ui->w_client), ui->color);
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

EVENT PASCAL UI_event( UI *ui, EVENT cmd)
  {
  EVENT ret = 0;
  if (ui->bid)
    UI_button_hilite_off( ui, ui->bid);
  ui->bid = ret = (EVENT)0;
  UI_status_printf( ui, "\0");
  if ( (ret = (EVENT)Q_pop( &(ui->event_q) ) ) )
    return (ret);
  if ( (ret = UI_mouse( ui) ) )
    return (ret);
  if (cmd)
    {
    if ( (ret = ACKSEL_is_pressed( &(ui->acksel), cmd) ) )
      return (ret);
    if ( (ret = ui->bid = UI_button_is_pressed( ui, cmd) ) )
      UI_button_hilite_on( ui, ui->bid);
    }
  return (ret);
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
    case  W_REFRESH:
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

/** TESTCODE **
short x = 0;
short y = 0;

void paintit( UI *ui, short i)
  {
  UI_paint( ui);
  UI_draw_printf( ui, 5, 5, "Count: %d", i);
  UI_draw_printf( ui, 6, 5, "x=%d, y=%d", x, y);
  UI_draw_hline( ui, 0, 0, 80);
  UI_draw_vline( ui, 0, 0, 80);
  UI_draw_box( ui, 4, 4, 4, 15);
  UI_draw_box( ui, 4, 4, 5, 17);
  UI_draw_box( ui, 4, 4, 6, 19);
  UI_draw_box( ui, 4, 4, 40, 80);
  }

int testit( void)
  {
  UI ui;
  short i;
  EVENT cmd;
  BOOL goon;
  x += 2; y += 2;
  UI_begin( &ui, "Testbilde", x, y, 15, 40, 6, color_entry);
  if ( !UI_button_new( &ui, tx[tx_Ceol ], UI_CMD_CLEAR_TO_EOL) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_cLn  ], UI_CMD_CLEAR_LINE) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Upper], UI_CMD_UPSHIFT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Lower], UI_CMD_DOWNSHIFT) ) return (FAIL);
  for (goon = TRUE; goon; )
    {
    if ( !(cmd = UI_event( &ui, cmd) ) )
      cmd = W_get();
    switch (cmd)
      {
      default:
        if (UI_dispatch( &ui, cmd) )
          break;
        i++;
      case  UI_CMD_PAINT:
        paintit( &ui, i);
        break;
      case  UI_CMD_HELP:
        UI_status_printf( &ui, "UI_CMD_HELP");
        testit();
        break;
      case  UI_CMD_OPT:
        UI_status_printf( &ui, "UI_CMD_HELP");
        testit();
        break;
      case  UI_CMD_QUIT:
        UI_status_printf( &ui, "UI_CMD_QUIT");
        goon = FALSE;
        break;
      }
    }
  UI_status_printf( &ui, "Saving changes");
  MSG_msg("   Quitting  ");
  UI_end( &ui);
  }

main( void)
  {
  W_begin();
  COLOR_begin();
  testit();
  COLOR_end();
  W_end();
  }
**/

