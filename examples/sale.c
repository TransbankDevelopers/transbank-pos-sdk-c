#include "transbank.h"
#include "transbank_serial_utils.h"

int main() {

  char* portName = "COM4";

  int retval = open_port(portName, 115200);
  if ( retval == TBK_OK ){

    printf("New Sale transaction...\n");
    if ( sale( 12500, 2, false) == TBK_OK){
      printf("New Sale\n");
    } else{
      printf("Error\n");
    }
  } else{
      printf("Unable to open selected port\n");
  }

  //Close Port
  retval = close_port();
  if(retval == SP_OK){
    puts("Serial port closed.\n");
  } else{
    puts("Unable to close serial port.\n");
  }

  return 0;
}
