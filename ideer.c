struct item {
  struct { id      } start;
  struct { id, occ } current;
  struct { id, occ } prev;
  struct { id, occ } hw_mark;
  struct { id, occ } end;
  };
typedef item ITEM;

void ITEM_begin( ITEM *item, short start_id, short start_occ, short end_id, short end_occ)
  {
  item->start.id = item->current.id = item->prev.id = item->hw_mark.id = start_id;
  item->end.id = end_id;
  item->current.occ = item->prev.occ = item->hw_mark.occ = start_occ;
  item->end.occ = end_occ;
  }

void ITEM_end( ITEM *item) { }

void ITEM_accept( ITEM *item)
  {
  item->prev.id = item->current.id;
  item->prev.occ = item->current.occ;
  if (item->current.id > item->hw_mark.id)
    item->hw_mark.id = item->current.id;
  if (item->current.occ > item->hw_mark.occ)
    item->hw_mark.occ = item->current.occ;
  }

void ITEM_dismiss( ITEM *item)
  {
  item->current.id = item->prev.id;
  item->current.occ = item->prev.occ;
  }

BOOL ITEM_dispatch( ITEM *item, short cmd)
  {
  struct citem *dp;
  assert( item->form.node);
  dp = item->form.node->data;
  assert( dp);
  switch (cmd)
    {
    default :
      return (UI_dispatch( item->ui, cmd) );
    case  ITEM_CMD_CURSOR_UP:
      if (dp->type == CLUSTER)
        item->current.occ--;
      else
        item->current.id--;
      break;
    case  ITEM_CMD_CURSOR_DOWN:
      if (dp->type == CLUSTER && item->hw_mark.occ > item->current.occ)
        item->current.occ++;
      else
        item->current.id++;
      break;
    case  ITEM_CMD_CURSOR_LEFT:
      item->current.id--;
      break;
    case  ITEM_CMD_CURSOR_RIGHT:
      item->current.id++;
      break;
    }
  ITEM_goto( item, item->current.id, item->current->occ);
  return (TRUE);
  }

