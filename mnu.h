#ifndef MNU_H
#define MNU_H

/** INCLUDE FILES **/
#include <std.h>
#include <ll.h>
#include <w.h>
#define NUM_CURTAINS 10

/** TYPES **/
struct cmnu { BYTE *text; short id; };
struct mnu { struct ll first, last, *node; short nrow, ncol; };
typedef struct mnu MNU;

struct cbar { BYTE *text; short id; MNU *mnu; short col;};
struct bar { struct ll first, last, *node; short row, col, len, count; W w;};
typedef struct bar MNUBAR;

struct cbutton { BYTE *text; short id, row, col; };
struct button { struct ll first, last, *node; short len; };
typedef struct button MNUBUTTON;


/** PROTOTYPES **/
void MNU_begin( MNU *mnu);
  /*
  Inits menu bar. Returns FALSE if memory shortage.
  */
BOOL MNU_new( MNU *mnu, BYTE *text, short id);
  /*
  Assigns string and id to menu item. Returns FALSE if memory shortage
  */
short MNU_popup( MNU *mnu, short row, short col, short *c);
  /*
  Standard popup menu. Returns id of menu selection or 0 on escape.
  */
void MNU_end( MNU *mnu);
  /*
  Ends menu usage.
  */
void MNUBAR_begin( MNUBAR *bar);
  /*
  Inits menu bar. Returns FALSE if memory shortage
  */
BOOL MNUBAR_new( MNUBAR *bar, BYTE *text, short mnu_id, short link_to_bar_id);
  /*
  Assigns string and ids to bar item. Returns FALSE if memory shortage
  */
void MNUBAR_mount( MNUBAR *bar, short row, short col, short ncol);
  /*
  Initial display of pulldown menubar.
  */
short MNUBAR_pulldown( MNUBAR *bar);
  /*
  Standard pulldown menubar. Returns id of menu selection or 0 on escape.
  */
void MNUBAR_end( MNUBAR *bar);
  /*
  Ends menu bar usage.
  */
void MNUBUTTON_begin( MNUBUTTON *button, short len);
  /*
  Inits button mnu. Returns FALSE if memory shortage
  */
void MNUBUTTON_end( MNUBUTTON *button);
  /*
  Ends button mnu usage.
  */
BOOL MNUBUTTON_new( MNUBUTTON *button, BYTE *text, short id, short row, short col);
  /*
  Assigns string and ids to button item and positions it in window.
  Returns FALSE if memory shortage.
  */
void MNUBUTTON_mount( MNUBUTTON *button, W *w);
  /*
  Initial display of button menu.
  */
short MNUBUTTON_is_pressed( MNUBUTTON *button, short chr);
  /*
  Loops through button list searching for an uppercase letter
  equal to toupper( chr).
  */
void MNUBUTTON_hilite( MNUBUTTON *button, W *w, short id, short attr);
  /*
  Internal function for display of sertain item with given attribute.
  */
void MNUBUTTON_hilite_on(  MNUBUTTON *button, W *w, short id);
  /*
  Hilites item with given id.
  */
void MNUBUTTON_hilite_off( MNUBUTTON *button, W *w, short id);
  /*
  Turns off hiliting of item with given id.
  */

#endif
