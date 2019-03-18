#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "transbank.h"
#include <cmocka.h>

int __wrap_write_message(struct sp_port *port, Message message){
    return mock();
}

int __wrap_read_ack(struct sp_port *port){
    return mock();
}

void test_pooling_ok(void **state){
    (void) state; /* unused */
    will_return(__wrap_write_message, TBK_OK);
    will_return(__wrap_read_ack, TBK_OK);
    assert_int_equal((int)TBK_OK, polling());
}

void test_pooling_write_nok(void **state){
    (void) state; /* unused */
    will_return_count(__wrap_write_message, TBK_NOK, 3);
    assert_int_equal((int)TBK_NOK, polling());
}

void test_pooling_ack_nok(void **state){
    (void) state; /* unused */
    will_return_count(__wrap_write_message, TBK_OK, 3);
    will_return_count(__wrap_read_ack, TBK_NOK, 3);
    assert_int_equal((int)TBK_NOK, polling());
}

void test_ok_on_second_trie(void **state){
    (void) state; /* unused */
    will_return(__wrap_write_message, TBK_NOK);
    will_return(__wrap_write_message, TBK_OK);
    will_return_count(__wrap_read_ack, TBK_OK, 1);

    assert_int_equal((int)TBK_OK, polling());
}

void test_ok_on_tird_trie(void **state){
    (void) state; /* unused */
    will_return_count(__wrap_write_message, TBK_NOK,2);
    will_return(__wrap_write_message, TBK_OK);
    will_return_count(__wrap_read_ack, TBK_OK, 1);

    assert_int_equal((int)TBK_OK, polling());
}

const struct CMUnitTest transbank_tests[] = {
    cmocka_unit_test(test_pooling_ok),
    cmocka_unit_test(test_pooling_write_nok),
    cmocka_unit_test(test_pooling_ack_nok),
    cmocka_unit_test(test_ok_on_second_trie),
    cmocka_unit_test(test_ok_on_tird_trie)
};

int main(void)
{
    return cmocka_run_group_tests(transbank_tests, NULL, NULL);
}
