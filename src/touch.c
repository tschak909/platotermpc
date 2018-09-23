
#include <i86.h>
#include <stdbool.h>
#include <stdio.h>
#include <graph.h>
#include "touch.h"
#include "touch_tables.h"
#include "protocol.h"

#define INT_MOUSE 0x33

bool touch_installed=false;
bool touch_soft_cursor=false;
bool touch_soft_cursor_visible=false;
uint16_t touch_soft_cursor_x;
uint16_t touch_soft_cursor_y;
bool touch_cursor_visible=false;
bool touch_pressed=false;
int16_t touch_x_offset; // centering offset correction.
int16_t touch_y_offset;
int16_t touch_x_max;
int16_t touch_y_max;
uint16_t* scaletx;
uint16_t* scalety;

extern union REGS regs;
extern uint16_t width;
extern uint16_t height;
extern uint8_t screen_mode;

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{
  char far *herc_flag = (char far *)0x449; // 0040:0049
  
  // If in hercules mode, try to enable herc mode.
  if (screen_mode==2)
    {
      *herc_flag=6;
    }
  
  regs.w.ax=0;
  int86(INT_MOUSE,&regs,&regs);
  if (regs.w.ax == 0xffff)
    touch_installed=true;
  
  if (width==320)
    scaletx=scaletx_320;
  else
    scaletx=scaletx_640;

  touch_x_max=639;
  
  switch(width)
    {
    case 320:
      touch_x_offset=0;
      break;
    case 640:
      touch_x_offset=-64;
      break;
    case 720:
      touch_x_offset=-104;
      break;
    }
  
  // Assign height touch table
  switch(height)
    {
    case 200:
      scalety=scalety_200;
      touch_y_offset=0;
      touch_y_max=199;
      break;
    case 350:
      if (screen_mode==2)
	scalety=scalety_200; // Hercules, Because, reasons...
      else
	scalety=scalety_350;
      touch_y_offset=-15;
      touch_y_max=364;
      break;
    case 480: // Dealing with brain damage, below..
      if (screen_mode==7)
	scalety=scalety_200; // SVGA, because... reasons...
      else
	scalety=scalety_480; // but standard VGA reports correctly.
      touch_y_offset=0;
      touch_y_max=479;
      break;
    }
  
}

/**
 * touch_draw_soft_cursor() - Draw soft cursor, if needed.
 */
void touch_draw_soft_cursor(uint16_t x, uint16_t y)
{
  short currentPlotAction = _getplotaction();
  _setplotaction(_GXOR);

  _moveto(x-8,y);
  _lineto(x+y,y);
  _moveto(x,y-8);
  _lineto(x,y+8);
  
  _setplotaction(currentPlotAction);
}

/**
 * touch_update_soft_cursor() - Update soft mouse cursor
 */
void touch_update_soft_cursor(uint16_t x, uint16_t y)
{
  // URGH.  
  /* if ((touch_soft_cursor_x==x) && (touch_soft_cursor_y==y)) */
  /*   return; // Don't update cursor if not moved. */

  /* /\* if (touch_soft_cursor_visible==true) *\/ */
  /* touch_draw_soft_cursor(touch_soft_cursor_x,touch_soft_cursor_y); */
  /* touch_draw_soft_cursor(x,y); */
  /* /\* touch_soft_cursor_visible=true; *\/ */
  /* /\* touch_draw_soft_cursor(x,y);     *\/ */
  
  /* // Update current position, for later. */
  /* touch_soft_cursor_x=x; */
  /* touch_soft_cursor_y=y; */
}

/**
 * touch_process() - Process touch event
 */
void touch_process(uint16_t x, uint16_t y)
{
  int16_t cx = (int16_t)x+touch_x_offset;
  int16_t cy = (int16_t)y+touch_y_offset;
  padPt touchCoord;
  
  // If we are off the PLATO area, 
  if ((cx<0) || (cy<0) || (cx>touch_x_max) || (cy>touch_y_max))
    return;

  touchCoord.x=scaletx[cx];
  touchCoord.y=scalety[cy];

  Touch(&touchCoord);  
}

/**
 * touch_main() - Main loop for touch screen
 */
void touch_main(void)
{
  if ((touch_installed==false) || (touch_cursor_visible==false))
    return;

  regs.w.ax=3;
  int86(INT_MOUSE,&regs,&regs);

  if (touch_soft_cursor!=false)
    touch_update_soft_cursor(regs.w.cx,regs.w.dx);
  
  if (((regs.w.bx&1)==1) && (touch_pressed==false)) // LMB pressed.
    {
      touch_pressed=true;
      touch_process(regs.w.cx,regs.w.dx);
    }
  else if (((regs.w.bx&1)==0) && (touch_pressed==true)) // LMB released.
    {
      touch_pressed=false;
    }
}

/**
 * touch_allow - Set whether touchpanel is active or not.
 */
void touch_allow(padBool allow)
{
  if (touch_installed==false)
    return;

  // This might seem weird, but there is an internal reference counter
  // specified by Microsoft Mouse drivers which get incremented each time that
  // show mouse cursor gets shown, and decremented when asked to hide.
  // It must not be assumed that PLATO will send the correct # of shows and hides, so
  // as to bring the reference counter back to 0, so we keep an exclusive state
  // and check it here.
  if (allow==true && touch_cursor_visible==false)
    {
      regs.w.ax=1;
      /* touch_soft_cursor_visible=false; */
      touch_cursor_visible=true;
    }
  else if (allow==false && touch_cursor_visible==true)
    {
      regs.w.ax=2;
      /* touch_soft_cursor_visible=false; */
      touch_cursor_visible=false;
    }

  int86(INT_MOUSE,&regs,&regs);
  
}

/**
 * handle_mouse - Process mouse events and turn into scaled touch events
 */
void handle_mouse(void)
{
}

/**
 * touch_hide() - hide the mouse cursor
 */
void touch_hide(void)
{
}

/**
 * touch_done() - Stop the mouse driver
 */
void touch_done(void)
{
  regs.w.ax=2;
  int86(INT_MOUSE,&regs,&regs);
}
