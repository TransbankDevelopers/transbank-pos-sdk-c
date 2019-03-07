#ifndef TBKHEADER_FILE
#define TBKHEADER_FILE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>

struct sp_port *port;
struct sp_port_config *config;

enum HexCodes{
  ACK = 0x06,
  NACK = 0x15,
  STX = 0x02,
  ETX = 0x03,
  PIPE = 0x7C
};

static const int TBK_OK = 0;
static const int TBK_NOK = -1;
static const int DEFAULT_TIMEOUT = 100;

static const int TBK_BAUD_RATE = 115200;
static const int TBK_BITS = 8;
static const int TBK_PARITY = SP_PARITY_NONE;
static const int TBK_STOP_BITS = 1;

typedef struct message_t Message;

extern char** list_ports();

extern char* get_configured_port_name();

extern int open_configured_port();

extern int select_port(char* portName);

extern int configure_port();

extern int get_totals();

extern int load_keys();

extern int polling();

extern int set_normal_mode();

extern int close_port();

#endif
