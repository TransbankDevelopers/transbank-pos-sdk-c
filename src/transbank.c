#include "transbank.h"

static char GET_TOTALS_MESSAGE[] = {0x02, 0x30, 0x37, 0x30, 0x30, 0x7C, 0x7C, 0x03, 0x04};
static char LOAD_KEYS_MESSAGE[] = {0x02, 0x30, 0x38, 0x30, 0x30, 0x03, 0x0B};
static char POLLING_MESSAGE[] = {0x02, 0x30, 0x31, 0x30, 0x30, 0x03, 0x02};
static char CHANGE_TO_NORMAL_MESSAGE[] = {0x02, 0x30, 0x33, 0x30, 0x30, 0x03, 0x00};

struct message_t {
  int payloadSize;
  int responseSize;
  int retries;
  char* payload;
};

static Message CHANGE_TO_NORMAL = {
  .payload = CHANGE_TO_NORMAL_MESSAGE,
  .payloadSize = 7,
  .responseSize = 1,
  .retries = 3
};


char ** list_ports() {
  struct sp_port **ports;
  char** portList;

  int retval = sp_list_ports(&ports);

  int portSize = 0;
  for (int i=0; ports[i] != NULL; i++){
    portSize++;
  }

  if (retval == SP_OK) {
    portList = malloc(portSize * sizeof(char*));
    if (portList != NULL ){
      for (int i = 0; i < portSize; i++) {
        char* name = sp_get_port_name(ports[i]);
        portList[i] = malloc(strlen(name));
        if (portList[i] != NULL)
        {
          strcpy(portList[i], name);
        }
      }
    }
  }else {
    printf("No serial devices detected\n");
  }
  sp_free_port_list(ports);
  return portList;
}

char* get_configured_port_name(){
  if (port != NULL)
    return sp_get_port_name(port);
  else
    return "No port configured";
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

int open_configured_port(){
  return sp_open(port, SP_MODE_WRITE | SP_MODE_READ);
}

int configure_port() {
  int retval = 0;

  retval += sp_new_config(&config);
  retval += sp_set_config_baudrate(config, TBK_BAUD_RATE);
  retval += sp_set_config_bits(config, TBK_BITS);
  retval += sp_set_config_parity(config, TBK_PARITY);
  retval += sp_set_config_stopbits(config, TBK_STOP_BITS);

  retval += sp_set_config(port, config);
  retval = sp_flush(port, SP_BUF_BOTH);
  sp_free_config(config);

  return retval;
}

int write_message(Message message){
  int retval = sp_blocking_write(port, message.payload, message.payloadSize, DEFAULT_TIMEOUT);
  if (retval == message.payloadSize && sp_drain(port))
  {
    retval = TBK_OK;
  }
  else
    retval -= message.payloadSize;
  sp_flush(port, SP_BUF_OUTPUT);
  return retval;
}

int read_bytes(char* buf, Message message){
  memset(buf, 0, message.responseSize);
  int retval = sp_blocking_read_next(port, buf, message.responseSize, DEFAULT_TIMEOUT);
  if (retval == message.responseSize)
    retval = TBK_OK;
  else
    retval -= message.responseSize;
  sp_flush(port, SP_BUF_INPUT);
  return retval;
}

int get_totals(){
  sp_flush(port, SP_BUF_BOTH);
  int messageSize = sizeof(GET_TOTALS_MESSAGE);
  int retval = 0;
  int bytesCount = sp_blocking_write(port, GET_TOTALS_MESSAGE, messageSize, 100);
  if (bytesCount == messageSize){
    char* returnBuf;
    retval = sp_drain(port);
    if (retval == SP_OK)
    {
      bytesCount = read_bytes(returnBuf, CHANGE_TO_NORMAL);
      if (bytesCount > 0){
        for (int i = 0; i < bytesCount; i++){
          printf("%02X ", returnBuf[i]);
        }
        putchar('\n');
        if (returnBuf[0] == ACK)
            retval = 0;
          else
            retval = -1;
      }
      else
      {
        retval = -1;
        printf("No bytes readed\n");
      }
    }
    else
    {
      retval = -1;
      printf("Unable to write all bytes...");
    }
  }
  else
    retval = bytesCount;
  sp_flush(port, SP_BUF_BOTH);
  return retval;
}

int load_keys(){
  sp_flush(port, SP_BUF_BOTH);
  int messageSize = sizeof(LOAD_KEYS_MESSAGE);
  int retval = 0;
  int bytesCount = sp_blocking_write(port, LOAD_KEYS_MESSAGE, messageSize, 100);
  if (bytesCount == messageSize){
    char* returnBuf;

    retval = sp_drain(port);
    if (retval == SP_OK)
    {
      bytesCount = read_bytes(returnBuf, CHANGE_TO_NORMAL);
      if (bytesCount > 0){
        for (int i = 0; i < bytesCount; i++){
          printf("%02X ", returnBuf[i]);
        }
        putchar('\n');
        if (returnBuf[0] == ACK)
            retval = 0;
          else
            retval = -1;
      }
      else
      {
        retval = -1;
        printf("No bytes readed\n");
      }
    }
    else
    {
      retval = -1;
      printf("Unable to write all bytes...");
    }
  }
  else
    retval = bytesCount;
  sp_flush(port, SP_BUF_BOTH);
  return retval;
}

int polling(){
  sp_flush(port, SP_BUF_BOTH);
  int messageSize = sizeof(POLLING_MESSAGE);
  int retval = 0;
  int bytesCount = sp_blocking_write(port, POLLING_MESSAGE, messageSize, 100);
  if (bytesCount == messageSize){
    char* returnBuf;

    retval = sp_drain(port);

    if (retval == SP_OK)
    {
      bytesCount = read_bytes(returnBuf, CHANGE_TO_NORMAL);
      if (bytesCount > 0){
        for(int i=0; i<bytesCount; i++)
          {
            printf("%02X ", returnBuf[i]);
          }
          putchar('\n');
          if (returnBuf[0] == ACK)
            retval = 0;
          else
            retval = -1;
      }
      else
      {
        retval = -1;
        printf("No bytes readed\n");
      }
    }
    else
    {
      retval = -1;
      printf("Unable to write all bytes...");
    }
  }
  else
    retval = bytesCount;
  sp_flush(port, SP_BUF_BOTH);
  return retval;
}

int set_normal_mode(){
  int tries = 0;
  do
  {
    int retval = write_message(CHANGE_TO_NORMAL);
    if (retval == TBK_OK){
      char* returnBuf;
      retval = read_bytes(returnBuf, CHANGE_TO_NORMAL);
      if (retval == TBK_OK && returnBuf[0] == ACK)
        return TBK_OK;
    }
    tries++;
  } while (tries < CHANGE_TO_NORMAL.retries);
  return TBK_NOK;
}

int close_port(){
  sp_flush(port, SP_BUF_BOTH);
  sp_free_port(port);
  return sp_close(port);
}
