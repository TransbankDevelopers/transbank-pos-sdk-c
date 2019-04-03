#include "hex_codes.h"
#include "transbank.h"
#include "transbank_serial_utils.h"

struct sp_port *port;

static int BITS = 8;
static int PARITY = SP_PARITY_NONE;
static int STOP_BITS = 1;
static int FLOW_CONTROL = SP_FLOWCONTROL_NONE;

static char GET_TOTALS_MESSAGE[] = {STX, 0x30, 0x37, 0x30, 0x30, PIPE, PIPE, ETX, 0x04};
static char LOAD_KEYS_MESSAGE[] = {STX, 0x30, 0x38, 0x30, 0x30, ETX, 0x0B};
static char POLLING_MESSAGE[] = {STX, 0x30, 0x31, 0x30, 0x30, ETX, 0x02};
static char CHANGE_TO_NORMAL_MESSAGE[] = {STX, 0x30, 0x33, 0x30, 0x30, ETX, 0x00};

static Message GET_TOTALS = {
  .payload = GET_TOTALS_MESSAGE,
  .payloadSize = 7,
  .responseSize = 21,
  .retries = 1
};

static Message LOAD_KEYS = {
  .payload = LOAD_KEYS_MESSAGE,
  .payloadSize = 7,
  .responseSize = 32,
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

int configure_port(int baud_rate){
  int retval = 0;
  struct sp_port_config *config;

  retval += sp_new_config(&config);
  retval += sp_set_config_baudrate(config, baud_rate);
  retval += sp_set_config_bits(config, BITS);
  retval += sp_set_config_parity(config, PARITY);
  retval += sp_set_config_stopbits(config, STOP_BITS);
  retval += sp_set_config_flowcontrol(config, FLOW_CONTROL);
  retval += sp_set_config(port, config);
  retval += sp_flush(port, SP_BUF_BOTH);
  sp_free_config(config);

  return retval;
}

int open_port(char* portName, int baudrate){
  int retval = sp_get_port_by_name(portName, &port);
  retval += sp_open(port, SP_MODE_READ_WRITE);
  retval += configure_port(baudrate);
  return retval;
}

char* substring(char* string, ParamInfo info){
  char* ret = malloc((sizeof(char) * info.length) +1);
  memset(ret, '\0', sizeof(ret));
  char cToStr[2];
  cToStr[1] = '\0';

  int limit = info.index + info.length;

  for (int i = info.index; i < limit; i++){
    cToStr[0] = string[i];
    strcat(ret, cToStr);
  }
  return ret;
}

LoadKeyCloseResponse* parse_load_keys_response(char* buf){

  ParamInfo function_info = {1,4};
  ParamInfo responseCode_info = {6,2};
  ParamInfo commerceCode_info= {9, 12};
  ParamInfo terminalId_info={22,8};

  LoadKeyCloseResponse* response = malloc(sizeof(response));

  response -> function = strtol(substring(buf,function_info), NULL, 10);
  response -> responseCode = strtol(substring(buf, responseCode_info), NULL, 10);
  response -> commerceCode = strtoll(substring(buf, commerceCode_info), NULL, 10);
  response -> terminalId = strtol(substring(buf, terminalId_info), NULL, 10);
  response -> initilized = TBK_OK;

  return response;
}

enum TbkReturn sale(){
  char msg[] = { 0x02, 0x30, 0x32, 0x30, 0x30, 0x7C, 0x30, 0x30, 0x30, 0x30, 0x32, 0x35, 0x30, 0x30, 0x30, 0x7C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x7C, 0x7C, 0x7C, 0x30, 0x03, 0x7B };

  printf("Writing Message: %s\n", msg);

  Message message = {
    .payload = msg,
    .payloadSize = 28,
    .responseSize = 146,
    .retries = 3
  };

  int retval = write_message(port, message);
  printf("Retval Write: %i\n", retval);

  int waiting = sp_input_waiting(port);
  do
  {
   waiting = sp_input_waiting(port);
  } while (waiting <= 0);

  retval = read_ack(port);
  printf("Retval ACK %i\n", retval);

  waiting = sp_input_waiting(port);
  do
  {
    waiting = sp_input_waiting(port);
  } while (waiting <= 0);

  char buf[146];
  retval = sp_blocking_read(port, buf, message.responseSize, 1500);
  printf("Retval Read %i\n", retval);
  printf("Readed Mesage:\n%s\n", buf);

  reply_ack(port);

  return TBK_OK;
}

LoadKeyCloseResponse load_keys(){
  int tries = 0;
  int retval, write_ok = TBK_NOK;
  LoadKeyCloseResponse *rsp;

  do{
    int retval = write_message(port, LOAD_KEYS);
    if (retval == TBK_OK){
      if (read_ack(port) == TBK_OK){
        write_ok = TBK_OK;
        break;
      }
    }
    tries++;
  } while (tries < LOAD_KEYS.retries);

  if (write_ok == TBK_OK){
    char* buf;
    tries = 0;
    buf = malloc(LOAD_KEYS.responseSize * sizeof(char));

    int wait = sp_input_waiting(port);
    do{
      if (wait == LOAD_KEYS.responseSize){
        retval = read_bytes(port, buf, LOAD_KEYS);
        if (retval == TBK_OK){
          retval = reply_ack(port,buf,LOAD_KEYS.responseSize);
          if (retval == TBK_OK){
            rsp = parse_load_keys_response(buf);
            return *rsp;
          } else {
            tries++;
          }
        } else{
          tries++;
        }
      }
      wait = sp_input_waiting(port);
    } while (tries < LOAD_KEYS.retries);
  }
  return *rsp;
}

enum TbkReturn polling(){
  int tries = 0;
  do{
    int retval = write_message(port, POLLING);
    if (retval == TBK_OK){
      if (read_ack(port) == TBK_OK){
        return TBK_OK;
      }
    }
    tries++;
  } while (tries < POLLING.retries);
  return TBK_NOK;
}

enum TbkReturn set_normal_mode(){
  int tries = 0;
  do{
    int retval = write_message(port, CHANGE_TO_NORMAL);
    if (retval == TBK_OK){
      if (read_ack(port) == TBK_OK){
        return TBK_OK;
      }
    }
    tries++;
  } while (tries < CHANGE_TO_NORMAL.retries);
  return TBK_NOK;
}

enum TbkReturn close_port(){
  int retval = sp_flush(port, SP_BUF_BOTH);
  retval += sp_close(port);
  sp_free_port(port);
  if (retval == TBK_OK){
    return TBK_OK;
  }
  return retval;
}
