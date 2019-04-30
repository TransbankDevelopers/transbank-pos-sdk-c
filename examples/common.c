#include "common.h"
#include "transbank_serial_utils.h"

char *select_port()
{
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
  int i = 0, l = 0, f = 0;

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
      ports[f] = malloc(strlen(w) * sizeof(char *));
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

  return portName;
}
