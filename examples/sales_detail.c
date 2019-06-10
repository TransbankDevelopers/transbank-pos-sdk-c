#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Sales detail...\n");

    SalesDetailResponse response = sales_detail();
    if (response.initilized == TBK_OK)
    {
      printf("SALES DETAIL RESPONSE\n=============\n");

      // printf("Function : %i \n", response.function);
      // printf("Response Code : %i \n", response.responseCode);
      // printf("Commerce Code : %llu \n", response.commerceCode);
      // printf("Terminal ID : %i \n", response.terminalId);
      // printf("Authorization Code : %i \n", response.authorizationCode);
      // printf("Operation ID : %i \n\n", response.operationID);
    }
    else
    {
      printf("Could not get sales detail.\n");
    }
  }
  else
  {
    printf("Unable to open selected port\n");
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
