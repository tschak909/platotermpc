#include <i86.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "io.h"
#include "protocol.h"
#include "prefs.h"
#include "config.h"
#include "screen.h"

extern ConfigInfo config;

#define FOSSIL 0x14
#define PORT 0

union REGS regs;

void io_init(void)
{
  unsigned char ch;
  // Initialize port.
  regs.x.dx = config.port;
  regs.h.ah = 0x04;
  int86(FOSSIL, &regs, &regs);

  if (regs.x.ax!=0x1954)
    {
      prefs_display("No FOSSIL loaded. Press any key to return to DOS.");
      ch=getch();
      io_done();
      screen_done();
      exit(0);
    }
  
  // Set line characteristics. 
  regs.h.al = config.baud;
  regs.h.al = (regs.h.al < 5) | 0x03;   /* 8/N/1 */
  regs.x.dx = PORT;
  regs.h.ah = 0x00;
  int86(FOSSIL,&regs,&regs);

  // Set RTS/CTS Flow control
  regs.h.ah = 0x0f;
  regs.h.al = 0x02;
  regs.x.dx = PORT;
  int86(FOSSIL,&regs,&regs);

  io_raise_dtr();
}

void io_send_byte(unsigned char b)
{
  regs.x.dx = config.port;
  regs.h.al = b;
  regs.h.ah = 0x01;
  int86(FOSSIL, &regs, &regs);
}

void io_lower_dtr(void)
{
  // Lower DTR
  regs.h.ah = 0x06;
  regs.h.al = 0x00;
  regs.x.dx = config.port;
  int86(FOSSIL,&regs,&regs);  
}

void io_raise_dtr(void)
{
  // Raise DTR
  regs.h.ah = 0x06;
  regs.h.al = 0x01;
  regs.x.dx = config.port;
  int86(FOSSIL,&regs,&regs);
}

void io_hang_up(void)
{
  io_lower_dtr();
  delay(500);
  io_raise_dtr();
}

void io_main(void)
{
  unsigned char ch;
  // Get port status
  regs.x.dx = config.port;
  regs.h.al = 0;
  regs.h.ah = 3;
  int86(FOSSIL, &regs, &regs);

  if ((regs.x.ax&0x100) !=0)
    {
      // Data is waiting, process it.
      regs.x.dx = config.port;
      regs.h.ah = 0x02;
      int86(FOSSIL, &regs, &regs);
      ch=regs.x.ax&0x00ff;
      ShowPLATO(&ch,1);
    }
}

void io_done(void)
{
  io_hang_up();
  
  // Deinitialize driver.
  regs.h.ah = 0x05;
  regs.x.dx = config.port;
  int86(FOSSIL,&regs,&regs);
}
