#ifndef DATE_H
#define DATE_H

#include "std.h"

#define DMY 0
#define YMD 1
#define MDY 2

struct date {
  short d;
  short m;
  short y;
  };
typedef struct date DATE;

BYTE *DATE_tostr( DATE *date, BYTE *str, BYTE *sep, short dmy);
BOOL DATE_fromstr( DATE *date, BYTE *str, short dmy);
BOOL DATE_valid( DATE *date);
DATE *DATE_fromdays( DATE *date, long days);
long DATE_todays( DATE *date);
short DATE_weekday( DATE *date); /* Sunday=1, ... Saturday=7 */

#endif
