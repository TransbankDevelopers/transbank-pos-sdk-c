#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Serial port successfully opened.\n");

    BaseResponse response = close();

    if (response.function == 510)
    {
      printf("Function: %i\n", response.function);
      printf("Response Code: %i\n", response.responseCode);
      printf("Commerce Code: %llu\n", response.commerceCode);
      printf("Terminal ID: %s\n", response.terminalId);

      if (response.responseCode == TBK_OK)
      {
        printf("Register closed successfully\n=================\n");
      }
      else
      {
        printf("Error: Close POS has failed\n=================\n");
      }
    }
    else
    {
      printf("Unable close POS.\n");
    }
  }
  //Close Port
  retval = close_port();
  if (retval == SP_OK)
  {
    printf("Serial port closed.\n");
  }
  else
  {
    printf("Unable to close serial port.\n");
  }

  return 0;
}
