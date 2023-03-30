
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "w.h"
#include "msg.h"
#include "keys.h"
#include "tx.h"
#include "ed.h"
#include "apl.h"

void APL_paint( APL *apl)
  {
  short nrow, ncol, i;
  BYTE s[BUFSIZ];
  W w, w_client;
  W_init( &w, 0, 0, W_maxrow( NULL), W_maxcol( NULL) );
  W_init( &w_client, (w.row+3), (w.col + 1), (w.nrow-4), (w.ncol-2) );
  W_clear( &w, color_background);
  W_clear( &w_client, color_background);
  W_box( &w, w.row, w.col, w.nrow, w.ncol, W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w, 1, ( (w.ncol / 2) - (strlen( apl->title) / 2) - 1), strlen( apl->title), apl->title);
  MNUBAR_mount( &(apl->bar), (w.row + 2), (w.col + 1), (w.ncol - 2) );
  STR_set( s, '±', w_client.ncol);
  for (i=0; i<w_client.nrow; i++)
    W_puts( &w_client, i, 0, w_client.ncol, s);
  }

BOOL APL_mnu_new( APL *apl, BYTE *text, short mnu_id, short link_to_bar_id)
  {
  return (MNUBAR_new( &(apl->bar), text, mnu_id, link_to_bar_id) );
  }

short APL_mnu_pulldown( APL *apl)
  {
  return (MNUBAR_pulldown( &(apl->bar) ) );
  }

BOOL APL_mnu_default( APL *apl)
  {
  if ( !APL_mnu_new( apl, tx[tx_options],1002, 0) ) return (FAIL);
  if ( !APL_mnu_new( apl, tx[tx_eXit],   1004, 0) ) return (FAIL);
  if ( !APL_mnu_new( apl, tx[tx_colors], 10021, 1002) ) return (FAIL);
  if ( !APL_mnu_new( apl, tx[tx_text],   10022, 1002) ) return (FAIL);
  if ( !APL_mnu_new( apl, tx[tx_Help],   10023, 1002) ) return (FAIL);
  if ( !APL_mnu_new( apl, tx[tx_quit],   APL_CMD_QUIT, 1004) ) return (FAIL);
  return (OK);
  }

void APL_cmd_default( APL *apl, short cmd)
  {
  switch (cmd)
    {
    default : break;
    case  10021: COLOR_modify(); APL_paint( apl); break;
    case  10022: TX_modify(); APL_paint( apl); break;
    case  10023: ED_stream( "SYSTEM.HLP", 5, 0, 12, 40, color_info); break;
    }
  }

void APL_begin( APL *apl, BYTE *title)
  {
  W_begin();
  COLOR_begin();
  TX_load();
  apl->title = title;
  MNUBAR_begin( &(apl->bar) );
  }

void APL_end( APL *apl)
  {
  W w;
  MNUBAR_end( &(apl->bar) );
  W_init( &w, 0, 0, W_maxrow( NULL), W_maxcol( NULL) );
  W_clear( &w, 0x07);
  COLOR_end();
  W_end();
  }

/** TESTCODE **

int main( )
  {
  short i, c;
  APL apl;
  APL_begin( &apl, " S Y S T E M E T ");
  if (!APL_mnu_default( &apl) )
    MSG_err( "Error 2");
  APL_paint( &apl);
  MSG_msg( "    Programmed by Einar Valen.  Copyright (c) 1991    ");
  while ( (i = APL_mnu_pulldown( &apl) ) != APL_CMD_QUIT)
    {
    switch (i)
      {
      default : APL_cmd_default( &apl, i); break;
      }
    }
  APL_end( &apl);
  }

**/
