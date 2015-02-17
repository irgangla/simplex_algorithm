#include <stdlib.h>
#include <check.h>

#include "check_simplex.h"
#include "check_rational.h"

int main(void)
{
    int number_failed;
    SRunner *sr;
    Suite *s_rational = rational_suite();
    Suite *s_simplex = simplex_suite();


    sr = srunner_create(s_simplex);
    srunner_add_suite(sr, s_rational);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return (number_failed == 0)? EXIT_SUCCESS : EXIT_FAILURE;
}
