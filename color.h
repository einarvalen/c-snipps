#ifndef COLOR_H
  #define COLOR_H

  #define BLACK     0
  #define BLUE      1
  #define GREEN     2
  #define CYAN      3
  #define RED       4
  #define MAGENTA   5
  #define BROWN     6
  #define WHITE     7
  #define GREY      8
  #define L_BLUE    9
  #define L_GREEN   10
  #define L_CYAN    11
  #define L_RED     12
  #define L_MAGENTA 13
  #define L_YELLOW  14
  #define L_WHITE   15
  #define BLINK     0x80
  #define UNDERLINE   1

/** MACROS **/
#define COLOR_ATTR(t,b)    ((short)(b<<4)+t)

short color_choise, color_entry, color_cursor, color_error, color_info, color_background;


/** PROTOTYPES **/
void  COLOR_begin( void);
void  COLOR_end( void);
short COLOR_or_mono( short color_attr, short mono_attr);  /* Returns mono_attr if VDO is monocrome else color attr */
short COLOR_select( void);
BYTE *COLOR_text( short attr, BYTE *s);
void  COLOR_modify( void);

#endif


