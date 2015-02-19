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

START_TEST(test_rational_create)
{
    struct Rational *r;

    r = rational_create();

    ck_assert_int_eq(r->n, 0);
    ck_assert_int_eq(r->d, 1);

    free(r);
}
END_TEST

START_TEST(test_rational_get)
{
    struct Rational *r;

    r = rational_get(2,3);

    ck_assert_int_eq(r->n, 2);
    ck_assert_int_eq(r->d, 3);

    free(r);
}
END_TEST

START_TEST(test_rational_clone)
{
    struct Rational *r, *s;

    r = rational_get(2,3);
    s = rational_clone(r);

    ck_assert_int_eq(s->n, 2);
    ck_assert_int_eq(s->d, 3);

    free(r);
    free(s);
}
END_TEST

START_TEST(test_rational_normalize)
{
    struct Rational *r;

    r = rational_create();
    r->n = 4;
    r->d = 6;

    rational_normalize(r);

    ck_assert_int_eq(r->n, 2);
    ck_assert_int_eq(r->d, 3);

    free(r);
}
END_TEST

START_TEST(test_rational_multiply)
{
    struct Rational *r, *s, *t;

    r = rational_get(2,3);
    s = rational_get(6,1);

    t = rational_multiply(r, s);

    ck_assert_int_eq(t->n, 4);
    ck_assert_int_eq(t->d, 1);

    free(r);
    free(s);
    free(t);
}
END_TEST

START_TEST(test_rational_divide)
{
    struct Rational *r, *s, *t;

    r = rational_get(2,3);
    s = rational_get(2,3);

    t = rational_divide(r, s);

    ck_assert_int_eq(t->n, 1);
    ck_assert_int_eq(t->d, 1);

    free(r);
    free(s);
    free(t);
}
END_TEST

START_TEST(test_rational_add)
{
    struct Rational *r, *s, *t;

    r = rational_get(2,3);
    s = rational_get(3,4);

    t = rational_add(r, s);

    ck_assert_int_eq(t->n, 17);
    ck_assert_int_eq(t->d, 12);

    free(r);
    free(s);
    free(t);
}
END_TEST

START_TEST(test_rational_subtract)
{
    struct Rational *r, *s, *t;

    r = rational_get(2,3);
    s = rational_get(3,4);

    t = rational_subtract(r, s);

    ck_assert_int_eq(t->n, -1);
    ck_assert_int_eq(t->d, 12);

    free(r);
    free(s);
    free(t);
}
END_TEST


START_TEST(test_rational_invert_sign)
{
    struct Rational *r, *s;

    r = rational_get(2,3);

    s = rational_invert_sign(r);

    ck_assert_int_eq(s->n, -2);
    ck_assert_int_eq(s->d, 3);

    free(r);
    free(s);
}
END_TEST

START_TEST(test_rational_is_a_smaller_than_b)
{
    struct Rational *r, *s;

    r = rational_get(2,3);
    s = rational_get(3,4);

    ck_assert_int_eq(rational_is_a_smaller_than_b(r, s), 1);
    ck_assert_int_eq(rational_is_a_smaller_than_b(s, r), 0);

    free(r);
    free(s);
}
END_TEST

Suite *rational_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Rational");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_rational_create);
    tcase_add_test(tc_core, test_rational_get);
    tcase_add_test(tc_core, test_rational_clone);
    tcase_add_test(tc_core, test_rational_normalize);
    tcase_add_test(tc_core, test_rational_multiply);
    tcase_add_test(tc_core, test_rational_divide);
    tcase_add_test(tc_core, test_rational_add);
    tcase_add_test(tc_core, test_rational_subtract);
    tcase_add_test(tc_core, test_rational_invert_sign);
    tcase_add_test(tc_core, test_rational_is_a_smaller_than_b);


    suite_add_tcase(s, tc_core);

    return s;
}

