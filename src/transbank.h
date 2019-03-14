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

struct message_t {
  int payloadSize;
  int responseSize;
  int retries;
  char* payload;
};

extern enum tbk_return open_port(char* portName, int baudrate);
extern enum tbk_return polling();
extern int set_normal_mode();
extern int close_port();

#endif
