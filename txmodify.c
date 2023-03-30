#include <stdio.h>
#include "w.h"
#include "tx.h"
#include "ed.h"
#include "str.h"

extern short tx_dim;

void TX_load( void)
  {
  short i;
  FILE *fp;
  BYTE s[BUFSIZ];
  if ( (fp = fopen( "TX.TXR", "r") ) == NULL)
    return;
  for (i=0; i < tx_dim; i++)
    {
    fgets( s, (BUFSIZ), fp);
    STR_trunc( s);
    strncpy( tx[i], s, strlen( tx[i]) );
    }
  fclose( fp);
  }

void TX_save( void)
  {
  short i;
  FILE *fp;
  if ( (fp = fopen( "TX.TXR", "w") ) == NULL)
    return;
  for (i=0; i < tx_dim; i++)
    fprintf( fp, "%s\n", tx[i]);
  fclose( fp);
  }

void TX_modify( void)
  {
  if (ED_array( tx, tx[tx_system_txt], W_CENTER, W_CENTER, 10, 10, tx_dim) )
    TX_save();
  }

/** TESTCODE **
#include "w.h"
#include "color.h"
int main()
  {
  short i;
  W_begin();
  COLOR_begin();
  TX_save();
  for (i=0; i<tx_dim; i++)
    printf( "|%s|\n", tx[i]);
  TX_modify();
  TX_load();
  for (i=0; i<tx_dim; i++)
    printf( "|%s|\n", tx[i]);
  COLOR_end();
  W_end();
  }
**/
