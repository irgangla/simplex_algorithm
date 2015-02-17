/**
 * @brief Source file for rational.
 *
 * This file implements the rational functions.
 *
 * @file rational.c
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#include <stdlib.h>
#include <stdio.h>

#include "rational.h"

/**
 * @brief Largest common divisor.
 *
 * This function calculates the largest common divisor of the given
 * two integer values.
 *
 * @param a
 *    integer a
 * @param b
 *    integer b
 * @return largest common divisor of a and b
 */
static int r_largest_common_divisor(int a, int b);

struct Rational *rational_create()
{
    return rational_get(0, 1);
}

struct Rational *rational_get(int nominator, int denominator)
{
    struct Rational *r = NULL;

    r = (struct Rational *)malloc(sizeof(struct Rational));
    if(r == NULL)
    {
        fprintf(stderr, ERROR_MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    r->n = nominator;
    r->d = denominator;

    return r;
}


void rational_normalize(struct Rational *r)
{
    int div = r_largest_common_divisor(r->n , r->d);

    r->n = (r->n)/div;
    r->d = (r->d)/div;

    if(r->d < 0)
    {
        r->n = -(r->n);
        r->d = -(r->d);
    }
}


struct Rational *rational_multiply(struct Rational *a, struct Rational *b)
{
    int n, d;
    struct Rational *r;

    n = (a->n) * (b->n);
    d = (a->d) * (b->d);

    r = rational_get(n, d);
    rational_normalize(r);

    return r;
}

struct Rational *rational_divide(struct Rational *a, struct Rational *b)
{
    int n, d;
    struct Rational *r = NULL;

    n = (a->n) * (b->d);
    d = (a->d) * (b->n);

    r = rational_get(n, d);
    rational_normalize(r);

    return r;
}

struct Rational *rational_add(struct Rational *a, struct Rational *b)
{
    int n, d;
    struct Rational *r;

    n = (a->n) * (b->d) + (b->n) * (a->d);
    d = (a->d) * (b->d);

    r = rational_get(n, d);
    rational_normalize(r);

    return r;
}


struct Rational *rational_subtract(struct Rational *a, struct Rational *b)
{
    int n, d;
    struct Rational *r;

    n = (a->n) * (b->d) - (b->n) * (a->d);
    d = (a->d) * (b->d);

    r = rational_get(n, d);
    rational_normalize(r);

    return r;
}

void rational_print(struct Rational *a)
{
    if(a->d == 1)
    {
        printf("%d", a->n);
    }
    else
    {
        printf("%d/%d", a->n, a->d);
    }
}

char **rational_to_string(struct Rational *a)
{
    char **string = NULL;
    int n;

    string = (char **)malloc(sizeof(char *));
    if(string == NULL)
    {
        fprintf(stderr, ERROR_MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }
    *string = (char *)malloc(BUFFER * sizeof(char));
    if(*string == NULL)
    {
        fprintf(stderr, ERROR_MALLOC_FAILED);
        exit(EXIT_FAILURE);
    }

    if(a->d == 1)
    {
        n = sprintf(*string, "%d", a->n);
    }
    else
    {
        n = sprintf(*string, "%d/%d", a->n, a->d);
    }

    if(n>(BUFFER - 1))
    {
        fprintf(stderr, "Memory error in toString() method of rational. Emergeny stop program!\n");
        exit(EXIT_FAILURE);
    }

    *string = (char *)realloc(*string, (n+1) * sizeof(char));

    return string;
}


static int r_largest_common_divisor(int a, int b)
{
    int tmp;

    a = (a<0)?-a:a;
    b = (b<0)?-b:b;

    do
    {
        if(b > a)
        {
            tmp = a;
            a = b;
            b = tmp;
        }
        a -= b;
    }
    while(b > 0);

    return a;
}

int rational_is_a_smaller_than_b(struct Rational *a, struct Rational *b)
{
    int va, vb;

    va = (a->n) * (b->d);
    vb = (b->n) * (a->d);

    if(va < vb)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

struct Rational *rational_invertSign(struct Rational *a)
{
    return rational_get(-a->n, a->d);
}

struct Rational *rational_clone(struct Rational *a)
{
    return rational_get(a->n, a->d);
}
