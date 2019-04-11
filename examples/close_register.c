#include "transbank.h"
#include "transbank_serial_utils.h"

int main() {

  char* portName = "COM4";
  int retval = open_port(portName, 115200);
  if ( retval == TBK_OK ){
    puts("Serial port successfully opened.\n");
    LoadKeyCloseResponse response = register_close();

    if (response.initilized ){
      printf("Function: %i\n", response.function);
      printf("Response Code: %i\n", response.responseCode);
      printf("Commerce Code: %llu\n", response.commerceCode);
      printf("Terminal ID: %lu\n", response.terminalId);
      puts("Keys loaded sucsesfully\n");
    }
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
