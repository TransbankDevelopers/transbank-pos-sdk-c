#include "transbank.h"
#include "transbank_serial_utils.h"

int main() {

  char* portName = "COM4";
  int retval = open_port(portName, 115200);
  if ( retval == TBK_OK ){
    puts("Serial port successfully opened.\n");

    BaseResponse rsp = register_close();

    printf("Function: %i\n", rsp.function);
    printf("Response Code: %i\n", rsp.responseCode);
    printf("Commerce Code: %llu\n", rsp.commerceCode);
    printf("Terminal ID: %lu\n", rsp.terminalId);
    puts("Register closed sucsesfully\n");
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
