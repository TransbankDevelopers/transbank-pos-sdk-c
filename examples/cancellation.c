#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Cancellation request...\n");
    printf("Please insert Operation Number:");
    int op;
    scanf("%i", &op);

    char *cancellationResponse = cancellation(op); // cancellation(transactionID)
    printf("Cancellation response %s\n", cancellationResponse);
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
