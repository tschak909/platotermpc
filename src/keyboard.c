#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include "io.h"
#include "protocol.h"
#include "plato_key.h"
#include "key.h"
#include "keyboard.h"

char is_escape;
char shift_lock;


void keyboard_out(unsigned char platoKey)
{
    if (platoKey==0xff)
    return;
  
  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;
  
}

void keyboard_main(void)
{
  unsigned char ch;
  if (kbhit())
    {
      ch=getch();
      if (is_escape==true && ch==0x1B) // ESC
  	{
  	  /* screen_beep(); */

  	  if (shift_lock==true)
  	    shift_lock=false;
  	  else
  	    shift_lock=true;
	  
  	  is_escape=false;
  	}
      else if (is_escape==false && ch==0x1B)
  	is_escape=true;
      /* else if (ch==0x1A) // CTRL-Z for prefs */
      /* 	prefs_run(); */
      else if (TTY)
  	{
  	  keyboard_out_tty(ch);
  	}
      else if (is_escape==true)
  	{
  	  keyboard_out(esc_key_to_pkey[ch]);
  	  is_escape=false;
  	}
      else if (shift_lock==true)
  	{
  	  keyboard_out(shiftlock_key_to_pkey[ch]);
  	}
      else
  	{
  	  keyboard_out(key_to_pkey[ch]);
  	}
    }
}

void keyboard_clear(void)
{
}

void keyboard_out_tty(int ch)
{
  io_send_byte(ch);
  if (ch==0x0D)
    io_send_byte(0x0A); // Send a linefeed along with CR in TTY mode.
}
