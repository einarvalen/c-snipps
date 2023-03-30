#include "std.h"
#include "str.h"

void STR_paste( BYTE *str, BYTE *paste, short pos, short maxlen)
  {
  short j, i;
  for (j = 0, i = pos; i < maxlen && paste[j]; i++, j++)
    str[i] = paste[j];
  }
