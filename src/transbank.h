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

enum TbkReturn
{
    TBK_OK = 0,
    TBK_NOK = -1
};

extern enum TbkReturn open_port(char *portName, int baudrate);
extern char *sale(int amount, int ticket, bool send_messages);
extern BaseResponse close();
extern BaseResponse load_keys();
extern TotalsResponse get_totals();
extern enum TbkReturn poll();
extern enum TbkReturn set_normal_mode();
extern enum TbkReturn close_port();

#endif
