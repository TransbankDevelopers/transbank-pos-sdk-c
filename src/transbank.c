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

int set_normal_mode(){
  int tries = 0;
  do{
    int retval = write_message(port, CHANGE_TO_NORMAL);
    if (retval == TBK_OK){
      char* returnBuf;
      retval = read_bytes(port, returnBuf, CHANGE_TO_NORMAL);
      if (retval == TBK_OK && returnBuf[0] == ACK){
        return TBK_OK;
      }
    }
    tries++;
  } while (tries < CHANGE_TO_NORMAL.retries);
  return TBK_NOK;
}

int close_port(){
  int retval = sp_flush(port, SP_BUF_BOTH);
  retval += sp_close(port);
  sp_free_port(port);
  return retval;
}
