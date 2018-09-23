/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.h - Input/output functions (serial/ethernet)
 */

#ifndef IO_H
#define IO_H

#define XON  0x11
#define XOFF 0x13

#define SER_BAUD_1200 0x04
#define SER_BAUD_2400 0x05
#define SER_BAUD_9600 0x07
#define SER_BAUD_19200 0x00
#define SER_BAUD_38400 0x01

/**
 * io_init() - Set-up the I/O
 */
void io_init(void);

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(unsigned char b);

/**
 * io_main() - The IO main loop
 */
void io_main(void);

void io_lower_dtr(void);

void io_raise_dtr(void);

void io_hang_up(void);

void io_done(void);

#endif /* IO_H */
