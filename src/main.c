#include "transbank.h"

int main() {

  char* portName = "/dev/cu.wchusbserial14240"; //Adaptador CH340

  printf("Selecting Port %s\n", portName);
  select_port(portName);

  printf("Open and Configure port: %s\r\n", sp_get_port_name(port));
  int retval = sp_open(port, SP_MODE_WRITE | SP_MODE_READ);
  if (retval == SP_OK)
  {
    puts("Serial port successfully opened.\n");

    if (ConfigureSerialPort(port) == 0)
    {
      printf("Start Writing...\n\n");
      Polling();
    }
    closePort();
    return 0;
  }
}
