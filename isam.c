struct ccisam_idx  {
  BYTE           *name;
  short type;
  short startpos;
  short maxlen;
  struct keydesc key;
  };

struct ccisam_field  {
  short type;
  short startpos;
  short maxlen;
  BYTE *name, *short_name;
  };

struct cisam_idx { struct ll first, *node, last; };

struct cisam_field { struct ll first, *node, last; };

struct cisam  {
  char              *name;
  int                fh;
  short              current_idx;
  short              idx_count;
  short              field_count;
  short              reclen;
  struct cisam_idx   idx;
  struct cisam_field field;
  };
typedef struct cisam CISAM;

void CISAM_begin( CISAM *cisam, BYTE *name)
  {
  assert( cisam);
  LL_begin( &(cisam->field.first), &(cisam->field.last) );
  cisam->field.node = &(cisam->field.first);
  LL_begin( &(cisam->idx.first), &(cisam->idx.last) );
  cisam->idx.node = &(cisam->idx.first);
  cisam->name = name;
  cisam->fh = cisam->current_idx = cisam->idx_count = cisam->field_count = cisam->reclen;
  }

void CISAM_end( CISAM *cisam)
  {
  assert( ui);  assert( &(ui->form.first) );  assert( &(ui->form.last) );
  LL_end( &(ui->form.first) );
  }

BOOL CISAM_idx_new( CISAM *cisam, short id, BYTE *name, short startpos, short len, short type)  /** INITIALIZES INDEX OF INDEX FILE PTR **/
  {
  struct ccisam_idx *new;
  assert( &(cisam->idx) );        assert( cisam);
  assert( &(cisam->idx.first) );  assert( &(cisam->idx.last) );  assert( &(cisam->idx.node) );
  if ( (new = (struct ccisam_idx *)calloc( sizeof( struct ccisam_idx), 1) ) == NULL)
    return (FAIL);
  cisam->num_idx++;
  if ( !cisam->current_idx)
    cisam->current_idx = id;
  new->name = name;   new->type = type;   new->id = id;   new->startpos;    new->maxlen = len
  return (TRUE);
  }


BOOL CISAM_field_new( CISAM *cisam, short id, BYTE *name, BYTE *short_name, short type, short keytype, short startpos, short maxlen)  /** INITIALIZES field **/
  {
  struct ccisam_field *new;
  assert( &(cisam->field) );        assert( cisam);
  assert( &(cisam->field.first) );  assert( &(cisam->field.last) );  assert( &(cisam->field.node) );
  if ( (new = (struct ccisam_field *)calloc( sizeof( struct ccisam_field), 1) ) == NULL)
    return (FAIL);
  cisam->num_field++;
  new->name = name;   new->type = type;   new->id = id;   new->startpos;
  new->maxlen = len   new->short_name = short_name;
  if (keytype)
    return (CISAM_idx_new( cisam, id, name, keytype, startpos, len) );
  return (TRUE);

BOOL CISAM_open( CISAM *cisam)        /** OPENS INDEX FILE **/
  {
  if ( (cisam->fh = isopen( cisam->name, (ISINOUT+ISMANULOCK) ) ) < SUCCESS)
    {
    short i;
    if ( (cisam->fh = isbuild( cisam->name, cisam->reclen, cisam->idx[0].key, (ISINOUT+ISMANULOCK) ) ) < SUCCESS)
      return (FAIL);
    for (i=1; i < cisam->num_idx; i++)
      if ( (isaddindex( cisam->fh, cisam->idx[i].key) ) < SUCCESS)
        return (FAIL);
    }
  return (CISAM_setidx( cisam, cisam->current_idx) )
  }

void CISAM_close( CISAM *cisam)       /** CLOSES INDEX FILKE **/
  {
  isclose( cisam->fh);
  cisam->fd = NULL;
  }

BYTE *CISAM_error( CISAM *cisam)  /* RETURNS PTR TO ERROR MSG */
  {
  static BYTE s[80];
  sprintf( s, "CISAM error number %d", iserrno);
  return (s);
  }

BOOL CISAM_insert( CISAM *cisam, BYTE *buf)  /** INSERTS NEW RECORD **/
  {
  return (iswrite( cisam->fh, buf) != ERR);
  }

BOOL CISAM_delete( CISAM *cisam)  /** DELETES CURRENT RECORD **/
  {
  return (isdelcurr( cisam->fh) != ERR);
  }

BOOL CISAM_update( CISAM *cisam, BYTE *buf)  /** UPDATES CURRENT RECORD **/
  {
  return (isrewcurr( cisam->fh, buf) != ERR);
  }

BOOL CISAM_first(CISAM *cisam, BYTE *buf)   /* RETRIEVES FIRST    RECORD */
  {
  return (isread( cisam->fh, buf, ISFIRST) != ERR);
  }

BOOL CISAM_last( CISAM *cisam, BYTE *buf)   /* RETRIEVES LAST     RECORD */
  {
  return (isread( cisam->fh, buf, ISLAST) != ERR);
  }

BOOL CISAM_next( CISAM *cisam, BYTE *buf)   /* RETRIEVES NEXT     RECORD */
  {
  return (isread( cisam->fh, buf, ISNEXT) != ERR);
  }

BOOL CISAM_prev( CISAM *cisam, BYTE *buf)   /* RETRIEVES PREVIOUS RECORD */
  {
  return (isread( cisam->fh, buf, ISPREV) != ERR);
  }

BOOL CISAM_ge( CISAM *cisam, BYTE *buf, BYTE *search)   /* RETRIEVES FIRST RECORD GREATER THAN OR EQUAL SEARCH VALUE */
  {
  strcpy( &buf[cisam->idx[cisam->current_idx]->startpos], search);
  return (isread( cisam->fh, buf, ISGTEQ) != ERR);
  }

BOOL CISAM_eq(   CISAM *cisam, BYTE *buf, BYTE *search)   /* RETRIEVES RECORD EQUAL SEARCH VALUE */
  {
  strcpy( &buf[cisam->idx[cisam->current_idx]->startpos], search);
  return (isread( cisam->fh, buf, ISEQUAL) != ERR);
  }

BOOL CISAM_gt(   CISAM *cisam, BYTE *buf, BYTE *search)   /* RETRIEVES RECORD EQUAL SEARCH VALUE */
  {
  strcpy( &buf[cisam->idx[cisam->current_idx]->startpos], search);
  return (isread( cisam->fh, buf, ISGREAT) != ERR);
  }

BOOL CISAM_setidx( CISAM *cisam, short id)
  {
  BYTE buf[BUFSIZ];
  struct ll *np;
  struct ccisam_idx *dp;
  assert( cisam);                  assert( &(cisam->idx) );
  assert( &(cisam->idx.first) );  assert( &(cisam->idx.last) );
  for (np = &(cisam->idx.first); ( (np = LL_GET_NEXT( np) ) != NULL); )
    {
    dp = np->data;
    assert( dp);
    if (dp->id == id)
      return (isstart( cisam->fh, dp->key, dp->maxlen, buf, ISFIRST) != ERR)
    }
  return (FAIL);
  }


