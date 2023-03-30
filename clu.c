#include <stdlib.h>
#include "std.h"
#include "ui.h"

/** TYPES **/
struct clu {
  UI    *ui;
  short  status,
  short  fid,  current_fid,  old_fid,  max_fid,  init_fid;
  short  occ,  current_occ,  old_occ,  max_occ;
  BYTE   old_field[BUFSIZ];
  BYTE   ***data;
  short  idx
  };
typedef struct clu CLU;

/** PROTOTYPES **/
BOOL CLU_begin( CLU *clu, UI *ui, row, col, max_fields, max_lines);
BOOL CLU_new( CLU *clu, BYTE *label, short maxlen);
void CLU_end( CLU *clu);
short CLU_get( CLU *clu, BYTE **data);
BOOL CLU_dispatch( CLU *clu, short cmd);

static enum stat { new, old, changed };

BOOL CLU_begin( CLU *clu, UI *ui, row, col, max_fields, max_lines)
  {
  BOOL ret;
  clu->ui = ui;
  clu->status = new;
  clu->fid = clu->current_fid = clu->old_fid = clu->init_fid = clu->next_fid
      = start_fid;
  clu->occ = clu->current_occ = clu->old_occ = 0;
  clu->max_fid = max_fields;  clu->max_occ = max_lines;
  clu->row = row + 1;    clu->col = clu->start_col = col;
  clu->old_field[0] = '\0';
  return ( (clu->data = (BYTE ***)calloc( sizeof( BYTE **), clu->max_fid) ) != NULL);
  }

BOOL CLU_new( CLU *clu, BYTE *label, short maxlen)
  {
  register short i;
  short len;
  BYTE **ptr, *dptr;
  if (clu->next_fid - clu->init_fid >= clu->max_fid)
    return (FAIL);
  if ( (ptr = (BYTE **)calloc( sizeof( BYTE *), clu->max_occ) ) == NULL)
    return (FAIL);
  clu->data[clu->idx] = ptr;
  clu->idx++;
  len = clu->max_occ * (maxlen + 1);
  if ( (dptr = (BYTE *)calloc( sizeof( BYTE), len) ) == NULL)
    return (FAIL);
  for (i = 0; i < clu->max_occ; i++)
    ptr[i] = &dptr[i * (maxlen + 1)];
  row=5; col=0, anb_fid = 0;
  if ( !UI_ITEM_CLUSTER( clu->ui, clu->next_fid, clu->data, maxlen,
      clu->max_occ, clu->row, clu->col, label, (clu->row-1), clu->col) )
    return (FAIL);
  clu->next_fid++;
  clu->col += (maxlen + 1);
  return (OK);
  }
  
void CLU_end( CLU *clu)
  {
  register short i;
  BYTE *bp;
  for (i = 0; i < clu->max_occ; i++)
    {
    bp = &(clu->data[i][0]);
    free( bp);
    free( clu->data[i]);
    }
  free( clu->data[i]);
  }

short CLU_get( CLU *clu, BYTE **data)
  {
  register short i;
  short n;
  for (i = 0, n = clu->max_fid - clu->init_fid; i < n; i++)
    strcpy( data[i], UI_item_search( clu->ui, (clu->init_fid + i), clu->occ) )
  return (clu->fid);
  }

static void CLU_status( CLU *clu)    /* Set clu->status */
  {
  if (clu->status == new && clu->occ < clu->current_occ)  /* up */
    clu->status = old;
  else if (clu->occ == clu->current_occ)
    clu->status = new;
  }

static BOOL CLU_dispatch_goto( CLU *clu, short cmd)
  {
  switch (cmd)
    {
    default : return (FALSE);
    case  UI_CMD_GOTO:      break;
    case  UI_CMD_DOWN:      clu->occ++;   break;
    case  UI_CMD_NL:
    case  UI_CMD_RIGHT:     clu->fid++;   break;
    case  UI_CMD_LEFT:      clu->fid++;   break;
    case  UI_CMD_UP:        clu->occ--;   break;
    case  UI_CMD_PAGE_DOWN: clu->occ+=ui->w_client.nrow;   break;
    case  UI_CMD_PAGE_UP:   clu->occ-=ui->w_client.nrow;   break;
    }
  /* Field altered ? */
  if (strcmp( old_field, UI_item_search( ui, old_fid, old_occ) ) != 0 )
    UI_cmd_post( clu->ui, UI_CMD_VALIDATE);
  /* Update */
  if (clu->old_fid == clu->max_fid && clu->status == new)  /* Aquire new line */
    UI_post( clu->ui, UI_CMD_INSERT);
  else if (clu->status == changed && clu->occ != clu->old_occ)
    UI_post( clu->ui, UI_CMD_UPDATE);
  /* bounds check */
  if (clu->fid > clu->max_fid) /* Too far right */
    clu->occ++;
  else if (clu->fid < clu->init_fid) /* Too far left */
    clu->fid = clu->init_fid;
  if (clu->occ < 0)         /* Too far up */
    clu->occ = 0;
  if (clu->occ >= clu->max_occ)   /* Too far down. Aquire new screenload */
    {
    UI_init_items( clu->ui);
    clu->current_fid = clu->fid = clu->init_fid;    clu->current_occ = clu->occ = 0;
    clu->status = new;
    }
  /* Within current bounds */
  if (clu->occ > clu->current_occ
      || (clu->fid > clu->current_fid && clu->occ == clu->current_occ) )
    {                     /* Jumps ahead */
    clu->fid = clu->current_fid;    clu->occ = clu->current_occ;
    }
  CLU_status( clu);
  return (OK);
  }

BOOL CLU_dispatch( CLU *clu, short cmd)
  {
  short ret;
  if (clu->fid < clu->init_fid || clu->fid > clu->max_fid)
    return (FAIL);
  switch (cmd)
    {
    default :
      if ( (ret = CLU_dispatch_goto( clu, cmd) ) == TRUE)
        UI_post( clu->ui, UI_VALIDATE);
      return (ret);
    case  UI_VALIDATE_OK:
      if (clu->status == old &&  /* Field altered ? */
          strcmp( old_field, UI_item_search( ui, old_fid, old_occ) ) != 0 )
        clu->status = changed;
      /* Set new old values */
      strcpy( clu->old_field, UI_item_search( clu->ui, clu->fid, clu->occ) );
      clu->old_fid = clu->fid;  clu->old_occ = clu->occ;
      UI_paint_client( clu->ui);
      break;
    case  UI_VALIDATE_FAIL:
      clu->fid = clu->old_fid;  clu->occ = clu->old_occ;
      CLU_status( clu);
      break;
    case  UI_UPDATE_OK:
      UI_status_printf( clu->ui, tx[tx_updated]);
      break;
    case  UI_INSERT_OK:
      UI_status_printf( clu->ui, tx[tx_updated]);
      clu->current_occ++;
      clu->current_fid = clu->init_fid;
      break;
    case  UI_CMD_DELETE_OK:
      tmp = clu->data[occ];
      for (i = clu->occ; i < clu->max_occ - 1; i++)
        clu->data[i] = clu->data[i+1];
      clu->data[clu->max_occ - 1] = tmp;
      for (i = 0, n = max_fid - init_fid; i < n; i++)
      memcpy( tmp, clu->max_occ * (maxlen + 1);
      break;
    }
  return (OK);
  }

