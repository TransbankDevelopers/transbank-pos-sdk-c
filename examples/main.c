#include "transbank.h"
#include <unistd.h>

int main() {

  //Select USB Serial Adapter
  char* portName = "/dev/cu.wchusbserial14240"; //CH340 Chipset

  //printf("List all ports\n");
  //char** ports = list_ports();
  //for (int i=0; i < 3; i++)

  printf("Selecting Port %s\n", portName);
  int retval = select_port(portName);
  if ( retval == SP_OK ){
    printf("Open and Configure port: %s\r\n", get_configured_port_name());
    retval = open_configured_port();
    if (retval == SP_OK)
    {
      puts("Serial port successfully opened.\n");
      if (configure_port() == TBK_OK)
      {
        //printf("List all ports\n");
        //char** ports = list_ports();
        //for (int i=0; i < 3; i++)
        //  printf("%s\n", ports[i]);

        //printf("Polling the POS...\n\n");
        //polling();

        //printf("Change to Normal Pos. \n\n");
        //if (change_normal_mode() == TBK_OK)
        //  printf("Success\n");
        //else
        //  printf("Nothing to do\n");

        //printf("Request to LoadKeys...\n\n");
        //load_keys();

        //printf("Get Totals...\n\n");
        //get_totals();

        //char MESSAGE[] = { 0x02, 0x30, 0x37, 0x30, 0x30, 0x7C, 0x7C, 0x03, 0x04 };
        //sp_blocking_write(port, MESSAGE, 16, 100);

        //getchar();

        //char ACK[] = {0x06};
        //sp_blocking_write(port, ACK, 16, 100);

      }

      printf("Press ENTER key to Close Port and Exit\n");
      getchar();

      //Close Port
      retval = close_port();
      if(retval == SP_OK)
      {
        puts("Serial port closed.");
      }
      else
      {
        puts("Unable to close serial port.");
      }
    }
    else
      printf("Unable to open selected port");
  }
  else
    printf("Unable to select specified port");
  return 0;
}
