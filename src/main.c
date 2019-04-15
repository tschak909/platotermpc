/**
 * PLATOTerm PC - Main
 */

#include <stdbool.h>
#include <string.h>
#include "protocol.h"
#include "conio.h"
#include "screen.h"
#include "splash.h"
#include "io.h"
#include "touch.h"
#include "terminal.h"
#include "keyboard.h"
#include "config.h"
#include "prefs.h"

unsigned char already_started=false;
extern unsigned char screen_mode;

void process_args(int argc, char* argv[])
{
  int i;
  for (i=1;i<=argc;++i)
    {
      if (!strcasecmp(argv[i],"/CGALO"))
	screen_mode=0;
      else if (!strcasecmp(argv[i],"/CGA"))
	screen_mode=1;
      else if (!strcasecmp(argv[i],"/HERCULES"))
	screen_mode=2;
      else if (!strcasecmp(argv[i],"/EGA"))
	screen_mode=3;
      else if (!strcasecmp(argv[i],"/VGAMONO"))
	screen_mode=4;
      else if (!strcasecmp(argv[i],"/VGA"))
	screen_mode=5;
      else if (!strcasecmp(argv[i],"/MCGA"))
	screen_mode=6;
      else if (!strcasecmp(argv[i],"/SVGA"))
	screen_mode=7;
      else if (!strcasecmp(argv[i],"/GRID"))
	screen_mode=9;
    }
}

void main(int argc, char* argv[])
{
  process_args(argc,argv);
  config_init();
  screen_init();
  io_init();
  touch_init();
  terminal_init();

  // HAAAACK!
  if (screen_mode==9)
    ShowPLATO(grid_splash,sizeof(grid_splash));
  else
    ShowPLATO(splash,sizeof(splash));

  prefs_show_greeting();
  terminal_initial_position();
  for (;;)
    {
        io_main();
        keyboard_main();
        touch_main();
    }
}
