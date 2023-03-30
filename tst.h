#include <stdio.h>

main()
  {
  short i;
  void *v;
  i = 50;
  v = i;
  printf( "v=%d\n", (short)v);
  i = (short)v;
  printf( "i=%d\n", i);
  i = 50;
  v = (void *)i;
  printf( "v=%d\n", (short)v);
  i = (short)v;
  printf( "i=%d\n", i);
  }

