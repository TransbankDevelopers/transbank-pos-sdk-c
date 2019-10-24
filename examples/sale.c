#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("New Sale transaction...\n");

    char *saleResponse = sale(2500, "000002", false); // sale(amount, ticket_number, send_messages)
    printf("Sale response %s\n", saleResponse);
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
