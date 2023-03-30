/** LIFO STACK functions **/

#include <stack.h>


void STACK_begin( STACK *stack)  {  LL_begin( &(stack->first), &(stack->last) );  }

void STACK_end( STACK *stack)  {  LL_end( &(stack->first) );  }

void *STACK_pop( STACK *stack)
  {
  struct ll *np, *np1;
  void *dp;
  np1 = &(stack->first);
  if ( (np = LL_GET_NEXT( np1 ) ) == NULL)
    return (NULL);
  dp = np->data;
  LL_del( np);
  return (dp);
  }

BOOL STACK_push( STACK *stack, void *data)  {  return (LL_ins( &(stack->first), data) != NULL);  }


/** TESTCODE **

void main( void)
  {
  STACK stack;
  STACK_begin( &stack);
  STACK_push( &stack, "En");
  STACK_push( &stack, "To");
  STACK_push( &stack, "Tre");
  STACK_push( &stack, "Fire");
  printf( "%s\n", STACK_pop( &stack) );
  printf( "%s\n", STACK_pop( &stack) );
  printf( "%s\n", STACK_pop( &stack) );
  printf( "%s\n", STACK_pop( &stack) );
  STACK_end( &stack);
  }

**/
