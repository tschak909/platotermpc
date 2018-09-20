#include <bios.h>
#include <stdio.h>
#include "io.h"
#include "protocol.h"

unsigned com1_status;
unsigned com1_send;
unsigned com1_rec;
unsigned mask=0x6100;
unsigned char ich;

void io_init(void)
{
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
}

void io_main(void)
{
}

void io_recv_serial(void)
{
}

void io_done()
{
}
