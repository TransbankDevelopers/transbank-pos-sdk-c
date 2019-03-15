#ifndef TBKHEADER_FILE
#define TBKHEADER_FILE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libserialport.h>
#include "message.h"

enum TbkReturn{
    TBK_OK = 0,
    TBK_NOK = -1
};

extern enum TbkReturn open_port(char* portName, int baudrate);
extern enum TbkReturn polling();
extern int set_normal_mode();
extern int close_port();

#endif
