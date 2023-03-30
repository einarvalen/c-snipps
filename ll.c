/** Linked list functions **/

#include <ll.h>
#include <malloc.h>

static BOOL ll_begin_is_set;
void LL_begin( struct ll *first, struct ll *last)
  {
  assert( first);
  assert( last);
  if (ll_begin_is_set)
    return;
  first->data = last->data = first->prev = last->next = NULL;
  first->next = last;
  last->prev = first;
  ll_begin_is_set = TRUE;
  }

struct ll *LL_ins( struct ll *node, void *data)
  {
  struct ll *new;
  assert( data);
  assert( node);
  if ( (new = (struct ll *)calloc( sizeof( struct ll), 1) ) == NULL)
    return (NULL);
  new->data = data;
  new->prev = node;
  new->next = node->next;
  if (node->next != NULL)
    node->next->prev = new;
  node->next = new;
  return (new);
  }

struct ll *LL_del( struct ll *node)
  {
  struct ll *np;
  assert( node);
  np = node->prev;
  np->next = node->next;
  np = node->next;
  np->prev = node->prev;
  free( node);
  return (np);
  }

void LL_end( struct ll *first)
  {
  struct ll *np, *np1;
  assert( first);
  ll_begin_is_set = FALSE;
  for ( np = first->next; np != NULL; )
    {
    if (np->data != NULL) /* ??? 131190 */
      {
      free( np->data);
      np = LL_del( np);
      }
    else
      np = LL_GET_NEXT( np);
    }
  }


/** TESTCODE **

main()
  {
  BYTE str[BUFSIZ], *s;
  struct ll *node, first, last;
  BOOL goon;
  node =&first;
  for  (goon = TRUE, LL_begin( &first, &last); goon == TRUE; )
    {
    if ( !gets( str) )
      goon = FALSE;
    else
      {
      if ( (s = (BYTE *)malloc( sizeof( BYTE) + strlen( str) + 1) ) == NULL)
        {
        printf( "Need ram\n");
        goon = FALSE;
        }
      else
        {
        strcpy( s, str);
        if ( (node = LL_ins( node, s) ) == NULL)
          {
          printf( "\n  Err - LL_ins()\n");
          goon = FALSE;
          }
        }
      }
    }

  for (node = &first; (node = LL_GET_NEXT( node) ) != NULL; )
    printf( "%s\n", (BYTE *)node->data);
  printf( "\n\n\n");
  for (node = &last; (node = LL_GET_PREV( node) ) != NULL; )
    printf( "%s\n", (BYTE *)node->data);
  LL_end( &first);
  }

**/
