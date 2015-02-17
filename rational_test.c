/**
 * @brief Little program using rational numbers.
 *
 * This file implements a little example using rational numbers.
 *
 * @file rational_test.c
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "rational.h"

/**
 * @brief Little example using rational numbers.
 *
 * This function is a little example how to use rational numbers.
 */
int main(void)
{
    struct Rational *r, *s, *t;
    char **string;

    r = rational_create(); /* Create a rational number with value 0. */
    r->n = 12; /* Set value of rational number. */
    r->d = 18;

    rational_normalize(r); /* Normalize the rational number. (Result: 2/3) */

    printf("r=");
    rational_print(r); /* Print rational number to stdout. */
    printf("\n");

    s = rational_get(1, 5); /* Create a rational number with given value. */

    string = rational_to_string(s); /* Convert number to string (pointer to char[] containg string representation) */
    printf("s=%8s\n", *string);
    free(*string); /* Do not forget to free also the array, not only the pointer. */
    free(string);

    printf("r * s = ");
    t = rational_multiply(r, s); /* Mulitply rational numbers. */
    rational_print(t);
    printf("\n");
    free(t);

    printf("r / s = ");
    t = rational_divide(r, s); /* Divide rational numbers. */
    rational_print(t);
    printf("\n");
    free(t);

    printf("r + s = ");
    t = rational_add(r, s); /* Add rational numbers. */
    rational_print(t);
    printf("\n");
    free(t);

    printf("r - s = ");
    t = rational_subtract(r, s); /* Subtract rational numbers. */
    rational_print(t);
    printf("\n");
    free(t);

    printf("r < s? %d\n", rational_is_a_smaller_than_b(r, s)); /* Compare rational numbers. */
    printf("s < r? %d\n", rational_is_a_smaller_than_b(s, r));

    free(r);
    free(s);

    return EXIT_SUCCESS;
}
