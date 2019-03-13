#ifndef TBKHEADER_FILE
#define TBKHEADER_FILE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>

enum tbk_return{
    TBK_OK = 0,
    TBK_NOK = -1
};

enum tbk_baudrate {
    TBK_1200 = 1200,
    TBK_2400 = 2400,
    TBK_4800 = 4800,
    TBK_9600 = 9600,
    TBK_19200 = 19200,
    TBK_115200 = 115200
};

typedef struct message_t Message;

extern void print_ports();

extern char* list_ports();

extern char* get_configured_port_name();

extern int open_configured_port();

extern int select_port(char* portName);

extern int configure_port(enum tbk_baudrate baud_rate);

extern int get_totals();

extern int load_keys();

extern enum tbk_return polling();

extern int set_normal_mode();

extern int close_port();

#endif
