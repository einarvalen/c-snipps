#ifndef STR_H
#define STR_H

#include <std.h>


/** PROTOTYPES **/
BYTE  *STR_trunc( BYTE *s);
BYTE  *STR_justleft( BYTE *s);
BYTE  *STR_justright( BYTE *s, short maxlen);
BYTE  *STR_zerofill( BYTE *s, short maxlen);
BYTE  *STR_upshift( BYTE *s);
BYTE  *STR_downshift( BYTE *s);
BOOL   STR_ismatch( BYTE *s, BYTE *cmp);
BOOL   STR_isxmatch( BYTE *s, BYTE *cmp); /** EXACT MATCH **/
BOOL   STR_ismember( BYTE *s, BYTE **cmp);
BYTE  *STR_pad( BYTE *s, short maxlen);
BYTE  *STR_encrypt( BYTE *str, BYTE *key);
BYTE  *STR_decrypt( BYTE *str, BYTE *key);
BYTE  *STR_set( BYTE *s, BYTE c, short len);
void  STR_paste( BYTE *str, BYTE *paste, short pos, short maxlen);

#endif
