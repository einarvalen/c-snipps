#include <sys\types.h>
#include <sys\stat.h>
#include <stdarg.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include "err.h"
#include "mth.h"
#include "w.h"
#include "tx.h"
#include "mnu.h"
#include "keys.h"
#include "msg.h"

#define PAUSE(n) { long t1,t2;  \
   for (t1=time(&t1); (!kbhit()) && (time(&t2)-t1<n); ); }
#define DURATION 2


void MSG_err( char *fmt, ...) /* Standard message display with pausing */
  {
  BYTE s[BUFSIZ];
  W w;
  short len, row, col, center;
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  ERR_log( s);
  len = MAX( (strlen(s) + 4), 14);
  center = (len / 2) - 2;
  W_init( &w, W_CENTER, W_CENTER, 6, len);
  W_save( &w);
  W_clear( &w, color_error);
  W_box( &w, 0, 0, (w.nrow), (w.ncol), W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w, 2, 2, strlen( s), s);
  W_putsa( &w, (w.nrow - 2), center, 4, tx[tx_ok], color_cursor);
  W_cursor_off();
  while (W_get() != CR) DO_NOTHING;
  W_cursor_on();
  W_restore( &w);
  }


void MSG_warn( char *fmt, ...) /* Standard message display with pausing */
  {
  BYTE s[BUFSIZ];
  W w;
  short len, row, col, center;
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  len = MAX( (strlen(s) + 4), 14);
  center = (len / 2) - 2;
  W_init( &w, W_CENTER, W_CENTER, 6, len);
  W_save( &w);
  W_clear( &w, color_info);
  W_box( &w, 0, 0, (w.nrow), (w.ncol), W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w, 2, 2, strlen( s), s);
  W_putsa( &w, (w.nrow - 2), center, 4, tx[tx_ok], color_cursor);
  W_cursor_off();
  while (W_get() != CR) DO_NOTHING;
  W_cursor_on();
  W_restore( &w);
  }

void MSG_msg( char *fmt, ...) /* Standard message display with pausing */
  {
  BYTE s[BUFSIZ];
  W w;
  short len, row, col, center;
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  len = MAX( (strlen(s) + 4), 14);
  center = (len / 2) - 3;
  W_init( &w, W_CENTER, W_CENTER, 5, len);
  W_save( &w);
  W_clear( &w, color_info);
  W_box( &w, 0, 0, (w.nrow), (w.ncol), W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w, 2, 2, strlen( s), s);
  W_cursor_off();
  PAUSE( DURATION);
  if (kbhit())
    W_get();
  W_cursor_on();
  W_restore( &w);
  }


void MSG_info( BYTE **text)       /* Standard information display */
  {
  W w;
  short row, col, nrow, ncol, i, center;
  for (nrow = 0, ncol = 14; text[nrow]; nrow++)
    ncol = MAX( ncol, strlen( text[nrow]) );
  nrow += 6; ncol += 4;
  center = (ncol / 2) - 2;
  W_init( &w, W_CENTER, W_CENTER, nrow, ncol);
  W_save( &w);
  W_clear( &w, color_info);
  W_box( &w, 0, 0, (w.nrow), (w.ncol), W_SINGLE_LINE_ALL_SIDES);
  for (i=0; text[i]; i++)
    W_puts( &w, (i+2), 2, strlen( text[i]), text[i]);
  W_putsa( &w, (w.nrow - 2), center, 4, tx[tx_ok], color_cursor);
  W_cursor_off();
  while (W_get() != CR) DO_NOTHING;
  W_cursor_on();
  W_restore( &w);
  }



void MSG_prompt( BYTE *label, BYTE *s, short maxlen) /* Standard gets display */
  {
  W w, w1;
  short len, row, col, center;
  BOOL go_on;
  len = strlen( label) + maxlen + 7;
  center = strlen( label) + 2;
  W_init( &w, W_CENTER, W_CENTER, 5, len);
  W_init( &w1, (w.row+2), (w.col+2), (w.nrow-4), (w.ncol-2) );
  W_save( &w);
  W_clear( &w, color_entry);
  W_box( &w, 0, 0, (w.nrow), (w.ncol), W_SINGLE_LINE_ALL_SIDES);
  W_printf( &w1, 0, 0, "%s: ", label);
  for (go_on = TRUE; go_on; )
    {
    W_putsa( &w1, 0, center, maxlen, s, color_cursor);
    switch ( W_gets( &w1, 0, center, s, maxlen) )
      {
      default:        break;
      case '\n':
      case '\r':      go_on = FALSE; break;
      case W_REFRESH: W_printf( &w, 0, 0, "%s: %s", label, s); break;
      }
    W_putsa( &w1, 0, center, maxlen, s, color_entry);
    }
  W_restore( &w);
  }

 BOOL MSG_question( char *fmt, ...)
  {
  BYTE s[BUFSIZ];
  W w;
  short len, row, col, chr, i;
  BOOL goon, ret;
  MNUBUTTON button;
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  len = MAX( (strlen(s) + 4), 14);
  W_init( &w, W_CENTER, W_CENTER, 7, len);
  if ( !W_save( &w) )
    return (-1);  /** Out of ram **/
  W_clear( &w, color_info);
  W_box( &w, 0, 0, w.nrow, w.ncol, W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w, 2, 2, strlen( s), s );
  len = MAX( strlen( tx[tx_yes]), strlen( tx[tx_no]) );
  col =  w.ncol / 2 - len - 1;
  MNUBUTTON_begin( &button, len);
  if ( !MNUBUTTON_new( &button, tx[tx_yes], 1, 4, col) )              return (-1);
  if ( !MNUBUTTON_new( &button, tx[tx_no],  2, 4, (col + len + 2) ) ) return (-1);
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
  return (ret ^ 1);
  }


void MSG_status_begin( W *hostwindow, W *statuswindow)
  {
  W_init( statuswindow, (hostwindow->row + hostwindow->nrow - 2), (hostwindow->col + 1), 1, (hostwindow->ncol - 2) );
  W_clear( statuswindow, color_info);
  }

void MSG_status_printf( W *statuswindow, char *fmt, ...)
  {
  short i;
  BYTE s[BUFSIZ];
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  W_clear( statuswindow, color_info);
  W_puts( statuswindow, 0, 2, statuswindow->ncol, s);
  }


/**  TESTCODE  **

  static unsigned char *text[]=
    {
    "This display is ment for help messages and information",
    "displays.",
    "  ",
    "It does not have a scrolling facility yet, but ",
    "that might be on for future enhancements.",
    "On second thougts however, perhaps it would be a",
    "better idea to let another function handle ",
    "tasks of that nature.",
    " ",
    "               Sincerely,",
    "               Einar valen",
    0};

main()
  {
  int i;
  BYTE s[11];
  W w, ws;
  W_begin();
  COLOR_begin();
  i=56;
  W_init( &w, 0, 0, 25, 80);
  W_clear( &w, color_entry);
  W_box( &w, 0, 0, 25, 80, W_SINGLE_LINE_ALL_SIDES);
  MSG_status_begin( &w, &ws);
  MSG_warn( "Warning: Record inserted");
  MSG_warn( "Warning: Records deleted %d", i);
  MSG_err( "Error: Record inserted");
  MSG_err( "Error: Records deleted %d", i);
  MSG_msg( "Message: Record inserted");
  MSG_msg( "Message: Records deleted %d", i);
  MSG_info( text);
  strcpy( s, "\0");
  MSG_prompt( "Fra Filnavn", s, 10);
  MSG_msg( "Fra Filnavn = %s", s);
  MSG_prompt( "Til Filnavn", s, 10);
  MSG_msg( "Til Filnavn = %s", s);
  if (MSG_question( "Er dette riktig einar?") )
    printf( "TRUE");
  else
    printf( "FALSE");
  MSG_status_printf( &ws, "Record %s updated", "was not");
  W_get();
  MSG_status_printf( &ws, "Error");
  W_get();
  MSG_status_end( &ws);
  W_clear( &w, color_entry);
  COLOR_end();
  W_end();
  }
**/
