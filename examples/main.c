#include "transbank.h"
#include <unistd.h>

int main() {

  //Select USB Serial Adapter
  //char* portName = "/dev/cu.wchusbserial14240"; //CH340 Chipset
  //char* portName = "/dev/cu.usbserial"; //Prolific AFC
  char* portName = "COM4";

  printf("List all ports\n");
  char** ports = list_ports();
  for (int i=0; i < 3; i++)
    printf("%s\n", ports[i]);


  printf("Selecting Port %s\n", portName);
  int retval = select_port(portName);
  if ( retval == TBK_OK ){
    printf("Open and Configure port: %s\r\n", get_configured_port_name());
    retval = open_configured_port();
    if (retval == TBK_OK)
    {
      puts("Serial port successfully opened.\n");
      if (configure_port() == TBK_OK)
      {
        printf("Polling the POS...\n");
        if (polling() == TBK_OK)
          printf("POS CONNECTED\n");
        else
          printf("Unable to poll the pos\n");

        //printf("Request to LoadKeys...\n");
        //if (load_keys() == TBK_OK)
        //  printf("Keys Loaded OK\n");
        //else
        //  printf("Unable to load keys\n");

        //sleep(200);
        //printf("Get Totals...\n");
        //if (get_totals() == TBK_OK)
        //  printf("Totals OK\n");
        //else
        //  printf("Unable to get Totals\n");

        //printf("Change to Normal Pos. \n\n");
        //if (change_normal_mode() == TBK_OK)
        //  printf("Success\n");
        //else
        //  printf("Nothing to do\n");
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
