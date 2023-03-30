#include <ans.h>

BOOL ANS_begin( ANS *ans)
  {
  short len, dim, i, j;
  BOOL status;
  dim = sizeof( ANS_tx) / sizeof( ANS_tx[0]);
  VC_tx_load( ANS_tx, VC_DIM( ANS_tx), ANS_TX_FILE);
  ans->fp[0] = ans->f1;   ans->len[0] = ANS_LEN_F1;
  ans->fp[1] = ans->f2;   ans->len[1] = ANS_LEN_F2;
  ans->fp[2] = ans->f3;   ans->len[2] = ANS_LEN_F3;
  CISAM_begin( &(ans->cisam), "ANS");
  for (i=j=0, status=TRUE; i<ANS_max_field; i++)
    {
    short fieldtype, keytype, ofs, id;
    keytype = CISAM_KEYTYPE_NOKEY;
    fieldtype = CISAM_FIELDTYPE_C;
    ofs = j; id = i;
    switch (i)
      {
      default :  break;
      case  ANS_f1:
        keytype = CISAM_KEYTYPE_UNIQUE;
        break;
      }
    status &= CISAM_field_new( &(ans->cisam), id,
        ANS_tx[i], fieldtype, keytype, ofs, ans->len[i]);
    j += ans->len[i];
    }
  if (status == OK)
    {
    if ( (status = CISAM_open( &(ans->cisam) ) ) == FAIL)
      MSG_msg( CISAM_error( &(ans->cisam) ) );
    }
  return (status);
  }

void ANS_end( ANS *ans)
  {
  CISAM_close( &(ans->cisam) );
  CISAM_end( &(ans->cisam) );
  }

BOOL ANS_ctrl( ANS *ans, UI *ui, short fid, BYTE *oldfield)
  {
  short status;
  BYTE *sp;
  status = OK;
  if (strcmp( oldfield, UI_item_search( ui, fid) ) != 0 ) /* is altered */
    {
    switch (fid)
      {
      default : status = OK; break;
      }
    if (status == FAIL)
      MSG_warn( tx[tx_field_err] );
    }
  return (status);
  }

BOOL ANS_get( ANS *ans, short fn)
  {
  if ( !VC_file_get( &(ans->cisam), fn) )
    {
    if ( !CISAM_is_eof() )
      MSG_msg( CISAM_error( &(ans->cisam) ) );
    return (FAIL);
    }
  if (fn == UI_CMD_SET_IDX)
    return (OK);
  return (VC_get_fields( ans->fp, &(ans->cisam), ANS_max_field) );
  }

BOOL ANS_put( ANS *ans, short fn)
  {
  BOOL status;
  if ( (status = VC_put_fields( ans->fp, &(ans->cisam), ANS_max_field) ) == FAIL)
    MSG_msg( tx[tx_buffer_err]);
  else
    {
    CISAM_transaction_begin( &(ans->cisam) );
    while ( !CISAM_lock( &(ans->cisam) ) )
      if (MSG_question( tx[tx_waiting_on_lock]) )
        return (FAIL);
    switch (fn)
      {
      case  UI_CMD_INSERT:  status = CISAM_insert(  &(ans->cisam) );  break;
      case  UI_CMD_UPDATE:  status = CISAM_update(  &(ans->cisam) );  break;
      case  UI_CMD_DELETE:  status = CISAM_delete(  &(ans->cisam) );  break;
      default : return (OK);
      }
    if (status == FAIL)
      {
      MSG_msg( CISAM_error( &(ans->cisam) ) );
      CISAM_transaction_rollback( &(ans->cisam) );
      }
    else
      CISAM_transaction_commit( &(ans->cisam) );
    }
  return (status);
  }

void ANS_options( ANS *ans, short id)
  {
  switch (id)
    {
    default : break;
    }
  }

BOOL ANS_maint( ANS *ans, short row, short col)
  {
  UI ui;
  BYTE oldfield[BUFSIZ], s[BUFSIZ];
  short cmd, fid, occ, oldfid, oldocc, i, j;
  BOOL goon;
  sprintf( s, "%s  -  %s", ANS_tx[ANS_max_field], tx[tx_maint]);
  if ( !UI_begin( &ui, s, row, col, 11, 40, UI_BUTTON_LEN, color_entry) ) return (FAIL);
  if ( !VC_default_mnubar_maint( &ui) ) return (FAIL);
  if ( !VC_default_button_maint( &ui) ) return (FAIL);
  if ( !VC_default_acksel_maint( &ui) ) return (FAIL);
  i = j = 0;
  if ( !UI_ITEM_FIELD( &ui, ANS_f1, ans->f1, ANS_LEN_F1, i++, 4, ANS_tx[ANS_f1],   j++, 0) ) return (FAIL);
  if ( !UI_ITEM_FIELD( &ui, ANS_f2, ans->f2, ANS_LEN_F2, i++, 4, ANS_tx[ANS_f2],   j++, 0) ) return (FAIL);
  if ( !UI_ITEM_FIELD( &ui, ANS_f3, ans->f3, ANS_LEN_F3, i++, 4, ANS_tx[ANS_f3],   j++, 0) ) return (FAIL);
  UI_init_items( &ui);
  UI_paint( &ui);
  fid = UI_FID; ui.new = TRUE;
  strcpy( oldfield, UI_item_search( &ui, fid) );
  for (goon = TRUE, occ = 0; goon; )
    {
    oldfid = fid;  oldocc = occ;
    cmd = UI_cmd_get( &ui, &fid, &occ);
    if (ui.new)
      UI_status_printf( &ui, tx[tx_new_entry]);
    else
      UI_status_printf( &ui, tx[tx_old_entry]);
    switch (cmd)
      {
      case  UI_CMD_PAINT:
        UI_paint_client( &ui);
        break;
      case  UI_CMD_QUIT:
        goon = FALSE;
        break;
      case  UI_CMD_NEW:
        UI_init_items( &ui);
        fid = UI_FID;
        CISAM_setidx( &(ans->cisam), fid);
        ui.new = TRUE;
        UI_paint_client( &ui);
        strcpy( oldfield, UI_item_search( &ui, fid) );
        break;
      case  UI_CMD_OPTIONS:
        ANS_options( ans, fid);
        break;
      case  UI_CMD_HELP:
        ED_stream( "ANSM.HLP", 10, 4, 10, 40, color_info);
        break;
      case  UI_CMD_GOTO:
        if (ANS_ctrl( ans, &ui, oldfid, oldfield) )
          strcpy( oldfield, UI_item_search( &ui, fid) );
        else
          fid = oldfid;
        break;
      case  UI_CMD_NL:
        KEYB_push( ARROW_START); /* Force repaint */
      case  UI_CMD_DOWN:
        if (ANS_ctrl( ans, &ui, fid, oldfield) )
          {
          if ( (fid < ANS_MAX_FID) )
            fid++;
          else if (ui.new && fid == ANS_MAX_FID)
            {
            if (MSG_question( tx[tx_File]) && ANS_put( ans, UI_CMD_INSERT) )
              {
              fid = UI_FID;
              UI_init_items( &ui);
              UI_paint_client( &ui);
              UI_status_printf( &ui, tx[tx_updated]);
              }
            else
              UI_status_printf( &ui, tx[tx_not_updated]);
            }
          strcpy( oldfield, UI_item_search( &ui, fid) );
          }
        break;
      case  UI_CMD_UP:
        if (fid > UI_FID && ANS_ctrl( ans, &ui, fid, oldfield) )
          {
          fid--;
          strcpy( oldfield, UI_item_search( &ui, fid) );
          }
        break;
      case  UI_CMD_GET_PREV:
      case  UI_CMD_GET_NEXT:
      case  UI_CMD_GET_LAST:
      case  UI_CMD_GET_FIRST:
      case  UI_CMD_GET_EQ:
      case  UI_CMD_GET_GE:
      case  UI_CMD_GET_GT:
      case  UI_CMD_GET_LE:
      case  UI_CMD_GET_LT:
      case  UI_CMD_SET_IDX:
      case  UI_CMD_SET_PATTERN:
        ui.new = FALSE;
        if (ANS_get( ans, cmd) )
          {
          UI_paint_client( &ui);
          strcpy( oldfield, UI_item_search( &ui, fid) );
          }
        break;
      case  UI_CMD_INSERT:
      case  UI_CMD_DELETE:
      case  UI_CMD_UPDATE:
        if (ANS_put( ans, cmd) )
          {
          if (cmd != UI_CMD_INSERT)
            {
            fid = UI_FID;
            UI_paint_client( &ui);
            }
          UI_status_printf( &ui, tx[tx_updated]);
          }
        else
          UI_status_printf( &ui, tx[tx_not_updated]);
        break;
       case  UI_CMD_TEXT:
         VC_tx_modify( ANS_tx, VC_DIM( ANS_tx), ANS_TX_FILE);
         UI_paint( &ui);
         break;
      default :
        UI_edit_cmd( &ui, cmd);
        break;
      }
    }
  UI_end( &ui);
  return (OK);
  }

BOOL ANS_popup( ANS *ans, short row, short col)
  {
  UI ui;
  short cmd, fid, occ, i, j, k, high_water_mark, max_cluster, len, fn;
  BOOL goon, ret;
  BYTE *cluster[ANS_CLUSTER];
  BYTE head[BUFSIZ];
  BYTE s[BUFSIZ];
  for (len=i=0; i<ANS_max_field; i++)
    len += (ans->len[i] + 1);
  for (i = 0, max_cluster = ANS_CLUSTER; i < ANS_CLUSTER; i++)
    if ( (cluster[i] = (BYTE *)calloc( sizeof( BYTE), (len+1) ) ) == NULL )
      {
      max_cluster = i;
      break;
      }
  STR_set( head, ' ', len);
  for (k=i=0; i<ANS_max_field; i++)
    {
    STR_paste( head, ANS_tx[i], k, len);
    k += (ans->len[i] + 1);
    }
  sprintf( s, "%s  -  %s", tx[tx_popup], ANS_tx[ANS_max_field]);
  if ( !UI_begin( &ui, s, row, col, max_cluster, len, UI_BUTTON_LEN, color_choise) ) return (FAIL);
  if ( !VC_default_mnubar_popup( &ui) ) return (FAIL);
  if ( !VC_default_button_pupup( &ui) ) return (FAIL);
  if ( !VC_default_acksel_popup( &ui) ) return (FAIL);
  if ( !UI_ITEM_CLUSTER( &ui, (UI_FID), cluster, len, max_cluster, 2, 0, head, 1, 0) ) return (FAIL);
  fn = UI_CMD_GET_FIRST;
fetch_data:
  UI_init_items( &ui);
  for (i=0; i<max_cluster; i++)
    {
    if (!ANS_get( ans, fn) )
      break;
    fn = UI_CMD_GET_NEXT;
    STR_set( cluster[i], ' ', len);
    for (k=j=0; j<ANS_max_field; j++)
      {
      STR_paste( cluster[i], ans->fp[j], k, len);
      k += (ans->len[j] + 1);
      }
    }
  high_water_mark = i;
  UI_paint( &ui);
  for (goon = TRUE, fid = UI_FID, occ = 0; goon; )
    {
    cmd = UI_cmd_get( &ui, &fid, &occ);
    UI_status_printf( &ui, tx[tx_null]);
    switch (cmd)
      {
      case  UI_CMD_PAINT:
        UI_paint_client( &ui);
        break;
      case  UI_CMD_TOP:
        occ = 0;
        break;
      case  UI_CMD_BOT:
        occ = high_water_mark - 1;
        break;
      case  UI_CMD_PAGE_DOWN:
        occ += ui.wcnrow;
        if (occ > (high_water_mark-1) )
          occ = (high_water_mark-1);
        break;
      case  UI_CMD_DOWN:
        if (occ < (high_water_mark-1) )
          occ++;
        break;
      case  UI_CMD_PAGE_UP:
        occ -= ui.wcnrow;
        if (occ < 0)
          occ = 0;
        break;
      case  UI_CMD_UP:
        if (occ > 0)
          occ--;
        break;
      case  UI_CMD_HELP:
        ED_stream( "POPUP.HLP", 10, 4, 10, 40, color_info);
        break;
      case  UI_CMD_QUIT:
        ret = goon = FALSE;
        break;
      case  UI_CMD_GOTO:
      case  UI_CMD_NL:
        strncpy( ans->f1, cluster[occ], ANS_LEN_F1);
        ans->f1[ANS_LEN_F1] = '\0';
        STR_trunc( ans->f1);
        if ( (ret = CISAM_eq( &(ans->cisam), ANS_f1, ans->f1) ) == OK)
          ret = VC_get_fields( ans->fp, &(ans->cisam), ANS_max_field);
        goon = FALSE;
        break;
      case  UI_CMD_NEW:
        ANS_maint( ans, (row+2), (col+2) );
        fn = UI_CMD_GET_FIRST;
        goto fetch_data;
      case  UI_CMD_SET_PATTERN:
        break;
      case  UI_CMD_SET_IDX:
        ANS_get( ans, cmd);
        fn = UI_CMD_GET_FIRST;
        goto fetch_data;
        break;
      case  UI_CMD_GET_FIRST:
      case  UI_CMD_GET_EQ:
      case  UI_CMD_GET_GE:
      case  UI_CMD_GET_GT:
      case  UI_CMD_GET_LE:
      case  UI_CMD_GET_LT:
        fn = cmd;
        goto fetch_data;
        break;
      default :
        break;
      }
    }
  for (i = 0; i < ANS_CLUSTER; i++)
    free( cluster[i]);
  UI_end( &ui);
  return (ret);
  }

BOOL ANS_report( ANS *ans, short row, short col)
  {
  UI ui;
  REP rep;
  FILE *fh;
  short cmd, fid, occ, i, k, len, ret, fn, field_check[ANS_max_field];
  short page_len=72, page_wid=80, page_count, line_count;
  BOOL goon;
  BYTE trash[20],s[BUFSIZ];
  extern BYTE *vc_dev[];
  BYTE filename[FILENAME_LEN], dev[10], s_page_len[4], s_page_wid[4], s_dev[4];
  time_t t;
  struct tm *tm_p;
  for (len=k=i=0; i<ANS_max_field; i++)
    field_check[i] = ON;
  strcpy( filename, "ans.rep");  strcpy( dev, vc_dev[0]);
  strcpy( s_page_len, "47");       strcpy( s_page_wid, "80");
  sprintf( s, "%s  -  %s", ANS_tx[ANS_max_field], tx[tx_report]);
  if ( !UI_begin( &ui, s, row, col, 8, 30, UI_BUTTON_LEN, color_entry) ) return (FAIL);
  if ( !UI_ITEM_FIELD( &ui, UI_FID,     filename,   31, 2, 15, tx[tx_filename], 2, 2) ) return (FAIL);
  if ( !UI_ITEM_FIELD( &ui, (UI_FID+1), dev,         4, 3, 15, tx[tx_dev], 3, 2) ) return (FAIL);
  if ( !UI_ITEM_FIELD( &ui, (UI_FID+2), s_page_len,  3, 4, 15, tx[tx_page_len], 4, 2) ) return (FAIL);
  if ( !UI_ITEM_FIELD( &ui, (UI_FID+3), s_page_wid,  3, 5, 15, tx[tx_page_wid], 5, 2) ) return (FAIL);
  if ( !VC_default_mnubar_report( &ui) ) return (FAIL);
  if ( !VC_default_button_report( &ui) ) return (FAIL);
  if ( !VC_default_acksel_report( &ui) ) return (FAIL);
  UI_paint( &ui);
  for (goon = TRUE, fid = UI_FID, occ = 0, fn = UI_CMD_GET_FIRST; goon; )
    {
    cmd = UI_cmd_get( &ui, &fid, &occ);
    UI_status_printf( &ui, tx[tx_null]);
    switch (cmd)
      {
      case  UI_CMD_QUIT:
        goon = FALSE;
        break;
      case  UI_CMD_PAINT:
        UI_paint_client( &ui);
        break;
      case  UI_CMD_SET_IDX:
        ANS_get( ans, cmd);
        fn = UI_CMD_GET_FIRST;
        break;
      case  UI_CMD_SELECT_FIELDS:
        break;
      case  UI_CMD_GO:
        UI_status_printf( &ui, tx[tx_processing]);
        page_len = atoi( s_page_len);   page_wid = atoi( s_page_wid);
        if (strcmp( dev, vc_dev[0]) == 0)  /* Disc */
          REP_begin( &rep, filename, page_len, page_wid, 5, 3);
        else
          REP_begin( &rep, dev, page_len, page_wid, 5, 3);
        REP_printf( &rep, REP_HEAD, 1, (page_wid/2 - strlen(s)/2), s);
        t = time( &t);    tm_p = localtime( &t);
        REP_printf( &rep, REP_HEAD, 1, 1, "135.26989.55", tm_p->tm_mday, (tm_p->tm_mon+1), (1900+tm_p->tm_year) );
        page_count = 1;
        REP_printf( &rep, REP_HEAD, 1, (page_wid-10), "%s %d", tx[tx_page], page_count);
        REP_hline( &rep, REP_HEAD, 2, 1, (page_wid-1), '-');
        REP_hline( &rep, REP_HEAD, 4, 1, (page_wid-1), '-');
        REP_hline( &rep, REP_TAIL, 1, 1, (page_wid-1), '-');
        for (i=0, col=1; i<ANS_max_field; i++)
          if (field_check[i])
            {
            REP_printf( &rep, REP_HEAD, 3, col, ANS_tx[i]);
            col += (ans->len[i] + 1);
            }
        for (row=line_count=0; ANS_get( ans, fn); )
          {
          line_count++;
          fn = UI_CMD_GET_NEXT;
          for (i=0,col=1; i<ANS_max_field; i++)
            if (field_check[i])
              {
              REP_printf( &rep, REP_REPORT, row, col, ans->fp[i]);
              col += (ans->len[i] + 1);
              }
          if (row < rep.report_len)
            row++;
          else
            {
            page_count++;
            REP_printf( &rep, REP_HEAD, 1, (page_wid-10), "%s %d", tx[tx_page], page_count);
            row = 0;
            REP_flush( &rep);
            }
          }
        fn = UI_CMD_GET_FIRST;
        REP_flush( &rep);
        REP_end( &rep);
        UI_status_printf( &ui, "%s %d", tx[tx_finished], tx[tx_pages], page_count, tx[tx_lines], line_count);
        break;
      default :
        VC_default_cmd_report( cmd, &fid, &fn, filename, dev);
        break;
      }
    }
  UI_end( &ui);
  return (ret);
  }

/** TESTCODE **/
#include <apl.h>
main( )
  {
  short inull;
  BOOL goon;
  MNU mnu;
  ANS ans;
  short i, c;
  APL apl;
  APL_begin( &apl, " TEST  -  ANS ");
  ANS_begin( &ans);
  if ( !APL_mnu_new( &apl, " ANS ", 1, 0) ) return (FAIL);
  if ( !APL_mnu_new( &apl, " Maintenance ", 11, 1) ) return (FAIL);
  if ( !APL_mnu_new( &apl, " Popup ", 12, 1) ) return (FAIL);
  if ( !APL_mnu_new( &apl, " Report", 13, 1) ) return (FAIL);
  if (!APL_mnu_default( &apl) )
    MSG_err( "Error 2");
  APL_paint( &apl);
  MSG_msg( "    Programmed by Einar Valen.  Copyright (c) 1991    ");
  while ( (i = APL_mnu_pulldown( &apl) ) != APL_CMD_QUIT)
    {
    switch (i)
      {
      default :
        APL_cmd_default( &apl, i); break;
      case  11:
        ANS_maint( &ans, 10, 10); break;
      case  12:
        if (ANS_popup( &ans, 10, 10) )
          MSG_msg( "Selection made: %s !", ans.f1);
        else
          MSG_msg( "No selection made !");
        break;
      case  13:
        ANS_report( &ans, 10, 10); break;
      }
    }
  ANS_end( &ans);
  APL_end( &apl);
  }
/**/
