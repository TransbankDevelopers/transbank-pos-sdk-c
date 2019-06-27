#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Sales detail...\n");
    printf("Please select option, 0 to print or 1 to send info:");
    int op;
    scanf("%i", &op);
    char *list = sales_detail(op);

    if (op == 1)
    {
      printf("SALES DETAIL RESPONSE\n=============\n");
      printf("Transactions list:\n%s\n", list);
    }
    else
    {
      printf("Sales details printed in POS\n");
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
