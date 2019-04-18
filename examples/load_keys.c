#include "transbank.h"
#include "transbank_serial_utils.h"

int main()
{
  // VELOCIDAD COM
  const int bRate = 115200;
  const char sep = '|';

  // Getting available ports
  char *list = strcat(list_ports(), "|"); // Add pipe for examples

  int x, count = 0;
  for (x = 0; x <= strlen(list); x++)
  {
    if (list[x] == sep)
    {
      count++;
    }
  }

  char **ports = malloc(count * sizeof(char *));
  char *w;
  int i, l, f = 0;

  printf("Available ports : \n=================\n");
  for (x = 0; x <= strlen(list); x++)
  {
    if (list[x] == sep)
    {
      w = malloc(l * sizeof(char *));
      strncpy(w, list + i, l);
      w[l] = 0;

      printf("%i) %s\n", (f + 1), w); // Display option

      // Store port (w) in array list
      ports[f] = malloc(sizeof(w));
      strcpy(ports[f], w);

      f++;
      i = x + 1; // Set new position
      l = 0;
      continue;
    }

    l++;
  }

  // Handle input option
  printf("Please insert port option:");
  int op;
  scanf("%i", &op);

  char *portName = ports[op - 1];
  printf("Using option: %i -> \"%s\" port ...\n=================\n", op, portName);

  int resp = open_port(portName, bRate);
  if (resp == TBK_OK)
  {
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
    if (resp == SP_OK)
    {
      printf("Serial port closed successfully.\n"); // Success!
    }
    else
    {
      puts("Unable to close serial port.\n");
    }
  }
  else
  {
    puts("Unable to open serial port.\n");
  }

  return 0;
}
