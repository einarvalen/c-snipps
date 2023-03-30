#include <keyb.h>
#include <keys.h>
#include <q.h>
#include <vdo.h>
#include <mth.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <dos.h>

#define CTRL_Q        17
#define DEL_TO_EOL    25
#define UPSHIFT       1    /* ^A */
#define DOWNSHIFT     2    /* ^B */
#define INS_CHAR      1874
#define DEL_CHAR      1875

#define BELL() /* printf( "\a") */


static Q keyb_queue;
static BOOL keyb_begin_is_set;



void PASCAL KEYB_begin( void)  /* Initiates this module */
  {
  if (keyb_begin_is_set)
    return;
  Q_begin( &keyb_queue);
  keyb_begin_is_set = TRUE;
  }


short PASCAL KEYB_pop( void)   /* Pop next key off keyb_queue */
  {
  short *c, ret;
  if ( (c = Q_pop( &keyb_queue) ) == NULL)
    return (FALSE);
  ret  = *c;
  free( c);
  return (ret);
  }


void  PASCAL KEYB_push( short key)  /* Pushes a key onto the keyb_queue */
  {
  short *c;
  if ( (c = (short *)malloc( sizeof( short) ) ) == NULL )
    return;
  *c = key;
  Q_push( &keyb_queue, c);
  }


void PASCAL KEYB_end( void)  /* Shuts up the use of this module  */
  {
  while (KEYB_pop() )
    DO_NOTHING;
  Q_end( &keyb_queue);
  keyb_begin_is_set = FALSE;
  }


short PASCAL KEYB_get( void)  /* Returns a character hit from keyboard */
  {
  short c;
  union REGS r;
  if ( (c = KEYB_pop() ) != 0)
    return (c);
  r.h.ah = 0x07;  /* _bios_keybd() */
  intdos( &r, &r);
  if (r.h.al)
    return ((short)r.h.al);
  else
    {
    intdos( &r, &r);
    return ((short)r.x.ax);
    }
  }


short PASCAL KEYB_gets( short row, short col, BYTE *data, short maxlen, short maxdisplay) /* Standard input function */
  {
  register short n;
  short c, i, k, go_on=TRUE;
  static short insert_mode=TRUE;
  static short pos;
  for (n = strlen( data); n>0 && (data[n] <= SPACE); n--)
    DO_NOTHING;
  if (n < 1)           /* UI 181090 */
    {                  /* UI 181090 */
    data[0] = '\0';    /* UI 181090 */
    n = 0;             /* UI 181090 */
    }                  /* UI 181090 */
  else                 /* UI 181090 */
    data[++n] = '\0';
  if ( !(col <= pos <= col+maxdisplay) || pos == 0)
    pos = col;             /** RESET INITIAL CURSOR POSITION **/
  pos = MIN( pos, (IF (n>=1) THEN (col+n) ELSE (col) ) );
  for (i = MIN( n, MAX( 0, (pos-col) ) ), k = MIN( strlen( data), maxlen); go_on;)
    {
    VDO_puts( row, col, maxdisplay, data);
    VDO_cursor_move( row, col+i);
    if (insert_mode)
      VDO_cursor_type( BOX);
    else
      VDO_cursor_type( ON);
    switch (c = KEYB_get() )
      {
      default:
        if ( (c >= SPACE) && (c < 0xff) )  /** PRINTABLE 8-BIT ASCII **/
          {
          if (i>=maxlen || (k>=maxlen && insert_mode) )
            {
            BELL();
            goto overflow;
            }
          if (insert_mode)
            {
            for (n=k; n>i; n--)
              data[n] = data[n-1];
            }
          if (i < maxlen)
            {
            data[i]=(BYTE)c;
            i++;
            }
          if (insert_mode || (k < i) )
            k++;
          }
        else
          go_on = FALSE;
overflow:
        break;
      case '\r':
      case '\n':
        i = 0;
        go_on = FALSE;
        break;
      case ARROW_LEFT:
        if (i>0)
          {
          if (i>=k && (data[i-1] == SPACE) )
            data[--k] = '\0';
          i--;
          }
        else
          go_on = FALSE;
        break;
      case ARROW_RIGHT:
        if (i < maxlen)
          {
          i++;
          if (i>k)
            data[k++] = SPACE;
          }
        else
          BELL();
        break;
      case '\b':
        if (i>0)
          {
          i--;
          for (n=i; n<k; n++)
            data[n] = data[n+1];
          if (k>0) k--;
          }
        else
          go_on = FALSE;
        break;
      case CTRL_Q:
        if ( (c = KEYB_get() ) == DEL_TO_EOL)
          {
          data[i] = '\0';
          k=i;
          }
          break;
      case DEL_CHAR:
        for (n=i; n<k; n++)
          data[n] = data[n+1];
        if (k > i)  k--;
        break;
      case INS_CHAR:
        insert_mode = IF (insert_mode) THEN (FALSE) ELSE (TRUE);
        break;
      case ARROW_END:
        for (i=k; data[i] <= SPACE && i>=0; i--)
         DO_NOTHING;
        i++;
        data[i]='\0';
        k=i;
        if (i>maxdisplay)
          go_on = FALSE;
        break;
      case ARROW_START:    /* ??? */
        i=0;
        go_on = FALSE;
        break;
      case UPSHIFT:
        if (i<k)
          {
          data[i] = toupper( data[i]);
          i++;
          }
        break;
      case DOWNSHIFT:
        if (i<k)
          {
          data[i] = tolower( data[i]);
          i++;
          }
        break;
      }
    data[k] = '\0';
    if ( ( i < 0) && (go_on != FALSE) )
      {
      c = ARROW_LEFT;
      go_on = FALSE;
      }
    else if (i > maxdisplay && (go_on != FALSE) )
      {
      c = ARROW_RIGHT;
      go_on = FALSE;
      }
    }
  if (i < 0)  i = 0;
  if (i > maxdisplay)  i = maxdisplay;
  pos = col + i;
  for (n = strlen( data); n>0 && (data[n] <= SPACE); n--)
    DO_NOTHING;
  data[++n] = '\0';
  VDO_cursor_type( ON);
  return (c);
  }


/** TESTCODE **

void main( void)
  {
  int l;
  char  d[25];
  strcpy(d,"\0");
  VDO_begin();
  KEYB_begin();
loop:
  VDO_clear( 0, 0, 25, 80, 0x70);
  VDO_puts( 9, 1, 30, "VDO_puts(9,1, !!)");
  KEYB_gets( 9, 25, d, 20, 10);
  VDO_puts( 10, 1, 30, "VDO_puts(10,1, !!)");
  l = KEYB_gets( 10, 25, d, 20, 10);
  KEYB_push( ARROW_END);
  KEYB_push( ARROW_LEFT);
  if (l != ESC)
     goto loop;
  strcpy( d, "1234567890          ");
  d[21] = '~'; d[22] = '~';
  VDO_puts( 11, 1, 30, "VDO_puts(11,1, !!)");
  d[3] = '\0';
  KEYB_gets(11,25,d,3,3);
  printf("\r\n\n\n");
  for (l=0; l<25; l++)
    printf("%x ",d[l]);
  printf("\n%s",d);
  KEYB_end();
  VDO_end();
  }
**/
