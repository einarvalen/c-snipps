/**  FIFO QUEUE functions **/
#ifndef Q_H
#define Q_H

/** INCLUDE FILES **/

#include <std.h>
#include <ll.h>


/** TYPES **/

struct  q {
  struct ll first;
  struct ll last;
  };
typedef struct q Q;


/** PROTOTYPES **/

void Q_begin( Q *q);
  /*
  Initiates a queue session.
  */
void Q_end( Q *q);
  /*
  Ends the session and frees occupied memory.
  */
void *Q_pop( Q *q);
  /*
  Returns a pointer to the data element of the first node within the queue.
  The node is also poped off the queue.
  Memory occupied by the node is freed for reuse.
  */
BOOL Q_push( Q *q, void *data);
  /*
  Appends a new data element to the end of teh queue
  */


#endif
