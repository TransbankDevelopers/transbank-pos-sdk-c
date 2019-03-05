#ifndef TBKHEADER_FILE
#define TBKHEADER_FILE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>

struct sp_port *port;

static char POLLING_MESSAGE[] = {0x02, 0x30, 0x31, 0x30, 0x30, 0x03, 0x02};
size_t POLLING_SIZE = sizeof(POLLING_MESSAGE);

enum HexCodes{
  ACK = 0x06,
  NACK = 0x15,
  STX = 0x02,
  ETX = 0x03,
  PIPE = 0x7C
};

//115200 Bps (8N1)
static const int TBK_BAUD_RATE = 115200;
static const int TBK_BITS = 8;
static const int TBK_PARITY = SP_PARITY_NONE;
static const int TBK_STOP_BITS = 1;

extern int list_ports();

extern int select_port(char* portName);

extern int ConfigureSerialPort(struct sp_port *port);

extern int ReadBytes(char* buf, int size);

extern int Polling();

extern int closePort();

#endif
