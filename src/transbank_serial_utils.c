#include "transbank_serial_utils.h"

static const int DEFAULT_TIMEOUT = 500;

char* list_ports(){
  struct sp_port **ports;
  char *portList;

  int retval = sp_list_ports(&ports);

  if (retval == SP_OK){
    int separators = 0;
    int chars = 0;
    for (int i=0; ports[i] != NULL; i++){
      separators++;
      chars += strlen(sp_get_port_name(ports[i]));
    }

    portList = malloc((separators) + chars * sizeof(char*));

    if (portList != NULL){
      strcpy(portList, sp_get_port_name(ports[0]));
      for (int i = 1; ports[i] != NULL; i++){
        strcat(portList, "|");
        strcat(portList, sp_get_port_name(ports[i]));
      }
    }
  } else{
    char* error = "No serial devices detected\n";
    portList = malloc(strlen(error) * sizeof(char*));
    strcpy(portList, error);
  }
  sp_free_port_list(ports);
  return portList;
}

char* get_port_name(struct sp_port *port){
  if (port != NULL){
    return sp_get_port_name(port);
  } else{
    return "No port configured";
  }
}

int read_bytes(struct sp_port *port, char* buf, Message message){
  memset(buf, 0, message.responseSize);
  int retval = sp_blocking_read_next(port, buf, message.responseSize, DEFAULT_TIMEOUT);
  if (retval == message.responseSize){
    retval = TBK_OK;
  } else{
    retval -= message.responseSize;
  }
  sp_flush(port, SP_BUF_INPUT);
  return retval;
}

int read_ack(struct sp_port *port){
  char buf[1];
  int retval = sp_blocking_read_next(port, buf, 1, DEFAULT_TIMEOUT);
  if (retval == 1 && buf[0] == ACK){
    return TBK_OK;
  } else{
    return TBK_NOK;
  }
}

int reply_ack(struct sp_port *port){
  char buf[] = {ACK};
  return sp_blocking_write(port, buf, 1, DEFAULT_TIMEOUT);
}

int write_message(struct sp_port *port, Message message){
  int retval = sp_blocking_write(port, message.payload, message.payloadSize, DEFAULT_TIMEOUT);
  if (retval == message.payloadSize && sp_drain(port)){
    retval = TBK_OK;
  } else{
    retval -= message.payloadSize;
  }
  sp_flush(port, SP_BUF_OUTPUT);
  return retval;
}
