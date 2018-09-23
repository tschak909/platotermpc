/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <graph.h>
#include "io.h"
#include "screen.h"
#include "keyboard.h"
#include "prefs.h"
#include "protocol.h"
#include "terminal.h"
#include "config.h"

extern ConfigInfo config;

uint8_t prefs_running;
extern padBool TTY;
extern padPt TTYLoc;
extern uint8_t CharWide;
extern uint8_t CharHigh;
extern DispMode CurMode;
extern padBool ModeBold;
extern padBool Rotate;
extern padBool Reverse;
extern uint16_t* scalex;
extern uint16_t* scaley;
extern TerminalState terminal_state;

uint8_t temp_val[8];
uint8_t ch;
uint8_t prefs_need_updating;
uint8_t touch_prefs_updated;
uint8_t io_prefs_updated;

/**
 * Run the preferences menu
 */
void prefs_run(void)
{
  keyboard_clear();
  prefs_running=true;
  touch_prefs_updated=false;
  io_prefs_updated=false;
  prefs_need_updating=false;
  prefs_clear();
  while (prefs_running)
    {
      switch (config.io_mode)
	{
	case IO_MODE_SERIAL:
	  prefs_serial();
	  break;
	}
    }

  if (prefs_need_updating)
    {
      prefs_update();
    }
  
  prefs_done();
}

/**
 * prefs_serial()
 * Preferences menu to show for serial devices.
 */
void prefs_serial(void)
{
  prefs_display("p)ort b)aud s)ave e)xit: ");

  ch=prefs_get_key_matching("pbsePBSE");

  switch(ch)
    {
    case 'p':
      prefs_select("port");
      prefs_port();
      break;
    case 'b':
      prefs_select("baud");
      prefs_baud();
      break;
    case 's':
      prefs_select("save");
      prefs_save();
      break;
    case 'e':
      prefs_running=false;
      break;
    }
  
}

/**
 * prefs_get_val()
 * get string with ip address numbers, terminated by return.
 */
void prefs_get_val(void)
{
  unsigned char strp=0;
  
  ch=0;

  while (ch != 0x0d)
    {
      ch=prefs_get_key_matching1("0123456789");
      if (ch==0x08) /* was translated from 0x14 to 0x08 */
  	{
	  if (strp>0)
	    {
	      --strp;
	      temp_val[strp]=0;
	      ShowPLATO(&ch,1);
	    }
  	}
      else if (ch==0x0d)
	{
	  // Don't append or show the CR
	}
      else
  	{
  	  temp_val[strp]=ch;
  	  ShowPLATO(&ch,1);
	  ++strp;
  	}
    }
}

/**
 * prefs_save(void)
 * Save preferences
 */
void prefs_save(void)
{
  prefs_display("saving preferences...");
#ifdef __ATARI__
  ser_close();
#endif
  config_save();
  prefs_select("ok");
#ifdef __ATARI__
  io_open();
#endif
  prefs_clear();
}

/**
 * prefs_port(void)
 * Preferences to set COM port
 */
void prefs_port(void)
{
  prefs_display("1)COM1 2)COM2 3)COM3 4)COM4");
  ch=prefs_get_key_matching("1234");

  switch(ch)
    {
    case '1':
      prefs_select("COM1");
      config.port=0;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case '2':
      prefs_select("COM2");
      config.port=1;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case '3':
      prefs_select("COM3");
      config.port=2;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case '4':
      prefs_select("COM4");
      config.port=3;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    }
}

/**
 * prefs_baud(void)
 * Preferences for selecting serial baud rate
 */
void prefs_baud(void)
{
  prefs_display("1)200 2)400 9)600 q)19200 w)38400 b)ack: ");
  
  ch=prefs_get_key_matching("129qwbQWB");

  switch(ch)
    {
    case '1':
      prefs_select("1200");
      config.baud=SER_BAUD_1200;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case '2':
      prefs_select("2400");
      config.baud=SER_BAUD_2400;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case '9':
      prefs_select("9600");
      config.baud=SER_BAUD_9600;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'q':
      prefs_select("19200");
      config.baud=SER_BAUD_19200;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'w':
      prefs_select("38400");
      config.baud=SER_BAUD_38400;
      io_prefs_updated=true;
      prefs_need_updating=true;
      break;
    case 'b':
      prefs_select("back");
      break;
    }
}

/**
 * prefs_display(text)
 * Display a line of the preferences menu
 * This routine contains some ifdefs to work around the fact that the commodore targets
 * for CC65 remap ASCII passed in character strings.
 */
void prefs_display(const char* text)
{
  terminal_save();
  CharWide=8;
  CharHigh=16;
  CurMem=M0;
  TTY=true;
  
  TTYLoc.x=0;
  TTYLoc.y=1;
  
  ShowPLATO((unsigned char*)text, strlen(text));
}

/**
 * Wait for a key matching input, return it.
 */
unsigned char prefs_get_key_matching(const char* matches)
{
  unsigned char ch;
  unsigned char i;
  
  while (true)
    {
      ch=tolower(getch());
      for (i=0;i<strlen(matches);++i)
	{
	  if (ch==matches[i])
	    return ch;
	}
    }
}

/**
 * TEMPORARY: Wait for a key matching input, return it.
 */
unsigned char prefs_get_key_matching1(const char* matches)
{
  unsigned char ch=0;
  unsigned char i;
  
  for (;;)
    {
      ch=getch();

      if (ch==0x0d || ch==0x9B)
	return 0x0d;
      else if (ch==0x14)
	return 0x08; /* convert PETSCII DEL to ASCII BS */
      
      for (i=0;i<strlen(matches);++i)
	{
	  if (ch==matches[i])
	    return ch;
	}
    }
}

/**
 * erase prefs bar
 */
void prefs_clear(void)
{
  _setcolor(0);
  _rectangle(_GFILLINTERIOR,scalex[0],scaley[0],scalex[511],scaley[16]);
  _setcolor(1);
  ShowPLATO("\n\v",2);
}

/**
 * indicate selection, display it, and wait a bit for visual confirmation.
 */
void prefs_select(const char* text)
{
  unsigned char i=0;
  ShowPLATO((unsigned char *)text,strlen(text));
  
  for (i=0;i<100;i++)
    {
      screen_wait();
    }

  prefs_clear();

}

/**
 * Update program state with selected preferences
 */
void prefs_update(void)
{
  if (io_prefs_updated==true)
    {
      io_done();
      io_init();
    }

  prefs_clear();
  
}

/**
 * Show PLATOTERM READY - Press '<key>' for setup.
 */
void prefs_show_greeting(void)
{
  prefs_display("PLATOTERM READY - F1 FOR SETUP. ALT-X to QUIT.");
}

/**
 * close prefs. 
 */
void prefs_done(void)
{
  prefs_clear();
  terminal_load();
}
