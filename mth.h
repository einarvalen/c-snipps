#ifndef MTH_H
#define MTH_H

#define _PI 3.141592653598793
#define _E 2.718281828459045
#define FRACTION(x) ((x)-(long)(x)) /* Fraction part of floating number */
#define ABS(x) (((x)<0) ? -(x) : (x)) /* Absolute value */
#define MIN(x,y) ((x)<(y) ? (x) : (y)) /* Returns the smaller of x and y */
#define MAX(x,y) ((x)>(y) ? (x) : (y)) /* Returns the bigger of x and y */
#define DIV(a,b) ((long)a)/((long)b)   /* Integer division */
#define MOD(a,b) ((long)a)%((long)b)   /* Modulation */
#define INCH2CM(x) ((x)*2.54)          /* Inches to cm conversion */
#define CM2INCH(x) ((x)*0.393701)      /* Cm to inches conversion */
#define RAD2DEG(x) ((x)*57.29577951)   /* Radians to degrees conv. */
#define DEG2RAD(x) ((x)*0.017453293)   /* Degees to radians conv. */
#define OZ2GRAM(x) ((x)*28.349523)     /* Onzes to grams conv. */
#define GRAM2OZ(x) ((x)*0.035274)      /* Grams to onces conversion */
#define HRS2DAYS(x) ((long)(x)+((x)-(long)(x))/0.60) /* Hours to days conv. */
#define DAYS2HRS(x) ((long)(x)+((x)-(long)(x))*0.60) /* Days to hours conv. */
#define CRCL_AREA(r) ((r)*(r)*_PI)  /* Returns the area of a circle */
#define RCTNGL_AREA(s1,s2) ((s1)*(s2)) /* Returns the area of a rectangle */
#define TRNGL_AREA(h,g) ((h)*(g)/2)    /* Returns the area of a triangle */
/*#define FACULTY( x) { long i; for (i=x; x>0; x--)   i *= x; } */

#endif

/** TESTCODE
main()
  {
#include <stdio.h>

  printf("\n_PI=%f", _PI);
  printf("\n_E=%f",_E);
  printf("\nFRACTION(123.45)=%f",FRACTION(123.45));
  printf("\nABS(-123.45)=%f",ABS(123.45));
  printf("\nMIN(123.45,678.90)=%f",MIN(123.45,678.90));
  printf("\nMAX(123.45,678.90)=%f",MAX(123.45,678.90));
  printf("\nINCH2CM(1)=%f",INCH2CM(1));
  printf("\nCM2INCH(1)=%f",CM2INCH(1));
  printf("\nRAD2DEG(1.56)=%f",RAD2DEG(1.56));
  printf("\nDEG2RAD(15)=%f",DEG2RAD(15));
  printf("\nOZ2GRAM(1.75)=%f",OZ2GRAM(1.75));
  printf("\nGRAM2OZ(28)=%f",GRAM2OZ(28));
  printf("\nHRS2DAYS(12.15)=%4.2f",HRS2DAYS(12.15));
  printf("\nDAYS2HRS(12.50)=%4.2f",DAYS2HRS(12.50));
  printf("\nCRCL_AREA(12.25)=%f",CRCL_AREA(12.25));
  printf("\nRCTNGL_AREA(3.75,4.05)=%f",RCTNGL_AREA(3.75,4.05));
  printf("\nTRNGL_AREA(4.14,2.23)=%f",TRNGL_AREA(4.14,2.23));
  }
*/
