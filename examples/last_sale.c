#include "transbank.h"
#include "common.c"

int main()
{
    char *portName = select_port();

    int retval = open_port(portName, bRate);
    if (retval == TBK_OK)
    {
        printf("Serial port successfully opened.\n");

        printf("Getting Last Sale from POS...\n");

        char *lastSaleResponse = last_sale();

        char command[5];
        char responseCode[3];
        strncpy(command, lastSaleResponse + 1, 4);
        strncpy(responseCode, lastSaleResponse + 6, 2);

        printf("COMMAND : %s\n", command);
        printf("RESPONSE CODE : %s\n", responseCode);

        if (strcmp(command, "0260") == 0)
        {
            // No existe venta
            if (strcmp(responseCode, "11") == 0)
            {
                printf("No hay registros de venta\n");
            }

            printf("Response success: %s\n", lastSaleResponse);
        }
        else
        {
            printf("An error has occurred: %s\n", lastSaleResponse);
        }

        //Close Port
        retval = close_port();
        if (retval == SP_OK)
        {
            printf("Serial port closed.\n");
        }
        else
        {
            printf("Unable to close serial port.\n");
        }
    }
    else
    {
        printf("Unable to open selected port\n");
    }

    return 0;
}
