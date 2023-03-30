/** LINKED LIST **/

#ifndef LL_H
#define LL_H

/** INCLUDE FILES **/

#include <std.h>


/** TYPES **/

struct ll {
  struct ll *next;
  struct ll *prev;
  void      *data;
  };


/** PROTOTYPES **/

void LL_begin( struct ll *first, struct ll *last);
  /*
  Initializes linked list. The nodes "first" and "last" only
  act as pointers to the beginning-  and ending node of the list.
  Memory for the two nodes must be allocated prior to calling
  this function.
  */
struct ll *LL_ins( struct ll *node, void *data);
  /*
  Inserts new node in list after the node pointed to by "node".
  Returns pointer to new node. NULL indicates memory shortage.
  */
struct ll *LL_del( struct ll *node);
  /*
  Deletes node from list and occupied memory is freed.
  Returns pointer to following node. NULL indicates end of list.
  */
void LL_end( struct ll *first);
  /*
  Ends the use of linked list. Occupied memory is freed.
  */

/** MACROS **/ 

#define LL_GET_NEXT( np) (IF (np->next->data == NULL) THEN (NULL) ELSE (np->next) )
  /*
  Returns pointer to next node in list, NULL if at end.
  */
#define LL_GET_PREV( np) (IF (np->prev->data == NULL) THEN (NULL) ELSE (np->prev) )
  /*
  Returns pointer to previous node in list, NULL if at beginning.
  */

#endif
