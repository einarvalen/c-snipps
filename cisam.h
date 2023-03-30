#ifndef CISAM_H
#define CISAM_H

#include <informix/isam.h>
#include <std.h>
#include <ll.h>

/** TYPES **/
struct ccisam_idx  {
  short          id;        /* Index handle */
  BYTE          *name;      /* Index name */
  short          type;      /* Index type (CISAM_IDXTYPE_UNIQUE | DUPLICATE) */
  short          startpos;  /* Starting char of index field within record buffer */
  short          maxlen;    /* Max. length of index field */
  struct keydesc key;       /* C-Isam key description struct */
  };

struct ccisam_field  {
  short id;                 /* Index handle */
  short type;               /* Index type (CISAM_IDXTYPE_UNIQUE | DUPLICATE) */
  short startpos;           /* Field starting position within record buffer */
  short maxlen;             /* Field max.length in chars */
  BYTE *name;               /* Field names */
  };

struct cisam_idx { struct ll first, *node, last; };

struct cisam_field { struct ll first, *node, last; };

struct cisam  {
  char              *name;        /* Name of isam */
  int                fh;          /* cisam file handle */
  short              current_idx; /* id of current index */
  short              idx_count;   /* number of indexes currently defined */
  short              field_count; /* Number of felds defined on file */
  short              reclen;      /* Record length of .dat dile */
  BYTE              *buf;         /* Ptr to data buffer containing current record */
  BOOL               is_locked;   /* Set true by CISAM_lock(). Reset by CISAM_unlock()  */
  struct ccisam_idx *primary;     /* Ptr to primary idx */
  struct cisam_idx   idx;         /* Linked list of index structs */
  struct cisam_field field;       /* Linked list of field structs */
  };
typedef struct cisam CISAM;

#define CISAM_KEYTYPE_NOKEY       0
#define CISAM_KEYTYPE_UNIQUE      1
#define CISAM_KEYTYPE_DUPLICATE   2

#define CISAM_FIELDTYPE_C    0
#define CISAM_FIELDTYPE_D    1
#define CISAM_FIELDTYPE_S    2
#define CISAM_FIELDTYPE_L    3

/**  PROTOTYPES  **/

void CISAM_begin( CISAM *cisam, BYTE *name);
  /*
  Initiates use of CISAM struct (constructor).
  */
void CISAM_end( CISAM *cisam);
  /*
  Destructor of CISAM struct.
  */
void CISAM_close( CISAM *cisam);
  /*
  CLOSES INDEX FILKE
  */
BOOL CISAM_idx_new( CISAM *cisam, short id, BYTE *name, short startpos, short len, short type);
  /*
  INITIALIZES INDEX OF INDEX FILE PTR
  If indexes are redefined, CISAM_FIELDTYPE_C (BYTE) schould be used.
  */
BOOL CISAM_field_new( CISAM *cisam, short id, BYTE *name, short type, short keytype, short startpos, short maxlen);
  /*
  INITIALIZES field.
  If fields are redefined, CISAM_FIELDTYPE_C (BYTE) schould be used.
  */
BOOL CISAM_open( CISAM *cisam);
  /*
  OPENS INDEX FILE
  */
BOOL CISAM_insert( CISAM *cisam);
  /*
  INSERTS NEW RECORD
  */
BOOL CISAM_delete( CISAM *cisam);
  /*
  DELETES CURRENT RECORD
  */
BOOL CISAM_update( CISAM *cisam);
  /*
  UPDATES CURRENT RECORD
  */
BOOL CISAM_first(CISAM *cisam);
  /*
  RETRIEVES FIRST    RECORD
  */
BOOL CISAM_last( CISAM *cisam);
  /*
  RETRIEVES LAST     RECORD
  */
BOOL CISAM_next( CISAM *cisam);
  /*
  RETRIEVES NEXT     RECORD
  */
BOOL CISAM_prev( CISAM *cisam);
  /*
  RETRIEVES PREVIOUS RECORD
  */
BOOL CISAM_ge( CISAM *cisam, short id, BYTE *search);
  /*
  RETRIEVES FIRST RECORD GREATER THAN OR EQUAL SEARCH VALUE
  */
BOOL CISAM_eq( CISAM *cisam, short id, BYTE *search);
  /*
  RETRIEVES RECORD EQUAL SEARCH VALUE
  */
BOOL CISAM_gt( CISAM *cisam, short id, BYTE *search);
  /*
  RETRIEVES RECORD EQUAL SEARCH VALUE
  */
BOOL CISAM_setidx( CISAM *cisam, short id);
  /*
  Sets current index.
  */
BOOL CISAM_field_put( CISAM *cisam, short id, BYTE *str);
  /*
  Converts and puts str into cisam record buffer.
  */
void CISAM_transaction_begin(  CISAM *cisam);
  /*
  Signals start of logical transaction.
  */
void CISAM_transaction_rollback(  CISAM *cisam);
  /*
  Rolls a logical transaction back to beginning.
  */
void CISAM_transaction_commit( CISAM *cisam);
  /*
  Final commitment of logical transaction.
  */
BOOL CISAM_lock( CISAM *cisam);
  /*
  Locks cisam file for concurrent access.
  */
BOOL CISAM_unlock( CISAM *cisam);
  /*
  Unlocks previous lock.
  */
BOOL CISAM_get_unique_id( CISAM *cisam, BYTE *unique_id);
  /*
  Provides a unique id for primary index.
  */
BOOL CISAM_is_eof( void);
  /*
  Returns TRUE if end of file is reached.
  */

BYTE *CISAM_error( CISAM *cisam);
  /*
  RETURNS PTR TO ERROR MSG
  */
BYTE *CISAM_field_get( CISAM *cisam, short id, BYTE *data);
  /*
  Converts and returns field data from cisam record buffer in str.
  */
BYTE *CISAM_field_get_name( CISAM *cisam, short id, short *maxlen);
  /*
  Returns field name from cisam field definition
  and max field len in bytes in maxlen.
  */

#endif
