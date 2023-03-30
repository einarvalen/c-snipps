#ifndef REP_H
#define REP_H

#include <stdio.h>

#define REP_PRN "PRN"
#define REP_HEAD   1
#define REP_TAIL   2
#define REP_REPORT 3
#define PRTXTSIZE 30

static BYTE  rep_format_seq[PRTXTSIZE+1]   = {"\0"};
static BYTE  rep_reset_seq[PRTXTSIZE+1] = {"\0"};
static BYTE  rep_dev[PRTXTSIZE+1]   = {"PRN"};


struct rep {
  short page_wid;
  short page_len;
  short head_len;
  short tail_len;
  short report_len;
  BYTE **head;
  BYTE **tail;
  BYTE **report;
  short page_count;
  short line_count;
  FILE *fh;
  BYTE *filename;
  BYTE *format_seq;
  BYTE *reset_seq;
  };
typedef struct rep REP;


void REP_printf( REP *rep, short where, short row, short col, char *fmt, ...);
  /*
  Standard printer output to reporting area, head area or tail area of page.
  The variable 'where' indicates witch.
  */
BOOL REP_begin( REP *rep, BYTE *filename, short page_len, short page_wid, short head_len, short tail_len);
  /*
  Initiates this module for use.
  */
void REP_end( REP *rep);
  /*
  Ends the use of this module.
  */
void REP_flush( REP *rep);
  /*
  Flushes current page to printer or disk and clears reporting area.
  */
void REP_hline( REP *rep, short where, short row, short col, short len, BYTE c);
  /*
  Prints a horizontal line of char in 'c' from position indicated by row and col
  with the length of len. The variable 'where' determins whitch of
  reporting-, head- or tail area the line is printed.
  */
void REP_vline( REP *rep, short where, short row, short col, short len, BYTE c);
  /*
  Prints a vertical line of char in 'c' from position indicated by row and col
  with the length of len. The variable 'where' determins whitch of
  reporting-, head- or tail area the line is printed.
  */
void REP_box( REP *rep, short where, short row, short col, short nrow, short ncol);
  /*
  Prints a box of '|' and '-' characters with upper left corner in
  position 'row' and 'col'. The width equals 'ncol' and depth 'nrow'.
  */


#endif
