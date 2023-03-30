
		/*-----------------------------+
		|           QELIB.H            |
		+------------------------------+
		|  Q+E Library Include File    |
		+-----------------------------*/

/*--------------------------------------------------------------+
|  Copyright: 1988-1993 Q+E Software, Inc.                      |
|                                                               |
|       This software contains confidential and proprietary     |
|       information of Q+E Software, Inc.                       |
+--------------------------------------------------------------*/

// ***************** IMPORTANT NOTES FOR BORLAND C USERS ********************
// Due to differences in the way Microsoft C returns floating point values,
// all routines that return a float or double must be passed an extra
// argument.  This argument must point to a floating point value ON THE
// STACK.  Furthermore, the routine returns a pointer to the floating point
// value.  For example, where a Microsoft user would type:
//      fltval = qeValFloat (hstmt, colno);
// a Borland C user would type (assuming fltval is a local variable):
//      fltval = *qeValFloat (hstmt, colno, &fltval);
// or simply:
//      qeValFloat (hstmt, colno, &fltval);
// If fltval is not a local variable, then the following will work:
//      {float temp; fltval = *qeValFloat (hstmt, colno, &temp);}
// **************************************************************************

#ifdef __BORLANDC__
#define qeAPI(rtn_type) rtn_type far pascal
#define qeLPSTR         char far *
#define qeLPDEC         qeLPSTR
#define qeLPVOID        void far *
#define qeLPLONG        long far *
#define qeHANDLE        short
#define qeSTATUS        short
#define qeRETFLT        float far *
#define qeRETDBL        double far *
#define qeHIDDENFLT     , float _ss *
#define qeHIDDENDBL     , double _ss *
#define qeLPFLT         float far *
#define qeLPDBL         double far *
#define qeLPINT         short far *
#endif

// **************** IMPORTANT NOTES FOR IBM OS/2 2.0 USERS ******************
// QELIB is a 16-bit DLL.  32-bit applications must take special care when
// calling a 16-bit DLL.  First, all QELIB routines must be declared FAR16.
// Second, any buffers passed to QELIB must not cross a 64K boundary.  This
// can be assured by allocating tiled memory.
// **************************************************************************

#ifdef __IBMC__
#define qeAPI(rtn_type) rtn_type _Far16 _Pascal
#define qeLPSTR         char *
#define qeLPDEC         qeLPSTR
#define qeLPVOID        void *
#define qeLPLONG        long *
#define qeHANDLE        short
#define qeSTATUS        short
#define qeRETFLT        float
#define qeRETDBL        double
#define qeHIDDENFLT
#define qeHIDDENDBL
#define qeLPFLT         float *
#define qeLPDBL         double *
#define qeLPINT         short *
#endif

// These are the declarations for use with the Microsoft C compiler:

#ifndef qeAPI
#define qeAPI(rtn_type) rtn_type __loadds __export far pascal
#define qeLPSTR         char far *
#define qeLPDEC         qeLPSTR
#define qeLPVOID        void far *
#define qeLPLONG        long far *
#define qeHANDLE        short
#define qeSTATUS        short
#define qeRETFLT        float
#define qeRETDBL        double
#define qeHIDDENFLT
#define qeHIDDENDBL
#define qeLPFLT         float far *
#define qeLPDBL         double far *
#define qeLPINT         short far *
#endif

//--------------------------------------------------------------------------
// Result Code Values.  All routines which return qeSTATUS will return a
// value from this set.  This also defines the set of values which will be
// returned from qeErr.

#define qeLOCK_CHANGE_REC       -8      // Attempt to lock a record and
					// the lock was obtained, but the
					// record has been changed since it
					// was originally read.  This can
					// only occur for DBMSs that require
					// a log file.
#define qeLOCK_MULTI_REC        -7      // Attempt to lock a record and
					// the lock was obtained, but more
					// than one record was locked. This
					// occured because the primary key
					// fields caused more than one
					// record to be selected.
#define qeLOCK_NO_REC           -6      // Attempt to lock a record, but
					// no record in the database was
					// selected by the primary key,
					// the record has been deleted by
					// another user, or another user
					// has changed the value of a
					// key field.
#define qeEOF                   -5      // No more records to read.
#define qeUSER_CANCELLED        -4      // User cancelled out of the logon
					// dialog box.
#define qeOUT_OF_MEMORY         -3      // Insufficient memory for operation.
#define qeNULL_DATA             -2      // Null value.  Returned as the
					// length from a qeDataLen call.
#define qeTRUNCATION            -1      // The buffer passed in to hold
					// a result is not large
					// enough to hold the result.
					// A partial result has been
					// returned in the buffer.
#define qeSUCCESS               0       // Success.
#define qeSUCCESS_WITH_INFO     1       // Success with information (warning).
#define qeNO_DATA_WITH_INFO     2       // EOF with additional information
					// (usually ESC during a fetch).
#define qeDBSYS_ERROR           4       // Error detected by the underlying
					// database system or driver. The
					// native error returned by qeDBErr
					// gives more specifics.

//---------------------------------------------------------------------------
// QELIB Data Types.  These are the values returned from qeColType.

#define qeCHAR                  1       // Blank padded fixed length string
#define qeVARCHAR               2       // Variable length string
#define qeDECIMAL               3       // BCD number
#define qeINTEGER               4       // 4-byte signed integer
#define qeSMALLINT              5       // 2-byte signed integer
#define qeFLOAT                 6       // 4-byte floating point
#define qeDOUBLEPRECISION       7       // 8-byte floating point
#define qeDATETIME              8       // 26-byte date time value:
					// YYYY-MM-DD HH:MM:SS.FFFFFF

//---------------------------------------------------------------------------
// Native data types. These types, plus the 8 QELIB types, are the values
// returned by qeColDBType.

#define qeLONGVARCHAR           100     // Large text fields.
#define qeBINARY                101     // Fixed length binary or raw data.
#define qeVARBINARY             102     // Variable length binary data.
#define qeLONGVARBINARY         103     // Large binary data fields.
#define qeNATIONALCHAR          104     // Fixed length char strings may
					// have 2-byte chars (e.g. Kanji).
#define qeVARNATIONALCHAR       105     // Variable length national strings.
#define qeLONGVARNATIONALCHAR   106     // Large national strings.
#define qeNUMERIC               107     // Numeric, no fixed scale.
#define qeBIGINT                108     // 8-byte signed integer.
#define qeTINYINT               109     // 1-byte signed integer.
#define qeBIT                   110     // Bit value (0 or 1).
#define qeDATE                  111     // Date.
#define qeTIME                  112     // Time.
#define qeRECID                 113     // Unique row ID.
#define qeRECVER                114     // Row version number.
#define qeSERIAL                115     // Automatically generated
					// sequential number.
#define qeMONEY                 116     // Money.

//---------------------------------------------------------------------------
// Routines

//--------------------------
// Data dictionary routines
//--------------------------
//--------------------------------------------------------------------------

qeAPI(qeHANDLE) qeSources (
	short           options);
			  // Options determine which data sources are to be
			  // returned, and must be one of the values from
			  // the OPTIONS constants defined below.

// Creates a handle to a statement execution (hstmt) that can be used to
// return information on the data sources (database systems) that can
// be accessed.  The records can be subsequently read using the qeFetch
// functions, and column values can be retrieved using either the qeVal
// or qeBindCol functions.
// RETURNS: the statement execution (hstmt), or 0 if the function fails.
// When you have finished retrieving sources (when you receive EOF from
// qeFetchNext), you must call qeEndSQL to terminate the hstmt.

// The resulting records contain the following columns:
//      Name            Char(32) - Source name.
//      Extension       Char(32) - File extension (may be null).
//      QELIB hdbc      Short    - If qeConnect has been used to
//                                 connect to this source, the QELIB hdbc.
//                                 This is 0 if not currently connected.
//      Remarks         Char(256)- Comment field for data sources.
//---------------------------------------------------------------------------

// OPTIONS: Values that may be used for the options parameter to qeSources.
// The value determines which Sources will be returned by the hstmt.

#define qeSRC_AVAIL_LOGON       1       // All sources
#define qeSRC_CONN_LOGON        2       // All connected sources

//---------------------------------------------------------------------------
// Number of columns in result set and defines for returned columns:

#define qeSRC_NUM_COLS          4       // Number of columns returned
#define qeSRC_NAME_COL          1       // Source name
#define qeSRC_EXT_COL           2       // File extension
#define qeSRC_HDBC_COL          3       // QELIB hdbc
#define qeSRC_REMARKS_COL       4       // Remarks

//---------------------------------------------------------------------------
// Maximum lengths of returned character column values (not including
// 0-terminator):

#define qeSRC_MAX_LEN           32      // Maximum length of a source name.
#define qeSRC_REMARK_MAX_LEN    256     // Maximum length of a data source
					// remark field.
//--------------------------------------------------------------------------

qeAPI(qeHANDLE) qeDatabases (
	qeHANDLE        hdbc);  // Connection Handle

// Creates a statement execution (hstmt) that will return information on
// the databases that are available to the connection represented by hdbc.
// The records can be subsequently read using the qeFetch functions, and
// column values can be retrieved using either the qeVal or qeBindCol
// functions.
// RETURNS: the statement execution (hstmt), or 0 if the function fails.
// When you have finished retrieving Databases (when you receive EOF from
// qeFetchNext), you must call qeEndSQL to terminate the hstmt.

// The resulting records contain the following columns:
//      Database        Char(128) - Database name
//      Remarks         Char(256) - Comment field for databases

//---------------------------------------------------------------------------
// Number of columns and column numbers:

#define qeDB_NUM_COLS           2       // Number of columns returned
#define qeDB_DATABASE_COL       1       // Database name
#define qeDB_REMARKS_COL        2       // Remarks

//---------------------------------------------------------------------------
// Maximum lengths of returned character column values (not including
// 0-terminator):

#define qeDB_MAX_LEN            128     // Maximum length of a database name.
#define qeDB_REMARK_MAX_LEN     256     // Maximum length of a database
					// remark field.
//--------------------------------------------------------------------------

qeAPI(qeHANDLE) qeTables (
	qeHANDLE        hdbc,           // Connection handle.
	qeLPSTR         qualifer_pattern,// Database (or directory) pattern.
	qeLPSTR         user_pattern,   // Pattern for selecting users.  If
					// the pattern is "", the current
					// user is assumed.  If the pattern
					// is "*", all users are selected.
					// Ignored for file-based databases,
					// for which the current working
					// directory is assumed.
	qeLPSTR         table_pattern,  // Pattern for selecting tables (or
					// files).  If the pattern is "",
					// all tables are selected.
	short           flags);         // Options that determine which
					// types of items are to be
					// returned.  See OPTIONS defined
					// below.

// Creates a statement execution (hstmt) that will return the set of
// database tables (or files) that can be accessed by the hdbc.
// RETURNS: the statement execution (hstmt), or 0 if the function fails.
// The records can be subsequently read using the qeFetch functions, and
// column values can be retrieved using either the qeVal or qeBindCol
// functions.
// When you have finished retrieving tables (when you receive EOF from
// qeFetchNext), you must call qeEndSQL to terminate the hstmt.

// The resulting records contain the following columns:
//      Table Qualifier Char(128)- Table qualifier.
//      Table User      Char(128)- Table owner.
//      Table Name      Char(128)- Table name.
//      Type            Short    - The type of item returned.  For
//                                 table-based sources, this can be used to
//                                 distinguish tables, views, stored
//                                 procedures, and synonym names, all of
//                                 which may be returned by this query.  For
//                                 file-based sources, the type is always
//                                 file.
//      Remarks         Char(256)- Remarks field for table.

//--------------------------------------------------------------------------
// OPTIONS: Values that may be used for the options parameter to qeTables.
// The value sent determines which types of items are to be returned by the
// hstmt. These values can be combined by adding them together.  For example
// qeTBL_TABLE+qeTBL_VIEW causes both tables and views to be returned.
// These are also the values that will be returned in the Type column.
// NOTE: qeTBL_DATABASE cannot be combined with the other values.

#define qeTBL_TABLE     0x0001          // Get table names
#define qeTBL_VIEW      0x0002          // Get view names
#define qeTBL_PROCEDURE 0x0004          // Get stored procedure names
#define qeTBL_SYSTABLE  0x0008          // Get system table names
#define qeTBL_SYNONYM   0x0010          // Get synonym names
#define qeTBL_DATABASE  0x0080          // Get database names
//--------------------------------------------------------------------------
// Number of columns and column numbers:

#define qeTBL_NUM_COLS          5       // Number of columns returned
#define qeTBL_TABLE_QUAL_COL    1       // Table qualifier
#define qeTBL_TABLE_OWNER_COL   2       // Table owner
#define qeTBL_TABLE_NAME_COL    3       // Table name
#define qeTBL_TYPE_COL          4       // Type of item returned
#define qeTBL_REMARK_COL        5       // Remarks
//--------------------------------------------------------------------------
// Maximum lengths of returned char column values (not including
// 0-terminator):

#define qeUSER_MAX_LEN          128     // Maximum length of a
					// user/directory name.
#define qeTBL_MAX_LEN           128     // Maximum length of a
					// table/file/qual name.
#define qeMAX_REMARK_LEN        256     // Maximum length of a remark fields.
//---------------------------------------------------------------------------

qeAPI(qeHANDLE) qeColumns (
	qeHANDLE        hdbc,           // Connection handle.
	qeLPSTR         table_name);    // Table for which columns are to be
					// returned.

// Creates a statement execution (hstmt) that will return the set of
// database Column definitions for a table (or file).
// RETURNS: the statement execution (hstmt), or 0 if the function fails.
// The records can be subsequently read using the qeFetch functions, and
// column values can be retrieved using either the qeVal or qeBindCol
// functions.
// When you have finished retrieving columns (when you receive EOF from
// qeFetchNext), you must call qeEndSQL to terminate the hstmt.

// The resulting records contain the following columns:
//      Table Qualifier Char(128)- Table qualifier
//      Table Owner Char(128)- Table owner
//      Table Name  Char(128)- Table name
//      Column      Char(128)- Column name
//      Type        Short    - Data type
//      Width       Short    - Width in bytes
//      DBType      Short    - Database data type
//      DBType Name Char(128)- Database name string
//      Attr1       Short    - Precision for Decimal, date start for Date
//      Attr2       Short    - Scale for Decimal, date end for Date
//      Nullable    Short    - Whether column is nullable
//      Remarks     Char(256)- Remarks field for column

//---------------------------------------------------------------------------
// Number of columns and column numbers:

#define qeCOL_NUM_COLS          12      // Number of columns in record

#define qeCOL_TABLE_QUAL_COL    1       // Table qualifier
#define qeCOL_TABLE_OWN_COL     2       // Table owner
#define qeCOL_TABLE_NAME_COL    3       // Table name
#define qeCOL_NAME_COL          4       // Column name
#define qeCOL_TYPE_COL          5       // Column type
#define qeCOL_WIDTH_COL         6       // Column width
#define qeCOL_DBTYPE_COL        7       // Column database type
#define qeCOL_DBTYPE_NAME_COL   8       // Column database type name
#define qeCOL_ATTR1_COL         9       // Column precision/date start
#define qeCOL_ATTR2_COL         10      // Column scale/date end
#define qeCOL_NULLABLE_COL      11      // Column nullable
#define qeCOL_REMARKS_COL       12      // Column remarks
//---------------------------------------------------------------------------
// Maximum length of returned char column value (not including 0-terminator):

#define qeCOL_MAX_LEN           128     // Maximum length of a column name
//--------------------------------------------------------------------------

qeAPI(qeHANDLE) qeTypeInfo (
	qeHANDLE        hdbc);  // Connection handle

// Creates a statement execution (hstmt) that will return the set of data
// types and their attributes for a database system.
// RETURNS: the statement execution (hstmt), or 0 if the function fails.
// The records can be subsequently read using the qeFetch functions, and
// column values can be retrieved using either the qeVal or qeBindCol
// functions.
// When you have finished retrieving TypeInfo (when you receive EOF from
// qeFetchNext), you must call qeEndSQL to terminate the hstmt.

// The resulting records contain the following columns:
//      Type_Name       Char(128) - Data source dependent data type name.
//      Type            Short     - Data type.
//      DB_Type         Short     - Database data type.
//      Width           Long      - Type size.
//      Attr1           Short     - Precision for Decimal,
//                                  date start for Date.
//      Attr2           Short     - Scale for Decimal, date end for Date.
//      Litteral_Prefix Char(128) - Characters used to prefix a literal,
//                                  null if not applicable.
//      Litteral_Suffix Char(128) - Characters used to terminate a literal.
//                                  null if not applicable.
//      Create_Params   Char(128) - The parameters necessary to use the type
//                                  in a create table statement
//                                  (Decimal would be "precision,scale").
//      Nullable        Short       Whether the type can be null.
//                                  Values: qeCOL_NULLABLE,
//                                  qeCOL_NOT_NULLABLE, and qeCOL_UNKNOWN.
//      Case_Sensitive  Short     - T/F, Whether the type can be treated as
//                                  case sensitive for sorting.
//      Searchable      Short     - How the type can be used in a WHERE
//                                  clause. Values: qeCOL_UNSEARCHABLE,
//                                  qeCOL_LIKE_ONLY, qeCOL_ALL_EXCEPT_LIKE,
//                                  qeCOL_SEARCHABLE.
//      Unsigned        Short     - T/F, whether type is unsigned, null if
//                                  not applicable.
//      Money           Short     - T/F, whether type is a money data type.
//      Auto_Increment  Short     - T/F, whether data type is
//                                  auto-incrementing.  Null if not
//                                  applicable.
//      Local_Type_Name Char(128) - Localized version of the data
//                                  source-dependent name of the data type.
//                                  Null if not applicable.

//---------------------------------------------------------------------------
// Number of columns and column numbers:

#define qeTYPE_NUM_COLS         16      // Number of columns in record

#define qeTYPE_TYPE_NAME_COL    1       // Database type name
#define qeTYPE_TYPE_COL         2       // QELIB type
#define qeTYPE_DBTYPE_COL       3       // Database type
#define qeTYPE_WIDTH_COL        4       // Bytes to represent type
#define qeTYPE_ATTR1_COL        5       // Precision/ date start
#define qeTYPE_ATTR2_COL        6       // Scale/ date end
#define qeTYPE_LIT_PREFIX_COL   7       // Literal prefix
#define qeTYPE_LIT_SUFFIX_COL   8       // Literal suffix
#define qeTYPE_CREATE_PARAM_COL 9       // Parameters in create table stmt
#define qeTYPE_NULLABLE_COL     10      // If type is nullable
#define qeTYPE_CASE_SENS_COL    11      // If type is case-sensitive for sort
#define qeTYPE_SEARCHABLE_COL   12      // If type can appear in WHERE clause
#define qeTYPE_UNSIGNED_COL     13      // If type is unsigned
#define qeTYPE_MONEY_COL        14      // If type is a money type
#define qeTYPE_AUTO_INCRE_COL   15      // If type is auto-incrementing
#define qeTYPE_LOCAL_NAME_COL   16      // Local type name
//---------------------------------------------------------------------------
// Maximum length of returned char column value (not including 0-terminator):

#define qeTYPE_MAX_LEN          128     // Maximum length of a column name
//---------------------------------------------------------------------------

qeAPI(qeHANDLE) qeProcedureColumns (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         proc_name);     // Procedure name

// Creates a statement execution (hstmt) that will return the set of
// database column definitions for a stored procedure.
// RETURNS: the statement execution (hstmt), or 0 if the function fails.
// The records can be subsequently read using the qeFetch functions, and
// column values can be retrieved using either the qeVal or qeBindCol
// functions.
// When you have finished retrieving procedure columns (when you receive
// EOF from qeFetchNext), you must call qeEndSQL to terminate the hstmt.

// The resulting records contain the following columns:
//      Proc Qualifier  Char(128)	- Proc qualifier.
//      Proc Owner  Char(128)		- Proc owner.
//      Proc Name   Char(128)		- Proc name.
//      Column      Char(128)		- Column name.
//      Col Type    Short		- If column is IN, OUT, or IN/OUT.
//					  From defines listed below.
//      Data Type   Short		- Data type.
//      DBType Name Short		- Database type name.
//      Width       Short		- Width in bytes.
//      Attr1       Short		- Precision for Decimal, date start
//					  for Date.
//      Attr2       Short		- Scale for Decimal, date end for Date.
//      Nullable    Short		- T/F Whether column is nullable.
//      Remarks     Char(256)		- Remarks field for column.

//---------------------------------------------------------------------------
// Number of columns and column numbers:

#define qePROC_NUM_COLS         12      // Number of columns in result.

#define qePROC_QUAL_COL         1       // Procedure qualifier.
#define qePROC_OWN_COL          2       // Procedure owner.
#define qePROC_NAME_COL         3       // Procedure name.
#define qePROC_COL_NAME_COL     4       // Column name.
#define qePROC_TYPE_COL         5       // See Col Type defines below.
#define qePROC_DATA_TYPE_COL    6       // Data type.
#define qePROC_DBTYPE_NAME_COL  7       // Database type name.
#define qePROC_WIDTH_COL        8       // Width in bytes.
#define qePROC_ATTR1_COL        9       // Precision / date start.
#define qePROC_ATTR2_COL        10      // Scale / date end.
#define qePROC_NULLABLE_COL     11      // Whether column can be null.
#define qePROC_REMARKS_COL      12      // Remarks for column.
//---------------------------------------------------------------------------
// Maximum length of returned char column value (not including 0-terminator):

#define qePROC_MAX_LEN          128     // Maximum length of a column name
//---------------------------------------------------------------------------
// Values for the Col Type column
#define qePARAM_UNKNOWN         1       // Don't know
#define qePARAM_INPUT           2       // Column only an input parameter
#define qePARAM_INOUT           3       // Column is an input/output param
#define qeRESULT_COL            4       // Column only an output parameter
//--------------------------------------------------------------------------

//--------------------------
// Error Handling functions
//--------------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeErr (void);
// RETURNS: the result code from the most recent call to QELIB.  The values
// that this function can return are defined at the top of this file, under
// the heading "Result Code Values."
//--------------------------------------------------------------------------
qeAPI(long) qeDBErr (void);
// RETURNS: the database system error code from the most recent call to
// a QELIB routine. These values are database-dependent, and do not have
// defines in QELIB.H.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeWarning (void);
// RETURNS: the warning code from the most recent call to QELIB.  This value
// is database-dependent, and there are no QELIB.H defines for it.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeErrMsg (void);
qeAPI(qeSTATUS) qeErrMsgBuf (
	qeLPSTR         msg_buf);       // Buffer for error message
// RETURNS: a pointer to the error messages from the most recent call to
// QELIB, either as the return value (qeErrMsg) or in a provided buffer
// (qeErrMsgBuf).  Multiple errors will be concatenated up to
// qeMAX_ERR_MSG_LEN bytes.  msg_buf must be at least this large, or an
// error may result.

#define qeMAX_ERR_MSG_LEN 512   // Maximum length of an error message (not
				// including 0-terminator).
//--------------------------------------------------------------------------

//---------------------
// Debugging functions
//---------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeTraceOn (
	qeLPSTR         file_pathname); // Pathname of trace file
// Turns debug tracing on. By default, tracing of all non-qeVal calls is
// enabled, as are calls to qeTraceUser. RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeTraceOff (void);
// Turns debug tracing off. RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetTraceOptions (
	short           flags);         // Flags to set, from flags below
// Sets the type of information that is sent to the trace file.  Each call
// resets the options to the value passed in flags. The default when
// qeTraceOn is called will be qeTRACE_NON_VAL_CALLS + qeTRACE_USER.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeGetTraceOptions (void);
// Gets the type of information that is sent to the trace file, from the flags
// below.
// RETURNS: current trace options.
//---------------------------------------------------------------------------
// Options for qeSet/GetTraceOptions
#define qeTRACE_NON_VAL_CALLS   0x0001 // Trace all calls except qeValXXX
#define qeTRACE_USER            0x0002 // Write calls from qeTraceUser
#define qeTRACE_VAL_CALLS       0x0004 // Trace qeValXXX calls
#define qeTRACE_WINDOW          0x0008 // Write trace info to a window
#define qeTRACE_ODBC            0x0010 // Tell ODBC driver to turn on tracing
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeTraceUser (
	qeLPSTR         trace_string);  // String to write to trace file
// Writes the trace_string to the tracefile, if tracing is turned on and the
// tracing of user strings are enabled (qeTRACE_USER bit is set).  Otherwise,
// it does nothing. RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetDriverTracefile (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         file_name);     // Pathname of file for ODBC tracing
// Sets a file as the target of the ODBC trace file.  The ODBC trace file
// logs all calls to the ODBC driver.  If ODBC tracing is enabled, and no
// file is specified, it will be written to sql.log by default.
// RETURNS: a result code.
//--------------------------------------------------------------------------

//-------------------------------
// Task functions
//-------------------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeLibInit (void);
// Performs task initialization.  Each task that calls QELIB should call
// this first, if possible.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeLibTerm (void);
// Performs task termination.  Each task that calls QELIB should call
// this when it is finished using QELIB, if possible.  RETURNS: a result code.
//--------------------------------------------------------------------------

//-------------------------------
// Database Connection functions
//-------------------------------

//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeConnect (
	qeLPSTR         con_string);    // Connection string
// Opens a connection to a database system and creates a handle to the
// connection (hdbc).  RETURNS: the connection handle, or 0 if connect fails.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeDisconnect (
	qeHANDLE        hdbc);          // Connection handle
// Closes a connection to a database system.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetLoginTimeout (
	long            seconds);       // Time to wait before timeout, in
					// seconds
// Sets the login timeout for connecting to a database.  This may have no
// effect on an individual connection, depending on whether the database
// system has implemented this feature.  The default is 15 seconds.  Setting
// the value to 0 indicates that logins are to wait indefinitely.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeGetLoginTimeout (void);
// Gets the login timeout for connecting to a database. The default is 15
// seconds.  A value of 0 indicates that logins are to wait indefinitely.
// RETURNS: the current login timeout.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetDB (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         database);      // Database to become current
// Change the current database.  This becomes the default database when
// SQL statements are executed.  This is only supported by a few database
// systems.  RETURNS: a result code.
//--------------------------------------------------------------------------

//-------------------------
// SQL execution functions
//-------------------------

//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeSQLPrepare (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         stmt);          // SQL statement to prepare
// Prepares an SQL statement for execution.  Following prepare, qeSetParam
// can be called to set parameter values. If sql_stmt is null, the statement
// is retrieved from the hdbc's SQL statement buffer, which is filled by
// calls to qeSetSQL and qeAppendSQL.
// To execute the statement, call qeSQLExecute.
// RETURNS: a handle to statement execution (hstmt), or 0 if the function
// fails.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSQLExecute (
	qeHANDLE        hstmt);		// Statement handle
// Executes an SQL statement that was previously prepared.
// RETURNS: a handle to statement execution (hstmt), or 0 if the function
// fails.
//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeExecSQL (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         sql_stmt);      // SQL statement to execute
// Executes an SQL statement.  If sql_stmt is null, the statement is retrieved
// from the hdbc's SQL statement buffer, which is filled by calls to
// qeSetSQL and qeAppendSQL.
// RETURNS: a handle to statement execution (hstmt), or 0 if the function
// fails.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetSQL (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         partial_stmt);  // Initial part of SQL statement
// Assigns sql_stmt to hdbc's SQL statement buffer.  Subsequent calls
// to qeAppend SQL will concatenate to the SQL statement to build the
// complete statement in pieces.
// RETURNS: a result code.
// Used to send SQL statements to QELIB in pieces.  Used primarily
// by Excel and other macro languages that have limits on the size
// of a character string.  Not needed by C programmers.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeAppendSQL (
	qeHANDLE        hdbc,           // Handle to database connection
	qeLPSTR         partial_stmt);  // Part of a SQL statement
// Appends partial_stmt to hdbc's SQL statement buffer.
// RETURNS: a result code.
// Used to send SQL statements to QELIB in pieces.  Used primarily
// by Excel and other macro languages that have limits on the size
// of a character string.  Not needed by C programmers.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeEndSQL (
	qeHANDLE        hstmt);         // Statement handle
// Ends a SQL statement's execution.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeNumModRecs (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the number of records affected by Insert, Update, or Delete
// SQL statements, or 0 if the statement is a Select statement.
//--------------------------------------------------------------------------

//-----------------------------
// Column definition functions
//-----------------------------

//--------------------------------------------------------------------------
qeAPI(short) qeNumCols (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the number of columns in a Select statement, or 0 for
// non-SELECT statements.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeColName (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
qeAPI(qeSTATUS) qeColNameBuf (
	qeHANDLE        hstmt,          // Statement Handle
	qeLPSTR         col_name,       // Buffer for column name
	short           col_num);       // Column number (1-origin)
// RETURNS: the name of the col_numth column in a SELECT statement, either
// as a return value (qeColName) or in a provided buffer (qeColNameBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeColAlias (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
qeAPI(qeSTATUS) qeColAliasBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         col_alias,      // Buffer for column alias
	short           col_num);       // Column number (1-origin)
// RETURNS: the alias of the col_numth column in a SELECT statement, either
// as a return value (qeAliasName) or in a provided buffer (qeAliasNameBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeColExpr (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
qeAPI(qeSTATUS) qeColExprBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         col_expr,       // Buffer for column expression
	short           col_num);       // Column number (1-origin)
// RETURNS: the expression of the col_numth column in a SELECT statement,
// either as a return value (qeColExpr) or in a provided buffer (qeColExprBuf).
//--------------------------------------------------------------------------
qeAPI(short) qeColType (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the QELIB data type of a column in a Select statement.
// See the top of this file for the list of QELIB data types.
//--------------------------------------------------------------------------
qeAPI(short) qeColDBType (
	qeHANDLE        hstmt,          // Statment handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the database data type of a column in a Select statement.
// See the top of this file for the list of values this function returns.
//--------------------------------------------------------------------------
qeAPI(long) qeColWidth (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the width (in bytes) of a column in a Select statement.
// Includes the zero terminator for Character or Date data types.
//--------------------------------------------------------------------------
qeAPI(short) qeColPrecision (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the number of significant digits for Decimal columns in a Select
// statement.
//--------------------------------------------------------------------------
qeAPI(short) qeColScale (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the number of digits to the right of decimal point for Decimal
// columns in a Select statement.
//--------------------------------------------------------------------------
qeAPI(short) qeColDateStart (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the offset of the first significant character (0-origin) for
// Date/Time columns in a Select statement.
//--------------------------------------------------------------------------
qeAPI(short) qeColDateEnd (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: the offset of the last significant character (0-origin) for
// Date/Time columns in a Select statement.
//--------------------------------------------------------------------------
qeAPI (short) qeColTypeAttr (
	qeHANDLE        hstmt,          // Identifies a statement execution
	short           col_num,        // Identifies column in statement
	short           attribute);     // Identifies the specific attribute
					// for which you are checking
// RETURNS: A result that reports the support of the specified attribute
// in the specified column of a Select statement.  Attributes and values are
// listed in defines below.
//---------------------------------------------------------------------------
// These constants are results from qeColTypeAttr.
// They are also returned in the result sets of several of the data
// dictionary routines.

#define qeCOL_UNKNOWN           100		// Database cannot tell
						// the value of the
						// attribute.

#define qeATTRIBUTE_UPDATABLE   1
// Tells whether the column may be modified in an UPDATE statement.

// qeCOL_UNKNOWN is a possible return value.
#define qeCOL_READ_ONLY         0		// Column is read-only
#define qeCOL_WRITABLE          1		// Column may be updated


#define qeATTRIBUTE_NULLABLE    2
// Tells whether the column may hold the value NULL.

// qeCOL_UNKNOWN is a possible return value
#define qeCOL_NOT_NULLABLE      0		// Column may not be NULL
#define qeCOL_NULLABLE          1		// Column may be NULL

#define qeATTRIBUTE_SEARCHABLE  3
// Tells whether the column may be used in a WHERE clause to search for
// records within a table.

// qeCOL_UNKNOWN is possible return value
#define qeCOL_UNSEARCHABLE      0		// Column may not appear in a
						// WHERE clause
#define qeCOL_LIKE_ONLY         1		// Column may only appear in a
						// WHERE clause with the LIKE
						// operator
#define qeCOL_ALL_EXCEPT_LIKE   2		// Column may appear in a WHERE
						// clause with any operator
						// except LIKE
#define qeCOL_SEARCHABLE        3		// Column may appear in a WHERE
						// clause


#define qeATTRIBUTE_UNSIGNED    4
// Tells whether the column may hold unsigned values.  Only meaningful for
// numeric data types.

#define qeCOL_SIGNED            0		// Column can only contain
						// signed values
#define qeCOL_UNSIGNED          1		// Column may contain unsigned
						// values

#define qeATTRIBUTE_MONEY       5
// Tells whether the column represents currency data types.

#define qeCOL_NOT_MONEY         0		// Column is not a MONEY type
#define qeCOL_MONEY             1		// Column is a MONEY type

#define qeATTRIBUTE_AUTO_INCRE  6
// Tells whether the column holds values which may be changed by the database
// whenever the row changes.

#define qeCOL_NOT_AUTO_INCRE    0		// Column is not auto-increment
#define qeCOL_AUTO_INCRE        1		// Column is auto-increment

//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeColDBTypeName (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
qeAPI(qeSTATUS) qeColDBTypeNameBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         type_name,      // Result buffer
	short           col_num);       // Column number (1-origin)
// RETURNS: the database type name for a column in a SELECT statement.  The
// value will be returned as the result of the function (qeColDBTypeName) or
// in the supplied buffer (qeColDBTypeNameBuf).
//--------------------------------------------------------------------------

//-------------------------
// Data fetching functions
//-------------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindCol (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPVOID        value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr);       // Buffer to receive column value
					// length
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get
// a record from the database and put the column values and their lengths
// in the buffers. When called, len_ptr must specify the size of value_ptr
// in bytes.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindColChar (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPSTR         value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr,        // Buffer to receive column value
					// length
	qeLPSTR         fmt_string);    // Display format
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get
// a record from the database and put the column values and their lengths
// in the buffers.  Converts the value to CHAR.
// When called, len_ptr must hold the size (in bytes) of value_ptr.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindColInt (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPINT         value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr);       // Buffer to receive column value
					// length
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get
// a record from the database and put the column values and their lengths
// in the buffers.  Converts the value to INT.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindColLong (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPLONG        value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr);       // Buffer to receive column value
					// length
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get a
// record from the database and put the column values and their lengths
// in the buffers.  Converts the value to LONG.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindColFloat (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPFLT         value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr);       // Buffer to receive column value
					// length
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get
// a record from the database and put the column values and their lengths
// in the buffers.  Converts the value to FLOAT.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindColDouble (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPDBL         value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr);       // Buffer to receive column value
					// length
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get
// record from the database and put the column values and their lengths
// in the buffers.  Converts the value to DOUBLE.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindColDecimal (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPDEC         value_ptr,      // Buffer to receive column values
	qeLPLONG        len_ptr,        // Buffer to receive column value
					// length
	short           precision,      // User requested precision
	short           scale);         // User requested scale
// Specifies the buffers to receive the value and lengths for one column
// from a Select statement.  Subsequent calls to qeFetch functions will get
// record from the database and put the column values and their lengths
// in the buffers.  Converts the value to DECIMAL.
// When called, len_ptr must contain the size of value_ptr in bytes.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeFetchSetOptions (
	qeHANDLE        hstmt,          // Statement handle
	long            options);       // Option flags
// Sets options that determine which fetch commands are allowed.  Options
// are defined below.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeFetchGetOptions (
	qeHANDLE        hstmt);		// Statement handle
// RETURNS: options that determine which fetch commands are allowed, from
// defines below.
//---------------------------------------------------------------------------
// Option flag values for qeFetchSet/GetOptions

#define qeFETCH_FORWARD     0x0000      // Only Fetch Next is allowed.
#define qeFETCH_RANDOM      0x0001      // Whether Random and Previous
					// fetching is enabled.

#define qeFORCE_LOG         0x0002      // Whether to force a log file when
					// it is not required.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetSelectOptions (
	qeHANDLE        hdbc,           // Connection handle
	long            flags);         // Option flags
// Sets options that determine which fetch commands and cursor behaviors
// are allowed.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeGetSelectOptions (
	qeHANDLE        hdbc);          // Connection handle
// RETURNS: options that determine which fetch commands are allowed.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Option flag values for qeSet/GetSelectOptions

#define qeFETCH_FORWARD_DIR 0x0001      // Only Fetch in the forward
					// direction is allowed.
#define qeFETCH_ANY_DIR     0x0002      // Fetching is forward or backward
					// enabled.

#define qeLOG_IF_NEEDED     0x0008      // Log only if needed for fetch
					// direction support.
#define qeLOG_ALWAYS        0x0010      // Whether to force a log file when
					// it is not required.

#define qeSELECT_INVALIDATE 0x0020      // Disable fetching at EOT.
#define qeSELECT_TRUNCATE   0x0040      // Truncate the select solution set
					// at EOT.
#define qeSELECT_PERSIST    0x0060      // Select persists at EOT.

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetOneHstmtPerHdbcOptions (
	qeHANDLE        hdbc,           // Connection handle
	long            flags);         // New options
// Sets options that determine which fetch commands and cursor behaviors
// are allowed, from defines below.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeGetOneHstmtPerHdbcOptions (
	qeHANDLE        hdbc);          // Connection handle
// RETURNS: options that determine which fetch commands are allowed, from
// defines below.
//--------------------------------------------------------------------------
// Option flag values for qeSet/GetOneHstmtPerHdbcOptions

#define qeREADAHEAD_AT_EXEC        0x0001  // Read entire solution set into
					   // log at execute time.
#define qeREADAHEAD_AT_UPDATE      0x0002  // Read remainder of solution set
					   // into log at update, delete, or
					   // lock.
#define qeREADAHEAD_COMMIT_UPDATES 0x0003  // All updates must be commited
					   // before any additional fetching.

#define qeROUTING_READ             0x0008  // Sends the statement to a read
					   // connection.
#define qeROUTING_UPDATE           0x0010  // Sends the statement to the
					   // transaction connection.
#define qeROUTING_DEFAULT          0x0018  // Lets qelib decide which
					   // connection to send the
					   // statement.

#define qeHSTMT_LOCAL              0x0020  // States that this hstmt cannot
					   // affect any other active hstmts
					   // in the same application.
#define qeHSTMT_NONLOCAL           0x0040  // States that this hstmt may
					   // affect other hstmts in the
					   // same application.
qeAPI(long) qeRecNum (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the current record number.  Returns 0 if there is no current
// record.
//--------------------------------------------------------------------------
qeAPI(short) qeRecState (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the state of current record, from the values defined below.
//---------------------------------------------------------------------------
// Values to be returned from qeRecState:
#define qeSTATE_NEW             1 // A new record which has not been sent to
				  // the database.
#define qeSTATE_UNCHANGED       2 // Record has no changes waiting to be
				  // sent to the database.
#define qeSTATE_CHANGED         3 // Record has changes waiting to be sent
				  // to the database.
#define qeSTATE_NOREC           4 // The cursor is not currently positioned
				  // on a record.
#define qeSTATE_NEW_CHANGED     5 // A new record which has not been sent to
				  // the database, and which has fields which
				  // have been updated.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeFetchLogClose (
	qeHANDLE        hstmt);         // Statement handle
// Closes the log files associated with an hstmt.  These files will
// automatically be reopened on the next qeFetch call.  Used only
// to reduce the number of open files.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeFetchNext (
	qeHANDLE        hstmt);         // Statement handle
// Gets the next record from the database system.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeFetchPrev (
	qeHANDLE        hstmt);         // Statement handle
// Gets the previous record from the database system.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeFetchRandom (
	qeHANDLE        hstmt,          // Statement handle
	long            rec_num);       // Record number (1-origin)
// Gets a record from the database system given its record number.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeFetchNumRecs (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the number of records resulting from the Select statement
// represented by hstmt.  Returns 0 otherwise. Can only be used if
// qeFetchSetOptions or qeSetSelectOptions is called to enable random fetching.
//--------------------------------------------------------------------------
qeAPI(long) qeDataLen (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the length of the column value most recently retrieved via a
// call to a qeVal function.  If the value is NULL, qeNULL_DATA (-2) is
// returned.  If the values is longer than the amount returned,
// (e.g., qeValChar was called with a non-zero req_len and the
// value was longer than req_len), qeTRUNCATION (-1) is returned.
// If a field is returned in chunks, the value returned only reflects the
// last chunk received.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeValChar (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPSTR         fmt_string,     // Controls formatting of dates and
					// numbers
	short           max_len);       // The maximum length of data
					// to be returned
qeAPI(qeSTATUS) qeValCharBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         char_val,       // Buffer for value
	short           col_num,        // Column number (1-origin)
	qeLPSTR         fmt_string,     // Controls formatting of dates and
					// numbers
	short           max_len);       // The requested maximum length of
					// data to be returned
// Retrieves a column value as a character string, and returns
// it (qeValChar) or puts it in a user-defined buffer (qeValCharBuf).  The
// value will have a zero-terminator appended to it.
// RETURNS: the column value as a character string (qeValChar), or a result
// code (qeValCharBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeValMultiChar (
	qeHANDLE        hstmt,          // Handle to Select statement
	short           start_col_num,  // First column to return
	short           end_col_num,    // Last column to return
	qeLPSTR         num_fmt_string, // Numeric format string
	qeLPSTR         date_fmt_string, // Date format string
	qeLPSTR         separator);     // Character separator
qeAPI(qeSTATUS) qeValMultiCharBuf (
	qeHANDLE        hstmt,          // Handle to Select statement
	qeLPSTR         dest_buf,       // Buffer for result string
	short           start_col_num,  // First column to return
	short           end_col_num,    // Last column to return
	qeLPSTR         num_fmt_string, // Numeric format string
	qeLPSTR         date_fmt_string, // Date format string
	qeLPSTR         separator);     // Character separator
// Returns/retrieves a pointer to a set of column values in a single
// character string.  Each column's value will be separated by the
// specified character (typically tab or comma).  The last value will be
// followed by a zero-terminator character.  All numeric values will be
// formatted using the num_fmt_string and all date values will be formatted
// using the date_fmt_string.
// RETURNS: the pointer to the set of column values (qeValMultiChar), or a
// result code (qeValMultiCharBuf).
//--------------------------------------------------------------------------
qeAPI(short) qeValInt (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: a column value as a 2-byte integer.
//--------------------------------------------------------------------------
qeAPI(long) qeValLong (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// RETURNS: a column value as a 4-byte integer.
//--------------------------------------------------------------------------
qeAPI(qeRETFLT) qeValFloat (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num         // Column number (1-origin)
	qeHIDDENFLT);                   // See Borland notes
// RETURNS: a column value as a 4-byte floating point number.
//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeValDouble (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num         // Column number (1-origin)
	qeHIDDENDBL);                   // See Borland notes
// RETURNS: a column value as an 8-byte floating point number
//--------------------------------------------------------------------------
qeAPI(qeLPDEC) qeValDecimal (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	short           precision,      // Number of significant digits in
					// result
	short           scale);         // Number of digits right of decimal
					// in result
qeAPI(qeSTATUS) qeValDecimalBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         dec_val,        // Buffer for value
	short           col_num,        // Column number (1-origin)
	short           precision,      // Number of significant digits in
					// result
	short           scale);         // Number of digits right of decimal
					// in result
// Returns/Retrieves a column value as a decimal number in BCD format.
// RETURNS: the column value (qeValDecimal), or a result code
// (qeValDecimalBuf).
//--------------------------------------------------------------------------

//-----------------------
// Update functions
//-----------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetKey (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	short           value);         // TRUE/FALSE column is part of key
// Declares that the column is part of a primary key for the record.
// The primary key is one or more columns that together uniquely identify
// records.  This is used by qeRecUpdate and qeRecDelete to identify the record
// that is to be deleted.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeRecGetKey (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// Returns whether the column is part of a primary key for the record.
// The primary key is one or more columns that together uniquely identify
// records.  This is used by qeRecUpdate and qeRecDelete to identify the record
// that is to be deleted.
// RETURNS: TRUE (1) if the column is part of the key, FALSE (0) otherwise.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecNew (
	qeHANDLE        hstmt,          // Statement handle
	long            rec_num);       // Position for the new record
// Creates a new record buffer for a record to be inserted as record number
// rec_num.  Following this, values may be written to the buffer with qePut*
// functions, and the record can be inserted with qeRecUpdate.  RETURNS: a
// result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecLock (
	qeHANDLE        hstmt);		// Statement handle
// Locks the current record.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecUpdate (
	qeHANDLE        hstmt);		// Statement handle
// Updates the current record with new values set using qePut functions.
// This will Insert records created via qeRecNew.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecDelete (
	qeHANDLE        hstmt);		// Statement handle
// Deletes the current record.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecUndo (
	qeHANDLE        hstmt);		// Statement handle
// Undoes all changes that have been made to the current record.  This will
// undo changes made to the current record that have not been sent to the
// database via qeRecUpdate.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutChar (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPSTR         fmt_string,     // Controls formatting of dates and
					// numbers
	qeLPSTR         new_val);       // New value for this column
// Updates a column value in the current record with a character value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutBinary (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	qeLPSTR         new_val,        // New value for this column
	long            val_len);       // Length of new value in bytes
// Updates a column value in the current record with a binary value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutDecimal (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	short           precision,      // Number of significant digits in
					// result
	short           scale,          // Number of digits right of decimal
					// in result
	qeLPSTR         new_val);       // New value for this column
// Updates a column value in the current record with a decimal value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutDouble (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	double          new_val);       // New value for this column
// Updates a column value in the current record with a double value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutFloat (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	float           new_val);       // New value for this column
// Updates a column value in the current record with a float value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutInt (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	short           new_val);       // New value for this column
// Updates a column value in the current record with an integer value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutLong (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-origin)
	long            new_val);       // New value for this column
// Updates a column value in the current record with a long value.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutNull (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num);       // Column number (1-origin)
// Updates a column value in the current record to have a value of NULL.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qePutUsingBindColumns (
	qeHANDLE        hstmt);		// Statement handle
// Updates the column values in the current record to reflect the values
// in the current bind buffers.  If the length buffer for a column is equal to
// qeNO_DATA_CHANGE, then that column is ignored and the column is left
// unchanged.  RETURNS: a result code.

#define qeNO_DATA_CHANGE        (-9)    // Indicates column has not changed
//--------------------------------------------------------------------------

//-----------------------
// Transaction functions
//-----------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBeginTran (
	qeHANDLE        hdbc);          // Connection handle
// Begins a transaction. RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeCommit (
	qeHANDLE        hdbc);          // Connection handle
// Commits all changes since qeBeginTran and ends the transaction.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRollback (
	qeHANDLE        hdbc);          // Connection handle
// Rolls back all changes since qeBeginTran and ends the transaction.
// RETURNS: a result code.
//--------------------------------------------------------------------------

//---------------------------
// Data Conversion Functions
//---------------------------

//--------------------------------------------------------------------------

	//---------------------
	// To Hex String
	//---------------------

//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeBinToHex (
	qeLPSTR         bin_value,      // Value to convert
	short           length);        // Length of value to convert
qeAPI(qeSTATUS) qeBinToHexBuf (
	qeLPSTR         hex_value,      // Buffer for converted value
	qeLPSTR         bin_value,      // Value to convert
	short           length);        // Length of value to convert
// Converts a binary value into a hex string.
// RETURNS: the converted value (qeBinToHex), or a result code (qeBinToHexBuf).
//--------------------------------------------------------------------------

	//---------------------
	// From Hex String
	//---------------------

//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeHexToBin (
	qeLPSTR         hex_value,      // Value to convert
	long            length);        // Length of value to convert
qeAPI(qeSTATUS) qeHexToBinBuf (
	qeLPSTR         bin_value,      // Buffer for converted value
	qeLPSTR         hex_value,      // Value to convert
	long            length);        // Length of value to convert
// Converts a hex string into a binary value.
// RETURNS: the converted value (qeHexToBin), or a result code (qeHexToBinBuf).
//--------------------------------------------------------------------------

	//---------------------
	// To Character String
	//---------------------

//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeDateToChar (
	qeLPSTR         date_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeDateToCharBuf (
	qeLPSTR         char_val,       // Buffer for character string result
	qeLPSTR         date_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a standard date value to a character string formatted using
// the format string.
// RETURNS: the converted value (qeDateToChar), or a result code
// (qeDateToCharBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeIntToChar (
	short           int_val,        // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeIntToCharBuf (
	qeLPSTR         char_val,       // Buffer for character string result
	short           int_val,        // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a 2-byte integer value to a character string formatted using
// the format string.
// RETURNS: the converted value (qeIntToChar), or a result code
// (qeIntToCharBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeLongToChar (
	long            long_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeLongToCharBuf (
	qeLPSTR         char_val,       // Buffer for character string result
	long            long_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a 4-byte integer value to a character string formatted using
// the format string.
// RETURNS: the converted value (qeLongToChar), or a result code
// (qeLongToCharBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeFloatToChar (
	float           flt_val,        // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeFloatToCharBuf (
	qeLPSTR         char_val,       // Buffer for character string result
	float           flt_val,        // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a 4-byte floating value to a character string formatted using
// the format string.
// RETURNS: the converted value (qeFloatToChar), or a result code
// (qeFloatToCharBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeDoubleToChar (
	double          dbl_val,        // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeDoubleToCharBuf (
	qeLPSTR         char_val,       // Buffer for character string result
	double          dbl_val,        // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts an 8-byte floating value to a character string formatted using
// the format string.
// RETURNS: the converted value (qeDoubleToChar), or a result code
// (qeDoubleToCharBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeDecimalToChar (
	qeLPDEC         dec_val,        // Value to convert
	short           precision,      // Number of significant digits in
					// value
	short           scale,          // Number of digits right of decimal
					// in value
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeDecimalToCharBuf (
	qeLPSTR         char_val,       // Buffer for character string result
	qeLPDEC         dec_val,        // Value to convert
	short           precision,      // Number of significant digits in
					// value
	short           scale,          // Number of digits right of decimal
					// in value
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a decimal value to a character string formatted using the format
// string.
// RETURNS: the converted value (qeDecimalToChar), or a result code
// (qeDecimalToCharBuf).
//-------------------------------------------------------------------------

	//------------
	//   To Date
	//------------

//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeCharToDate (
	qeLPSTR         char_value,     // Character string of a formatted date
					// value
	qeLPSTR         format_string); // Format template
qeAPI(qeSTATUS) qeCharToDateBuf (
	qeLPSTR         date_value,     // Destination buffer
	qeLPSTR         char_value,     // Character string of a formatted date
					// value
	qeLPSTR         fmt_string);    // Format template
// Converts a char string of a formatted date value into the default date
// value.
// RETURNS: the converted value (qeCharToDate), or a result code
// (qeCharToDateBuf).
//--------------------------------------------------------------------------

	//------------
	// To Integer
	//------------

//--------------------------------------------------------------------------
qeAPI(short) qeCharToInt (
	qeLPSTR         char_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a character string to a 2-byte integer.  The character string
// is formatted as specified by the format_string.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(short) qeLongToInt (
	long            long_val);      // Value to convert
// Converts a 4-byte integer to a 2-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(short) qeFloatToInt (
	float           flt_val);       // Value to convert
// Converts a 4-byte float to a 2-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(short) qeDoubleToInt (
	double          dbl_val);       // Value to convert
// Converts a 8-byte float to a 2-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(short) qeDecimalToInt (
	qeLPDEC         dec_val,        // Value to convert
	short           precision,      // Number of significant digits in
					// value
	short           scale);         // Number of digits right of decimal
					// in value
// Converts a decimal number to a 2-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------

	//-----------------
	// To Long Integer
	//-----------------

//--------------------------------------------------------------------------
qeAPI(long) qeCharToLong (
	qeLPSTR         char_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a character string to a 4-byte integer.  The character string
// is formatted as specified by the format_string.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(long) qeDateToLong (
	qeLPSTR         date_val);      // Value to convert
// Converts a standard date value to a Julian value returned as a 4-byte
// integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(long) qeIntToLong (
	short           int_val);       // Value to convert
// Converts a 2-byte integer to a 4-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(long) qeFloatToLong (
	float           flt_val);       // Value to convert
// Converts a 4-byte float to a 4-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(long) qeDoubleToLong (
	double          dbl_val);       // Value to convert
// Converts a 8-byte float to a 4-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------
qeAPI(long) qeDecimalToLong (
	qeLPDEC         dec_val,        // Value to convert
	short           precision,      // Number of significant digits in
					// value
	short           scale);         // Number of digits right of decimal
					// in value
// Converts a decimal number to a 4-byte integer.
// RETURNS: the converted integer.
//--------------------------------------------------------------------------

	//-------------------
	// To Floating Point
	//-------------------

//--------------------------------------------------------------------------
qeAPI(qeRETFLT) qeCharToFloat (
	qeLPSTR         char_val,       // Value to convert
	qeLPSTR         fmt_string      // Format string for the conversion
	qeHIDDENFLT);                   // See Borland notes
// Converts a character string to a 4-byte float.  The character string
// is formatted as specified by the format_string.
// RETURNS: the converted float.
//--------------------------------------------------------------------------
qeAPI(qeRETFLT) qeIntToFloat (
	short           int_val         // Value to convert
	qeHIDDENFLT);                   // See Borland notes
// Converts a 2-byte integer to a 4-byte float.
// RETURNS: the converted float.
//--------------------------------------------------------------------------
qeAPI(qeRETFLT) qeLongToFloat (
	long            long_val        // Value to convert
	qeHIDDENFLT);                   // See Borland notes
// Convert a 4-byte integer to a 4-byte float.
// RETURNS: the converted float.
//--------------------------------------------------------------------------
qeAPI(qeRETFLT) qeDoubleToFloat (
	double          dbl_val         // Value to convert
	qeHIDDENFLT);                   // See Borland notes
// Converts an 8-byte float to a 4-byte float.
// RETURNS: the converted float.
//--------------------------------------------------------------------------
qeAPI(qeRETFLT) qeDecimalToFloat (
	qeLPDEC         dec_value,      // Value to convert
	short           precision,      // Number of significant digits in
					// value
	short           scale           // Number of digits right of decimal
					// in value
	qeHIDDENFLT);                   // See Borland notes
// Converts a decimal number to a 4-byte float.
// RETURNS: the converted float.
//--------------------------------------------------------------------------

	//------------------------------------
	// To Double Precision Floating Point
	//------------------------------------

//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeCharToDouble (
	qeLPSTR         char_val,       // Value to convert
	qeLPSTR         fmt_string      // Format string for the conversion
	qeHIDDENDBL);                   // See Borland notes
// Converts a character string to an 8-byte float.  The character string
// is formatted as specified by the format_string.
// RETURNS: the converted double.
//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeDateToDouble (
	qeLPSTR         date_val        // Value to convert
	qeHIDDENDBL);                   // See Borland notes
// Converts a standard date value to a Julian value returned as an 8-byte
// float.
// RETURNS: the converted double.
//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeIntToDouble (
	short           int_val         // Value to convert
	qeHIDDENDBL);                   // See Borland notes
// Converts a 2-byte integer to an 8-byte float.
// RETURNS: the converted double.
//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeLongToDouble (
	long            long_val        // Value to convert
	qeHIDDENDBL);                   // See Borland notes
// Converts a 4-byte integer to an 8-byte float.
// RETURNS: the converted double.
//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeFloatToDouble (
	float           flt_value       // Value to convert
	qeHIDDENDBL);                   // See Borland notes
// Converts a 4-byte float to an 8-byte float.
// RETURNS: the converted double.
//--------------------------------------------------------------------------
qeAPI(qeRETDBL) qeDecimalToDouble (
	qeLPDEC         dec_val,        // Value to convert
	short           precision,      // Number of significant digits in
					// value
	short           scale           // Number of digits right of decimal
					// in value
	qeHIDDENDBL);                   // See Borland notes
// Converts a decimal number to an 8-byte float.
// RETURNS: the converted double.
//--------------------------------------------------------------------------

	//------------
	// To Decimal
	//------------

//--------------------------------------------------------------------------
qeAPI(qeLPDEC) qeCharToDecimal (
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	qeLPSTR         char_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
qeAPI(qeSTATUS) qeCharToDecimalBuf (
	qeLPDEC         dec_val,        // Buffer for the decimal result
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	qeLPSTR         char_val,       // Value to convert
	qeLPSTR         fmt_string);    // Format string for the conversion
// Converts a character string to a decimal number.
// RETURNS: the converted decimal (qeCharToDecimal), or a result code
// (qeCharToDecimalBuf)
//--------------------------------------------------------------------------
qeAPI(qeLPDEC) qeIntToDecimal (
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	short           int_val);       // Value to convert
qeAPI(qeSTATUS) qeIntToDecimalBuf (
	qeLPDEC         dec_val,        // Buffer for the decimal result
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	short           int_val);       // Value to convert
// Converts a 2-byte integer to a decimal number.
// RETURNS: the converted decimal value (qeIntToDecimal), or a result code
// (qeIntToDecimalBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPDEC) qeLongToDecimal (
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	long            long_val);      // Value to convert
qeAPI(qeSTATUS) qeLongToDecimalBuf (
	qeLPDEC         dec_val,        // Buffer for the decimal result
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	long            long_val);      // Value to convert
// Convert a 4-byte integer to a decimal number.
// RETURNS: the converted decimal value (qeLongToDecimal), or a result code
// (qeLongToDecimalBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPDEC) qeFloatToDecimal (
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	float           flt_val);       // Value to convert
qeAPI(qeSTATUS) qeFloatToDecimalBuf (
	qeLPDEC         dec_val,        // Buffer for the decimal result
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	float           flt_val);       // Value to convert
// Converts a 4-byte float to a decimal number.
// RETURNS: the converted decimal value (qeFloatToDecimal), or a result code
// (qeFloatToDecimalBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPDEC) qeDoubleToDecimal (
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	double          dbl_val);       // Value to convert
qeAPI(qeSTATUS) qeDoubleToDecimalBuf (
	qeLPDEC         dec_val,        // Buffer for the decimal result
	short           precision,      // Number of significant digits for
					// the result
	short           scale,          // Number of digits right of decimal
					// for the result
	double          dbl_val);       // Value to convert
// Converts an 8-byte float to a decimal number.
// RETURNS: the converted decimal value (qeDoubleToDecimal), or a result code
// (qeDoubleToDecimalBuf).
//--------------------------------------------------------------------------

//-----------------------
// System Info functions
//-----------------------

//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeVerNum (void);
qeAPI(qeSTATUS) qeVerNumBuf (
	qeLPSTR         ver_num);       // Buffer for the version number
// Retrieves/Returns a character string containing the QELIB version number.
// RETURNS: the version string (qeVerNum), or a result code (qeVerNumBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeSetupInfo (void);
qeAPI(qeSTATUS) qeSetupInfoBuf (
	qeLPSTR         dest_buf);      // Result buffer
// Retrieves/Returns a character string containing the User Name,
// Company Name, and Serial Number as a tab-separated character string.
// RETURNS: the setup string (qeSetupInfo), or a result code (qeSetupInfoBuf).
//--------------------------------------------------------------------------

//-----------------------
// Find Functions
//-----------------------
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecClearConditions (
	qeHANDLE        hstmt);         // Statement handle
// Clears all find conditions for a statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionBinary (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	qeLPSTR         value,          // Binary comparison value
	short           length);        // Length of value (in bytes)
// Sets a condition on a column of type Binary.  This condition will be used
// in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionChar (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	qeLPSTR         value,          // Character comparison value
					// (0-terminated)
	qeLPSTR         fmt_string,     // Format string
	short           case_sens);     // T/F Case-sensitive comparison
// Sets a condition on a column of type Character. This condition will be
// used in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionDecimal (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	qeLPSTR         value,          // Decimal comparison value
	short           precision,      // Precision of value (in digits)
	short           scale);         // Scale (for decimals)
// Sets a condition on a column of type Decimal. This condition will be
// used in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionDouble (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	double          value);         // Double comparison value
// Sets a condition on a column of type Double.  This condition will be
// used in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionFloat (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	float           value);         // Float comparison value
// Sets a condition on a column of type Float.  This condition will be
// used in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionLong (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	long            value);         // Long comparison value
// Sets a condition on a column of type Long.  This condition will be 
// used in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionInt (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper,           // Comparison operator (see below)
	short           value);         // Integer comparison value
// Sets a condition on a column of type Integer. This condition will be 
// used in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeRecSetConditionNull (
	qeHANDLE        hstmt,          // Statement handle
	short           col_num,        // Column number (1-based)
	short           oper);          // Comparison operator (see below)
// Sets a condition on a column of type null.  This condition will be used
// in calls to qeRecFind and qeQBEPrepare.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeRecFind (
	qeHANDLE        hstmt,          // Statement handle
	short           start_pos,      // Starting position
	short           flags);         // Direction to search
// Searches the current result set for a row which matches the search
// conditions which were set using the qeRecSetCondition functions.
// Uses start_pos to figure where to begin the search, and direction
// to indicate which direction to search in.  If the record is found,
// it becomes the current record.  Otherwise, the current record remains
// unchanged.
// RETURNS: the new record number, if the find is successful, 0 otherwise.
//---------------------------------------------------------------------------
// Start position constants:

#define qeFIND_BEGIN    1       // From beginning of result set
#define qeFIND_END      2       // From end of result set
#define qeFIND_CURRENT  3       // From current position in result set
//---------------------------------------------------------------------------
// Direction constants:

#define qeFIND_BACKWARD 0x0001  // Set this flag for backward finds
				// (default is forward).
#define qeFIND_SKIP_ROW 0x0002  // Set this flag to skip the current row
				// (meaningful only for qeFIND_CURRENT;
				// default is to start with current row).
//---------------------------------------------------------------------------
// Find operator definitions:

#define qeFIND_LESS_THAN                1
#define qeFIND_LESS_THAN_OR_EQ          2
#define qeFIND_GREATER_THAN             3
#define qeFIND_GREATER_THAN_OR_EQ       4
#define qeFIND_EQUAL                    5
#define qeFIND_NOT_EQUAL                6
#define qeFIND_LIKE                     7
#define qeFIND_NOT_LIKE                 8
#define qeFIND_IN                       9
//-------------------------------------------------------------------------
qeAPI(qeHANDLE) qeQBEPrepare (
	qeHANDLE        hstmt);         // Statement handle
// Creates a new hstmt which is the result of adding the FIND conditions
// of the old hstmt into the WHERE clause of the old statement. This
// statement can be executed with qeSQLExecute.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeClauseGet (
	qeHANDLE        hstmt,          // Statement handle
	short           which_clause);  // Clause to return from defines below
qeAPI(qeSTATUS) qeClauseGetBuf (
	qeHANDLE        hstmt,          // Statement handle
	short           which_clause,   // Clause to return from defines below
	qeLPSTR         clause_buf);    // Clause returned
// RETURNS: a clause from the SELECT statement (qeClauseGet), or a result code
// (qeClauseGetBuf).
//---------------------------------------------------------------------------
// Clause defines

#define qeCLAUSE_WHERE          1       // Return WHERE clause
#define qeCLAUSE_HAVING         2       // Return HAVING clause
#define qeCLAUSE_GROUPBY        3       // Return GROUP BY clause
#define qeCLAUSE_ORDERBY        4       // Return ORDER BY clause
#define qeCLAUSE_COMPUTEBY      5       // Return COMPUTE BY clause
#define qeCLAUSE_FROM           6       // Return FROM clause
#define qeCLAUSE_OTHER          7       // Return OTHER clause
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeUniqueWhereClause (
	qeHANDLE        hstmt);         // Statement handle
qeAPI(qeSTATUS) qeUniqueWhereClauseBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         clause_buf);    // Unique where clause returned
// Returns/Retrieves a where clause which uniquely identifies the current row
// in an active SELECT statement.
// Uses the columns specified to qeRecSetKey (if that function has
// been called) or will generate the list of columns on its own.
// Returns: the clause (qeUniqueWhereClause), or a result code
// (qeUniqueWhereClauseBuf).
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeNativeSQL (
	qeHANDLE        hstmt);         // Statement handle
qeAPI(qeSTATUS) qeNativeSQLBuf (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         stmt_buf);      // Native SQL returned
// RETURNS: the SQL string as translated by the driver (qeNativeSQL), or a
// result code (qeNativeSQLBuf).
//--------------------------------------------------------------------------

//---------------------------------
// Parameter Bind and Set Routines
//---------------------------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeClearParam (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num);     // Parameter number (1-based)
// Clears any parameter information for a parameter.
// qeSetParamNull, qeSetParam<type>, or qeBindParam<type> must
// be called before the statement is executed again, otherwise an error will
// be returned on the next execution.
// This call allows resetting a chunked parameter.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamNull (
	qeHANDLE hstmt,			// Statement handle
	short   param_num,		// Parameter number (1-based)
	short   param_type,		// QELIB data type
	long    precision,		// Decimal's precision, maximum
					// size of a character, or length (in
					// bytes) of a datetime value
	short   scale);			// Decimal's scale
// Sets this parameter to null.  To supply a value for
// this parameter, call qeSetParam<type> or qeBindParam<type>.
// qeNO_DATA_TYPE is legal only if this parameter has
// previously been given a type with qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type>.  RETURNS: a result code.

#define qeNO_DATA_TYPE 0
//--------------------------------------------------------------------------
qeAPI(short) qeParamNum (
	qeHANDLE        hstmt,          // Statement handle
	qeLPSTR         param_name);    // Parameter name
// RETURNS: the parameter number corresponding to the parameter name
// (1-based).  If the name does not match any parameter, 0 (zero) is 
// returned.  If the parameter name matches more than one parameter number,
// the lowest number will be returned.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamBinary (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Binary value
	qeLPLONG        param_len);     // Binary value length in bytes
// Binds this parameter to the value passed.  The param_val pointer and
// param_len pointer are saved and both must be valid when the statement
// is executed.
// When called, param_len should contain the maximum length for the value.
// Set param_len to qeNULL_DATA before execution for this parameter to be
// NULL.  This parameter will continue to point to this value until
// qeSetParamNull, qeSetParam<type>, or qeBindParam<type> is called again
// for this parameter.  Note that all parameters with the same name as
// this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamBinary (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Binary value
	long            param_len);     // Binary value length in bytes
// Sets this parameter to the param_val passed. A copy of the value passed is
// made so the pointer need not remain valid after this call.
// This parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter. Note that all parameters with the same name as parameter
// number will be affected. This function may called multiple times
// for large values.  At statement execution, the values will be put
// together in the order this function was called. Lengths of zero will be
// ignored.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamChar (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Character value, (0-terminated)
	qeLPLONG        param_len);     // Character value length in bytes
// Binds this parameter to the value passed.  The param_val and param_len
// pointers are saved and must be valid when the statement is executed.
// When called, param_len should contain the maximum length for the value.
// Set param_len to qeNULL_DATA before execution for this parameter to be
// NULL. This parameter will continue to point to this value
// until qeSetParamNull, qeSetParam<type>,  or qeBindParam<type> is called
// again for this parameter.  Note that all parameters with the same name 
// as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamChar (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Character value, (0-terminated)
	long            max_len);       // Maximum length of char value
// Sets this parameter to the param_val passed. A copy of the value passed 
// is made so the pointer need not remain valid after this call.
// This parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter.  Note that all parameters with the same name as this parameter
// will be affected.  This function may called multiple
// times for large values.  At statement execution, the values are put
// together in the order this function was called.  Lengths of zero are
// ignored.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamDate (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Date value, 26-byte string
	qeLPLONG        param_len);     // Date value length in bytes
// Binds this parameter to the param_val passed.  The param_val and
// param_len pointers are saved and must be valid when the statement is
// executed. When called, param_len should contain the maximum length for
// the value.  Set length to qeNULL_DATA before execution for this parameter
// to be null.  This parameter will continue to point to this value
// until qeSetParamNull, qeSetParam<type>,  or qeBindParam<type> is called
// again for this parameter.  Note that all parameters with the same name as 
// this parameter number will be affected.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamDate (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val);     // Date value, 26-byte string
// Sets this parameter to the param_val passed.  A copy of the value passed
// made so the pointer need not remain valid after this call. This
// parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter.  Note that all parameters with the same name as parameter
// number will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamDateTime (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Date time value, 26-byte string
	qeLPLONG        param_len);     // Date time value length in bytes
// Binds this parameter to the value passed.  The param_val and param_len
// pointers are saved and must be valid when the statement is executed.
// When called, param_len should contain the maximum length for the value.
// Set length to qeNULL_DATA before execution for this parameter to be null.
// This parameter will continue to point to this value until qeSetParamNull, 
// qeSetParam<type>, or qeBindParam<type> is called again for this 
// parameter.  Note that all parameters with the same name as parameter
// number will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamDateTime (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Date time value, 26-byte string
	short           precision);     // Date time precision
// Sets this parameter to the param_val passed. A copy of the value passed is
// made so the pointer need not remain valid after this call. This
// parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter. Note that all parameters with the same name as parameter
// number will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamDecimal (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Decimal value
	qeLPLONG        param_len,      // Decimal value length in bytes
	short           scale);         // Decimal's scale
// Binds this parameter to the value passed.  The param_val and param_len
// pointers are saved and must be valid when the statement is executed.
// When called, param_len should contain the maximum length for the value.
// Set length to qeNULL_DATA before execution for this parameter to be null.
// This parameter will continue to point to this value until qeSetParamNull, 
// qeSetParam<type>, or qeBindParam<type> is called again for this 
// parameter.  Note that all parameters with the same name as parameter
// number will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamDecimal (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Decimal value
	short           precision,      // Decimal's precision
	short           scale);         // Decimal's scale
// Sets this parameter to the param_val passed. A copy of the value passed
// is made so the pointer need not remain valid after this call. This
// parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter. Note that all parameters with the same name as parameter
// number will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamDouble (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPDBL         param_val,      // Double value
	qeLPLONG        param_len);     // Double value length in bytes
// Binds this parameter to the value passed.  The param_val and param_len
// pointers are saved and must be valid when the statement is executed.
// The param_len pointer may be NULL.
// Set length to qeNULL_DATA before execution for this parameter to be null.
// This parameter will continue to point to this value
// until qeSetParamNull, qeSetParam<type>, or qeBindParam<type> is called 
// again for this parameter.  Note that all parameters with the same name 
// as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamDouble (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	double          param_val);     // Double value
// Sets this parameter to the param_val passed. This parameter will
// continue to have this value until qeSetParamNull, qeSetParam<type>, or
// qeBindParam<type> is called again for this parameter.  Note that all
// parameters with the same name as parameter number will be
// affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamFloat (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPFLT         param_val,      // Float value
	qeLPLONG        param_len);     // Float value length in bytes
// Binds this parameter to the param_val pointer passed.  The param_val and
// param_len pointers are saved and must be valid when the statement is
// executed. The param_len pointer may be NULL.
// Set length to qeNULL_DATA before execution for this parameter to be NULL.
// This parameter will continue to point to this value
// until qeSetParamNull, qeSetParam<type>, or qeBindParam<type> is called
// again for this parameter.  Note that all parameters with the same name 
// as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamFloat (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	float           value);         // Float value
// Sets this parameter to the value passed.  This parameter will continue 
// to have this value until qeSetParamNull, qeSetParam<type>, or 
// qeBindParam<type> is called again for this parameter.  Note that all 
// parameters with the same name as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamInt (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPINT         param_val,      // Int value
	qeLPLONG        param_len);     // Int value length in bytes
// Binds this parameter to the value passed.  The param_val and param_len
// pointers are saved and must be valid when the statement is executed.
// The length pointer may be null.
// Set length to qeNULL_DATA before execution for this parameter to be null.
// This parameter will continue to point to this value 
// until qeSetParamNull, qeSetParam<type>, or qeBindParam<type> is called 
// again for this parameter.  Note that all parameters with the same name 
// as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamInt (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	short           param_val);     // Int value
// Sets this parameter to the param_val passed. A copy of the value passed 
// is made so the pointer need not remain valid after this call. This
// parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter. Note that all parameters with the same name as parameter
// number will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamLong (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPLONG        param_val,      // Long value
	qeLPLONG        param_len);     // Long value length in bytes
// Binds this parameter to the value passed.  The param_val and param_len
// pointers are saved and must be valid when the statement is executed.
// The length pointer may be null.
// Set param_len to qeNULL_DATA before execution for this parameter to be
// null. This parameter will continue to point to this value
// until qeSetParamNull, qeSetParam<type>, or qeBindParam<type> is called 
// again for this parameter.  Note that all parameters with the same name 
// as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamLong (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	long            param_val);     // Long value
// Sets this parameter to the param_val passed.  This parameter will 
// continue to have this value until qeSetParamNull, qeSetParam<type>, or
// qeBindParam<type> is called again for this parameter. Note that all
// parameters with the same name as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeBindParamTime (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val,      // Time value, 26-byte string
	qeLPLONG        param_len);     // Time value length in bytes
// Binds this parameter to the value passed.  The param_val and param_len
// pointers  are saved and must be valid when the statement is executed.
// When called, param_len should contain the maximum length for the value.
// Set length to qeNULL_DATA before execution for this parameter to be null.
// This parameter will continue to point to this value
// until qeSetParamNull, qeSetParam<type>, or qeBindParam<type> is called 
// again for this parameter.  Note that all parameters with the same name 
// as this parameter will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetParamTime (
	qeHANDLE        hstmt,          // Statement handle
	short           param_num,      // Parameter number (1-based)
	qeLPSTR         param_val);     // Date time value, 26-byte string
// Sets this parameter to the param_val passed. A copy of the value passed 
// is made so the pointer need not remain valid after this call. This
// parameter will continue to have this value until qeSetParamNull,
// qeSetParam<type>, or qeBindParam<type> is called again for this
// parameter. Note that all parameters with the same name as this parameter
// will be affected.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeNumParams (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the number of distinct question mark parameters in the statement
// represented by hstmt.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeMoreResults (
	qeHANDLE        hstmt);         // Statement handle
// Ends the current result set and starts a new result set.  If EOF is
// returned, there are on more result sets. This procedure is used to
// handle statements which can return multiple result sets, such as stored
// procedures.
// RETURNS: a result code, which is qeEOF if there are no more results.
//--------------------------------------------------------------------------
qeAPI(qeLPVOID) qeGetODBCHenv (void);
// RETURNS: the ODBC henv for QELIB. qeLibInit and qeLibTerm invalidate the
// henv returned.
//--------------------------------------------------------------------------
qeAPI(qeLPVOID) qeGetODBCHdbc (
	qeHANDLE        hdbc);          // Connection handle
// RETURNS: the ODBC hdbc associated with the QELIB hdbc.
// qeLibInit, qeLibTerm, and qeDisconnect invalidate the hdbc returned.
//--------------------------------------------------------------------------
qeAPI(qeLPVOID) qeGetODBCHstmt (
	qeHANDLE        hstmt);         // Statement handle
// RETURNS: the ODBC hstmt associated with the QELIB hstmt.
// qeLibInit, qeLibTerm, qeDisconnect, and qeEndSQL invalidate the hdbc
// returned.
//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeSetODBCHdbc (
	qeLPVOID        ODBCHdbc);      // ODBC connection handle
// RETURNS: a QELIB connection for the ODBC hdbc.
//---------------------------------------------------------------------------
qeAPI(long) qeGetODBCInfoLong (
	qeHANDLE        hdbc,           // Connection handle.
	short           option);        // Which info to get (see below).
// RETURNS: the information requested.
//---------------------------------------------------------------------------
qeAPI(qeSTATUS) qeGetODBCInfoCharBuf (
	qeHANDLE        hdbc,           // Connection handle.
	short           option,         // Which info to get (see below).
	qeLPSTR         char_val);      // Value returned.
// RETURNS: the information requested.
//---------------------------------------------------------------------------
qeAPI(qeLPSTR) qeGetODBCInfoChar (
	qeHANDLE        hdbc,           // Connection handle.
	short           option);        // Which info to get (see below).
// RETURNS: the information requested.
//--------------------------------------------------------------------------
// Definitions for the qeGetODBCInfo calls:

#define qeINFO_ACTIVE_CONNECTIONS        0      // Long
#define qeINFO_ACTIVE_STATEMENTS         1      // Long
#define qeINFO_DRIVER_VER                7      // Char
#define qeINFO_SEARCH_PATTERN_ESCAPE    14      // Char
#define qeINFO_DATA_SOURCE_READ_ONLY    25      // Char
#define qeINFO_EXPRESSIONS_IN_ORDERBY   27      // Char
#define qeINFO_IDENTIFIER_CASE          28      // Long
#define qeINFO_IDENTIFIER_QUOTE_CHAR    29      // Char
#define qeINFO_MAX_COLUMN_NAME_LEN      30      // Long
#define qeINFO_MAX_CURSOR_NAME_LEN      31      // Long
#define qeINFO_MAX_OWNER_NAME_LEN       32      // Long
#define qeINFO_MAX_PROCEDURE_NAME_LEN   33      // Long
#define qeINFO_MAX_QUALIFIER_NAME_LEN   34      // Long
#define qeINFO_MAX_TABLE_NAME_LEN       35      // Long
#define qeINFO_OUTER_JOINS              38      // Char
#define qeINFO_OWNER_TERM               39      // Char
#define qeINFO_PROCEDURE_TERM           40      // Char
#define qeINFO_QUALIFIER_NAME_SEPARATOR 41      // Char
#define qeINFO_QUALIFIER_TERM           42      // Char
#define qeINFO_TABLE_TERM               45      // Char
#define qeINFO_CONVERT_FUNCTIONS        48      // Long
#define qeINFO_NUMERIC_FUNCTIONS        49      // Long
#define qeINFO_STRING_FUNCTIONS         50      // Long
#define qeINFO_SYSTEM_FUNCTIONS         51      // Long
#define qeINFO_TIMEDATE_FUNCTIONS       52      // Long

// Conversion functions:

#define qeSQL_FN_CVT_CONVERT                  0x00000001L

// String functions:

#define qeSQL_FN_STR_CONCAT                 0x00000001L
#define qeSQL_FN_STR_INSERT                 0x00000002L
#define qeSQL_FN_STR_LEFT                   0x00000004L
#define qeSQL_FN_STR_LTRIM                  0x00000008L
#define qeSQL_FN_STR_LENGTH                 0x00000010L
#define qeSQL_FN_STR_LOCATE                 0x00000020L
#define qeSQL_FN_STR_LCASE                  0x00000040L
#define qeSQL_FN_STR_REPEAT                 0x00000080L
#define qeSQL_FN_STR_REPLACE                0x00000100L
#define qeSQL_FN_STR_RIGHT                  0x00000200L
#define qeSQL_FN_STR_RTRIM                  0x00000400L
#define qeSQL_FN_STR_SUBSTRING              0x00000800L
#define qeSQL_FN_STR_UCASE                  0x00001000L
#define qeSQL_FN_STR_ASCII                  0x00002000L
#define qeSQL_FN_STR_CHAR                   0x00004000L

// Numeric functions:

#define qeSQL_FN_NUM_ABS                    0x00000001L
#define qeSQL_FN_NUM_ACOS                   0x00000002L
#define qeSQL_FN_NUM_ASIN                   0x00000004L
#define qeSQL_FN_NUM_ATAN                   0x00000008L
#define qeSQL_FN_NUM_ATAN2                  0x00000010L
#define qeSQL_FN_NUM_CEILING                0x00000020L
#define qeSQL_FN_NUM_COS                    0x00000040L
#define qeSQL_FN_NUM_COT                    0x00000080L
#define qeSQL_FN_NUM_EXP                    0x00000100L
#define qeSQL_FN_NUM_FLOOR                  0x00000200L
#define qeSQL_FN_NUM_LOG                    0x00000400L
#define qeSQL_FN_NUM_MOD                    0x00000800L
#define qeSQL_FN_NUM_SIGN                   0x00001000L
#define qeSQL_FN_NUM_SIN                    0x00002000L
#define qeSQL_FN_NUM_SQRT                   0x00004000L
#define qeSQL_FN_NUM_TAN                    0x00008000L
#define qeSQL_FN_NUM_PI                     0x00010000L
#define qeSQL_FN_NUM_RAND                   0x00020000L

// Date/time functions:

#define qeSQL_FN_TD_NOW                     0x00000001L
#define qeSQL_FN_TD_CURDATE                 0x00000002L
#define qeSQL_FN_TD_DAYOFMONTH              0x00000004L
#define qeSQL_FN_TD_DAYOFWEEK               0x00000008L
#define qeSQL_FN_TD_DAYOFYEAR               0x00000010L
#define qeSQL_FN_TD_MONTH                   0x00000020L
#define qeSQL_FN_TD_QUARTER                 0x00000040L
#define qeSQL_FN_TD_WEEK                    0x00000080L
#define qeSQL_FN_TD_YEAR                    0x00000100L
#define qeSQL_FN_TD_CURTIME                 0x00000200L
#define qeSQL_FN_TD_HOUR                    0x00000400L
#define qeSQL_FN_TD_MINUTE                  0x00000800L
#define qeSQL_FN_TD_SECOND                  0x00001000L

// System functions:

#define qeSQL_FN_SYS_USERNAME               0x00000001L
#define qeSQL_FN_SYS_DBNAME                 0x00000002L
#define qeSQL_FN_SYS_IFNULL                 0x00000004L
//--------------------------------------------------------------------------

//--------------
// Query Builder
//--------------

//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQryBuilder (
	qeHANDLE        hqry,           // Query Builder handle
	short           parent_window,  // Handle to parent window of
					// caller
	short           flags,          // Options: see choices below
	short           init_dialog);   // Initial dialog box: see below
// Runs the Query Builder based on the query represented by hqry.  Any
// editing is applied to the hqry.  RETURNS: a result code.
//---------------------------------------------------------------------------
// Definitions for flags parameter:

#define qeQRY_NO_COL_ALIAS      0x0001  // Column aliases disallowed.
#define qeQRY_EXIT_AFTER_DLG    0x0002  // Exit after dialog is exited.
					// Valid only if initial
					// dialog is specified.
#define qeQRY_ALLOW_SRC_CHANGE  0x0004  // Source can be changed in file
					// open box.
#define qeQRY_SYSTABLES         0x0008  // List system tables in table
					// dialog box.
#define qeQRY_SYNONYMS          0x0010  // List synonyms in table
					// dialog box.
#define qeQRY_TABLES            0x0020  // List tables in table
					// dialog box.
#define qeQRY_VIEWS             0x0040  // List views in table
					// dialog box.
#define qeQRY_NO_PARAMS         0x0080  // Disallow parameters.
#define qeQRY_BIG_ICONS         0x0100  // Use big icons in icon bar.
#define qeQRY_VALIDATE          0x0200  // Use database to validate SQL.
#define qeQRY_SAMPLE            0x0400  // Show sample values in conditions
					// dialog box.
//---------------------------------------------------------------------------
// Definitions for init_dialog parameter

#define qeQRY_DEFAULT           1       // Bring up the default initial
					// dialog box
#define qeQRY_FILE              2       // File dialog box
#define qeQRY_JOIN              3       // Join dialog box
#define qeQRY_SELECT            4       // Select dialog box
#define qeQRY_ORDER             5       // Order By dialog box
#define qeQRY_WHERE             6       // Where By dialog box
#define qeQRY_GROUP             7       // Group dialog box
#define qeQRY_HAVING            8       // Having dialog box
#define qeQRY_TEXT              9       // Edit query text dialog box
//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeQryAllocate (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         statement);     // Statement text, may be null
// RETURNS: a handle to a query builder statement.
//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeQryPrepare (
	qeHANDLE        hqry);          // Query Builder handle
// Prepares the statement represented by hqry for execution. Call qeSQLExecute
// to execute the statement.
// RETURNS: a handle for statement execution.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQryFree (
	qeHANDLE        hqry);          // Query Builder handle
// Frees any memory associated with the Query Builder statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeQryOpenQueryFile (
	qeLPSTR         pathname);      // File pathname
// RETURNS: a handle to a query builder statement taken from the query file.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySaveQueryFile (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         pathname);      // File pathname, may be null
// Writes a query out to a file.  If pathname is null, hqry must have a file
// name assoicated with it. If the query was initially read from a query file,
// the contents of the query file which do not deal with the query or the
// parameters are preserved.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeHANDLE) qeQryGetHdbc (
	qeHANDLE        hqry);          // Query Builder handle
// RETURNS: the hdbc associated with a Query Builder statement.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetHdbc (
	qeHANDLE        hqry,           // Query Builder handle
	qeHANDLE        hdbc);          // Connection handle
// Sets the hdbc associated with a Query Builder statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetSource (
	qeHANDLE        hqry);          // Query Builder handle
qeAPI(qeSTATUS) qeQryGetSourceBuf (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         source_name);   // Buffer for source name
// Gets the source name associated with a query.  This is the name used in
// the query file that indicates which data source name the query file is
// meant to operate on.
// RETURNS: the source name, or a result code if using the Buf version.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetSource (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         source);        // New source name for hqry
// Sets the source name associated with a query.  This is the name used in
// the query file that indicates which data source name the query file is
// meant to operate on.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetStmt (
	qeHANDLE        hqry);          // Query Builder handle
qeAPI(qeSTATUS) qeQryGetStmtBuf (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         stmt);          // Statement text returned
// RETURNS: the statement text associated with a Query Builder statement
// (qeQryGetStmt), or a result code (qeQryGetStmtBuf).
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetStmt (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         stmt);          // Statement text
// Sets the statement text associated with a Query Builder statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeQryGetNumParams (
	qeHANDLE        hqry);          // Query Builder handle
// RETURNS: the number of parameters associated with a Query Builder statement.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetNumParams (
	qeHANDLE        hqry,           // Query Builder handle
	short           num_params);    // Number of parameters
// Sets the number of parameters associated with a Query Builder statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetFileName (
	qeHANDLE        hqry);          // Query Builder handle
qeAPI(qeSTATUS) qeQryGetFileNameBuf (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         file_name);     // File name returned
// RETURNS: the file name associated with a Query Builder statement
// (qeQryGetFileName), or a result code (qeQryGetFileNameBuf).
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetFileName (
	qeHANDLE        hqry,           // Query Builder handle
	qeLPSTR         file_name);     // File name
// Sets the file name associated with a Query Builder statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeQryGetFileOffset (
	qeHANDLE        hqry);          // Query Builder handle
// RETURNS: the offset within the query file of extra information
// associated with a Query Builder statement.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetParamName (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num);     // Parameter number
qeAPI(qeSTATUS) qeQryGetParamNameBuf (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_name);    // Parameter name returned
// RETURNS: the name associated with a parameter number in a Query Builder
// statement (qeQryGetParamName), or a result code (qeQryGetParamNameBuf).
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetParamName (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_name);    // Parameter name
// Sets the name associated with a parameter number in a Query Builder
// statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeQryGetParamType (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num);     // Parameter number
// RETURNS: the type associated with a parameter number in a Query Builder
// statement.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetParamType (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	short           param_type);    // Parameter type
// Sets the type associated with a parameter number in a Query Builder
// statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
// Definition of datatypes for parameters
#define qeQRYPARM_CHAR          1
#define qeQRYPARM_NUM           2
#define qeQRYPARM_DATE          3
#define qeQRYPARM_TIME          4
#define qeQRYPARM_DATETIME      5
#define qeQRYPARM_LOGICAL       6
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetParamPrompt (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num);     // Parameter number
qeAPI(qeSTATUS) qeQryGetParamPromptBuf (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_prompt);  // Parameter prompt returned
// RETURNS: the prompt associated with a parameter number in a Query Builder
// statement (qeQryGetParamPrompt), or a result code (qeQryGetParamPromptBuf).
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetParamPrompt (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_prompt);  // Parameter prompt
// Sets the prompt associated with a parameter number in a Query Builder
// statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetParamDefault (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num);     // Parameter number
qeAPI(qeSTATUS) qeQryGetParamDefaultBuf (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_default); // Parameter default value returned
// RETURNS: the default value associated with a parameter number in a Query
// Builder statement (qeQryGetParamDefaultBuf), or a result code
// (qeQryGetParamDefaultBuf).
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetParamDefault (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_default); // Parameter default value
// Sets the default value associated with a parameter number in a Query
// Builder statement.  RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeLPSTR) qeQryGetParamFormat (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num);     // Parameter number
qeAPI(qeSTATUS) qeQryGetParamFormatBuf (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_fmt);     // Parameter format string returned
// RETURNS: the format string associated with a parameter number in a Query
// Builder statement (qeQryGetParamFormatBuf), or a result code
// (qeQryGetParamFormatBuf).
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeQrySetParamFormat (
	qeHANDLE        hqry,           // Query Builder handle
	short           param_num,      // Parameter number
	qeLPSTR         param_fmt);     // Parameter format string
// Sets the format string associated with a parameter number in a Query
// Builder statement.
// RETURNS: a result code.
//--------------------------------------------------------------------------

//---------
// Options
//---------

//---------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetQueryTimeout (
	qeHANDLE        hdbc,           // Connection handle
	long            seconds);       // Query timeout, in seconds;
					// zero for no timeout
// Sets the query timeout for statements created after this function is
// called.
// RETURNS: a result code.
//---------------------------------------------------------------------------
qeAPI(long) qeGetQueryTimeout (
	qeHANDLE        hdbc);          // Connection handle
// Gets the query timeout for statements.
// RETURNS: the current query timeout.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetTableCaching (
	qeHANDLE        hdbc,           // Connection handle
	short           setting);       // Table cache options (see below).
// Sets the table caching for this connection.
// RETURNS: a result code.
//---------------------------------------------------------------------------
qeAPI(short) qeGetTableCaching (
	qeHANDLE        hdbc);          // Connection handle
// Gets the table caching for this connection.
// RETURNS: the current caching setting.
//---------------------------------------------------------------------------
// Definitions for qeSetTableCaching setting:

#define qeCACHE_PERMANENT       1       // Turn caching on and have it remain
					// on across sessions.
#define qeCACHE_SESSION         2       // Turn caching on for this session.
#define qeCACHE_OFF             3       // Turn caching off.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetCacheFileName (
	qeHANDLE        hdbc,           // Connection handle
	qeLPSTR         file_name);     // File name for table caching;
					// null for a system generated
					// file name
// Sets the table cache file for this connection.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetMaxRows (
	qeHANDLE        hdbc,           // Connection handle
	long            max_rows);      // Maximum number of row statement
					// will return
// Sets the maximum number of rows to return for select statements
// created after this function is called.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(long) qeGetMaxRows (
	qeHANDLE        hdbc);          // Connection handle
// Gets the maximum number of rows to return for select statements.
// RETURNS: the current max rows setting.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetLockOptions (
	qeHANDLE        hdbc,           // Connection handle
	short           option);        // Lock option (see below)
// Sets the locking options for the statement.
// RETURNS: a result code.
//---------------------------------------------------------------------------
qeAPI(short) qeGetLockOptions (
	qeHANDLE        hdbc);          // Connection handle
// Gets the locking options for the statement.
// RETURNS: the current locking options.
//---------------------------------------------------------------------------
// Values for qeSetLockOptions:
#define qeLOCK_NO_OPTIONS  0    // Neither comparing nor refreshing the row
				// wil be done.
#define qeLOCK_COMPARE     1    // Lock will compare current buffer with row
				// in logfile, raise a warning if different.
#define qeLOCK_REFRESH     2    // Lock will refresh the logfile with new
				// values.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetAutoUpdate (
	qeHANDLE        hdbc,           // Connection handle
	short           option);        // T/F. TRUE: generate updates or
					// inserts for changed columns when
					// current record is changed without
					// qeRecUpdate being explicitly called
					// for the row.
// Determines what is done when moving off a row with modified columns and
// no explict qeRecUpdate or qeRecInsert has been done.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeGetAutoUpdate (
	qeHANDLE        hdbc);          // Connection handle
// Returns what is done when moving off a row with modified columns and
// no explict update or insert has been done.
// RETURNS: current auto update option setting from defines below.
//--------------------------------------------------------------------------
// Values for qeSet/GetAutoUpdate:
#define qeAUTOUPD_DISCARD       1       // Modifications to the current
					// record will be discarded when
					// user moves off record.
#define qeAUTOUPD_DEFER         2       // Modifications to the current
					// will be saved when user
					// moves off record.  They may
					// be applied to the database or
					// discarded by later calls.
#define qeAUTOUPD_UPDATE        3       // Modifications to the current
					// record are immediately sent to
					// the database.
//--------------------------------------------------------------------------
qeAPI(qeSTATUS) qeSetIsolationLevel (
	qeHANDLE        hdbc,           // Connection handle
	short           level);         // Isolation level to set
// Sets the isolation level for the connection.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI(short) qeGetIsolationLevel (
	qeHANDLE        hdbc);          // Connection handle
// Returns the isolation level for the connection. 
// RETURNS: an isolation level.
//--------------------------------------------------------------------------
qeAPI(short) qeGetSupportedIsolationLevels (
	qeHANDLE        hdbc);          // Connection handle
// Returns the legal isolation levels for the connection.
// RETURNS: a flag with bits set for all supported isolation levels.
//--------------------------------------------------------------------------
// Values for qeSet/GetIsolationLevel:
#define qeISO_READ_UNCOMMITTED  0x0001 // Dirty read, non-repeatable read,
				       // phantoms possible.
#define qeISO_READ_COMMITTED    0x0002 // Non-repeatable read, phantoms
				       // possible.
#define qeISO_REPEATABLE_READ   0x0004 // Repeatable read, phantoms possible.
#define qeISO_SERIALIZABLE      0x0008 // Repeatable read, no phantoms.
#define qeISO_VERSIONING        0x0010 // Repeatable read, no phantoms,
				       // more concurrency.

//--------------------------------------------------------------------------
qeAPI (qeSTATUS) qeRecUndo (
	qeHANDLE        hstmt);         // Statement handle
// Undoes all deferred (not applied to database by by qeRecUpdate) changes to
// the current row.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI (qeSTATUS) qeUndoAll (
	qeHANDLE        hstmt);         // Statement handle
// Undo all deferred (not applied to database by qeRecUpdate) changes 
// to the statement.  Equivalent to qeRecUndo on every result row.
// RETURNS: a result code.
//--------------------------------------------------------------------------
qeAPI (qeSTATUS) qeApplyAll (
	qeHANDLE        hstmt);         // Statement handle
// Apply all deferred (not applied to database by qeRecUpdate) changes 
// for the statement to the database.  Equivalent to qeRecUpdate on every 
// result row.  RETURNS: a result code.
