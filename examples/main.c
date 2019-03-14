#include "transbank.h"

int main() {

  char* portName = "COM4";

  printf("List all ports\n");
  printf("%s\n", list_ports());

  printf("Selecting Port %s\n", portName);
  int retval = select_port(portName);
  if ( retval == TBK_OK ){
    printf("Open and Configure port: %s\r\n", get_configured_port_name());
    retval = open_configured_port();
    if (retval == TBK_OK)
    {
      puts("Serial port successfully opened.\n");
      if (configure_port(TBK_115200) == TBK_OK)
      {
        printf("Polling the POS...\n");
        if (polling() == TBK_OK)
          printf("POS CONNECTED\n");
        else
          printf("Unable to poll the pos\n");
      }

      printf("Press ENTER key to Close Port and Exit\n");
      getchar();

      //Close Port
      retval = close_port();
      if(retval == SP_OK)
      {
        puts("Serial port closed.\n");
      }
      else
      {
        puts("Unable to close serial port.\n");
      }
    }
    else
      printf("Unable to open selected port\n");
  }
  else
    printf("Unable to select specified port\n");
  return 0;
}
