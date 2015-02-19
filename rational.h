/**
 * @brief Header file for rational.
 *
 * This file describes the rational functions and the rational data structure.
 *
 * @file rational.h
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#ifndef RATIONAL_H
#define RATIONAL_H RATIONAL_H

#define BUFFER 40 /**< Buffer size for string representations of rational numbers.  */
#define ERROR_MALLOC_FAILED "Not enough memory! The call to malloc failed and the programm will stop."

/**
 * @brief rational number.
 *
 * This structure represents a rational number. n is the nummerator of the numerator
 * and d is the denominator of the number.
 */
struct Rational
{
    int n; /**> Numerator of number. For 2/3 = 2 */
    int d; /**> Denominator of number. For 2/3 = 3 */
};

/**
 * @brief Create a new rational number with value 0/1.
 *
 * This function creates a new rational number with value 0, i.e.
 * n = 0, d = 1.
 *
 * @return new rational number representing 0
 */
struct Rational *rational_create();

/**
 * @brief Create a new rational number.
 *
 * This function creates a new rational number with the
 * value nominator/denominator.
 *
 * @return new rational number nominator/denominator
 */
struct Rational *rational_get(int nominator, int denominator);

/**
 * @brief Clone a rational number.
 *
 * This function creates a new rational number with the
 * value of the given rational number.
 *
 * @return clone of given number
 */
struct Rational *rational_clone(struct Rational *a);

/**
 * @brief Normalize rational number.
 *
 * This function normalize the given rational number. After
 * normalizing the denimonator is > 0 and the largest common
 * divisor of nominator and denominator is 1.
 */
void rational_normalize(struct Rational *r);

/**
 * @brief Multiply two rational numbers.
 *
 * This function multiplies to rational numbers and returns the result
 * as new rational number.
 *
 * @return result of multiplication
 */
struct Rational *rational_multiply(struct Rational *a, struct Rational *b);

/**
 * @brief Divide two rational numbers.
 *
 * This function divides to rational numbers and returns the result
 * as new rational number.
 *
 * @return result of division
 */
struct Rational *rational_divide(struct Rational *a, struct Rational *b);

/**
 * @brief Add two rational numbers.
 *
 * This function adds to rational numbers and returns the result
 * as new rational number.
 *
 * @return result of addition
 */
struct Rational *rational_add(struct Rational *a, struct Rational *b);

/**
 * @brief Subtract two rational numbers.
 *
 * This function subtracts to rational numbers and returns the result
 * as new rational number.
 *
 * @return result of subtraction
 */
struct Rational *rational_subtract(struct Rational *a, struct Rational *b);

/**
 * @brief Invert the sign of a rational number.
 *
 * This function inverts the sign of the given rational number and returns
 * the result as new rational number.
 *
 * @return new rational number with inverted sign
 */
struct Rational *rational_invert_sign(struct Rational *a);

/**
 * @brief Compare two rational numbers.
 *
 * This function compares the given rational numbers and returns 1 if a < b
 * and 0 else.
 *
 * @return 1 if a<b, 0 else
 */
int rational_is_a_smaller_than_b(struct Rational *a, struct Rational *b);

/**
 * @brief Print rational number to stdout.
 *
 * This function prints the given rational number to stdout as
 * nominator/denominator.
 */
void rational_print(struct Rational *a);

/**
 * @brief Converts the rational number to a string.
 *
 * This function converts the rational number to a string, i.e. it returns
 * a char** with content "nominator/denominator\0".
 *
 * @return pointer to string representation
 */
char **rational_to_string(struct Rational *a);

#endif
