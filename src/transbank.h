#ifndef TBKHEADER_FILE
#define TBKHEADER_FILE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libserialport.h>
#include "message.h"
#include "responses.h"

enum TbkReturn{
    TBK_OK = 0,
    TBK_NOK = -1
};

extern enum TbkReturn open_port(char* portName, int baudrate);
extern enum TbkReturn sale(long amount, int ticket, bool send_messages);
extern LoadKeyCloseResponse register_close();
extern LoadKeyCloseResponse load_keys();
extern enum TbkReturn polling();
extern enum TbkReturn set_normal_mode();
extern enum TbkReturn close_port();

#endif
