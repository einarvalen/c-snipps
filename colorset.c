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
#define MOD_LEN 22

FILE *fh;
BYTE s_tab[17][80];
short a_tab[17];

static void _show( W *w)
  {
  short i;
  BYTE s[80], str[80];
  for (i=0; i<16; i++)
    {
    sprintf( s, "%s    = %s ", s_tab[i], COLOR_text( a_tab[i], str) );
    W_putsa( w, i, 4, (MOD_WID-7), s, a_tab[i]);
    }
  }

int main( int ac, char *av[])
  {
  BOOL goon;
  short i, attr, c;
  W w, w1;
  BYTE str[80];
  W_begin();
  COLOR_begin();
  if (ac != 2)
    {
    MSG_msg( "COLROSET filnavn");
    return (1);
    }
  if ( (fh = fopen( av[1], "r") ) == NULL)
    {
    MSG_msg( "Filen %s finnes ikke !", av[1]);
    return (1);
    }
  for (i=0; i<16; i++)
    if (fscanf( fh, "%s %x", s_tab[i], &a_tab[i]) == EOF)
      {
      MSG_msg( "Filen skal ha 16 linjer med navn og farge.");
      return (1);
      }
  fclose( fh);
  W_init( &w1, 1, 1, MOD_LEN, MOD_WID);
  W_save( &w1);
  W_clear( &w1, COLOR_ATTR( WHITE, BLACK) );
  W_box( &w1, 0, 0, MOD_LEN, MOD_WID, W_SINGLE_LINE_ALL_SIDES);
  W_puts( &w1, 2, 6, (MOD_WID-10), "Velg farge som skal forandres.");
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
        if ( (attr = COLOR_select() ) != (-1) )
          {
          a_tab[i] = attr;
          _show( &w);
          }
        break;
      case ARROW_UP:   if (i>0) i--; break;
      case ARROW_DOWN: if (i<15) i++; break;
      case  ESC:       goon = FALSE; break;
      default :break;
      }
    }
  W_restore( &w1);
  if ( (fh = fopen( av[1], "w") ) == NULL)
    {
    MSG_msg( "Filen %s finnes ikke !", av[1]);
    return (1);
    }
  for (i=0; i<16; i++)
    fprintf( fh, "%s %x\n", s_tab[i], a_tab[i]);
  fclose( fh);
  COLOR_end();
  W_end();
  return (0);
  }
