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

    // Get totals
    if (strcmp(command, "0700") == 0)
    {
        char get_total_response[] = {0x02,
                                     0x30, 0x37, 0x31, 0x30, 0x7C,
                                     0x30, 0x30, 0x7C,
                                     0x32, 0x7C,
                                     0x31, 0x33, 0x36, 0x35, 0x30, 0x7C,
                                     0x03, 0x06, '\0'};
        strcpy(buf, get_total_response);
    }
    else
    {
        char response[] = {0x02,
                           0x30, 0x38, 0x31, 0x30, 0x07,
                           0x30, 0x30, 0x07,
                           0x35, 0x39, 0x37, 0x30, 0x32, 0x39, 0x34, 0x31, 0x34, 0x33, 0x30, 0x30, 0x07,
                           0x37, 0x35, 0x30, 0x30, 0x31, 0x30, 0x38, 0x37, 0x03, 0x30, '\0'};
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

void test_pooling_ok(void **state)
{
    (void)state; /* unused */
    will_return(__wrap_write_message, TBK_OK);
    will_return(__wrap_read_ack, TBK_OK);
    assert_int_equal((int)TBK_OK, polling());
}

void test_pooling_write_nok(void **state)
{
    (void)state; /* unused */
    will_return_count(__wrap_write_message, TBK_NOK, 3);
    assert_int_equal((int)TBK_NOK, polling());
}

void test_pooling_ack_nok(void **state)
{
    (void)state; /* unused */
    will_return_count(__wrap_write_message, TBK_OK, 3);
    will_return_count(__wrap_read_ack, TBK_NOK, 3);
    assert_int_equal((int)TBK_NOK, polling());
}

void test_pooling_ok_on_second_try(void **state)
{
    (void)state; /* unused */
    will_return(__wrap_write_message, TBK_NOK);
    will_return(__wrap_write_message, TBK_OK);
    will_return_count(__wrap_read_ack, TBK_OK, 1);

    assert_int_equal((int)TBK_OK, polling());
}

void test_pooling_ok_on_third_try(void **state)
{
    (void)state; /* unused */
    will_return_count(__wrap_write_message, TBK_NOK, 2);
    will_return(__wrap_write_message, TBK_OK);
    will_return_count(__wrap_read_ack, TBK_OK, 1);

    assert_int_equal((int)TBK_OK, polling());
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
    assert_int_equal(2, response.txCount);
    assert_int_equal(13650, response.txTotal);
}

const struct CMUnitTest transbank_tests[] = {
    cmocka_unit_test(test_pooling_ok),
    cmocka_unit_test(test_pooling_write_nok),
    cmocka_unit_test(test_pooling_ack_nok),
    cmocka_unit_test(test_pooling_ok_on_second_try),
    cmocka_unit_test(test_pooling_ok_on_third_try),
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
    cmocka_unit_test(test_get_totals_ok),
};

int main(void)
{
    return cmocka_run_group_tests(transbank_tests, NULL, NULL);
}
