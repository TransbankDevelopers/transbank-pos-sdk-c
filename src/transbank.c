#include "transbank.h"

static char POLLING_MESSAGE[] = {0x02, 0x30, 0x31, 0x30, 0x30, 0x03, 0x02};
size_t POLLING_SIZE = sizeof(POLLING_MESSAGE);


int list_ports() {
  struct sp_port **ports;
  int retval = sp_list_ports(&ports);
  if (retval == SP_OK) {
    for (int i = 0; ports[i]; i++) {
      printf("port: '%s'\n", sp_get_port_name(ports[i]));
    }
  } else {
    printf("No serial devices detected\n");
  }
  sp_free_port_list(ports);
  return retval;
}

int select_port(char* portName) {
  int retval = sp_get_port_by_name(portName, &port);
  if (retval == SP_OK) {
    printf("Port Selected\n\n");
  } else {
    printf("Unable to select Port\n");
  }
  return retval;
}

int ConfigureSerialPort(struct sp_port *port) {
  int retval = 0;

  if (SP_OK != sp_set_baudrate(port, TBK_BAUD_RATE))
    retval = -1;
  else if(SP_OK != sp_set_bits(port, TBK_BITS))
    retval = -1;
  else if (SP_OK !=  sp_set_parity(port, TBK_PARITY))
    retval = -1;
  else if (SP_OK != sp_set_stopbits(port, TBK_STOP_BITS))
    retval = -1;
  else
    puts("Port configured.\n");
  return retval;
}

int ReadBytes(char* buf, int size){
  int retval = 0;
  do{
    printf("Waitin bytes...\n");
  }while(sp_input_waiting(port) < 0);

  int bytesWaiting = sp_input_waiting(port);

  if (bytesWaiting > 0)
  {
    printf("Pending bytes: %i \n", bytesWaiting);
    memset(buf, 0, size);
    retval = sp_blocking_read(port, buf, size, 100);
    if (retval < 0)
    {
      printf("Error reading from serial port: %d\r\n", retval);
      retval = -1;
    }
  }else
  {
    printf("Error getting bytes available from serial port: %d\r\n", bytesWaiting);
    retval = -1;
  }
  return retval;
}

int Polling(){
  int messageSize = sizeof(POLLING_MESSAGE);
  int retval = sp_nonblocking_write(port, POLLING_MESSAGE, messageSize);
  if (retval == messageSize){
    int returnBufSize = 2;
    char returnBuf[returnBufSize];

    do{
      printf("Writing Data...\n");
    }while(sp_drain(port) != SP_OK);

    retval = ReadBytes(returnBuf, returnBufSize);
    if (retval > 0){
      for(int i=0; i<retval; i++)
        {
          printf("%02X ", returnBuf[i]);
        }
        putchar('\n');
        if (returnBuf[0] == ACK)
          retval = 0;
        else
          retval = -1;
    }
  }else
  {
    retval = -100;
  }
  return retval;
}

int closePort(){
  sp_free_port(port);
  int retval = sp_close(port);
  if(retval == SP_OK)
  {
    puts("Serial port closed.");
  }
  else
  {
    puts("Unable to close serial port.");
  }
  return retval;
}
