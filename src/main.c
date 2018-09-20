/**
 * PLATOTerm PC - Main
 */

#include <stdbool.h>
#include "protocol.h"
#include "conio.h"
#include "screen.h"
#include "splash.h"
#include "io.h"
#include "touch.h"
#include "terminal.h"
#include "keyboard.h"

unsigned char already_started=false;

void main(void)
{
  screen_init();
  io_init();
  touch_init();
  terminal_init();
  ShowPLATO(splash,sizeof(splash));
  terminal_initial_position();
  for (;;)
    {
        io_main();
        keyboard_main();
        touch_main();
    }
}
