#include "transbank.h"
#include "common.c"

int main()
{
  char *portName = select_port();

  int retval = open_port(portName, bRate);
  if (retval == TBK_OK)
  {
    printf("Refund request...\n");
    printf("Please insert Operation Number:");
    int op;
    scanf("%i", &op);

    RefundResponse response = refund(op); // refund(transactionID)
    if (response.initilized == TBK_OK)
    {
      printf("REFUND RESPONSE\n=============\n");

      printf("Function : %i \n", response.function);
      printf("Response Code : %i \n", response.responseCode);
      printf("Commerce Code : %llu \n", response.commerceCode);
      printf("Terminal ID : %i \n", response.terminalId);
      printf("Authorization Code : %i \n", response.authorizationCode);
      printf("Operation ID : %i \n\n", response.operationID);

      switch (response.responseCode)
      {
      case -1:
        printf("Response Message: Operacion Cancelada.\n");
        break;

      case 0:
        printf("Response Message: Operacion Exitosa.\n");
        break;

      case 5:
        printf("Response Message: Venta no existe.\n");
        break;

      case 6:
        printf("Response Message: Venta ya anulada.\n");
        break;

      case 9:
        printf("Response Message: Error lectura de tarjeta.\n");
        break;

      case 20:
        printf("Response Message: Tarjeta invalida.\n");
        break;

      case 21:
        printf("Response Message: Anulacion no permitida.\n");
        break;

      default:
        printf("Response Message: Error desconocido.\n");
        break;
      }
    }
    else
    {
      printf("Could not perform refund process.\n");
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
