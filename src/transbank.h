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
