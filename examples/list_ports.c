#include "transbank_serial_utils.h"

int main()
{
  // Example, listing available ports
  char *list = list_ports();

  const char *delim = "|";
  char *ptr = strtok(list, delim);

  printf("Available ports : \n");
  while (ptr != NULL)
  {
    printf("-> %s\n", ptr);
    ptr = strtok(NULL, delim);
  }

  return 0;
}
