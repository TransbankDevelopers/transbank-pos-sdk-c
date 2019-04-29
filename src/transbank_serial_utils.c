#include "hex_codes.h"
#include "transbank_serial_utils.h"

static const int DEFAULT_TIMEOUT = 1500;

char *list_ports()
{
  struct sp_port **ports;
  char *portList;

  int retval = sp_list_ports(&ports);

  if (retval == SP_OK)
  {
    int separators = 0;
    int chars = 0;
    for (int i = 0; ports[i] != NULL; i++)
    {
      separators++;
      chars += strlen(sp_get_port_name(ports[i]));
    }

    portList = malloc((separators) + chars * sizeof(char *));

    if (portList != NULL)
    {
      strcpy(portList, sp_get_port_name(ports[0]));
      for (int i = 1; ports[i] != NULL; i++)
      {
        strcat(portList, "|");
        strcat(portList, sp_get_port_name(ports[i]));
      }
    }
  }
  else
  {
    char *error = "No serial devices detected\n";
    portList = malloc(strlen(error) * sizeof(char *));
    strcpy(portList, error);
  }
  sp_free_port_list(ports);
  return portList;
}

char *get_port_name(struct sp_port *port)
{
  if (port != NULL)
  {
    return sp_get_port_name(port);
  }
  else
  {
    return "No port configured";
  }
}

int read_bytes(struct sp_port *port, char *buf, Message message)
{
  int retval = TBK_NOK;
  if (buf != NULL && sp_input_waiting(port) > 0)
  {
    int retval = sp_blocking_read(port, buf, message.responseSize, DEFAULT_TIMEOUT);
    sp_flush(port, SP_BUF_INPUT);
    return retval;
  }

  buf = NULL;
  sp_flush(port, SP_BUF_INPUT);
  return retval;
}

int read_ack(struct sp_port *port)
{
  char buf[1];
  int retval = sp_blocking_read_next(port, buf, 1, DEFAULT_TIMEOUT);
  if (retval == 1 && buf[0] == ACK)
  {
    return TBK_OK;
  }
  else
  {
    return TBK_NOK;
  }
}

unsigned char calculate_lrc(char *message, int length)
{
  unsigned char result = message[1];
  for (int n = 2; n < length - 1; n++)
  {
    result ^= (unsigned char)message[n];
  }
  return result;
}

int reply_ack(struct sp_port *port, char *message, int length)
{
  char buf[] = {NAK};
  int retval = TBK_NOK;

  sp_flush(port, SP_BUF_BOTH);
  unsigned char lrc = calculate_lrc(message, length);
  if (lrc == (unsigned char)message[length - 1])
  {
    buf[0] = ACK;
    retval = TBK_OK;
  }

  int a = sp_blocking_write(port, buf, 1, DEFAULT_TIMEOUT);
  retval += a - 1;
  return retval;
}

int write_message(struct sp_port *port, Message message)
{
  sp_flush(port, SP_BUF_BOTH);
  int retval = sp_blocking_write(port, message.payload, message.payloadSize, DEFAULT_TIMEOUT);
  if (retval == message.payloadSize && sp_drain(port))
  {
    retval = TBK_OK;
  }
  else
  {
    retval -= message.payloadSize;
  }
  sp_flush(port, SP_BUF_OUTPUT);
  return retval;
}
