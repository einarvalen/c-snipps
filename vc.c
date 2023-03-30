#include <vc.h>

BYTE *vc_dev[] = { "Disk", "Prn", "Lpt1", "lPt2", "lpT3" };

BOOL VC_default_mnubar_maint( UI *ui)
  {
  if ( !UI_mnubar_new( ui, tx[tx_File], UI_FILE, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Search], UI_SEARCH, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Edit], UI_EDIT, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Help], UI_HELP, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_eXit], UI_EXIT, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Insert], UI_CMD_INSERT, UI_FILE) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Delete], UI_CMD_DELETE, UI_FILE) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Update], UI_CMD_UPDATE, UI_FILE) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_text], UI_CMD_TEXT, UI_FILE) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_First], UI_CMD_GET_FIRST, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Next], UI_CMD_GET_NEXT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Prev], UI_CMD_GET_PREV, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Last], UI_CMD_GET_LAST, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_eQ], UI_CMD_GET_EQ, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Gt], UI_CMD_GET_GT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_gE], UI_CMD_GET_GE, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_lt], UI_CMD_GET_LT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_le], UI_CMD_GET_LE, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_set_Idx], UI_CMD_SET_IDX, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Matchpattern], UI_CMD_SET_PATTERN, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Upshift], UI_CMD_UPSHIFT, UI_EDIT) )  return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Downshift], UI_CMD_DOWNSHIFT, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Clear_line], UI_CMD_CLEAR_LINE, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_clear_to_eol], UI_CMD_CLEAR_TO_EOL, UI_EDIT) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_options], UI_CMD_OPTIONS, UI_HELP) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_doc], UI_CMD_HELP, UI_HELP) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_quit], UI_CMD_QUIT, UI_EXIT) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_mnubar_popup( UI *ui)
  {
  if ( !UI_mnubar_new( ui, tx[tx_Search], UI_SEARCH, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Help], UI_HELP, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_eXit], UI_EXIT, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_First], UI_CMD_GET_FIRST, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Next], UI_CMD_PAGE_DOWN, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Prev], UI_CMD_PAGE_UP, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_eQ], UI_CMD_GET_EQ, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Gt], UI_CMD_GET_GT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_gE], UI_CMD_GET_GE, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_lt], UI_CMD_GET_LT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_le], UI_CMD_GET_LE, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_set_Idx], UI_CMD_SET_IDX, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Matchpattern], UI_CMD_SET_PATTERN, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_quit], UI_CMD_QUIT, UI_EXIT) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_doc], UI_CMD_HELP, UI_HELP) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_mnubar_report( UI *ui)
  {
  if ( !UI_mnubar_new( ui, tx[tx_File], UI_FILE, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Search], UI_SEARCH, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Help], UI_HELP, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_eXit], UI_EXIT, 0) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_go], UI_CMD_GO, UI_FILE) ) return (FAIL);
/*
  if ( !UI_mnubar_new( ui, tx[tx_load], UI_CMD_LOAD_REPORT, UI_FILE) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_save], UI_CMD_SAVE_REPORT, UI_FILE) ) return (FAIL);
*/
  if ( !UI_mnubar_new( ui, tx[tx_view], UI_CMD_VIEW, UI_FILE) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_First], UI_CMD_GET_FIRST, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_eQ], UI_CMD_GET_EQ, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Gt], UI_CMD_GET_GT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_gE], UI_CMD_GET_GE, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_lt], UI_CMD_GET_LT, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_le], UI_CMD_GET_LE, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_select], UI_CMD_SELECT_FIELDS, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_set_Idx], UI_CMD_SET_IDX, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_Matchpattern], UI_CMD_SET_PATTERN, UI_SEARCH) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_quit], UI_CMD_QUIT, UI_EXIT) ) return (FAIL);
  if ( !UI_mnubar_new( ui, tx[tx_doc], UI_CMD_HELP, UI_HELP) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_button_maint( UI *ui)
  {
  if ( !UI_button_new( ui, tx[tx_neW], UI_CMD_NEW) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Nxt], UI_CMD_GET_NEXT) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Prv], UI_CMD_GET_PREV) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Opt], UI_CMD_OPTIONS) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Del], UI_CMD_DELETE) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Ceol], UI_CMD_CLEAR_TO_EOL) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_cLn], UI_CMD_CLEAR_LINE) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Upper], UI_CMD_UPSHIFT) ) return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Lower], UI_CMD_DOWNSHIFT) ) return (FAIL);
  if ( !UI_button_new( ui, tx[tx_quit], UI_CMD_QUIT) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_button_pupup( UI *ui)
  {
  if ( !UI_button_new( ui, tx[tx_neW], UI_CMD_NEW) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Nxt], UI_CMD_PAGE_DOWN) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Prv], UI_CMD_PAGE_UP) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_top], UI_CMD_TOP) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_bot], UI_CMD_BOT) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_quit], UI_CMD_QUIT) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_button_report( UI *ui)
  {
  if ( !UI_button_new( ui, tx[tx_go], UI_CMD_GO) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_Opt], UI_CMD_OPTIONS) )  return (FAIL);
  if ( !UI_button_new( ui, tx[tx_quit], UI_CMD_QUIT) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_acksel_popup( UI *ui)
  {
  if ( !UI_acksel_new( ui, UI_CMD_PAGE_UP, PAGE_UP) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_PAGE_DOWN, PAGE_DN) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_HELP, K_F1) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_QUIT, K_F10) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_acksel_maint( UI *ui)
  {
  if ( !UI_acksel_new( ui, UI_CMD_GET_PREV, PAGE_UP) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_GET_NEXT, PAGE_DN) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_HELP, K_F1) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_QUIT, K_F10) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_OPTIONS, K_F4) ) return (FAIL);
  return (OK);
  }

BOOL VC_default_acksel_report( UI *ui)
  {
  if ( !UI_acksel_new( ui, UI_CMD_HELP, K_F1) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_QUIT, K_F10) ) return (FAIL);
  if ( !UI_acksel_new( ui, UI_CMD_OPTIONS, K_F4) ) return (FAIL);
  return (OK);
  }


BOOL VC_file_get( CISAM *cisam, short fn)
  {
  short len, fntn, id, inull;
  BOOL status;
  BYTE s[BUFSIZ], name[BUFSIZ], *sp, *snull;
  MNU mnu;
  struct ll *np;
  struct ccisam_idx *dp;
  status = OK;
  switch (fn)
    {
    case  UI_CMD_SET_IDX:
      MNU_begin( &mnu);
      for (np = &(cisam->idx.first); ( (np = LL_GET_NEXT( np) ) != NULL); )
        {
        dp = np->data;
        sp = CISAM_field_get_name( cisam, dp->id, &inull);
        MNU_new( &mnu, sp, (dp->id+1) );
        }
      if ( (id = MNU_popup( &mnu, W_CENTER, W_CENTER, &inull) ) != 0)
        CISAM_setidx( cisam, (id-1) );
      MNU_end( &mnu);
      break;
    case  UI_CMD_GET_NEXT:  status = CISAM_next(  cisam );  break;
    case  UI_CMD_GET_PREV:  status = CISAM_prev(  cisam );  break;
    case  UI_CMD_GET_FIRST: status = CISAM_first( cisam );  break;
    case  UI_CMD_GET_LAST:  status = CISAM_last(  cisam );  break;
    case  UI_CMD_GET_EQ:
    case  UI_CMD_GET_GT:
    case  UI_CMD_GET_GE:
    case  UI_CMD_GET_LT:
    case  UI_CMD_GET_LE:
      sp = CISAM_field_get_name( cisam, cisam->current_idx, &len);
      s[0] = '\0';
      MSG_prompt( sp, s, len);
      if (strlen( s) == 0)
        return (OK);
      switch (fn)
        {
        case  UI_CMD_GET_EQ:
          status = CISAM_eq( cisam, cisam->current_idx, s);  break;
        case  UI_CMD_GET_GT:
          status = CISAM_gt( cisam, cisam->current_idx, s);  break;
        case  UI_CMD_GET_GE:
          status = CISAM_ge( cisam, cisam->current_idx, s);  break;
        case  UI_CMD_GET_LE:
          status = CISAM_le( cisam, cisam->current_idx, s);  break;
        case  UI_CMD_GET_LT:
          status = CISAM_lt( cisam, cisam->current_idx, s);  break;
        default : break;
        }
      break;
    default : break;
    }
  return (status);
  }

void VC_tx_modify( BYTE **txt, short dim, BYTE *filename)
  {
  short i;
  FILE *fp;
  if (ED_array( txt, tx[tx_system_txt], W_CENTER, W_CENTER, 10, 10, dim) )
    if ( (fp = fopen( filename, "w") ) == NULL)
      MSG_err( tx[tx_open_err], filename);
    else
      {
      for (i=0; i<dim; i++)
        fprintf( fp, "%s\n", txt[i]);
      fclose( fp);
      }
  }

void VC_tx_load( BYTE **txt, short dim, BYTE *filename)
  {
  short i, len;
  FILE *fp;
  BYTE s[BUFSIZ];
  if ( (fp = fopen( filename, "r") ) == NULL)
    return;
  for (i=0; i<dim; i++)
    {
    fgets( s, (BUFSIZ), fp);
    len = strlen( s) - 1;
    STR_trunc( s);
    STR_pad( s, i);
    strncpy( txt[i], s, strlen( txt[i]) );
    }
  fclose( fp);
  }

BOOL VC_get_fields( BYTE **field, CISAM *cisam, short max_field)
  {
  short i;
  BOOL status;
  for (status=OK, i=0; i<max_field; i++)
    status &= (NULL != CISAM_field_get( cisam, i, field[i]) );
  if ( status == FAIL)
      MSG_msg( tx[tx_buffer_err]);
  return (status);
  }

BOOL VC_put_fields( BYTE **field, CISAM *cisam, short max_field)
  {
  short i;
  BOOL status;
  for (status=OK, i=0; i<max_field; i++)
    status &= CISAM_field_put( cisam, i, field[i]);
  if ( status == FAIL)
    MSG_msg( tx[tx_buffer_err]);
  return (status);
  }

void VC_default_cmd_report( short cmd, short *fid, short *fn, BYTE *filename, BYTE *dev)
  {
  switch (cmd)
    {
  case  UI_CMD_NL:
      KEYB_push( ARROW_START); /* Force repaint */
    case  UI_CMD_DOWN:
      if (*fid < (UI_FID+3) )
        (*fid)++;
      break;
    case  UI_CMD_UP:
      if (*fid > UI_FID)
        (*fid)--;
      break;
    case  UI_CMD_OPTIONS:
      if (*fid == (UI_FID+1) )
        {
        short i, id, inull;
        MNU mnu;
        MNU_begin( &mnu);
        for (i=0; i<5; i++)
          MNU_new( &mnu, vc_dev[i], (i+1) );
        if ( (id = MNU_popup( &mnu, W_CENTER, W_CENTER, &inull) ) != 0)
          strcpy( dev, vc_dev[id-1]);
        MNU_end( &mnu);
        }
      break;
    case  UI_CMD_HELP:
      ED_stream( "REPORT.HLP", W_CENTER, W_CENTER, 10, 40, color_info);
      break;
    case  UI_CMD_VIEW:
      ED_stream( filename, W_CENTER, W_CENTER, 10, 40, color_info);
      break;
    case  UI_CMD_GOTO:
      break;
    case  UI_CMD_GET_FIRST:
    case  UI_CMD_GET_EQ:
    case  UI_CMD_GET_LT:
    case  UI_CMD_GET_LE:
    case  UI_CMD_GET_GT:
    case  UI_CMD_GET_GE:
      *fn = cmd;
      break;
    }
  }

BOOL VC_check_date( BYTE *s)
  {
  DATE d;
  if ( !DATE_fromstr( &d, s, VC_DMY) )
    return (FAIL);
  DATE_tostr( &d, s, VC_SEP, VC_DMY);
  return (OK);
  }

void VC_option_amount( void)
  {
  MSG_msg( " [ -999999999.99 , 999999999.99 ] ");
  }

void VC_option_date( void)
  {
  BYTE *s;
  switch (VC_DMY)
    {
    case  DMY: s = "DMY"; break;
    case  YMD: s = "YMD"; break;
    case  MDY: s = "MDY"; break;
    }
  MSG_msg( tx[tx_date_format], s);
  }

