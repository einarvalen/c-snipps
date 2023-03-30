#ifndef VC_H
#define VC_H

/** INCLUDE FILES **/
#include <stdlib.h>
#include <time.h>
#include <std.h>
#include <ui.h>
#include <tx.h>
#include <field.h>
#include <cisam.h>
#include <keys.h>
#include <mnu.h>
#include <str.h>
#include <rep.h>
#include <ed.h>
#include <date.h>

/** TEMPLATES **/
#define VC_LOOKUP_ON( modull, modulu, viaidx, usingfield) \
  { modulu modull; \
  if ( (status = ##modulu##_begin( &##modull) ) == FAIL) break; \
  if (strlen( usingfield) == 0)  status = ##modulu##_popup( &##modull); \
  else  { strcpy( viaidx, usingfield); status = ##modulu##_fetch( &##modull); } \
  if (status == OK)  { strcpy( usingfield, viaidx); UI_paint_client( ui); } \
  ##modulu##_end( &##modull); }
#define VC_IS_UNIQUE( modull, viaidx, usingfield) \
  if (CISAM_eq( &(##modull##->cisam), viaidx, usingfield) ) { ui->new = FALSE; \
  status = VC_get_fields( ##modull##->fp, &(##modull##->cisam), ##modull##_max_field); \
  UI_paint_client( ui); UI_status_printf( ui, tx[tx_existing_record] ); }
#define VC_FIELD_IS_ALTERED( modul, id, oldvalue) \
  (strcmp( oldvalue, ##modul->fp[id]) != 0 \
  || strlen( ##modul->fp[id]) == 0)

/** MACROS **/
#define VC_DIM( array) (sizeof( array) / sizeof( array[0]) )
#define VC_DMY DMY
#define VC_SEP "."
#define FILENAME_LEN 32

/** PROTOTYPES **/
BOOL VC_default_mnubar_maint( UI *ui);
BOOL VC_default_mnubar_popup( UI *ui);
BOOL VC_default_mnubar_report( UI *ui);
BOOL VC_default_button_maint( UI *ui);
BOOL VC_default_button_pupup( UI *ui);
BOOL VC_default_button_report( UI *ui);
BOOL VC_default_button_report( UI *ui);
BOOL VC_default_acksel_maint( UI *ui);
BOOL VC_default_acksel_popup( UI *ui);
BOOL VC_default_acksel_report( UI *ui);
BOOL VC_file_get( CISAM *cisam, short fn);
void VC_tx_load( BYTE **txt, short dim, BYTE *filename);
void VC_tx_modify( BYTE **txt, short dim, BYTE *filename);
BOOL VC_get_fields( BYTE **field, CISAM *cisam, short max_field);
BOOL VC_put_fields( BYTE **field, CISAM *cisam, short max_field);
void VC_default_cmd_report( short cmd, short *fid, short *fn, BYTE *filename, BYTE *dev);
void VC_option_amount( void);
BOOL VC_check_date( BYTE *s);
void VC_option_date( void);

#endif
