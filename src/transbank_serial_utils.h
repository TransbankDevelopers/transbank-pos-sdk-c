#ifndef TBKSERIALHEADER_FILE
#define TBKSERIALHEADER_FILE

#include "transbank.h"

enum HexCodes{
  ACK = 0x06,
  NACK = 0x15,
  STX = 0x02,
  ETX = 0x03,
  PIPE = 0x7C
};

extern char* list_ports();
extern char* get_port_name(struct sp_port *port);
extern int read_bytes(struct sp_port *port, char* buf, Message message);
extern int read_ack(struct sp_port *port);
extern int reply_ack(struct sp_port *port);
extern int write_message(struct sp_port *port, Message message);

#endif
