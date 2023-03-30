#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "date.h"

long DATE_todays( DATE *date)
  {
  short a, b, d, m, y;
  float year_corr;
  y = date->y;  m = date->m;  d = date->d;
  year_corr = (y > 0 ? 0.0 : 0.75);
  if (m <= 2)
    {    y--; m += 12;    }   /* Leap year */
  b = 0;
  if ( (y * 10000.0 + m * 100.0 + d) >= 15821015.0)
    {          /* Gregorian callendar reform */
    a = y / 100;
    b = 2 - a + (a / 4);
    }
  return ( (long)(365.25 * y - year_corr) + (long)(30.6001 * (m+1) + d + 1720994L + b) );
  }

DATE *DATE_fromdays( DATE *date, long days)
  {
  long a, b, c, d, e, z, alpha;
  z = days + 1;
  if (z < 2299161L)
    a = z;
  else
    {
    alpha = (long)( (z - 1867216.25) / 36524.25);
    a = z + 1 + alpha - (alpha/4);
    }
  b = a + 1524;
  c = (long)( (b-122.1) / 365.25);
  d = (long)(365.25 * c);
  e = (long)( (b-d) / 30.6001);
  date->d = (short)b - d - (long)(30.6001 * e);
  date->m = (short)(e < 13.5) ? (e-1) : (e-13);
  date->y = (short)(date->m > 2.5) ? (c - 4716) : (c - 4715);
  return (date);
  }

short DATE_weekday( DATE *date) /* Sunday=1, ... Saturday=7 */
  {
  return ( (short)( (DATE_todays( date) + 2 ) % 7 + 1 )  );
  }

BOOL DATE_valid( DATE *date)
  {
  DATE d;
  DATE_fromdays( &d, DATE_todays( date) ); /* OK if not changed */
  return ( (d.d == date->d) && (d.m == date->m) && (d.y == date->y) );
  }

BOOL DATE_fromstr( DATE *date, BYTE *str, short dmy)
  {
  short n, d1, d2, d3;
  struct tm *tmp;
  time_t t;
  BYTE c;
  t = time( &t);    tmp = localtime( &t);
  date->d = tmp->tm_mday;
  date->m = tmp->tm_mon + 1;
  date->y = tmp->tm_year;
  n = sscanf( str, "%d%c%d%c%d", &d1, &c, &d2, &c, &d3);
  if ( n > 0)
    {
    switch (dmy)
      {
      default : break;
      case  DMY:
        if (n >= 1)
          date->d = d1;
        if (n >= 3)
          date->m = d2;
        if (n == 5)
          date->y = d3;
        break;
      case  MDY:
        if (n <= 2)
          date->d = d1;
        else if (n >= 3)
          { date->m = d1; date->d = d2; }
        if (n == 5)
          date->y = d3;
        break;
      case  YMD:
        if (n <= 2)
          date->d = d1;
        else if (n <= 4)
          { date->m = d1; date->d = d2; }
        else if (n == 5)
          { date->y = d1; date->m = d2; date->d = d3; }
        break;
      }
    }
  if (date->y < 100)
    date->y += 1900;
  return (DATE_valid( date) );
  }

BYTE *DATE_tostr( DATE *date, BYTE *str, BYTE *sep, short dmy)
  {
  switch (dmy)
    {
    default :   sprintf( str, "%d%s%d%s%d", date->d, sep, date->m, sep, date->y);  break;
    case  MDY:  sprintf( str, "%d%s%d%s%d", date->m, sep, date->d, sep, date->y);  break;
    case  YMD:  sprintf( str, "%d%s%d%s%d", date->y, sep, date->m, sep, date->d);  break;
    }
  return (str);
  }

/** TESTCODE **
main()
  {
  BYTE s[100];
  DATE d;
  if (DATE_fromstr( &d, "", DMY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", DMY) );
  if (DATE_fromstr( &d, "12", DMY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", DMY) );
  if (DATE_fromstr( &d, "31/12", DMY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", DMY) );
  if (DATE_fromstr( &d, "31/12-90", DMY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", DMY) );
  if (DATE_fromstr( &d, "31 12.1990", DMY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", DMY) );
  if (DATE_fromstr( &d, "", YMD) )
    printf( "%s\n", DATE_tostr( &d, s, ".", YMD) );
  if (DATE_fromstr( &d, "12", YMD) )
    printf( "%s\n", DATE_tostr( &d, s, ".", YMD) );
  if (DATE_fromstr( &d, "12/31", YMD) )
    printf( "%s\n", DATE_tostr( &d, s, ".", YMD) );
  if (DATE_fromstr( &d, "90 12/31", YMD) )
    printf( "%s\n", DATE_tostr( &d, s, ".", YMD) );
  if (DATE_fromstr( &d, "1990.12.31", YMD) )
    printf( "%s\n", DATE_tostr( &d, s, ".", YMD) );
  if (DATE_fromstr( &d, "", MDY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", MDY) );
  if (DATE_fromstr( &d, "12", MDY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", MDY) );
  if (DATE_fromstr( &d, "12/31", MDY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", MDY) );
  if (DATE_fromstr( &d, "12/31-90", MDY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", MDY) );
  if (DATE_fromstr( &d, "12.31.1990", MDY) )
    printf( "%s\n", DATE_tostr( &d, s, ".", MDY) );
  if ( !DATE_fromstr( &d, "2.29.1973", MDY) )
    printf( "Invalid date!!\n");
  if (DATE_fromstr( &d, "", MDY) )
    printf( "Today weekday=%d\n", DATE_weekday( &d)  );
  }
**/

