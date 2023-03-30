/** FIFO QUEUE functions **/

#include <q.h>
static BOOL q_begin_is_set;

void Q_begin( Q *q)
  {
  if (q_begin_is_set)
    return;
  LL_begin( &(q->first), &(q->last) );
  q_begin_is_set = TRUE;
  }

void Q_end( Q *q)
  {
  LL_end( &(q->first) );
  q_begin_is_set = FALSE;
  }

void *Q_pop( Q *q)
  {
  struct ll *np, *np1;
  void *dp;
  np1 = &(q->last);
  if ( (np = LL_GET_PREV( np1 ) ) == NULL)
    return (NULL);
  dp = np->data;
  LL_del( np);
  return (dp);
  }

BOOL Q_push( Q *q, void *data)  {  return (LL_ins( &(q->first), data) != NULL);  }


/** TESTCODE **

void main( void)
  {
  Q q;
  Q_begin( &q);
  Q_push( &q, "En");
  Q_push( &q, "To");
  Q_push( &q, "Tre");
  Q_push( &q, "Fire");
  printf( "%s\n", Q_pop( &q) );
  printf( "%s\n", Q_pop( &q) );
  printf( "%s\n", Q_pop( &q) );
  printf( "%s\n", Q_pop( &q) );
  Q_end( &q);
  }

**/
