/**
 * @brief Check unit tests for rational numbers.
 *
 * This file contains the unit tests for the rational numbers.
 *
 * @file check_rational.c
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#include <stdlib.h>
#include <check.h>

#include "rational.h"

START_TEST(test_rational_get)
{
    struct Rational *r;

    r = rational_get(2,3);

    ck_assert_int_eq(r->n, 2);
    ck_assert_int_eq(r->d, 3);

    free(r);
}
END_TEST

Suite *rational_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Rational");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_rational_get);
    suite_add_tcase(s, tc_core);

    return s;
}

