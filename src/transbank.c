#include "transbank.h"

struct sp_port *port;
struct sp_port_config *config;

static const int DEFAULT_TIMEOUT = 500;
static const int TBK_BAUD_RATE = 115200;
static const int TBK_BITS = 8;
static const int TBK_PARITY = SP_PARITY_NONE;
static const int TBK_STOP_BITS = 1;

enum HexCodes{
  ACK = 0x06,
  NACK = 0x15,
  STX = 0x02,
  ETX = 0x03,
  PIPE = 0x7C
};

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

static Message GET_TOTALS = {
  .payload = GET_TOTALS_MESSAGE,
  .payloadSize =7,
  .responseSize = 21,
  .retries = 1
};

static Message LOAD_KEYS = {
  .payload = LOAD_KEYS_MESSAGE,
  .payloadSize = 7,
  .responseSize = 33,
  .retries = 3
};

static Message POLLING = {
  .payload = POLLING_MESSAGE,
  .payloadSize = 7,
  .responseSize = 1,
  .retries = 3
};

static Message CHANGE_TO_NORMAL = {
  .payload = CHANGE_TO_NORMAL_MESSAGE,
  .payloadSize = 7,
  .responseSize = 1,
  .retries = 3
};

void print_ports(){
  struct sp_port **ports;
  int retval = sp_list_ports(&ports);
  for (int i=0; ports[i] != NULL; i++){
    printf("%s\n", sp_get_port_name(ports[i]));
  }
}

char * list_ports() {
  struct sp_port **ports;
  char* portList;

  int retval = sp_list_ports(&ports);

  if (retval == SP_OK) {
    int separators = 0;
    int chars = 0;
    for (int i=0; ports[i] != NULL; i++){
      separators++;
      chars += strlen(sp_get_port_name(ports[i]));
    }

    portList = malloc((separators) + chars * sizeof(char*));
    if (portList != NULL ){
      for (int i = 0; i < separators; i++) {
        char* name = sp_get_port_name(ports[i]);
        strcat(portList, name);
        if (i < separators -1)
        {
          strcat(portList, "|");
        }
      }
    }
  }else {
    char* error = "No serial devices detected\n";
    portList = malloc(strlen(error) * sizeof(char*));
    strcpy(portList, error);
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

int reply_ack(){
  char buf[] = {ACK};
  return sp_blocking_write(port, buf, 1, DEFAULT_TIMEOUT);
}

int read_bytes(char* buf, Message message){
  memset(buf, 0, message.responseSize);
  int retval = sp_blocking_read_next(port, buf, message.responseSize, DEFAULT_TIMEOUT);
  if (retval == message.responseSize)
    retval = TBK_OK;
  else{
    retval -= message.responseSize;
  }
  sp_flush(port, SP_BUF_INPUT);
  return retval;
}

int read_ack(){
  char* buf;
  int retval = sp_blocking_read_next(port, buf, 1, DEFAULT_TIMEOUT);
  if (retval == 1 && buf[1] == ACK)
    return TBK_OK;
  else
    return retval;
}

int get_totals(){
  int tries = 0;
  do
  {
    int retval = write_message(GET_TOTALS);
    if (retval == TBK_OK){
      char* returnBuf;
      retval = read_bytes(returnBuf, GET_TOTALS);
      if (retval == TBK_OK && returnBuf[0] == ACK)
        return TBK_OK;
    }
    tries++;
  } while (tries < GET_TOTALS.retries);
  return TBK_NOK;
}

int load_keys(){
  int tries = 0;
  do
  {
    int retval = write_message(LOAD_KEYS);
    if (retval == TBK_OK){
      char* returnBuf;
      retval = read_ack();
      if (retval == TBK_OK && returnBuf[0] == ACK){
        retval = read_bytes(returnBuf, LOAD_KEYS);
        if (retval == TBK_OK)
          for(int i=0; i < sizeof(returnBuf); i++)
            printf("%0X2",returnBuf[i]);
      }
    }
    tries++;
  } while (tries < LOAD_KEYS.retries);

  return TBK_NOK;
}

int polling(){
  int tries = 0;
  do
  {
    int retval = write_message(POLLING);
    if (retval == TBK_OK){
      if (read_ack() == TBK_OK)
        return TBK_OK;
    }
    tries++;
  } while (tries < POLLING.retries);
  return TBK_NOK;
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
