#include <stdlib.h>
#include <stdio.h>
#include <std.h>
#include <ui.h>
#include <tx.h>
#include <keys.h>
#include <str.h>

#define ED_CLUSTER 100
#define ED_LINE_LEN 133

BOOL ED_stream( BYTE *stream, short row, short col, short nrow, short ncol, short color)
  {
  UI ui;
  short cmd, fid, occ, i, j, k, max_cluster, len;
  BOOL goon, ret;
  BYTE *cluster[ED_CLUSTER+2];
  FILE *fp;
  if ( (fp = fopen( stream, "r") ) == NULL )
    {
    MSG_err( "Cannot fopen() file %s", stream);
    return (FAIL);
    }
  for (i=0; i<=ED_CLUSTER; i++)
    if ( (cluster[i] = (BYTE *)calloc( sizeof( BYTE), (ED_LINE_LEN+1) ) ) == NULL)
      {
      MSG_warn( " Out of ram!");
      return (FAIL);
      }
  for (i=0; (fgets( cluster[i], ED_LINE_LEN, fp) != NULL) && i<ED_CLUSTER; i++)
    {
    for (j=0; cluster[i][j]; j++)
      if (cluster[i][j] == '\n')
        cluster[i][j] = ' ';
    }
  max_cluster = i;
  if ( !UI_begin( &ui, stream, row, col, 15, 40, UI_BUTTON_LEN, color) )
    return (FAIL);
/*
  if ( !UI_mnubar_new( &ui, tx[tx_File], UI_FILE, 0) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Search], UI_SEARCH, 0) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Edit], UI_EDIT, 0) ) return (FAIL);
  */
  if ( !UI_mnubar_new( &ui, tx[tx_eXit], UI_EXIT, 0) ) return (FAIL);
  /*
  if ( !UI_mnubar_new( &ui, tx[tx_save], UI_CMD_SAVE_REPORT, UI_FILE) ) return (FAIL);
    if ( !UI_mnubar_new( &ui, tx[tx_Matchpattern], UI_CMD_SET_PATTERN, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Insert], UI_CMD_NEW, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Delete], UI_CMD_DELETE, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Upshift], UI_CMD_UPSHIFT, UI_EDIT) )  return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Downshift], UI_CMD_DOWNSHIFT, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Clear_line], UI_CMD_CLEAR_LINE, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_clear_to_eol], UI_CMD_CLEAR_TO_EOL, UI_EDIT) ) return (FAIL);
*/
  if ( !UI_mnubar_new( &ui, tx[tx_quit], UI_CMD_QUIT, UI_EXIT) ) return (FAIL);
/*
  if ( !UI_button_new( &ui, tx[tx_quit],  UI_CMD_QUIT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_neW  ], UI_CMD_NEW) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Del  ], UI_CMD_DELETE) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Ceol ], UI_CMD_CLEAR_TO_EOL) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_cLn  ], UI_CMD_CLEAR_LINE) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Upper], UI_CMD_UPSHIFT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Lower], UI_CMD_DOWNSHIFT) ) return (FAIL);
  */
  if ( !UI_button_new( &ui, tx[tx_Nxt ],  UI_CMD_GET_NEXT) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Prv ],  UI_CMD_GET_PREV) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_top ],  UI_CMD_TOP) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_bot ],  UI_CMD_BOT) )  return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_PAGE_UP, PAGE_UP) ) return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_PAGE_DOWN, PAGE_DN) ) return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_HELP, K_F1) ) return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_QUIT, K_F10) ) return (FAIL);
  if ( !UI_ITEM_CLUSTER( &ui, (UI_FID), cluster, ED_LINE_LEN, max_cluster, 0, 0, "\0", 0, 0) ) return (FAIL);
  UI_paint( &ui);
  for (goon = TRUE, fid = UI_FID, occ = 0; goon; )
    {
    UI_status_printf( &ui, " %s: %d    ( %d )", tx[tx_lines], max_cluster, (occ+1) );
    cmd = UI_cmd_get( &ui, &fid, &occ);
    switch (cmd)
      {
      case  UI_CMD_PAINT:
        UI_paint_client( &ui);
        break;
      case  UI_CMD_QUIT:
        ret = TRUE;  goon = FALSE;
        break;
      case  UI_CMD_GOTO:
        break;
      case  UI_CMD_HELP:
      case  UI_CMD_DELETE:
      case  UI_CMD_NEW:
        break;
      case  UI_CMD_PAGE_UP:
      case  UI_CMD_GET_PREV:
        occ -= ui.wcnrow;
        if (occ < 0)
          occ = 0;
        break;
      case  UI_CMD_TOP:
        occ = 0;
        break;
      case  UI_CMD_PAGE_DOWN:
      case  UI_CMD_GET_NEXT:
        occ += ui.wcnrow;
        if (occ > (max_cluster-1) )
          occ = (max_cluster-1);
        break;
      case  UI_CMD_BOT:
        occ = max_cluster - 1;
        break;
      case  UI_CMD_SAVE_REPORT:
      case  UI_CMD_NL:
        KEYB_push( ARROW_START); /* Force repaint */
      case  UI_CMD_DOWN:
        if (occ < (max_cluster-1) )
          occ++;
        break;
      case  UI_CMD_UP:
        if (occ > 0)
          occ--;
        break;
      default :
/*        UI_edit_cmd( &ui, cmd);*/
        break;
      }
    }
  UI_end( &ui);
  for (i=0; i<=ED_CLUSTER; i++)
    free( cluster[i]);
  return (ret);
  }

BOOL ED_array( BYTE **array, BYTE *title, short row, short col, short nrow, short ncol, short dim)
  {
  UI ui;
  short cmd, fid, occ, i, j, k, max_cluster, len;
  BOOL goon, ret;
  if ( !UI_begin( &ui, title, row, col, dim, 40, UI_BUTTON_LEN, color_entry) )
    return (FAIL);
/*
  if ( !UI_mnubar_new( &ui, tx[tx_Search], UI_SEARCH, 0) ) return (FAIL);
  */
  if ( !UI_mnubar_new( &ui, tx[tx_Edit], UI_EDIT, 0) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_eXit], UI_EXIT, 0) ) return (FAIL);
  /*
  if ( !UI_mnubar_new( &ui, tx[tx_Matchpattern], UI_CMD_SET_PATTERN, UI_SEARCH) ) return (FAIL);
*/
  if ( !UI_mnubar_new( &ui, tx[tx_Upshift], UI_CMD_UPSHIFT, UI_EDIT) )  return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Downshift], UI_CMD_DOWNSHIFT, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_Clear_line], UI_CMD_CLEAR_LINE, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_clear_to_eol], UI_CMD_CLEAR_TO_EOL, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_ok], UI_CMD_OK, UI_EXIT) ) return (FAIL);
  if ( !UI_mnubar_new( &ui, tx[tx_cancel], UI_CMD_QUIT, UI_EXIT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_ok],  UI_CMD_OK) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_cancel],  UI_CMD_QUIT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Ceol ], UI_CMD_CLEAR_TO_EOL) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_cLn  ], UI_CMD_CLEAR_LINE) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Upper], UI_CMD_UPSHIFT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Lower], UI_CMD_DOWNSHIFT) ) return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Nxt ],  UI_CMD_GET_NEXT) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_Prv ],  UI_CMD_GET_PREV) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_top ],  UI_CMD_TOP) )  return (FAIL);
  if ( !UI_button_new( &ui, tx[tx_bot ],  UI_CMD_BOT) )  return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_PAGE_UP, PAGE_UP) ) return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_PAGE_DOWN, PAGE_DN) ) return (FAIL);
  if ( !UI_acksel_new( &ui, UI_CMD_QUIT, K_F10) ) return (FAIL);
  for (i=0; i<dim; i++)
    if ( !UI_ITEM_FIELD( &ui, (UI_FID+i), array[i], strlen(array[i]), i, 0, "\0", 0, 0) ) return (FAIL);
  UI_paint( &ui);
  for (goon = TRUE, fid = UI_FID, occ = 0; goon; )
    {
    UI_status_printf( &ui, " %s: %d    ( %d )", tx[tx_lines], dim, (fid+1) );
    cmd = UI_cmd_get( &ui, &fid, &occ);
    switch (cmd)
      {
      case  UI_CMD_PAINT:
        UI_paint_client( &ui);
        break;
      case  UI_CMD_OK:
        ret = TRUE;  goon = FALSE;
        break;
      case  UI_CMD_QUIT:
        ret = FALSE;  goon = FALSE;
        break;
      case  UI_CMD_GOTO:
        break;
      case  UI_CMD_DELETE:
      case  UI_CMD_NEW:
        break;
      case  UI_CMD_PAGE_UP:
      case  UI_CMD_GET_PREV:
        fid -= ui.wcnrow;
        if (fid < UI_FID)
          fid = UI_FID;
        break;
      case  UI_CMD_TOP:
        fid = UI_FID;
        break;
      case  UI_CMD_PAGE_DOWN:
      case  UI_CMD_GET_NEXT:
        fid += ui.wcnrow;
        if (fid > (dim-1) )
          fid = (dim-1);
        break;
      case  UI_CMD_BOT:
        fid = dim - 1;
        break;
      case  UI_CMD_NL:
        KEYB_push( ARROW_START); /* Force repaint */
      case  UI_CMD_DOWN:
        if (fid < (dim-1) )
          fid++;
        break;
      case  UI_CMD_UP:
        if (fid > UI_FID)
          fid--;
        break;
      default :
        UI_edit_cmd( &ui, cmd);
        break;
      }
    }
  UI_end( &ui);
  return (ret);
  }

/** TESTCODE **
static BYTE *a[] = {
  "Kunde",
  "Navn",
  "Adresse",
  "Einar Valen",
  "Betsy K. Valen",
  "Rasmus K. Valen",
  "Jonas K. Valen",
  "Tlf",
  "Fax",
  "Kontaktperson"
  };

int main( short ac, char *av[])
  {
  short dim;
  W_begin();
  COLOR_begin();
  ED_stream( "edstream.c", 10, 10, color_entry);
  dim = sizeof( a) / sizeof( a[0]);
  ED_array( a, "T E S T", 10, 10, dim);
  COLOR_end();
  W_end();
  }

**/
