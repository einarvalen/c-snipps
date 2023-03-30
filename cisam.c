#include <stdlib.h>
#include "cisam.h"
#include "mth.h"

#define SUCCESS  0
#define ISAM_OPEN_SHARE   (ISINOUT|ISMANULOCK)  /*(ISINOUT|ISMANULOCK|ISTRANS)*/
#define ISAM_OPEN_NOSHARE (ISINOUT|ISEXCLLOCK)

void CISAM_begin( CISAM *cisam, BYTE *name)
  {
  assert( cisam);
  LL_begin( &(cisam->field.first), &(cisam->field.last) );
  cisam->field.node = &(cisam->field.first);
  LL_begin( &(cisam->idx.first), &(cisam->idx.last) );
  cisam->idx.node = &(cisam->idx.first);
  cisam->name = name;
  cisam->primary = NULL;
  cisam->fh = cisam->current_idx = cisam->idx_count = cisam->field_count = cisam->reclen = 0;
  }

void CISAM_end( CISAM *cisam)
  {
  assert( cisam);  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );
  LL_end( &(cisam->field.first) );
  }

BOOL CISAM_idx_new( CISAM *cisam, short id, BYTE *name, short startpos, short len, short type)  /** INITIALIZES INDEX OF INDEX FILE PTR **/
  {
  struct ccisam_idx *new;
  assert( &(cisam->idx) );        assert( cisam);
  assert( &(cisam->idx.first) );  assert( &(cisam->idx.last) );  assert( &(cisam->idx.node) );
  if ( (new = (struct ccisam_idx *)calloc( sizeof( struct ccisam_idx), 1) ) == NULL)
    return (FAIL);
  if ( !cisam->current_idx)
    cisam->current_idx = id;
  new->name = name;   new->type = type;   new->id = id;   new->startpos;    new->maxlen = len;
  if ( (cisam->idx.node = LL_ins(cisam->idx.node, new) ) == NULL)
    return (FAIL);
  if (type == CISAM_KEYTYPE_UNIQUE)
    {
    new->key.k_flags  = (ISNODUPS+LCOMPRESS);
    cisam->primary = new;
    }
  else
    new->key.k_flags  = (ISDUPS+COMPRESS);
  new->key.k_nparts = 1;
  new->key.k_part[0].kp_start = startpos;
  new->key.k_part[0].kp_leng  = len;
  new->key.k_part[0].kp_type  = CHARTYPE;
  cisam->idx_count++;
  return (TRUE);
  }


BOOL CISAM_field_new( CISAM *cisam, short id, BYTE *name, short type, short keytype, short startpos, short maxlen)  /** INITIALIZES field **/
  {
  struct ccisam_field *new;
  assert( &(cisam->field) );        assert( cisam);
  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );  assert( &(cisam->field.node) );
  if ( (new = (struct ccisam_field *)calloc( sizeof( struct ccisam_field), 1) ) == NULL)
    return (FAIL);
  cisam->field_count++;
  new->name = name;   new->type = type;   new->id = id;
  new->startpos = startpos;
  if ( (cisam->field.node = LL_ins(cisam->field.node, new) ) == NULL)
    return (FAIL);
  switch (new->type)
    {
    default :                new->maxlen = maxlen;     break;
    case  CISAM_FIELDTYPE_C: new->maxlen = maxlen;     break;
    case  CISAM_FIELDTYPE_D: new->maxlen = DOUBLESIZE; break;
    case  CISAM_FIELDTYPE_L: new->maxlen = LONGSIZE;   break;
    case  CISAM_FIELDTYPE_S: new->maxlen = INTSIZE;    break;
    }
  cisam->reclen = MAX( cisam->reclen, (new->startpos + new->maxlen + 1) );
  if (keytype)
    {
    if (type != CHARTYPE)
      return (FAIL);
    else
      return (CISAM_idx_new( cisam, id, name, startpos, maxlen, keytype) );
    }
  return (TRUE);
  }

BOOL CISAM_open( CISAM *cisam)        /** OPENS INDEX FILE **/
  {
  short i;
  struct ll *np;
  struct ccisam_idx *dp;
  assert( cisam);
  assert( cisam->primary);
  assert( &(cisam->idx) );
  assert( &(cisam->idx.first) );  assert( &(cisam->idx.last) );      assert( &(cisam->idx.node) );
  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );  assert( &(cisam->field.node) );
  cisam->fh = isopen( cisam->name, ISAM_OPEN_SHARE);
  if (cisam->fh < SUCCESS)
    {
    if ( (cisam->fh = isbuild( cisam->name, cisam->reclen, &(cisam->primary->key), ISAM_OPEN_SHARE ) ) < SUCCESS)
      return (FAIL);
    if (isclose( cisam->fh) < SUCCESS)
      return (FAIL);
    if ( (cisam->fh = isopen( cisam->name, ISAM_OPEN_NOSHARE ) ) < SUCCESS)
      return (FAIL);
    for (np = &(cisam->idx.first); (np = LL_GET_NEXT( np) ) != NULL; )
      {
      dp = np->data;
      if (dp == cisam->primary)
        continue;
      else if ( (isaddindex( cisam->fh, &(dp->key) ) ) < SUCCESS)
        return (FAIL);
        }
    if (isclose( cisam->fh) < SUCCESS)
      return (FAIL);
    if ( (cisam->fh = isopen( cisam->name, ISAM_OPEN_SHARE ) ) < SUCCESS)
      return (FAIL);
    }
  if (cisam->reclen > BUFSIZ)   /* Max record size */
    return (FAIL);
  if ( (cisam->buf = (BYTE *)malloc( sizeof( BYTE) * cisam->reclen) ) != NULL)
    return (isstart( cisam->fh, &(cisam->primary->key), 0, cisam->buf, ISFIRST) >= SUCCESS);
  return (FAIL);
  }

void CISAM_close( CISAM *cisam)       /** CLOSES INDEX FILKE **/
  {
  isclose( cisam->fh);
  cisam->fh = NULL;
  free( cisam->buf);
  }

BOOL CISAM_is_eof( void)
  {
  return (iserrno == 110);
  }

BYTE *CISAM_error( CISAM *cisam)  /* RETURNS PTR TO ERROR MSG */
  {
  short i;
  static BYTE s[100];
  if (iserrno >= 100 && iserrno < is_nerr)
    sprintf( s, "%s %d: %s\n", cisam->name, iserrno, is_errlist[iserrno-100]);
  else
    strcpy( s, "Out of Ram\0");
  for (i=strlen( s)-1; i>0; i--)
    if (s[i] == '\n')
      s[i] = ' ';
  return (s);
  }

BOOL CISAM_insert( CISAM *cisam)  /** INSERTS NEW RECORD **/
  {
  assert( cisam); assert( cisam->buf);
  return (iswrite( cisam->fh, cisam->buf) >= SUCCESS);
  }

BOOL CISAM_delete( CISAM *cisam)  /** DELETES CURRENT RECORD **/
  {
  assert( cisam); assert( cisam->buf);
  return (isdelcurr( cisam->fh) >= SUCCESS);
  }

BOOL CISAM_update( CISAM *cisam)  /** UPDATES CURRENT RECORD **/
  {
  assert( cisam); assert( cisam->buf);
  return (isrewcurr( cisam->fh, cisam->buf) >= SUCCESS);
  }

BOOL CISAM_first(CISAM *cisam)   /* RETRIEVES FIRST    RECORD */
  {
  assert( cisam); assert( cisam->buf);
  return (isread( cisam->fh, cisam->buf, ISFIRST) >= SUCCESS);
  }

BOOL CISAM_last( CISAM *cisam)   /* RETRIEVES LAST     RECORD */
  {
  assert( cisam); assert( cisam->buf);
  return (isread( cisam->fh, cisam->buf, ISLAST) >= SUCCESS);
  }

BOOL CISAM_next( CISAM *cisam)   /* RETRIEVES NEXT     RECORD */
  {
  assert( cisam); assert( cisam->buf);
  return (isread( cisam->fh, cisam->buf, ISNEXT) >= SUCCESS);
  }

BOOL CISAM_prev( CISAM *cisam)   /* RETRIEVES PREVIOUS RECORD */
  {
  assert( cisam); assert( cisam->buf);
  return (isread( cisam->fh, cisam->buf, ISPREV) >= SUCCESS);
  }

BOOL CISAM_setidx( CISAM *cisam, short id)
  {
  BYTE buf[BUFSIZ];
  struct ll *np;
  struct ccisam_idx *dp;
  assert( cisam);  assert( cisam->buf); assert( &(cisam->idx) );
  assert( &(cisam->idx.first) );  assert( &(cisam->idx.last) );
  for (np = &(cisam->idx.first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    dp = np->data;
    assert( dp);
    if (dp->id == id)
      {
      cisam->current_idx = dp->id;
      return (isstart( cisam->fh, &(dp->key), dp->maxlen, cisam->buf, ISFIRST) >= SUCCESS);
      }
    }
  return (FAIL);
  }

BOOL CISAM_ge( CISAM *cisam, short id, BYTE *search)   /* RETRIEVES FIRST RECORD GREATER THAN OR EQUAL SEARCH VALUE */
  {
  assert( cisam); assert( cisam->buf);
  if (id != cisam->current_idx)
    if ( !CISAM_setidx( cisam, id) )
      return (FAIL);
  if (search != NULL)
    CISAM_field_put( cisam, id, search);
  return (isread( cisam->fh, cisam->buf, ISGTEQ) >= SUCCESS);
  }

BOOL CISAM_eq( CISAM *cisam, short id, BYTE *search)   /* RETRIEVES RECORD EQUAL SEARCH VALUE */
  {
  assert( cisam); assert( cisam->buf);
  if (id != cisam->current_idx)
    if ( !CISAM_setidx( cisam, id) )
      return (FAIL);
  if (search != NULL)
    CISAM_field_put( cisam, id, search);
  return (isread( cisam->fh, cisam->buf, ISEQUAL) >= SUCCESS);
  }

BOOL CISAM_gt( CISAM *cisam, short id, BYTE *search)   /* RETRIEVES RECORD EQUAL SEARCH VALUE */
  {
  assert( cisam); assert( cisam->buf);
  if (id != cisam->current_idx)
    if ( !CISAM_setidx( cisam, id) )
      return (FAIL);
  if (search != NULL)
    CISAM_field_put( cisam, id, search);
  return (isread( cisam->fh, cisam->buf, ISGREAT) >= SUCCESS);
  }

BOOL CISAM_le( CISAM *cisam, short id, BYTE *search)   /* RETRIEVES FIRST RECORD GREATER THAN OR EQUAL SEARCH VALUE */
  {
  if (CISAM_gt( cisam, id, search) )
    return (CISAM_prev( cisam) );
  return (FAIL);
  }

BOOL CISAM_lt( CISAM *cisam, short id, BYTE *search)   /* RETRIEVES FIRST RECORD GREATER THAN OR EQUAL SEARCH VALUE */
  {
  if (CISAM_ge( cisam, id, search) )
    return (CISAM_prev( cisam) );
  return (FAIL);
  }

BYTE *CISAM_field_get_name( CISAM *cisam, short id, short *maxlen)
  {
  struct ll *np;
  struct ccisam_field *dp;
  assert( cisam); assert( cisam->buf);  assert( &(cisam->field) );
  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );
  for (np = &(cisam->field.first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    dp = np->data;
    assert( dp);
    if (dp->id == id)
      {
      *maxlen = dp->maxlen;
      return (dp->name);
      }
    }
  return (NULL);
  }

BYTE *CISAM_field_get( CISAM *cisam, short id, BYTE *str)
  {
  struct ll *np;
  struct ccisam_field *dp;
  assert( cisam); assert( cisam->buf);  assert( &(cisam->field) );
  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );
  for (np = &(cisam->field.first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    dp = np->data;
    assert( dp);
    if (dp->id == id)
      {
      switch (dp->type)
        {
        case  CISAM_FIELDTYPE_C: ldchar( (cisam->buf + dp->startpos), dp->maxlen, str);  break;
        case  CISAM_FIELDTYPE_D: sprintf( str, "%f", lddbl( (cisam->buf + dp->startpos) ) ); break;
        case  CISAM_FIELDTYPE_L: strcpy( str, ltoa( ldlong( (cisam->buf + dp->startpos) ), str, 10) ); break;
        case  CISAM_FIELDTYPE_S: strcpy( str, itoa( ldint( (cisam->buf + dp->startpos) ), str, 10) ); break;
        default : strcpy( str, "¨"); break;
        }
      return (str);
      }
    }
  return (str);
  }


BOOL CISAM_field_put( CISAM *cisam, short id, BYTE *str)
  {
  struct ll *np;
  struct ccisam_field *dp;
  assert( cisam); assert( cisam->buf); assert( &(cisam->field) );
  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );
  for (np = &(cisam->field.first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    dp = np->data;
    assert( dp);
    if (dp->id == id)
      {
      switch (dp->type)
        {
        case  CISAM_FIELDTYPE_C: stchar( str, (cisam->buf + dp->startpos), dp->maxlen);  break;
        case  CISAM_FIELDTYPE_D: stdbl(  atof( str), (cisam->buf + dp->startpos) ); break;
        case  CISAM_FIELDTYPE_L: stlong( atol( str), (cisam->buf + dp->startpos) ); break;
        case  CISAM_FIELDTYPE_S: stint(  atoi( str), (cisam->buf + dp->startpos) ); break;
        default : return (FAIL);
        }
      return (OK);
      }
    }
  return (FAIL);
  }

void CISAM_transaction_begin(  CISAM *cisam)  { isbegin(); }

void CISAM_transaction_rollback(  CISAM *cisam)
  {
  assert( cisam);
  isrollback();
  isflush( cisam->fh);
  }

void  CISAM_transaction_commit( CISAM *cisam)
  {
  assert( cisam);
  iscommit();
  isflush( cisam->fh);
  }

BOOL CISAM_unlock( CISAM *cisam)
  {  return (isunlock( cisam->fh) == SUCCESS);  }

BOOL CISAM_lock( CISAM *cisam)
  {  return (islock( cisam->fh) == SUCCESS);  }

BOOL CISAM_get_unique_id( CISAM *cisam, BYTE *unique_id)
  {
  BOOL ret;
  long luniqueid;
  assert( cisam);
  if (isuniqueid( cisam->fh, &luniqueid) != SUCCESS)
    return (FAIL);
  ltoa( luniqueid, unique_id, 10);
  return (OK);
  }


/**  TESTCODE  **

#define TST_ERR { printf( "\n%s\n", CISAM_error( &cisam) ); return(-1); }

int main()
  {
  short c, i, len;
  BYTE s[BUFSIZ], *sn;
  CISAM cisam;
  CISAM_begin( &cisam, "TEST");
  if ( !CISAM_field_new( &cisam, 10, "Id",     CISAM_FIELDTYPE_C, CISAM_KEYTYPE_UNIQUE,    0, 10) ) TST_ERR;
  if ( !CISAM_field_new( &cisam, 11, "Key",    CISAM_FIELDTYPE_C, CISAM_KEYTYPE_DUPLICATE, 0,  5) ) TST_ERR;
  if ( !CISAM_field_new( &cisam, 12, "Field1", CISAM_FIELDTYPE_C, CISAM_KEYTYPE_NOKEY,     5,  5) ) TST_ERR;
  if ( !CISAM_field_new( &cisam, 13, "Field2", CISAM_FIELDTYPE_C, CISAM_KEYTYPE_NOKEY,    10, 10) ) TST_ERR;
  if ( !CISAM_open( &cisam) ) TST_ERR;
  do
    {
    printf( "\nIns,Del,Upd,First,Last,Next,Prev,Gt,gRe,Eq,lSe,lT,idX,Quit:");
    c = toupper( getch() );
    switch (c)
      {
      default :  break;
      case  'I':
        for (i = 11; i <= 13; i++)
          {
          printf( "\n%s : ", CISAM_field_get_name( &cisam, i, &len) );
          scanf( "%s", s);
          if ( !CISAM_field_put( &cisam, i, s) ) TST_ERR;
          }
        CISAM_transaction_begin( &cisam);
        if ( !CISAM_lock( &cisam) ) TST_ERR;
        if ( !CISAM_insert( &cisam) ) TST_ERR;
        if ( !CISAM_unlock( &cisam) ) TST_ERR;
        CISAM_transaction_commit( &cisam);
        break;
      case  'D':
        CISAM_transaction_begin( &cisam);
        if ( !CISAM_lock( &cisam) ) TST_ERR;
        if ( !CISAM_delete( &cisam) ) TST_ERR;
        if ( !CISAM_unlock( &cisam) ) TST_ERR;
        CISAM_transaction_commit( &cisam);
        break;
      case  'U':
        for (i = 11; i <= 13; i++)
          {
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
          scanf( "%s", s);
          if ( !CISAM_field_put( &cisam, i, s) ) TST_ERR;
          }
        CISAM_transaction_begin( &cisam);
        if ( !CISAM_lock( &cisam) ) TST_ERR;
        if ( !CISAM_update( &cisam) ) TST_ERR;
        if ( !CISAM_unlock( &cisam) ) TST_ERR;
        CISAM_transaction_commit( &cisam);
        break;
      case  'G':
        printf( "\n%s : ", CISAM_field_get_name( &cisam, cisam.current_idx, &len) );
        scanf( "%s", s);
        CISAM_gt( &cisam, cisam.current_idx, s);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'E':
        printf( "\n%s : ", CISAM_field_get_name( &cisam, cisam.current_idx, &len) );
        scanf( "%s", s);
        CISAM_eq( &cisam, cisam.current_idx, s);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'S':
        printf( "\n%s : ", CISAM_field_get_name( &cisam, cisam.current_idx, &len) );
        scanf( "%s", s);
        CISAM_le( &cisam, cisam.current_idx, s);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'T':
        printf( "\n%s : ", CISAM_field_get_name( &cisam, cisam.current_idx, &len) );
        scanf( "%s", s);
        CISAM_lt( &cisam, cisam.current_idx, s);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'R':
        printf( "\n%s : ", CISAM_field_get_name( &cisam, cisam.current_idx, &len) );
        scanf( "%s", s);
        CISAM_ge( &cisam, cisam.current_idx, s);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'F':
        CISAM_first( &cisam);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'L':
        CISAM_last( &cisam);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'N':
        CISAM_next( &cisam);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'P':
        CISAM_prev( &cisam);
        for (i = 10; i <= 13; i++)
          printf( "\n%s : %s\n", CISAM_field_get_name( &cisam, i, &len), CISAM_field_get( &cisam, i, s) );
        break;
      case  'X':
        do
          {
          printf( "\nSelect idx (10 | 11): ");
          scanf( "%d", &i);
          }
        while (!CISAM_setidx( &cisam, i) );
        break;
      case  'Q': break;
      }
    if (iserrno >= 100)
      printf( "%s\n", CISAM_error( &cisam) );
    }
  while (c != 'Q');
  CISAM_close( &cisam);
  CISAM_end( &cisam);
  }

**/
