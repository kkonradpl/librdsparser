#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "librds.h"
#include "parser.h"

typedef struct {
    librds_t rds;
    librds_data_t data;
    librds_error_t error;
    uint16_t pi;
    uint8_t pty;
    bool tp;
    bool ta;
    bool ms;
    uint8_t af;
    uint8_t af2;
    uint8_t ecc;
    char ps[9];
    char rt[2][65];
} test_context_t;

static int
group_setup(void **state)
{
    test_context_t *ctx = calloc(sizeof(test_context_t), 1);
    *state = ctx;
    return 0;
}

static int
group_teardown(void **state)
{
    test_context_t *ctx = *state;
    free(ctx);
    return 0;
}

static int
test_setup(void **state)
{
    test_context_t *ctx = *state;
    librds_init(&ctx->rds);
    librds_set_user_data(&ctx->rds, ctx);
    return 0;
}

static int
test_teardown(void **state)
{
    test_context_t *ctx = *state;
    (void)ctx;
    return 0;
}

static void
callback_pi(uint16_t  pi,
            void     *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(pi, ctx->pi);
    function_called();
}

static void
callback_pty(uint8_t  pty,
             void    *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(pty, ctx->pty);
    function_called();
}

static void
callback_tp(bool  tp,
            void *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(tp, ctx->tp);
    function_called();
}

static void
callback_ta(bool  ta,
            void *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(ta, ctx->ta);
    function_called();
}

static void
callback_ms(bool  ms,
            void *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(ms, ctx->ms);
    function_called();
}

static void
callback_af(uint8_t  af,
            void    *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(af, ctx->af);
    ctx->af = ctx->af2;
    function_called();
}

static void
callback_ecc(uint8_t  ecc,
             void    *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_int_equal(ecc, ctx->ecc);
    function_called();
}

static void
callback_ps(const char                  *ps,
            const librds_string_error_t *errors,
            void                        *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_string_equal(ps, ctx->ps);
    function_called();
}

static void
callback_rt(const char                  *rt,
            const librds_string_error_t *errors,
            librds_rt_flag_t             flag,
            void                        *user_data)
{
    test_context_t *ctx = (test_context_t*)user_data;
    assert_string_equal(rt, ctx->rt[flag]);
    function_called();
}

/* ------------------------------------------------ */

static void
parser_test_pi(void **state)
{
    test_context_t *ctx = *state;
    librds_register_pi(&ctx->rds, callback_pi);

    ctx->data[0] = 0xBEEF;
    ctx->error[0] = 0;
    ctx->pi = ctx->data[0];
    expect_function_call(callback_pi);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[0] = 0x1337;
    ctx->pi = ctx->data[0];
    expect_function_call(callback_pi);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_pi_error(void **state)
{
    test_context_t *ctx = *state;
    librds_register_pi(&ctx->rds, callback_pi);

    ctx->data[0] = 0xBEEF;
    ctx->error[0] = 1;
    ctx->pi = ctx->data[0];
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_pty(void **state)
{
    test_context_t *ctx = *state;
    librds_register_pty(&ctx->rds, callback_pty);

    ctx->data[1] = 0x0000;
    ctx->error[1] = 0;
    ctx->pty = 0;
    expect_function_call(callback_pty);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x0462;
    ctx->pty = 3;
    expect_function_call(callback_pty);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_pty_error(void **state)
{
    test_context_t *ctx = *state;
    librds_register_pty(&ctx->rds, callback_pty);

    ctx->data[1] = 0x0462;
    ctx->error[1] = 1;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_tp(void **state)
{
    test_context_t *ctx = *state;
    librds_register_tp(&ctx->rds, callback_tp);

    ctx->data[1] = 0x0000;
    ctx->error[1] = 0;
    ctx->tp = 0;
    expect_function_call(callback_tp);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x0462;
    ctx->tp = 1;
    expect_function_call(callback_tp);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_tp_error(void **state)
{
    test_context_t *ctx = *state;
    librds_register_tp(&ctx->rds, callback_tp);

    ctx->data[1] = 0x0462;
    ctx->error[1] = 1;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ta(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ta(&ctx->rds, callback_ta);

    ctx->data[1] = 0x0000;
    ctx->error[1] = 0;
    ctx->ta = 0;
    expect_function_call(callback_ta);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x055F;
    ctx->ta = 1;
    expect_function_call(callback_ta);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ta_error(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ta(&ctx->rds, callback_ta);

    ctx->data[1] = 0x055F;
    ctx->error[1] = 1;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ms(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ms(&ctx->rds, callback_ms);

    ctx->data[1] = 0x0000;
    ctx->error[1] = 0;
    ctx->ms = 0;
    expect_function_call(callback_ms);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x055F;
    ctx->ms = 1;
    expect_function_call(callback_ms);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ms_error(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ms(&ctx->rds, callback_ms);

    ctx->data[1] = 0x055F;
    ctx->error[1] = 1;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_af(void **state)
{
    test_context_t *ctx = *state;
    librds_register_af(&ctx->rds, callback_af);

    ctx->data[1] = 0x0547;
    ctx->data[2] = 0x01A4;

    ctx->error[1] = 0;
    ctx->error[2] = 0;

    ctx->af = 0x01;
    ctx->af2 = 0xA4;
    expect_function_calls(callback_af, 2);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_af_error1(void **state)
{
    test_context_t *ctx = *state;
    librds_register_af(&ctx->rds, callback_af);

    ctx->data[1] = 0x0547;
    ctx->data[2] = 0x01A4;

    ctx->error[1] = 1;
    ctx->error[2] = 0;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_af_error2(void **state)
{
    test_context_t *ctx = *state;
    librds_register_af(&ctx->rds, callback_af);

    ctx->data[1] = 0x0547;
    ctx->data[2] = 0x01A4;

    ctx->error[1] = 0;
    ctx->error[2] = 1;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ecc(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ecc(&ctx->rds, callback_ecc);

    ctx->data[1] = 0x1000;
    ctx->data[2] = 0x00E2;
    ctx->error[1] = 0;
    ctx->error[2] = 0;
    ctx->ecc = 0xE2;
    expect_function_call(callback_ecc);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x1000;
    ctx->data[2] = 0x00E3;
    ctx->ecc = 0xE3;
    expect_function_call(callback_ecc);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ecc_error1(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ecc(&ctx->rds, callback_ecc);

    ctx->data[1] = 0x1000;
    ctx->data[2] = 0x00E2;

    ctx->error[1] = 1;
    ctx->error[2] = 0;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ecc_error2(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ecc(&ctx->rds, callback_ecc);

    ctx->data[1] = 0x1000;
    ctx->data[2] = 0x00E2;

    ctx->error[1] = 0;
    ctx->error[2] = 1;

    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_ps(void **state)
{
    test_context_t *ctx = *state;
    librds_register_ps(&ctx->rds, callback_ps);

    ctx->error[1] = 0;
    ctx->error[3] = 0;

    ctx->data[1] = 0x054C;
    ctx->data[3] = 0x202A;
    snprintf(ctx->ps, sizeof(ctx->ps), " *      ");
    expect_function_call(callback_ps);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x0549;
    ctx->data[3] = 0x4B52;
    snprintf(ctx->ps, sizeof(ctx->ps), " *KR    ");
    expect_function_call(callback_ps);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x054A;
    ctx->data[3] = 0x4450;
    snprintf(ctx->ps, sizeof(ctx->ps), " *KRDP  ");
    expect_function_call(callback_ps);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x054F;
    ctx->data[3] = 0x2A20;
    snprintf(ctx->ps, sizeof(ctx->ps), " *KRDP* ");
    expect_function_call(callback_ps);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x054F;
    ctx->data[3] = 0x2A20;
    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x0549;
    ctx->data[3] = 0x2020;
    snprintf(ctx->ps, sizeof(ctx->ps), " *  DP* ");
    expect_function_call(callback_ps);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_rt(void **state)
{
    test_context_t *ctx = *state;
    librds_register_rt(&ctx->rds, callback_rt);

    ctx->error[1] = 0;
    ctx->error[2] = 0;
    ctx->error[3] = 0;

    ctx->data[1] = 0x2010;
    ctx->data[2] = 0x6F29;
    ctx->data[3] = 0x2054;
    snprintf(ctx->rt[1], sizeof(ctx->rt), "o) T                                                            ");
    expect_function_call(callback_rt);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x2559;
    ctx->data[2] = 0x656C;
    ctx->data[3] = 0x2064;
    snprintf(ctx->rt[1], sizeof(ctx->rt), "o) T                                el d                        ");
    expect_function_call(callback_rt);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x27FF;
    ctx->data[2] = 0x6F20;
    ctx->data[3] = 0x7265;
    snprintf(ctx->rt[1], sizeof(ctx->rt), "o) T                                el d                    o re");
    expect_function_call(callback_rt);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);

    ctx->data[1] = 0x27FF;
    ctx->data[2] = 0x6F20;
    ctx->data[3] = 0x7265;
    /* Same value, no callback */
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_rt_with_end(void **state)
{
    test_context_t *ctx = *state;
    librds_register_rt(&ctx->rds, callback_rt);

    ctx->error[1] = 0;
    ctx->error[2] = 0;
    ctx->error[3] = 0;

    ctx->data[1] = 0x2509;
    ctx->data[2] = 0x2020;
    ctx->data[3] = 0x200D;
    snprintf(ctx->rt[0], sizeof(ctx->rt), "                                       ");
    expect_function_call(callback_rt);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

static void
parser_test_rt_with_end2(void **state)
{
    test_context_t *ctx = *state;
    librds_register_rt(&ctx->rds, callback_rt);

    ctx->error[1] = 0;
    ctx->error[2] = 0;
    ctx->error[3] = 0;

    ctx->data[1] = 0x2500;
    ctx->data[2] = 0x0D20;
    ctx->data[3] = 0x2020;
#pragma GCC diagnostic ignored "-Wformat-zero-length"
    snprintf(ctx->rt[0], sizeof(ctx->rt), "");
#pragma GCC diagnostic warning "-Wformat-zero-length"
    expect_function_call(callback_rt);
    librds_parser_process(&ctx->rds, ctx->data, ctx->error);
}

const struct CMUnitTest tests[] =
{
    cmocka_unit_test_setup_teardown(parser_test_pi, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_pi_error, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_pty, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_pty_error, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_tp, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_tp_error, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ta, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ta_error, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ms, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ms_error, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_af, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_af_error1, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_af_error2, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ecc, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ecc_error1, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ecc_error2, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_ps, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_rt, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_rt_with_end, test_setup, test_teardown),
    cmocka_unit_test_setup_teardown(parser_test_rt_with_end2, test_setup, test_teardown)
};

int
main(void)
{
    return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
