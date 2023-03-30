#include <stdlib.h>
#include <string.h>
#include <str.h>


BYTE *STR_crypt( BYTE *str, BYTE *key)
  {
  short i, code;
  for (i=code=0; key[i]; i++)
    code ^= key[i];
  srand( (int)(0xff & code) );
  for (i=0; str[i]; i++)
    str[i] = str[i] ^ (0xff & rand() );
  return ( str);
  }


BYTE *STR_encrypt( BYTE *str, BYTE *key) {  return (STR_crypt( str, key) );  }
BYTE *STR_decrypt( BYTE *str, BYTE *key) {  return (STR_crypt( str, key) );  }


/** TESTCODE **
main()
  {
  BYTE s[20];
  strcpy( s, "einar");
  printf( "string=%s\n", s);
  STR_encrypt( s, "tore");
  printf( "encrypted=%s\n", s);
  STR_decrypt( s, "tore");
  printf( "decrypted=%s\n", s);
  }
**/
