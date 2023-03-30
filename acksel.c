#include <stdlib.h>
#include "std.h"
#include "acksel.h"

void ACKSEL_begin( ACKSEL *ackselerator)
  {
  assert( ackselerator);  assert( &(ackselerator->first) );  assert( &(ackselerator->last) );
  LL_begin( &(ackselerator->first), &(ackselerator->last) );
  ackselerator->node = &(ackselerator->first);
  }

void ACKSEL_end( ACKSEL *ackselerator)
  {
  assert( ackselerator);  assert( &(ackselerator->first) );  assert( &(ackselerator->last) );
  LL_end( &(ackselerator->first) );
  }

BOOL ACKSEL_new( ACKSEL *ackselerator, short id, short key)
  {
  struct cackselerator *new;
  assert( ackselerator);  assert( &(ackselerator->first) );  assert( &(ackselerator->last) );
  if ( (new = (struct cackselerator *)calloc( sizeof( struct cackselerator), 1) ) == NULL)
    return (FAIL);
  new->id = id;   new->key = key;
  return ( (ackselerator->node = LL_ins( ackselerator->node, new) ) != NULL);
  }

short ACKSEL_is_pressed( ACKSEL *ackselerator, short key)
  {
  short j;
  struct ll *np;
  struct cackselerator *m;
  assert( ackselerator);  assert( &(ackselerator->first) );  assert( &(ackselerator->last) );
  for (np = &(ackselerator->first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    m = np->data;
    if (key == m->key)
      return (m->id);
    }
  return (0);
  }

/** TESTCODE **

#include "keys.h"
#include "keyb.h"

int main()
  {
  short id, c, goon;
  ACKSEL ack;
  KEYB_begin();
  ACKSEL_begin( &ack);
  ACKSEL_new( &ack, 1, ARROW_UP);
  ACKSEL_new( &ack, 2, ARROW_DOWN);
  ACKSEL_new( &ack, 3, ARROW_LEFT);
  ACKSEL_new( &ack, 4, ARROW_RIGHT);
  ACKSEL_new( &ack, 5, ARROW_START);
  ACKSEL_new( &ack, 6, ARROW_END);
  printf( "\nUse Arrows, Home and End\n");
  for (goon = TRUE; goon; )
    {
    c = KEYB_get();
    id = ACKSEL_is_pressed( &ack, c);
    switch (id)
      {
      default: printf( "ackcelerator_id=%d\n", id); break;
      case  6: goon = FALSE; break;
      }
    }
  ACKSEL_end( &ack);
  KEYB_end();
  return (FALSE);
  }

**/

