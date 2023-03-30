#ifndef TX_H
#define TX_H

#include <std.h>

enum tx_idx {
  tx_null, tx_yes, tx_no, tx_ok, tx_open_err, tx_existing_record,
  tx_field_err, tx_buffer_err, tx_printing, tx_processing, tx_finished, tx_filename,
  tx_updated, tx_not_updated,  tx_File,  tx_Search,  tx_Edit,  tx_Help,
  tx_eXit,  tx_Insert,  tx_Delete,  tx_Update,  tx_First,  tx_Next,
  tx_Prev,  tx_Last,  tx_eQ,  tx_Gt,  tx_gE,  tx_lt,  tx_le,  tx_set_Idx,
  tx_Matchpattern,  tx_Upshift,  tx_Downshift,  tx_Clear_line,  tx_clear_to_eol,
  tx_options,  tx_doc,  tx_quit,  tx_neW,  tx_Nxt,  tx_Prv,  tx_Opt,  tx_Del,
  tx_Ceol,  tx_cLn,  tx_Upper,  tx_Lower, tx_bot, tx_top, tx_waiting_on_lock,
  tx_load, tx_save, tx_go, tx_select, tx_dev, tx_page_len, tx_page_wid,
  tx_page, tx_pages, tx_lines, tx_colors, tx_text, tx_system_txt, tx_view,
  tx_new_entry, tx_old_entry, tx_cancel, tx_maint, tx_report, tx_popup,
  tx_date_format
  };

extern BYTE *tx[];

/* PROTOTYPES */
void TX_modify( void);
void TX_save( void);
void TX_load( void);

#endif
