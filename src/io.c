#include <i86.h>
#include <stdio.h>
#include "io.h"
#include "protocol.h"

#define FOSSIL 0x14
#define PORT 0

union REGS regs;

void io_init(void)
{
  // Initialize port.
  regs.x.dx = PORT;
  regs.h.ah = 0x04;
  int86(FOSSIL, &regs, &regs);

  // Set line characteristics. 
  regs.h.al = 0x04; // 1200bps for now
  regs.h.al = (regs.h.al < 5) | 0x03;   /* 8/N/1 */
  regs.x.dx = PORT;
  regs.h.ah = 0x00;
  int86(FOSSIL,&regs,&regs);
  // Come back here and put in error handling.
}

/**
 * Not needed for now, remove.
 */
void io_init_funcptrs(void)
{
}

void io_open(void)
{
}

void io_send_byte(unsigned char b)
{
  regs.x.dx = PORT;
  regs.h.al = b;
  regs.h.ah = 0x01;
  int86(FOSSIL, &regs, &regs);
}

void io_main(void)
{
  unsigned char ch;
  // Get port status
  regs.x.dx = PORT;
  regs.h.al = 0;
  regs.h.ah = 3;
  int86(FOSSIL, &regs, &regs);

  if ((regs.x.ax&0x100) !=0)
    {
      // Data is waiting, process it.
      regs.x.dx = PORT;
      regs.h.ah = 0x02;
      int86(FOSSIL, &regs, &regs);
      ch=regs.x.ax&0x00ff;
      ShowPLATO(&ch,1);
    }
}

void io_recv_serial(void)
{
}

void io_done()
{
}
