#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "transbank.h"
#include <cmocka.h>

int __wrap_write_message(struct sp_port *port, Message message)
{
  return mock();
}

int __wrap_read_ack(struct sp_port *port)
{
  return mock();
}

int __wrap_read_bytes(struct sp_port *port, char *buf, Message message)
{
  char command[5];
  strncpy(command, message.payload + 1, 4);

  if (strcmp(command, "0800") == 0) //Load Keys
  {
    char response[] = {0x02,
                       0x30, 0x38, 0x31, 0x30, 0x7C,
                       0x30, 0x30, 0x7C,
                       0x35, 0x39, 0x37, 0x30, 0x32, 0x39, 0x34, 0x31, 0x34, 0x33, 0x30, 0x30, 0x7C,
                       0x37, 0x35, 0x30, 0x30, 0x31, 0x30, 0x38, 0x37, 0x03, 0x30, '\0'};
    strcpy(buf, response);
  }
  else if (strcmp(command, "0500") == 0) //Close
  {
    char response[] = {0x02,
                       0x30, 0x35, 0x31, 0x30, 0x7C,
                       0x30, 0x30, 0x7C,
                       0x35, 0x39, 0x37, 0x30, 0x32, 0x39, 0x34, 0x31, 0x34, 0x33, 0x30, 0x30, 0x7C,
                       0x37, 0x35, 0x30, 0x30, 0x31, 0x30, 0x38, 0x37, 0x03, 0x30, '\0'};
    strcpy(buf, response);
  }
  else if (strcmp(command, "0200") == 0) //Sale
  {
    char response[] = {0x02,
                       0x30, 0x32, 0x31, 0x30, 0x7C,
                       0x30, 0x30, 0x7C,
                       0x35, 0x39, 0x37, 0x30, 0x32, 0x39, 0x34, 0x31, 0x34, 0x33, 0x30, 0x30, 0x7C,
                       0x37, 0x35, 0x30, 0x30, 0x31, 0x30, 0x38, 0x37, 0x7C,
                       0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x30, 0x7C,
                       0x32, 0x35, 0x30, 0x30, 0x7C,
                       0x30, 0x7c, 0x7c,
                       0x03, 0x30, '\0'};
    strcpy(buf, response);
  }
  else if (strcmp(command, "0250") == 0) // Last Sale
  {
    // 0260|00|597029414300|75001089|000002|181278|2500|3|834|6590|000063|CR|||VI|14052019|111555|0|0|x
    char response[] = {0x02,
                       0x30, 0x32, 0x36, 0x30, 0x7C,
                       0x30, 0x30, 0x7C,
                       0x35, 0x39, 0x37, 0x30, 0x32, 0x39, 0x34, 0x31, 0x34, 0x33, 0x30, 0x30, 0x7C,
                       0x37, 0x35, 0x30, 0x30, 0x31, 0x30, 0x38, 0x39, 0x7C,
                       0x30, 0x30, 0x30, 0x30, 0x30, 0x32, 0x7C,
                       0x31, 0x38, 0x31, 0x32, 0x37, 0x38, 0x7C,
                       0x32, 0x35, 0x30, 0x30, 0x7C, 0x33, 0x7C,
                       0x38, 0x33, 0x34, 0x7C,
                       0x36, 0x35, 0x39, 0x30, 0x7C,
                       0x30, 0x30, 0x30, 0x30, 0x36, 0x33, 0x7C,
                       0x43, 0x52, 0x7C, 0x7C, 0x7C,
                       0x56, 0x49, 0x7C,
                       0x31, 0x34, 0x30, 0x35, 0x32, 0x30, 0x31, 0x39, 0x7C,
                       0x31, 0x31, 0x31, 0x35, 0x35, 0x35, 0x7C,
                       0x30, 0x7C,
                       0x30, 0x7C,
                       0x03, 0x78, '\0'};
    strcpy(buf, response);
  }
  else if (strcmp(command, "0700") == 0) // Get Totals
  {
    char response[] = {0x02,
                       0x30, 0x37, 0x31, 0x30, 0x7C,
                       0x30, 0x30, 0x7C, 0x31, 0x7C,
                       0x32, 0x35, 0x30, 0x30, 0x7C,
                       0x03, 0x33, '\0'};
    strcpy(buf, response);
  }
  else if (strcmp(command, "1200") == 0)
  {
    char response[] = {
        0x02,
        0x31, 0x32, 0x31, 0x30, 0x7c,
        0x30, 0x30, 0x7c,
        0x35, 0x39, 0x37, 0x30, 0x32, 0x39, 0x34, 0x31, 0x34, 0x33, 0x30, 0x30, 0x7c,
        0x37, 0x35, 0x30, 0x30, 0x31, 0x30, 0x38, 0x39, 0x7c,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x7c,
        0x31, 0x30,
        0x03, 0x78, '\0'};

    strcpy(buf, response);
  }

  return mock();
}

int __wrap_sp_input_waiting(struct sp_port *port)
{
  return mock();
}

int __wrap_reply_ack(struct sp_port *port, char *message, int length)
{
  return mock();
}

void test_poll_ok(void **state)
{
  (void)state; /* unused */
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  assert_int_equal((int)TBK_OK, poll());
}

void test_poll_write_nok(void **state)
{
  (void)state; /* unused */
  will_return_count(__wrap_write_message, TBK_NOK, 3);
  assert_int_equal((int)TBK_NOK, poll());
}

void test_poll_ack_nok(void **state)
{
  (void)state; /* unused */
  will_return_count(__wrap_write_message, TBK_OK, 3);
  will_return_count(__wrap_read_ack, TBK_NOK, 3);
  assert_int_equal((int)TBK_NOK, poll());
}

void test_poll_ok_on_second_try(void **state)
{
  (void)state; /* unused */
  will_return(__wrap_write_message, TBK_NOK);
  will_return(__wrap_write_message, TBK_OK);
  will_return_count(__wrap_read_ack, TBK_OK, 1);

  assert_int_equal((int)TBK_OK, poll());
}

void test_poll_ok_on_third_try(void **state)
{
  (void)state; /* unused */
  will_return_count(__wrap_write_message, TBK_NOK, 2);
  will_return(__wrap_write_message, TBK_OK);
  will_return_count(__wrap_read_ack, TBK_OK, 1);

  assert_int_equal((int)TBK_OK, poll());
}

void test_set_normal_mode_ok(void **state)
{
  (void)state; /* unused */
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  assert_int_equal((int)TBK_OK, set_normal_mode());
}

void test_set_normal_mode_write_nok(void **state)
{
  (void)state; /* unused */
  will_return_count(__wrap_write_message, TBK_NOK, 3);
  assert_int_equal((int)TBK_NOK, set_normal_mode());
}

void test_set_normal_mode_ack_nok(void **state)
{
  (void)state; /* unused */
  will_return_count(__wrap_write_message, TBK_OK, 3);
  will_return_count(__wrap_read_ack, TBK_NOK, 3);
  assert_int_equal((int)TBK_NOK, set_normal_mode());
}

void test_set_normal_mode_ok_on_second_try(void **state)
{
  (void)state; /* unused */
  will_return(__wrap_write_message, TBK_NOK);
  will_return(__wrap_write_message, TBK_OK);
  will_return_count(__wrap_read_ack, TBK_OK, 1);

  assert_int_equal((int)TBK_OK, set_normal_mode());
}

void test_set_normal_mode_ok_on_third_try(void **state)
{
  (void)state; /* unused */
  will_return_count(__wrap_write_message, TBK_NOK, 2);
  will_return(__wrap_write_message, TBK_OK);
  will_return_count(__wrap_read_ack, TBK_OK, 1);

  assert_int_equal((int)TBK_OK, set_normal_mode());
}

void test_load_keys_ok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return(__wrap_read_bytes, 33);
  will_return(__wrap_reply_ack, 0);
  will_return(__wrap_sp_input_waiting, 32);

  BaseResponse response = load_keys();

  assert_int_equal(810, response.function);
  assert_int_equal(0, response.responseCode);
  assert_int_equal(597029414300, response.commerceCode);
  assert_int_equal(75001087, response.terminalId);
  assert_int_equal(TBK_OK, response.initilized);
}

void test_load_keys_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_NOK, 3);

  BaseResponse response = load_keys();
  assert_int_equal(NULL, response.initilized);
}

void test_load_keys_ack_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_OK, 3);
  will_return_count(__wrap_read_ack, TBK_NOK, 3);

  BaseResponse response = load_keys();
  assert_int_equal(NULL, response.initilized);
}

void test_load_keys_read_bytes_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, -1, 3);
  will_return_count(__wrap_sp_input_waiting, 32, 4);

  BaseResponse response = load_keys();
  assert_int_equal(NULL, response.initilized);
}

void test_load_keys_reply_ack_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, 33, 3);
  will_return_count(__wrap_sp_input_waiting, 32, 4);
  will_return_count(__wrap_reply_ack, -1, 3);

  BaseResponse response = load_keys();
  assert_int_equal(NULL, response.initilized);
}

void test_close_ok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return(__wrap_read_bytes, 33);
  will_return(__wrap_reply_ack, 0);
  will_return(__wrap_sp_input_waiting, 32);

  BaseResponse response = close();

  assert_int_equal(510, response.function);
  assert_int_equal(0, response.responseCode);
  assert_int_equal(597029414300, response.commerceCode);
  assert_int_equal(75001087, response.terminalId);
  assert_int_equal(TBK_OK, response.initilized);
}

void test_close_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_NOK, 3);

  BaseResponse response = close();
  assert_int_equal(NULL, response.initilized);
}

void test_close_ack_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_OK, 3);
  will_return_count(__wrap_read_ack, TBK_NOK, 3);

  BaseResponse response = close();
  assert_int_equal(NULL, response.initilized);
}

void test_close_read_bytes_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, -1, 3);
  will_return_count(__wrap_sp_input_waiting, 32, 4);

  BaseResponse response = close();
  assert_int_equal(NULL, response.initilized);
}

void test_close_reply_ack_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, 33, 3);
  will_return_count(__wrap_sp_input_waiting, 32, 4);
  will_return_count(__wrap_reply_ack, -1, 3);

  BaseResponse response = close();
  assert_int_equal(NULL, response.initilized);
}

void test_sale_ok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return(__wrap_sp_input_waiting, 70);
  will_return(__wrap_read_bytes, 70);
  will_return(__wrap_reply_ack, 0);

  char *response = sale(2500, 1, false);

  char command[5];
  memset(command, '\0', sizeof(command));
  strncpy(command, response + 1, 4);
  assert_int_equal(0, strcmp(command, "0210"));

  char code[3];
  memset(code, '\0', sizeof(code));
  strncpy(code, response + 6, 2);
  assert_int_equal(0, strcmp(code, "00"));

  char ammount[5];
  memset(ammount, '\0', sizeof(ammount));
  strncpy(ammount, response + 41, 4);
  assert_int_equal(0, strcmp(ammount, "2500"));
}

void test_sale_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_NOK, 3);

  char *response = sale(2500, 1, false);
  assert_int_equal(0, strcmp("Unable to request sale\n", response));
}

void test_sale_ack_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_OK, 3);
  will_return_count(__wrap_read_ack, TBK_NOK, 3);

  char *response = sale(2500, 1, false);
  assert_int_equal(0, strcmp("Unable to request sale\n", response));
}

void test_sale_read_bytes_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, -1, 3);
  will_return_count(__wrap_sp_input_waiting, 70, 4);

  char *response = sale(2500, 1, false);
  assert_int_equal(0, strcmp("Unable to request sale\n", response));
}

void test_sale_reply_ack_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, 70, 3);
  will_return_count(__wrap_sp_input_waiting, 70, 4);
  will_return_count(__wrap_reply_ack, -1, 3);

  char *response = sale(2500, 1, false);
  assert_int_equal(0, strcmp("Unable to request sale\n", response));
}

// Last sale
void test_last_sale_ok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return(__wrap_sp_input_waiting, 98);
  will_return(__wrap_read_bytes, 98);
  will_return(__wrap_reply_ack, 0);

  char *response = last_sale();

  char command[5];
  memset(command, '\0', sizeof(command));
  strncpy(command, response + 1, 4);
  assert_int_equal(0, strcmp(command, "0260"));

  char code[3];
  memset(code, '\0', sizeof(code));
  strncpy(code, response + 6, 2);
  assert_int_equal(0, strcmp(code, "00"));
}

void test_last_sale_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_NOK, 3);

  char *response = last_sale();
  assert_int_equal(0, strcmp("Unable to request last sale\n", response));
}

void test_last_sale_ack_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_OK, 3);
  will_return_count(__wrap_read_ack, TBK_NOK, 3);

  char *response = last_sale();
  assert_int_equal(0, strcmp("Unable to request last sale\n", response));
}

void test_last_sale_read_bytes_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, -1, 3);
  will_return_count(__wrap_sp_input_waiting, 98, 4);

  char *response = last_sale();
  assert_int_equal(0, strcmp("Unable to request last sale\n", response));
}

void test_last_sale_reply_ack_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, 98, 3);
  will_return_count(__wrap_sp_input_waiting, 98, 4);
  will_return_count(__wrap_reply_ack, -1, 3);

  char *response = last_sale();
  assert_int_equal(0, strcmp("Unable to request last sale\n", response));
}

// Get Totals
void test_get_totals_ok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return(__wrap_sp_input_waiting, 32);
  will_return(__wrap_read_bytes, 19);
  will_return(__wrap_reply_ack, 0);

  TotalsResponse response = get_totals();

  assert_int_equal(710, response.function);
  assert_int_equal(0, response.responseCode);
  assert_int_equal(1, response.txCount);
  assert_int_equal(2500, response.txTotal);
}

void test_get_totals_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_NOK, 3);

  TotalsResponse response = get_totals();
  assert_int_equal(TBK_NOK, response.initilized);
}

void test_get_totals_read_bytes_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, -1, 3);
  will_return_count(__wrap_sp_input_waiting, 32, 4);

  TotalsResponse response = get_totals();
  assert_int_equal(TBK_NOK, response.initilized);
}

void test_get_totals_reply_ack_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, 19, 3);
  will_return_count(__wrap_sp_input_waiting, 32, 4);
  will_return_count(__wrap_reply_ack, -1, 3);

  TotalsResponse response = get_totals();
  assert_int_equal(TBK_NOK, response.initilized);
}

// Cancellation
void test_cancellation_ok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return(__wrap_sp_input_waiting, 32);
  will_return(__wrap_read_bytes, 34);
  will_return(__wrap_reply_ack, 0);

  CancellationResponse response = cancellation(10);

  assert_int_equal(1210, response.function);
  assert_int_equal(0, response.responseCode);
  assert_int_equal(10, response.operationID);
}

void test_cancellation_nok(void **state)
{
  (void)state;
  will_return_count(__wrap_write_message, TBK_NOK, 3);

  CancellationResponse response = cancellation(9);
  assert_int_equal(TBK_NOK, response.initilized);
}

void test_cancellation_read_bytes_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, -1, 3);
  will_return_count(__wrap_sp_input_waiting, 34, 4);

  CancellationResponse response = cancellation(9);
  assert_int_equal(TBK_NOK, response.initilized);
}

void test_cancellation_reply_ack_nok(void **state)
{
  (void)state;
  will_return(__wrap_write_message, TBK_OK);
  will_return(__wrap_read_ack, TBK_OK);
  will_return_count(__wrap_read_bytes, 15, 3);
  will_return_count(__wrap_sp_input_waiting, 34, 4);
  will_return_count(__wrap_reply_ack, -1, 3);

  CancellationResponse response = cancellation(9);
  assert_int_equal(TBK_NOK, response.initilized);
}

const struct CMUnitTest transbank_tests[] = {
    cmocka_unit_test(test_poll_ok),
    cmocka_unit_test(test_poll_write_nok),
    cmocka_unit_test(test_poll_ack_nok),
    cmocka_unit_test(test_poll_ok_on_second_try),
    cmocka_unit_test(test_poll_ok_on_third_try),
    cmocka_unit_test(test_set_normal_mode_ok),
    cmocka_unit_test(test_set_normal_mode_write_nok),
    cmocka_unit_test(test_set_normal_mode_ack_nok),
    cmocka_unit_test(test_set_normal_mode_ok_on_second_try),
    cmocka_unit_test(test_set_normal_mode_ok_on_third_try),
    cmocka_unit_test(test_load_keys_ok),
    cmocka_unit_test(test_load_keys_nok),
    cmocka_unit_test(test_load_keys_ack_nok),
    cmocka_unit_test(test_load_keys_read_bytes_nok),
    cmocka_unit_test(test_load_keys_reply_ack_nok),
    cmocka_unit_test(test_close_ok),
    cmocka_unit_test(test_close_nok),
    cmocka_unit_test(test_close_ack_nok),
    cmocka_unit_test(test_close_read_bytes_nok),
    cmocka_unit_test(test_close_reply_ack_nok),
    cmocka_unit_test(test_sale_ok),
    cmocka_unit_test(test_sale_nok),
    cmocka_unit_test(test_sale_ack_nok),
    cmocka_unit_test(test_sale_read_bytes_nok),
    cmocka_unit_test(test_sale_reply_ack_nok),
    cmocka_unit_test(test_last_sale_ok),
    cmocka_unit_test(test_last_sale_nok),
    cmocka_unit_test(test_last_sale_ack_nok),
    cmocka_unit_test(test_last_sale_read_bytes_nok),
    cmocka_unit_test(test_last_sale_reply_ack_nok),
    cmocka_unit_test(test_get_totals_ok),
    cmocka_unit_test(test_get_totals_nok),
    cmocka_unit_test(test_get_totals_read_bytes_nok),
    cmocka_unit_test(test_get_totals_reply_ack_nok),
    cmocka_unit_test(test_cancellation_ok),
    cmocka_unit_test(test_cancellation_nok),
    cmocka_unit_test(test_cancellation_read_bytes_nok)};

int main(void)
{
  return cmocka_run_group_tests(transbank_tests, NULL, NULL);
}
