
#include <i86.h>
#include <stdbool.h>
#include <stdio.h>
#include "touch.h"

#define INT_MOUSE 0x33

bool touch_installed=false;
bool touch_soft_cursor=false;
bool touch_cursor_visible=false;

extern union REGS regs;

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{
  regs.w.ax=0;
  int86(INT_MOUSE,&regs,&regs);
  if (regs.w.ax == 0xffff)
    touch_installed=true;
}

/**
 * touch_main() - Main loop for touch screen
 */
void touch_main(void)
{
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
      touch_cursor_visible=true;
    }
  else if (allow==false && touch_cursor_visible==true)
    {
      regs.w.ax=2;
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
  
}
