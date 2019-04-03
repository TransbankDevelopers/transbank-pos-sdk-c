#ifndef TBKSERIALHEADER_FILE
#define TBKSERIALHEADER_FILE

#include "transbank.h"

extern char* list_ports();
extern char* get_port_name(struct sp_port *port);
extern int read_bytes(struct sp_port *port, char* buf, Message message);
extern int read_ack(struct sp_port *port);
extern unsigned char calculate_lrc(char* message, int length);
extern int reply_ack(struct sp_port *port, char* message, int length);
extern int write_message(struct sp_port *port, Message message);

#endif
