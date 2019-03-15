#include "transbank.h"
#include "transbank_serial_utils.h"

int main() {

  char* portName = "COM4";

  printf("List all ports\n");
  printf("Puertos: %s\n", list_ports());

  printf("Open Port %s\n", portName);
  int retval = open_port(portName, 115200);
  if ( retval == TBK_OK ){
    puts("Serial port successfully opened.\n");
    printf("Polling the POS...\n");
    if (polling() == TBK_OK){
      printf("POS CONNECTED\n");
    } else{
      printf("Unable to poll the pos\n");
    }
  } else{
      printf("Unable to open selected port\n");
  }

  printf("Press ENTER key to Close Port and Exit\n");
  getchar();

  //Close Port
  retval = close_port();
  if(retval == SP_OK){
    puts("Serial port closed.\n");
  } else{
    puts("Unable to close serial port.\n");
  }

  return 0;
}
