#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Serial port successfully opened.\n");

    printf("Getting Totals from POS...\n");

    TotalsResponse response = get_totals();
    if (response.initilized == TBK_OK)
    {
      printf("POS CONNECTED\n=============\n");

      printf("Function : %i \n", response.function);
      printf("Response Code : %i \n", response.responseCode);
      printf("TX Count : %i \n", response.txCount);
      printf("TX Total : %i \n", response.txTotal);
    }
    else
    {
      printf("Unable to obtain totals information from POS.\n");
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
  }
  else
  {
    printf("Unable to open selected port\n");
  }

  return 0;
}
