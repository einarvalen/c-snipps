#ifndef FIELD_H
#define FIELD_H
#include "std.h"

#define FIELD_LEN_NAME 30
#define FIELD_LEN_ID   10
#define FIELD_LEN_TLF  10
#define FIELD_LEN_UNIT 4
#define FIELD_LEN_AMOUNT 13
#define FIELD_LEN_DATE 10
#define FIELD_LEN_IVALUE 9

BOOL FIELD_amount_store( BYTE *s, double d);
BOOL FIELD_amount_check( BYTE *s);
BOOL FIELD_ivalue_store( BYTE *s, long l);
BOOL FIELD_ivalue_check( BYTE *s);

#endif
