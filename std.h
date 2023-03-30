#ifndef STDH
#define STDH

#include <dbg.h>  /** For debugging **/

  /* Boleens */
#define TRUE 1
#define FALSE 0
#define FAIL 0
#define SUCCEED 1
#define OK 1
#define ON 1
#define OFF 0
#define YES 1
#define NO 0

  /* To decryptify C */
#define NOT(i) !(i)
#define AND &&
#define OR ||
#define IF
#define THEN ?
#define ELSE :
#define DO_NOTHING

  /*  Nonprintable ascii characters. */
#define ENQ 05
#define ACK 06
#define BEEP 07
#define BS 010
#define LF 012
#define FF 014
#define CR 015
#define XON 021
#define XOF 023
#define NAK 025
#define ESC 033
#define SPACE 040

#define PASCAL /*pascal*/

  /*  Types  */
typedef unsigned char BYTE;
typedef int           BOOL;

#endif

