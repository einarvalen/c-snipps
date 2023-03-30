#include <stdio.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <string.h>
#include <io.h>
#include "std.h"
#include "vdo.h"
#include "color.h"
#include "w.h"
#include "str.h"
#include "keys.h"

#define TOP 5
#define MOD_WID 50
#define MOD_LEN 12

static BOOL color_begin_is_set;
static BYTE *txt[] = {
           "BLACK    ",
           "BLUE     ",
           "GREEN    ",
           "CYAN     ",
           "RED      ",
           "MAGENTA  ",
           "BROWN    ",
           "WHITE    ",
           "GREY     ",
           "L_BLUE   ",
           "L_GREEN  ",
           "L_CYAN   ",
           "L_RED    ",
           "L_MAGENTA",
           "L_YELLOW ",
           "L_WHITE  "
         };

extern short color_choise, color_entry, color_cursor, color_error, color_info, color_background;

FILE *fh;

short COLOR_or_mono( short color_attr, short mono_attr)  /* Returns mono_attr if VDO is monocrome else color attr */
  {
  return (IF (VDO_ISMONO() ) THEN ( (short)(mono_attr) ) ELSE ( (short)(color_attr) ) );
  }

void COLOR_begin( void)
  {
  short i;
  BOOL color_set_on_file;
  if (color_begin_is_set)
    return;
  color_begin_is_set = color_set_on_file = TRUE;
  if ( (fh = fopen( "color.set", "r") ) == NULL)
    {
    if (creat( "color.set", (S_IREAD | S_IWRITE) ) == -1)
      color_set_on_file = FALSE;  /** OPEN FAILED **/
    else
      if ( (fh = fopen( "color.set", "r") ) == NULL )
        color_set_on_file = FALSE;  /** OPEN FAILED **/
    }
  if (color_set_on_file)
    {
    if (fscanf( fh, "%d %d %d %d %d %d", &color_choise, &color_entry, &color_cursor, &color_error, &color_info, &color_background) == EOF)
      color_set_on_file = FALSE;
    fclose( fh);
    }
  if ( !color_set_on_file)
    {
    color_choise     = COLOR_or_mono( COLOR_ATTR( BLUE,    WHITE), COLOR_ATTR( GREY,    WHITE) );
    color_entry      = COLOR_or_mono( COLOR_ATTR( L_WHITE, BLUE),  COLOR_ATTR( L_WHITE, BLACK) );
    color_cursor     = COLOR_or_mono( COLOR_ATTR( L_WHITE, RED),   COLOR_ATTR( L_BLUE,  BLACK) );
    color_error      = COLOR_or_mono( COLOR_ATTR( L_CYAN,MAGENTA), COLOR_ATTR( GREY,    WHITE) );
    color_info       = COLOR_or_mono( COLOR_ATTR( WHITE,   BLUE),  COLOR_ATTR( WHITE,   BLACK) );
    color_background = COLOR_or_mono( COLOR_ATTR( CYAN,    BLUE),  COLOR_ATTR( WHITE,   BLACK) );
    }
  }

void COLOR_end( void )
  {
  color_begin_is_set = FALSE;
  }

short COLOR_select( void)
  {
  short i, j, c, k, ret;
  BOOL go_on;
  W w, w1;
  W_init( &w1, 5, 40, 25, 30);
  W_save( &w1);
  W_clear( &w1, COLOR_ATTR( WHITE, BLACK) );
  W_box( &w1, 0, 0, 25, 30, W_SINGLE_LINE_ALL_SIDES);
  W_init( &w, (w1.row+2), (w1.col+2), (w1.nrow-3), (w1.ncol-4) );
  for (c=0, k=TOP, go_on=TRUE; (go_on);)
    {
    i = TOP; j = 6;
    W_printf( &w, 0, 0, "Background is %s", txt[c]);
    W_printf( &w, 1, 0, "    to change background");
    W_printf( &w, 2, 0, "    to toggle choise");
    W_printf( &w, 3, 0, "  ды for selection");
    W_putsa( &w, i++, j, 15, "   BLACK       ",   COLOR_ATTR(BLACK,c) );
    W_putsa( &w, i++, j, 15, "   BLUE        " ,  COLOR_ATTR(BLUE,c) );
    W_putsa( &w, i++, j, 15, "   GREEN       ",   COLOR_ATTR(GREEN,c) );
    W_putsa( &w, i++, j, 15, "   CYAN        " ,  COLOR_ATTR(CYAN,c) );
    W_putsa( &w, i++, j, 15, "   RED         "  , COLOR_ATTR(RED,c) );
    W_putsa( &w, i++, j, 15, "   MAGENTA     " ,  COLOR_ATTR(MAGENTA,c) );
    W_putsa( &w, i++, j, 15, "   BROWN       ",   COLOR_ATTR(BROWN,c) );
    W_putsa( &w, i++, j, 15, "   WHITE       ",   COLOR_ATTR(WHITE,c) );
    W_putsa( &w, i++, j, 15, "   GREY        " ,  COLOR_ATTR(GREY,c) );
    W_putsa( &w, i++, j, 15, "   L_BLUE      " ,  COLOR_ATTR(L_BLUE,c) );
    W_putsa( &w, i++, j, 15, "   L_GREEN     ",   COLOR_ATTR(L_GREEN,c) );
    W_putsa( &w, i++, j, 15, "   L_CYAN      " ,  COLOR_ATTR(L_CYAN,c) );
    W_putsa( &w, i++, j, 15, "   L_RED       "  , COLOR_ATTR(L_RED,c) );
    W_putsa( &w, i++, j, 15, "   L_MAGENTA   ",   COLOR_ATTR(L_MAGENTA,c) );
    W_putsa( &w, i++, j, 15, "   L_YELLOW    " ,  COLOR_ATTR(L_YELLOW,c) );
    W_putsa( &w, i++, j, 15, "   L_WHITE     "   ,COLOR_ATTR(L_WHITE,c) );
    W_putsa( &w, k, (j-3), 3, "--", COLOR_ATTR(L_WHITE,BLACK) );
    switch ( W_get() )
      {
      case ARROW_DOWN:   W_puts( &w, k, j-3, 3, "   "); if (k<(15+TOP)) k++; break;
      case ARROW_UP:     W_puts( &w, k, j-3, 3, "   "); if (k>TOP)      k--; break;
      case ARROW_LEFT:   if (c > 0)  c--; else c = 15; break;
      case ARROW_RIGHT:  if (c < 15) c++; else c = 0;  break;
      case ESC:  ret=(-1); go_on = FALSE; break;
      case '\n':
      case '\r': ret=(c<<4)+(k-TOP); go_on = FALSE; break;
      default: break;
      }
    }
  W_restore( &w1);
  return (ret);
  }

BYTE *COLOR_text( short attr, BYTE *s)  /* Returns ptr to str of form "green on blue" */
  {
  short t, b;
  BYTE ttxt[20], btxt[20];
  t = attr & 0x0f;
  b = attr >> 4;
  strcpy( ttxt, txt[t]);
  strcpy( btxt, txt[b]);
  STR_justleft( ttxt);
  STR_justleft( btxt);
  STR_trunc( ttxt);
  STR_trunc( btxt);
  STR_downshift( ttxt);
  STR_downshift( btxt);
  sprintf( s, "%s on %s", ttxt, btxt);
  return (s);
  }

static void _show( W *w)
  {
  BYTE s[80], str[80];;
  strcpy( s, "color_choise     = ");
  strcat( s, COLOR_text( color_choise, str) );
  W_putsa( w, 0, 4, (MOD_WID-7), s, color_choise);
  strcpy( s, "color_entry      = ");
  strcat( s, COLOR_text( color_entry, str) );
  W_putsa( w, 1, 4, (MOD_WID-7), s, color_entry);
  strcpy( s, "color_cursor     = ");
  strcat( s,COLOR_text( color_cursor, str) );
  W_putsa( w, 2, 4, (MOD_WID-7), s, color_cursor);
  strcpy( s, "color_error      = ");
  strcat( s, COLOR_text( color_error, str) );
  W_putsa( w, 3, 4, (MOD_WID-7), s, color_error);
  strcpy( s, "color_info       = ");
  strcat( s, COLOR_text( color_info, str) );
  W_putsa( w, 4, 4, (MOD_WID-7), s, color_info);
  strcpy( s, "color_background = ");
  strcat( s, COLOR_text( color_background, str) );
  W_putsa( w, 5, 4, (MOD_WID-7), s, color_background);
  }

void COLOR_modify( void)
  {
  BOOL goon;
  short i, attr, c;
  W w, w1;
  W_init( &w1, 1, 1, MOD_LEN, MOD_WID);
  W_save( &w1);
  W_clear( &w1, COLOR_ATTR( WHITE, BLACK) );
  W_box( &w1, 0, 0, MOD_LEN, MOD_WID, W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w1, 2, 6, (MOD_WID-10), "Select color attribute to modify");
  W_init( &w, (w1.row+4), (w1.col+2), (w1.nrow-6), (w1.ncol-4) );
  _show( &w);
  for (i=0, goon=TRUE; goon; )
    {
    W_putsa( &w, i, 0, 3, "--", COLOR_ATTR(L_WHITE,BLACK) );
    c = W_get();
    W_putsa( &w, i, 0, 3, "   ", COLOR_ATTR(L_WHITE,BLACK) );
    switch (c)
      {
      case  '\n':
      case  '\r':
        {
        if ( (attr = COLOR_select() ) != (-1) )
          {
          switch (i)
            {
            case 0: color_choise = attr; break;
            case 1: color_entry = attr; break;
            case 2: color_cursor = attr; break;
            case 3: color_error = attr; break;
            case 4: color_info = attr; break;
            case 5: color_background = attr; break;
            default: break;
            }
          _show( &w);
          }
        break;
        }
      case ARROW_UP:   if (i>0) i--; break;
      case ARROW_DOWN: if (i<5) i++; break;
      case  ESC:       goon = FALSE; break;
      default :break;
      }
    }
  W_restore( &w1);
  if ( (fh = fopen( "color.set", "w") ) == NULL)
    {
    if (creat( "color.set", (S_IREAD | S_IWRITE) ) == -1)
      return; /** CREATION FAILED **/
    else
      if ( (fh = fopen( "color.set", "w") ) == NULL )
        return;  /** OPEN FAILED **/
    }
  fprintf( fh, "%d %d %d %d %d %d\n", color_choise, color_entry, color_cursor, color_error, color_info, color_background);
  fclose( fh);
  }


/** TESTCODE **
main()
  {
  short attr;
  BYTE str[80];
  W_begin();
  COLOR_begin();
  printf( "color_choise     = %s\n", COLOR_text( color_choise, str) );
  printf( "color_entry      = %s\n", COLOR_text( color_entry, str) );
  printf( "color_cursor     = %s\n", COLOR_text( color_cursor, str) );
  printf( "color_error      = %s\n", COLOR_text( color_error, str) );
  printf( "color_info       = %s\n", COLOR_text( color_info, str) );
  printf( "color_background = %s\n", COLOR_text( color_background, str) );
  if ( (attr = COLOR_select() ) != (-1) )
    printf( "\n%s\n", COLOR_text( attr, str) );
  printf( "\nattr=%4X\n", attr);
  COLOR_modify();
  COLOR_end();
  W_end();
  }
**/
