#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "std.h"
#include "rep.h"

void REP_printf( REP *rep, short where, short row, short col, char *fmt, ...)
  {
  short i, j;
  BYTE s[BUFSIZ];
  va_list arg_ptr;
  va_start( arg_ptr, fmt);
  vsprintf( s, fmt, arg_ptr);
  va_end( arg_ptr);
  if (rep->fh == NULL)
    {
    if ( (rep->fh = fopen( rep->filename, "w") ) == NULL )
      {
      MSG_warn( "OPEN err %s", rep->filename);
      return;
      }
    fprintf( rep->fh, rep->format_seq);
    }
  switch (where)
    {
    default :
      break;
    case  REP_REPORT:
      if (row >= rep->report_len || col >= rep->page_wid)
        return;
      for (j = 0, i = col; i < rep->page_wid && s[j]; i++, j++)
        rep->report[row][i] = s[j];
      break;
    case  REP_HEAD:
      if (row >= rep->head_len || col >= rep->page_wid)
        return;
      for (j = 0, i = col; i < rep->page_wid && s[j]; i++, j++)
        rep->head[row][i] = s[j];
      break;
    case  REP_TAIL:
      if (row >= rep->tail_len || col >= rep->page_wid)
        return;
      for (j = 0, i = col; i < rep->page_wid && s[j]; i++, j++)
        rep->tail[row][i] = s[j];
      break;
    }
  }

BOOL REP_begin( REP *rep, BYTE *filename, short page_len, short page_wid, short head_len, short tail_len)
  {
  short i;
  rep->page_wid = page_wid;
  rep->page_len = page_len;
  rep->head_len = head_len;
  rep->tail_len = tail_len;
  rep->report_len = page_len - tail_len - head_len;
  rep->page_count = 0;
  rep->fh = NULL;
  rep->filename = filename;
  rep->format_seq = rep_format_seq;
  rep->reset_seq =  rep_reset_seq;
  if ( (rep->head = (BYTE **)calloc( sizeof( BYTE *), (rep->head_len + 1) ) ) == NULL)
    return (FAIL);
  for (i = 0; i < rep->head_len; i++)
    {
    if ( (rep->head[i] = (BYTE *)calloc( sizeof( BYTE), (rep->page_wid + 1) ) ) == NULL)
      return (FAIL);
    STR_set( rep->head[i], ' ', rep->page_wid);
    rep->head[i][rep->page_wid] = '\0';
    }
  if ( (rep->tail = (BYTE **)calloc( sizeof( BYTE *), (rep->tail_len + 1) ) ) == NULL)
    return (FAIL);
  for (i = 0; i < rep->tail_len; i++)
    {
    if ( (rep->tail[i] = (BYTE *)calloc( sizeof( BYTE), (rep->page_wid + 1) ) ) == NULL)
      return (FAIL);
    STR_set( rep->tail[i], ' ', rep->page_wid);
    rep->head[i][rep->page_wid] = '\0';
    }
  if ( (rep->report = (BYTE **)calloc( sizeof( BYTE *), (rep->report_len + 1) ) ) == NULL)
    return (FAIL);
  for (i = 0; i < rep->report_len; i++)
    {
    if ( (rep->report[i] = (BYTE *)calloc( sizeof( BYTE), (rep->page_wid + 1) ) ) == NULL)
      return (FAIL);
    STR_set( rep->report[i], ' ', rep->page_wid);
    rep->report[i][rep->page_wid] = '\0';
    }
  return (OK);
  }

/*  Flushes current page to printer or disk and clears reporting area.  */
void  REP_flush( REP *rep)
  {
  short i;
  for (i = 0; i < rep->head_len; i++)
    fprintf( rep->fh, "%s\n", rep->head[i]);
  for (i = 0; i < rep->report_len; i++)
    {
    fprintf( rep->fh, "%s\n", rep->report[i]);
    STR_set( rep->report[i], ' ', rep->page_wid);
    }
  for (i = 0; i < rep->tail_len; i++)
    fprintf( rep->fh, "%s\n", rep->tail[i]);
  fprintf( rep->fh, "\f");
  rep->page_count++;
  }

/*
Prints a horizontal line of char in 'c' from position indicated by row and col
with the length of len. The variable 'where' determins whitch of
reporting-, head- or tail area the line is printed.
*/
void REP_hline( REP *rep, short where, short row, short col, short len, BYTE c)
  {
  BYTE s[BUFSIZ+1];
  STR_set( s, c, len);
  REP_printf( rep, where, row, col, s);
  }

/*
Prints a vertical line of char in 'c' from position indicated by row and col
with the length of len. The variable 'where' determins whitch of
reporting-, head- or tail area the line is printed.
*/
void REP_vline( REP *rep, short where, short row, short col, short len, BYTE c)
  {
  short i;
  for (i=0; i<=len; i++)
    REP_printf( rep, where, (row+i), col, "%c", c);
  }

/*
Prints a vertical line of char in 'c' from position indicated by row and col
with the length of len. The variable 'where' determins whitch of
reporting-, head- or tail area the line is printed.
*/
void REP_box( REP *rep, short where, short row, short col, short nrow, short ncol)
  {
  REP_hline( rep, where, row, col, ncol, '-');
  REP_hline( rep, where, (row+nrow), col, ncol, '-');
  REP_vline( rep, where, row, col, nrow, '|');
  REP_vline( rep, where, row, (col+ncol), nrow, '|');
  }


void REP_end( REP *rep)
  {
  short i;
  fprintf( rep->fh, "%s\f", rep->reset_seq);
  fclose( rep->fh);
  for (i = 0; i < rep->head_len; i++)
    free( rep->head[i]);
  free( rep->head);
  for (i = 0; i < rep->tail_len; i++)
    free( rep->tail[i]);
  free( rep->tail);
  for (i = 0; i < rep->report_len; i++)
    free( rep->report[i]);
  free( rep->report);
  }

/** TESTCODE **
main()
  {
  REP rep;
  register short i, j;
  REP_begin( &rep, "test.rep", 24, 80, 10, 10);
  for (i=0; i<5; i++)
    {
    REP_box( &rep, REP_HEAD,   0, 0, 9, 79);
    REP_box( &rep, REP_TAIL,   0, 0, 9, 79);
    REP_hline( &rep, REP_REPORT, 0, 0, 90, '#');
    REP_hline( &rep, REP_REPORT, 1, 0, 90, '#');
    REP_hline( &rep, REP_REPORT, 2, 0, 90, '#');
    REP_hline( &rep, REP_REPORT, 3, 0, 90, '#');
    REP_hline( &rep, REP_REPORT, 4, 0, 90, '#');
    REP_hline( &rep, REP_REPORT, 5, 0, 90, '#');
    REP_flush( &rep);
    }
  REP_end( &rep);
  }

**/
