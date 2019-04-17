#include "transbank.h"
#include "transbank_serial_utils.h"

int main()
{
  // VELOCIDAD COM
  const int bRate = 115200;

  // Getting available ports
  char *list = list_ports();
  const char *delim = "|";
  char *ptr = strtok(list, delim);

  printf("Available ports : \n=================\n");
  int count = 1;
  while (ptr != NULL)
  {
    printf("%i) %s\n", count, ptr);
    ptr = strtok(NULL, delim);
    count++;
  }

  // Populate array list
  char **ports = malloc(count * sizeof(char *));
  ptr = strtok(list, delim);
  int n = 0;
  while (ptr != NULL)
  {
    strcpy(ports[n], ptr);
    ptr = strtok(NULL, delim);
    n++;
  }

  // Handle option
  printf("Please insert port option:");
  int op;
  scanf("%i", &op);

  char *portName = ports[op - 1];
  printf("Using \"%s\" port ...\n=================\n", portName);
  free(ports);

  int resp = open_port(portName, bRate);
  if (resp != TBK_OK)
  {
    puts("Unable to open serial port.\n");
    return 0;
  }

  printf("Serial port successfully opened...\n");

  LoadKeyCloseResponse lcr = load_keys();

  // TODO(lm): Check if response is correct :/
  printf("Function: %i\n", lcr.function);
  printf("Response Code: %i\n", lcr.responseCode);
  printf("Commerce Code: %llu\n", lcr.commerceCode);
  printf("Terminal ID: %lu\n", lcr.terminalId);
  puts("Keys loaded successfully.\n=================\n");

  //Close Port
  resp = close_port();
  if (resp != SP_OK)
  {
    puts("Unable to close serial port.\n");
    return 0;
  }

  // Success!
  printf("Serial port closed successfully.\n");
  return 0;
}
