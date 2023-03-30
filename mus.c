#include <dos.h>

#define MOUSE_RESET    0  /* Reset the mouse driver                       */
#define MOUSE_ON       1  /* Turn Mouse Cursor on                         */
#define MOUSE_OFF      2  /* Turn Mouse Cursor off                        */
#define MOUSE_STATUS   3  /* Obtain status of the Mouse Driver            */
#define MOUSE_SET      4  /* Set Mouse Cursor position                    */
#define MOUSE_PRESSED  5  /* Get button press information                 */
#define MOUSE_RELEASED 6  /* Get button release  information              */
#define MOUSE_COLMAX   7  /* Set horizontal Mouse Cursor boundaries       */
#define MOUSE_ROWMAX   8  /* Set vertical   Mouse Cursor boundaries       */
#define MOUSE_GIMAGE   9  /* Map new graphics image onto the mouse cursor */
#define MOUSE_TIMAGE   10 /* Map new character onto the mouse cursor      */

#define LEFTBUTTON   1
#define RIGHTBUTTON  2
#define BOTHBUTTONS  (LEFTBUTTON | RIGHTBUTTON)

#define LEFT_STATUS   0
#define RIGHT_STATUS  1


int mouse( int service, int *bx_register, int *cx_register, int *dx_register)
  {
  union REGS inregs, outregs;
  inregs.x.ax = service;
  inregs.x.bx = *bx_register;
  inregs.x.cx = *cx_register;
  inregs.x.dx = *dx_register;
  int86( 51, &inregs, &outregs);
  *bx_register = outregs.x.bx;
  *cx_register = outregs.x.cx;
  *dx_register = outregs.x.dx;
  return (outregs.x.ax);
  }

void install_cursor_image( int picture[16][2])
  {
  struct SREGS  sregs, isregs;
  union REGS iregs, oregs;
  segread( &sregs);
  iregs.x.ax = MOUSE_GIMAGE;
  iregs.x.bx = 0;
  iregs.x.cx = 0;
  iregs.x.dx = (int)picture;
  iregs.x.dx = (int)picture;
  isregs.es = sregs.ds;
  int86x( 51, &iregs, &oregs, &isregs);
  }

/** TESTCODE **/
#include <std.h>

int pencil[16][2] = {
  {0x07ff, 0x03ff}, {0x01ff, 0x00ff}, {0x807f, 0xc03f}, {0xe01f, 0xf00f},
  {0xf807, 0xfc03}, {0xfe01, 0xff00}, {0xff80, 0xffc0}, {0xffe1, 0xfff3},
  {0xf800, 0xcc00}, {0xae00, 0x8b00}, {0x7980, 0x24c0}, {0x1260, 0x0930},
  {0x0498, 0x024c}, {0x0126, 0x0093}, {0x004b, 0x0027}, {0x0016, 0x000c}
  };

void change_mode( int mode)
  {
  union REGS inregs, outregs;
  inregs.h.ah = 0; /* Video Service 0 */
  inregs.h.al = mode;
  int86( 0x10, &inregs, &outregs);
  }

void pixel_on( int x, int y)
  {
  union REGS inregs, outregs;
  inregs.h.ah = 12; /* Video Service */
  inregs.h.al = 15; /* White */
  inregs.h.bh = 0;
  inregs.x.cx = x;
  inregs.x.dx = y;
  int86( 0x10, &inregs, &outregs);
  }


int main( void)
  {
  int ax, bx = 0, cx = 0, dx = 0;
  if (!mouse( MOUSE_RESET, &bx, &cx, &dx) )
    {
    printf( "Microsoft Mouse Interface Driver is not installed\n");
    return (1);
    }
/*
  change_mode( 6);
  install_cursor_image( pencil);
*/
  mouse( MOUSE_ON, &bx, &cx, &dx);
  for (; TRUE; mouse( MOUSE_STATUS, &bx, &cx, &dx) )
    {
    if (bx)
      {
      mouse( MOUSE_OFF, &bx, &cx, &dx);
      switch (bx)
        {
        default : break;
/*        case  LEFTBUTTON:  pixel_on( cx, dx); break;*/
        case  LEFTBUTTON: printf( "bx=%d, row=%d, col=%d\n", bx, (cx>>3), (dx>>3) ); break;
        case  RIGHTBUTTON:/* change_mode( 2);*/ break; /* Clear sketching area */
        case  BOTHBUTTONS:
          mouse( MOUSE_OFF, &bx, &cx, &dx); change_mode( 2); return (0);
        }
      mouse( MOUSE_ON, &bx, &cx, &dx);
      }
    }
  }
