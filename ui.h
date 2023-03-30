#ifndef UI_H
#define UI_H

/** INCLUDES **/
#include <std.h>
#include <w.h>
#include <q.h>
#include <mnu.h>
#include <acksel.h>

/** DEFINES **/
#define UI_BUTTON_LEN 6
#define UI_CMD_LEAP 100

enum ui_ids {
  UI_MID  = 1,
  UI_FILE,
  UI_SEARCH,
  UI_EDIT,
  UI_HELP,
  UI_EXIT
  };

enum ui_events {
  UI_CMD =  0x1000,
  UI_CMD_PAINT,   /* Processed partly by UI_dispatcher() */
  UI_CMD_UPSHIFT,
  UI_CMD_DOWNSHIFT,
  UI_CMD_CLEAR_LINE,
  UI_CMD_CLEAR_TO_EOL,
  UI_CMD_HELP,    /* Not processed by UI_dispatcher() */
  UI_CMD_OPT,     /* Not processed by UI_dispatcher() */
  UI_CMD_QUIT,    /* Not processed by UI_dispatcher() */
  UI_CMD_KEYS,
  UI_CMD_MNU,
  UI_CMD_SCROLL_TOP,
  UI_CMD_SCROLL_PAGE_DOWN,
  UI_CMD_SCROLL_PAGE_UP,
  UI_CMD_SCROLL_PAGE_RIGHT,
  UI_CMD_SCROLL_PAGE_LEFT,
  UI_CMD_SCROLL_RIGHT,
  UI_CMD_SCROLL_LEFT,
  UI_CMD_SCROLL_UP,
  UI_CMD_SCROLL_DOWN,
  UI_CMD_MOUSE    /* Processed partly by UI_dispatcher() */
  };

/** TYPES **/
struct ui {
  short row, col, wcnrow, wcncol;
  W w, w_client, w_status, w_button;
  BYTE *title;
  MNUBAR bar;
  MNUBUTTON button;
  ACKSEL acksel;
  Q event_q;
  short button_len, button_n;
  BOOL is_winit;
  BOOL new;
  short bid;   /* privat to UI_event */
  short color;
  };
typedef struct ui UI;
typedef short EVENT;

/** PROTOTYPES **/
void UI_status_printf( UI *ui, char *fmt, ...);
BOOL UI_mnubar_new( UI *ui, BYTE *text, short id, short to_id);
short UI_mnubar_pulldown( UI *ui);
BOOL UI_begin( UI *ui, BYTE *title, short row, short col, short wcnrow, short wcncol, short button_len, short color);
void UI_end( UI *ui);
void UI_paint( UI *ui);
BOOL UI_button_new( UI *ui, BYTE *text, short id);
void UI_button_hilite_on( UI *ui, short id);
void UI_button_hilite_off( UI *ui, short id);
BOOL UI_button_is_pressed( UI *ui, short chr);
BOOL UI_acksel_new( UI *ui, short id, short key);
  /*
  Adds new ackselerator key
  */
BOOL PASCAL UI_dispatch( UI *ui, EVENT cmd);
EVENT PASCAL UI_event( UI *ui, EVENT cmd);   /* use caution */
void PASCAL UI_mail( UI *ui, EVENT mail);
void UI_draw_printf( UI *ui, short row, short col, char *fmt, ...);
void UI_draw_box( UI *ui, short row, short col, short nrow, short ncol);
void UI_draw_hline( UI *ui, short row, short col, short ncol);
void UI_draw_vline( UI *ui, short row, short col, short nrow);

#endif


