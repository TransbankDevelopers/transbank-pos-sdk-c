#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "transbank_serial_utils.h"
#include <cmocka.h>

static char message[] = {0x02, 0x30, 0x38, 0x30, 0x30, 0x03, 0x0B};

int __wrap_sp_blocking_write(struct sp_port *port, const void *buf, size_t count, unsigned int timeout_ms)
{
    return mock();
}

void test_calculate_lrc(void **state)
{
    (void)state;
    int length = strlen(message);
    assert_true(message[length - 1] == calculate_lrc(message, length));
}

void test_reply_ack_ok_lrc_ok(void **state)
{
    (void)state;
    struct sp_port *port;
    will_return(__wrap_sp_blocking_write, 1);
    int ret = reply_ack(port, message, strlen(message));

    assert_int_equal(0, ret);
}

void test_reply_ack_nok_lrc_ok(void **state)
{
    (void)state;
    struct sp_port *port;
    will_return(__wrap_sp_blocking_write, -1);
    int ret = reply_ack(port, message, strlen(message));
    assert_true(ret < 0);
}

void test_reply_ack_nok_lrc_nok(void **state)
{
    (void)state;
    struct sp_port *port;
    will_return(__wrap_sp_blocking_write, -1);
    int ret = reply_ack(port, message, strlen(message) - 1);

    assert_true(ret < 0);
}

const struct CMUnitTest transbank_serial_utils_tests[] = {
    cmocka_unit_test(test_calculate_lrc),
    cmocka_unit_test(test_reply_ack_ok_lrc_ok),
    cmocka_unit_test(test_reply_ack_nok_lrc_ok),
    cmocka_unit_test(test_reply_ack_nok_lrc_nok)};

int main(void)
{
    return cmocka_run_group_tests(transbank_serial_utils_tests, NULL, NULL);
}
