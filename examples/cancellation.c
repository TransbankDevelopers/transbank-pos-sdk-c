#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Cancellation request...\n");
    printf("Please insert Operation Number:");
    int op;
    scanf("%i", &op);

    CancellationResponse response = cancellation(op); // cancellation(transactionID)
    if (response.initilized == TBK_OK)
    {
      printf("CANCELLATION RESPONSE\n=============\n");

      printf("Function : %i \n", response.function);
      printf("Response Code : %i \n", response.responseCode);
      printf("Commerce Code : %llu \n", response.commerceCode);
      printf("Terminal ID : %i \n", response.terminalId);
      printf("Authorization Code : %i \n", response.authorizationCode);
      printf("Operation ID : %i \n\n", response.operationID);

      switch (response.responseCode)
      {
      case -1:
        printf("Response Message: \"Operación Cancelada\" \n");
        break;

      case 0:
        printf("Response Message: \"Operación Existosa\" \n");
        break;

      case 5:
        printf("Response Message: \"Venta no existe\" \n");
        break;

      case 6:
        printf("Response Message: \"Venta ya anulada\" \n");
        break;

      case 21:
        printf("Response Message: \"Anulación no permitida\" \n");
        break;

      default:
        printf("Response Message: \"Error desconocido\" \n");
        break;
      }
    }
    else
    {
      printf("Could not perform cancellation process.\n");
    }
  }
  else
  {
    printf("Unable to open selected port\n");
  }

  //Close Port
  retval = close_port();
  if (retval == SP_OK)
  {
    puts("Serial port closed.\n");
  }
  else
  {
    puts("Unable to close serial port.\n");
  }

  return 0;
}
