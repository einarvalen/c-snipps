#ifndef ED_H
#define ED_H

#include <std.h>

/** PROTOTYPES **/
BOOL ED_stream( BYTE *stream, short row, short col, short nrow, short ncol, short color);
BOOL ED_array( BYTE **array, BYTE *title, short row, short col, short nrow, short ncol, short dim);

#endif
