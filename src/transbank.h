#ifndef TBKHEADER_FILE
#define TBKHEADER_FILE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>

static const int TBK_OK = 0;
static const int TBK_NOK = -1;

typedef struct message_t Message;

extern void print_ports();

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
