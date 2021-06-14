#include "hex_codes.h"
#include "transbank.h"
#include "transbank_serial_utils.h"
#include "stdio.h"
#include "libserialport.h"

struct sp_port *port;

static int BITS = 8;
static int PARITY = SP_PARITY_NONE;
static int STOP_BITS = 1;
static int FLOW_CONTROL = SP_FLOWCONTROL_NONE;

static char CLOSE_MESSAGE[] = {STX, 0x30, 0x35, 0x30, 0x30, PIPE, PIPE, ETX, 0x06};
static char GET_TOTALS_MESSAGE[] = {STX, 0x30, 0x37, 0x30, 0x30, PIPE, PIPE, ETX, 0x04};
static char LOAD_KEYS_MESSAGE[] = {STX, 0x30, 0x38, 0x30, 0x30, ETX, 0x0B};
static char POLL_MESSAGE[] = {STX, 0x30, 0x31, 0x30, 0x30, ETX, 0x02};
static char CHANGE_TO_NORMAL_MESSAGE[] = {STX, 0x30, 0x33, 0x30, 0x30, ETX, 0x00};
static char LAST_SALE_MESSAGE[] = {STX, 0x30, 0x32, 0x35, 0x30, PIPE, ETX, 0x78};
static char SALES_DETAIL_MESSAGE[] = {STX, 0x30, 0x32, 0x36, 0x30, PIPE, 0x31, PIPE, ETX, 0x36};
static char SALES_DETAIL_MESSAGE_PRINT[] = {STX, 0x30, 0x32, 0x36, 0x30, PIPE, 0x30, PIPE, ETX, 0x37};

static Message CLOSE = {
    .payload = CLOSE_MESSAGE,
    .payloadSize = 9,
    .responseSize = 34,
    .retries = 3,
};

static Message GET_TOTALS = {
    .payload = GET_TOTALS_MESSAGE,
    .payloadSize = 9,
    .responseSize = 24,
    .retries = 3
};

static Message LOAD_KEYS = {
    .payload = LOAD_KEYS_MESSAGE,
    .payloadSize = 7,
    /*Independientemente del equipo (Verifone o Ingenico), debe ser 33*/
    .responseSize = 33,
    .retries = 3
};

static Message POLL = {
    .payload = POLL_MESSAGE,
    .payloadSize = 7,
    .responseSize = 1,
    .retries = 3
};

static Message CHANGE_TO_NORMAL = {
    .payload = CHANGE_TO_NORMAL_MESSAGE,
    .payloadSize = 7,
    .responseSize = 1,
    .retries = 3
};

static Message SALES_DETAIL = {
    .payload = SALES_DETAIL_MESSAGE,
    .payloadSize = 10,
    .responseSize = 197,
    .retries = 3
};

static Message SALES_DETAIL_PRINT = {
    .payload = SALES_DETAIL_MESSAGE_PRINT,
    .payloadSize = 10,
    .responseSize = 1,
    .retries = 3
};

int configure_port(int baud_rate) {
    int retval = 0;
    struct sp_port_config *config;

    retval += sp_new_config(&config);
    retval += sp_set_config_baudrate(config, baud_rate);
    retval += sp_set_config_bits(config, BITS);
    retval += sp_set_config_parity(config, PARITY);
    retval += sp_set_config_stopbits(config, STOP_BITS);
    retval += sp_set_config_flowcontrol(config, FLOW_CONTROL);
    retval += sp_set_config(port, config);
    retval += sp_flush(port, SP_BUF_BOTH);
    sp_free_config(config);

    if (retval< -1) {
        return -1;
    }
    return retval;
}

int open_port(char *portName, int baudrate) {
    int retval = sp_get_port_by_name(portName, &port);
    retval += sp_open(port, SP_MODE_READ_WRITE);
    retval += configure_port(baudrate);
    return retval;
}

char *substring(char *string, ParamInfo info) {
    char *ret = malloc((sizeof (char) * info.length) + 1);
    memset(ret, '\0', sizeof (&ret));
    char cToStr[2];
    cToStr[1] = '\0';

    int limit = info.index + info.length;

    for (int i = info.index; i < limit; i++) {
        cToStr[0] = string[i];
        strcat(ret, cToStr);
    }
    return ret;
}

BaseResponse *parse_load_keys_close_response(char *buf) {
    BaseResponse *response = malloc(sizeof (BaseResponse));

    char *word;
    int init_pos = 1, length = 0, found = 0;
    for (int x = init_pos; x < strlen(buf); x++) {
        if (buf[x] == '|' || (unsigned char) buf[x] == ETX) {
            word = malloc(length * sizeof (char *));
            strncpy(word, buf + init_pos, length);
            word[length] = 0;

            found++;
            init_pos = x + 1;
            length = 0;

            // Found words
            switch (found) {
                case 1:
                    response->function = strtol(word, NULL, 10);
                    break;

                case 2:
                    response->responseCode = strtol(word, NULL, 10);
                    break;

                case 3:
                    response->commerceCode = strtoll(word, NULL, 10);
                    break;

                case 4:
                    strcpy(response->terminalId, word);
                    break;

                default:
                    break;
            }

            continue;
        }

        length++;
    }

    response->initilized = TBK_OK;
    return response;
}

TotalsCResponse *parse_get_totals_response(char *buf) {
    TotalsCResponse *response = malloc(sizeof (TotalsCResponse));

    char *word;
    int init_pos = 1, length = 0, found = 0;
    for (int x = init_pos; x < strlen(buf); x++) {
        if (buf[x] == '|' || (unsigned char) buf[x] == ETX) {
            word = malloc(length * sizeof (char *));
            strncpy(word, buf + init_pos, length);
            word[length] = 0;

            found++;
            init_pos = x + 1;
            length = 0;

            // Found words
            switch (found) {
                case 1:
                    response->function = strtol(word, NULL, 10);
                    break;

                case 2:
                    response->responseCode = strtol(word, NULL, 10);
                    break;

                case 3:
                    response->txCount = strtol(word, NULL, 10);
                    break;

                case 4:
                    response->txTotal = strtol(word, NULL, 10);
                    break;

                default:
                    break;
            }

            continue;
        }

        length++;
    }

    response->initilized = TBK_OK;
    return response;
}

Message prepare_sale_message(long amount, char* ticket, bool send_messages) {
    char operation[] = {STX, 0x30, 0x32, 0x30, 0x30, '\0'};
    char pipe[] = {PIPE, '\0'};
    char etx[] = {ETX, '\0'};
    char lrc_string[] = {0x30, '\0'};

    char amount_string[10];
    sprintf(amount_string, "%09ld", amount);

    char send_message_string[2] = {send_messages + '0', '\0'};

    char *msg = malloc(sizeof (char) * 28);
    memset(msg, '\0', sizeof (&msg));

    strcat(msg, operation);
    strcat(msg, pipe);
    strcat(msg, amount_string);
    strcat(msg, pipe);
    strcat(msg, ticket);
    strcat(msg, pipe);
    strcat(msg, pipe);
    strcat(msg, pipe);
    strcat(msg, send_message_string);
    strcat(msg, etx);
    strcat(msg, lrc_string);

    unsigned char lrc = calculate_lrc(msg, 28);
    msg[strlen(msg) - 1] = lrc;

    Message message = {
        .payload = msg,
        .payloadSize = 28,
        .responseSize = 146,
        .retries = 3
    };

    return message;
}

char *sale(int amount, char* ticket, bool send_messages) {
    int tries = 0;
    int retval, write_ok = TBK_NOK;

    if (strlen(ticket) != 6) {
        return "tiket must have 6 characters";
    }

    Message sale_message = prepare_sale_message(amount, ticket, send_messages);

    do {
        retval = write_message(port, sale_message);
        if (retval == TBK_OK) {
            retval = read_ack(port);
            if (retval == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < sale_message.retries);

    if (write_ok == TBK_OK) {
        tries = 0;
        char *buf;
        buf = malloc(sale_message.responseSize * sizeof (char));

        int wait = sp_input_waiting(port);
        do {
            if (wait > 0) { //lo mas seguro es simplemente evaluar que hayan mas de 0 bytes esperando para ser leidos
                int readedbytes = read_bytes(port, buf, sale_message);
                if (readedbytes > 0) {
                    sale_message.responseSize = readedbytes;
                    retval = reply_ack(port, buf, sale_message.responseSize);
                    if (retval == TBK_OK) {
                        return buf;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
            if (wait == SP_ERR_FAIL) {
                return "Unable to request sale. POS connection was lost.\n";
            }
            wait = sp_input_waiting(port);
        } while (tries < sale_message.retries);
    } else {
        return "Unable to request sale. Check POS connection.\n";
    }
    return "Unable to request sale\n";
}

char *last_sale() {
    int tries = 0;
    int retval, write_ok = TBK_NOK;

    Message last_sale_message = {
        .payload = LAST_SALE_MESSAGE,
        .payloadSize = sizeof (LAST_SALE_MESSAGE),
        .responseSize = 146,
        .retries = 3
    };

    do {
        retval = write_message(port, last_sale_message);
        if (retval == TBK_OK) {
            retval = read_ack(port);
            if (retval == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < last_sale_message.retries);

    if (write_ok == TBK_OK) {
        tries = 0;
        char *buf;
        buf = malloc(last_sale_message.responseSize * sizeof (char));

        int wait = sp_input_waiting(port);
        if (wait == SP_ERR_FAIL) {
            return "Unable to request last sale\n";
        }
        do {
            if (wait > 0) {
                int readedbytes = read_bytes(port, buf, last_sale_message);

                if (readedbytes > 0) {
                    last_sale_message.responseSize = readedbytes;
                    retval = reply_ack(port, buf, last_sale_message.responseSize);

                    if (retval == TBK_OK) {
                        return buf;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
            wait = sp_input_waiting(port);
        } while (tries < last_sale_message.retries);
    } else {
        return "Unable to request last sale\n";
    }
    return "Unable to request last sale\n";
}

BaseResponse do_close() {
    int tries = 0;
    int retval, write_ok = TBK_NOK;
    BaseResponse *rsp = malloc(sizeof (BaseResponse));

    do {
        int retval = write_message(port, CLOSE);
        if (retval == TBK_OK) {
            if (read_ack(port) == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < CLOSE.retries);

    if (write_ok == TBK_OK) {
        char *buf;
        tries = 0;
        buf = malloc(CLOSE.responseSize * sizeof (char));

        int wait = sp_input_waiting(port);
        do {
            if (wait > 0) {
                int readedbytes = read_bytes(port, buf, CLOSE);
                if (readedbytes > 0) {
                    retval = reply_ack(port, buf, CLOSE.responseSize);
                    if (retval == TBK_OK) {
                        rsp = parse_load_keys_close_response(buf);
                        return *rsp;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
            wait = sp_input_waiting(port);
        } while (tries < CLOSE.retries);
    }
    return *rsp;
}

BaseResponse load_keys() {
    int tries = 0;
    int retval, write_ok = TBK_NOK;
    BaseResponse *rsp;

    do {
        int retval = write_message(port, LOAD_KEYS);
        if (retval == TBK_OK) {
            if (read_ack(port) == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < LOAD_KEYS.retries);

    if (write_ok == TBK_OK) {
        char *buf;
        tries = 0;
        buf = malloc(LOAD_KEYS.responseSize * sizeof (char));

        int wait = sp_input_waiting(port);
        do {
            /* No se puede usar LOAD_KEYS.responseSize ya que el 
             * tamaño de la respuesta varia segun el equipo en uso:
             * 32 bytes: Equipo Verifone
             * 33 bytes: Equipo Ingenico
             */
            if (wait >= 32) {
                int readedbytes = read_bytes(port, buf, LOAD_KEYS);
                if (readedbytes > 0) {
                    /*
                     * Para enviar correctamente el ACK, debe usarse el tamaño
                     * requerido por cada modelo
                     * 32 bytes: Equipo Verifone
                     * 33 bytes: Equipo Ingenico
                     * La forma mas simple de hacer esto es enviar el tamaño 
                     * de las respuesta que envió el equipo (wait) en lugar
                     * de enviar el tamaño de la respuesta definido en el 
                     * struct LOAD_KEYS
                     */
                    retval = reply_ack(port, buf, wait /* LOAD_KEYS.responseSize*/);
                    if (retval == TBK_OK) {
                        rsp = parse_load_keys_close_response(buf);
                        return *rsp;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
            if (wait == SP_ERR_FAIL) {
                break;
            }
            wait = sp_input_waiting(port);
        } while (tries < LOAD_KEYS.retries);
    }
    return *rsp;
}

enum TbkReturn do_poll() {
    int tries = 0;
    do {
        int retval = write_message(port, POLL);
        if (retval == TBK_OK) {
            if (read_ack(port) == TBK_OK) {
                return TBK_OK;
            }
        }
        tries++;
    } while (tries < POLL.retries);
    return TBK_NOK;
}

enum TbkReturn set_normal_mode() {
    int tries = 0;
    do {
        int retval = write_message(port, CHANGE_TO_NORMAL);
        if (retval == TBK_OK) {
            if (read_ack(port) == TBK_OK) {
                return TBK_OK;
            }
        }
        tries++;
    } while (tries < CHANGE_TO_NORMAL.retries);
    return TBK_NOK;
}

enum TbkReturn close_port() {
    int retval = sp_flush(port, SP_BUF_BOTH);
    retval += sp_close(port);
    sp_free_port(port);
    if (retval == TBK_OK) {
        return TBK_OK;
    }
    return retval;
}

TotalsCResponse get_totals() {
    int tries = 0;
    int retval, write_ok = TBK_NOK;

    TotalsCResponse *rsp = malloc(sizeof (TotalsCResponse));
    rsp->initilized = TBK_NOK;

    do {
        retval = write_message(port, GET_TOTALS);
        if (retval == TBK_OK) {
            retval = read_ack(port);
            if (retval == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < GET_TOTALS.retries);

    if (write_ok == TBK_OK) {
        char *buf;
        tries = 0;
        buf = malloc(GET_TOTALS.responseSize * sizeof (char));

        int wait = sp_input_waiting(port);
        do {
            if (wait > 10) {
                int readedbytes = read_bytes(port, buf, GET_TOTALS);
                if (readedbytes > 0) {
                    retval = reply_ack(port, buf, readedbytes);
                    if (retval == TBK_OK) {
                        rsp = parse_get_totals_response(buf);
                        return *rsp;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
            if (wait == SP_ERR_FAIL) {
                break;
            }
            wait = sp_input_waiting(port);
        } while (tries < GET_TOTALS.retries);
    }

    return *rsp;
}

Message prepare_refund_message(int transactionID) {
    const int payloadSize = 15;
    //const int responseSize = 46; //El tamaño correcto es 48. 46 es cuando la transaccion no pudo ser realizada, 48 cuando si pudo ser realizada
    const int responseSize = 48;

    char command[] = {STX, 0x31, 0x32, 0x30, 0x30, '\0'};
    char pipe[] = {PIPE, '\0'};
    char etx[] = {ETX, '\0'};
    char lrc_string[] = {0x30, '\0'};

    char transaction_string[7];
    sprintf(transaction_string, "%06d", transactionID);

    char *msg = malloc(sizeof (char) * payloadSize);
    memset(msg, '\0', sizeof (&msg));

    strcat(msg, command);
    strcat(msg, pipe);
    strcat(msg, transaction_string);
    strcat(msg, pipe);
    strcat(msg, etx);
    strcat(msg, lrc_string);

    unsigned char lrc = calculate_lrc(msg, payloadSize);
    msg[strlen(msg) - 1] = lrc;

    Message message = {
        .payload = msg,
        .payloadSize = payloadSize,
        .responseSize = responseSize,
        .retries = 3
    };

    return message;
}

RefundCResponse *parse_refund_response(char *buf) {
    RefundCResponse *response = malloc(sizeof (RefundCResponse));
    response->initilized = TBK_NOK;

    char *word;
    int init_pos = 1, length = 0, found = 0;
    for (int x = init_pos; x < strlen(buf); x++) {
        if (buf[x] == '|' || (unsigned char) buf[x] == ETX) {
            word = malloc((length + 1) * sizeof (char *));
            strncpy(word, buf + init_pos, length);
            word[length] = 0;

            found++;
            init_pos = x + 1;
            length = 0;

            // Found words
            switch (found) {
                case 1:
                    response->function = strtol(word, NULL, 10);
                    break;

                case 2:
                    response->responseCode = strtol(word, NULL, 10);
                    break;

                case 3:
                    response->commerceCode = strtol(word, NULL, 10);
                    break;

                case 4:
                    strcpy(response->terminalId, word);
                    break;

                case 5:
                    strcpy(response->authorizationCode, word);
                    break;

                case 6:
                    response->operationID = strtol(word, NULL, 10);
                    break;

                default:
                    break;
            }

            continue;
        }

        length++;
    }

    response->initilized = TBK_OK;
    return response;
}

RefundCResponse refund(int transactionID) {
    int tries = 0;
    int retval, write_ok = TBK_NOK;

    RefundCResponse *rsp = malloc(sizeof (RefundCResponse));
    rsp->initilized = TBK_NOK;

    Message refund_message = prepare_refund_message(transactionID);

    do {
        retval = write_message(port, refund_message);
        if (retval == TBK_OK) {
            retval = read_ack(port);
            if (retval == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < refund_message.retries);

    if (write_ok == TBK_OK) {
        tries = 0;
        char *buf;
        buf = malloc(refund_message.responseSize * sizeof (char));

        int wait = sp_input_waiting(port);
        do {
            if (wait > 0) {
                int readedbytes = read_bytes(port, buf, refund_message);
                if (readedbytes > 0) {
                    refund_message.responseSize = readedbytes;
                    retval = reply_ack(port, buf, refund_message.responseSize);
                    if (retval == TBK_OK) {
                        rsp = parse_refund_response(buf);
                        return *rsp;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
            if (wait == SP_ERR_FAIL) {
                rsp->responseCode = 3;
                break;
            }
            wait = sp_input_waiting(port);
        } while (tries < refund_message.retries);
    }

    return *rsp;
}

char *concatLine(const char *s1, const char *s2) {
    char *res = malloc(strlen(s1) + strlen(s2) + 2);
    if (strlen(s1) > 0) {
        strcpy(res, s1);
        strcat(res, "\n");
    }
    strcat(res, s2);
    return res;
}

char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL; // handle NULL string
    if (!*s)
        return s; // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr)
        ;
    ptr[1] = '\0';
    return s;
}

char *get_authorizationCode(char *buf) {
    char *word;
    int init_pos = 1, length = 0, found = 0;

    for (int x = init_pos; x < strlen(buf); x++) {
        if (buf[x] == '-' && found == 0) {
            break;
        }

        if (buf[x] == '|' || (unsigned char) buf[x] == ETX) {
            word = malloc((length + 1) * sizeof (char *));
            strncpy(word, buf + init_pos, length);
            word[length] = 0;

            found++;
            init_pos = x + 1;
            length = 0;

            // Found words
            if (found == 6) {
                return trim(word);
            }

            continue;
        }

        length++;
    }
    return "-1";
}

// sales_detail works only with POS 19.1 version

char *sales_detail(bool print_on_pos) {
    int tries = 0;
    int retval, write_ok = TBK_NOK;
    char *rsp = "";

    do {
        if (print_on_pos == true) {
            retval = write_message(port, SALES_DETAIL_PRINT);
        } else {
            retval = write_message(port, SALES_DETAIL);
        }

        if (retval == TBK_OK) {
            retval = read_ack(port);
            if (retval == TBK_OK) {
                write_ok = TBK_OK;
                break;
            }
        }
        tries++;
    } while (tries < SALES_DETAIL.retries);


    /*
     * Aca se produce un problema.
     * 
     * - El equipo Ingenico espera respuesta del host cuando no 
     *   existen ventas, mientras que cuando hay ventas no espera 
     *   respuesta del host.
     * 
     * - El equipo Verifone nunca espera respuesta del host, 
     * 
     * Por lo tanto, el comportamiento de esta funcion ira bien cuando:
     * 
     *    -- Se ejecute en un equipo Verifone 
     *    -- Se ejecute en un equipo Ingenico que si tenga transacciones.
     */
    if (write_ok == TBK_OK && print_on_pos == false) {
        char *buf;
        tries = 0;
        buf = malloc(SALES_DETAIL.responseSize * sizeof (char));

        int wait;
        do {
            wait = sp_input_waiting(port);
            if (wait == SP_ERR_FAIL) {
                return "Unable to request sales detail. POS connection was lost.\n";
            }
            if (wait > 1) {
                int readedbytes = read_bytes(port, buf, SALES_DETAIL);
                if (readedbytes > 0) {
                    retval = reply_ack(port, buf, readedbytes);
                    if (retval == TBK_OK) {
                        if (strcmp(get_authorizationCode(buf), "") != 0) {
                            // Append to string list
                            rsp = concatLine(rsp, buf);
                            tries = 0;
                            continue;
                        }
                        return rsp;
                    } else {
                        tries++;
                    }
                } else {
                    tries++;
                }
            }
        } while (tries < SALES_DETAIL.retries);
    }

    return rsp;
}
