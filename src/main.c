#include "transbank.h"

int main() {

  char* portName = "/dev/cu.wchusbserial14240"; //Adaptador CH340

  printf("Selecting Port %s\n", portName);
  int retval = select_port(portName);
  if ( retval == SP_OK ){
    printf("Open and Configure port: %s\r\n", sp_get_port_name(port));
    retval = sp_open(port, SP_MODE_WRITE | SP_MODE_READ);
    if (retval == SP_OK)
    {
      puts("Serial port successfully opened.\n");
      if (ConfigureSerialPort(port) == SP_OK)
      {
        printf("Start Writing...\n\n");
        Polling();
      }
      closePort();
      return 0;
    }
  }
}
