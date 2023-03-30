#ifndef APL_H
#define APL_H

#include "std.h"
#include "mnu.h"

#define APL_CMD_QUIT 10031

struct apl {
  MNUBAR bar;
  BYTE *title;
  };
typedef struct apl APL;

void APL_paint( APL *apl);
BOOL APL_mnu_new( APL *apl,BYTE *text, short mnu_id, short link_to_bar_id);
BOOL APL_mnu_default( APL *apl);
short APL_mnu_pulldown( APL *apl);
void APL_begin( APL *apl, BYTE *title);
void APL_end( APL *apl);
void APL_cmd_default( APL *apl, short cmd);

#endif
