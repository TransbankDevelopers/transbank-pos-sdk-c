#include "transbank.h"
#include "transbank_serial_utils.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    puts("Serial port successfully opened.\n");

    BaseResponse rsp = register_close();

    printf("Function: %i\n", rsp.function);
    printf("Response Code: %i\n", rsp.responseCode);
    printf("Commerce Code: %llu\n", rsp.commerceCode);
    printf("Terminal ID: %i\n", rsp.terminalId);
    puts("Register closed successfully\n");
  }
  //Close Port
  retval = close_port();
  if (retval == SP_OK)
  {
    puts("Serial port closed.\n");
  }
  else
  {
    puts("Unable to close serial port.\n");
  }

  return 0;
}
