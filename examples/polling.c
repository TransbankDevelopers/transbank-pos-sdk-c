#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    puts("Serial port successfully opened.\n");
    printf("Polling the POS...\n");
    if (poll() == TBK_OK)
    {
      printf("POS CONNECTED\n");
    }
    else
    {
      printf("Unable to poll the pos\n");
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
