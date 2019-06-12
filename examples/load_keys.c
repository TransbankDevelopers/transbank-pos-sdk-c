#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Serial port successfully opened...\n");

    BaseResponse response = load_keys();

    if (response.function == 810)
    {
      printf("Function: %i\n", response.function);
      printf("Response Code: %i\n", response.responseCode);
      printf("Commerce Code: %llu\n", response.commerceCode);
      printf("Terminal ID: %i\n", response.terminalId);

      if (response.responseCode == TBK_OK)
      {
        puts("Keys loaded successfully.\n=================\n");
      }
      else
      {
        puts("Error: Keys loaded failed.\n=================\n");
      }
    }
    else
    {
      puts("Unable load keys on selected port.\n");
    }

    //Close Port
    retval = close_port();
    if (retval == SP_OK)
    {
      printf("Serial port closed successfully.\n"); // Success!
    }
    else
    {
      puts("Unable to close serial port.\n");
    }
  }
  else
  {
    puts("Unable to open serial port.\n");
  }

  return 0;
}
