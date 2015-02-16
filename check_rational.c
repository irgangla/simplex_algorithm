#include <stdlib.h>
#include <check.h>

#include "rational.h"

START_TEST(test_getRational)
{
  struct Rational *r;

  r = getRational(2,3);

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

  tcase_add_test(tc_core, test_getRational);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void){
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = rational_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}

