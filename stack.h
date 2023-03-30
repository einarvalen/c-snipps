/**  LIFO STACK functions **/
#ifndef STACK_H
#define STACK_H

/** INCLUDE FILES **/

#include <std.h>
#include <ll.h>


/** TYPES **/

struct  stack {
  struct ll first;
  struct ll last;
  };
typedef struct stack STACK;


/** PROTOTYPES **/

void STACK_begin( STACK *stack);
  /*
  Initiates a stack session.
  */
void STACK_end( STACK *stack);
  /*
  Ends the session and frees occupied memory.
  */
void *STACK_pop( STACK *stack);
  /*
  Returns a pointer to the data element of the top of thestack.
  The node is also poped off the stack.
  Memory occupied by the node is freed for reuse.
  */
BOOL STACK_push( STACK *stack, void *data);
  /*
  Adds a new data element to the top of the stack
  */


#endif
