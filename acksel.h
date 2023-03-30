#ifndef ACKSEL_H
#define ACKSEL_H

/** INCLUDES **/

#include "ll.h"


/** TYPES **/

struct cackselerator { short id, key; };
struct ackselerator { struct ll first, last, *node; };
typedef struct ackselerator ACKSEL;


/** PROTOTYPES **/

void ACKSEL_begin( ACKSEL *ackselerator);
  /*
  Inits ackselerator.
  */
void ACKSEL_end( ACKSEL *ackselerator);
  /*
  Ends the usage of this ackselerator.
  */
BOOL ACKSEL_new( ACKSEL *ackselerator, short id, short key);
  /*
  Adds new ackselerator key
  */
short ACKSEL_is_pressed( ACKSEL *ackselerator, short key);
  /*
  Returns the id of apropriate ackselerator if specified key is
  pressed.
  */

#endif
