/**
 * @brief Check unit tests for simplex algorithm.
 *
 * This file contains the unit tests for the simples algorithm.
 *
 * @file check_simplex.c
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#include <stdlib.h>
#include <check.h>

#include "simplex.h"

Suite *simplex_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Simplex");

    tc_core = tcase_create("Core");

    /*tcase_add_test(tc_core, test_);*/
    suite_add_tcase(s, tc_core);

    return s;
}




