#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    puts("Serial port successfully opened.\n");

    printf("Getting Totals from POS...\n");

    TotalsResponse response = get_totals();
    if (response.function)
    {
      printf("POS CONNECTED\n=============\n");

      printf("FUNCTION : %i \n", response.function);
      printf("RESPONSE : %i \n", response.responseCode);
      printf("TX COUNT : %i \n", response.txCount);
      printf("TX TOTAL : %i \n", response.txTotal);
    }
    else
    {
      printf("Unable to obtain totals information\n");
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
  }
  else
  {
    printf("Unable to open selected port\n");
  }

  return 0;
}
