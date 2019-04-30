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

    int res = set_normal_mode();
    if (res == TBK_OK)
    {
      puts("Normal model activated successfully\n");
    }
    else
    {
      puts("Setting Normal mode has failed\n");
    }
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
