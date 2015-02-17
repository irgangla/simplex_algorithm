/**
 * @brief Header file for simplex.
 *
 * This file describes the simplex functions and the simplex tableau, which is the
 * data structrue used for the algorithm.
 *
 * @file simplex.h
 * @author Thomas Irgang
 * @date 16 Feb 2015
 */

#ifndef SIMPLEX_H
#define SIMPLEX_H SIMPLEX_H

#include "rational.h"

/**
 * @brief Data structure for simplex algorithm.
 *
 * This structure groups all data used from the simplex algorithm.
 */
struct Tableau
{
    int rows; /**< Number of equations = number of rows of A. */
    int cols; /**< Number of none basis variables = number of columns of A. */
    struct Rational ***A; /**< Matrix A. Ax = b. */
    struct Rational **b; /**< Limits vector b. Ax = b. */
    struct Rational **c; /**< Target function a. cx = z. */
    struct Rational *z; /**< Target function value. cx = z. */
    int pivotLine; /**< Current pivot line. */
    int pivotColumn; /**< Current pivot column. */
    int *bvs;  /**< Current basis variables. */
    int *nbvs; /**< Current none basis variables. */
};

/**
 * @brief Create a new tableau.
 *
 * This function creates a new 0/1-filled Tableau structure with the given number of
 * equations and the given number of variables.
 *
 * @param equations
 *    number of equations of new tableau
 * @param variables
 *    number of variables of new tableau
 * @return pointer to new tableau
 */
struct Tableau* simplex_create_tableau(int equations, int variables);


/**
 * @brief Free memory of given tableau.
 *
 * This function frees the memory which is associated with the given tableau.
 *
 * @param tableau
 *    pointer to tableau to free
 */
void simplex_free_tableau(struct Tableau *tableau);

/**
 * @brief Print the tableau to stdout.
 *
 * This function prints the given tableau to stdout.
 *
 * @param tableau
 *    pointer to tableau to print
 */
void simplex_print_tableau(struct Tableau *tableau);

/**
 * @brief Get current solution of tableau.
 *
 * This function returns the current solution of the given tableau.
 * solution[i] = value of variable i as rational number
 *
 * @param tableau
 *    tableau to read solution
 * @return current solution of tableau
 */
struct Rational **simplex_get_solution(struct Tableau *tableau);

/**
 * @brief Print tableau to stdout.
 *
 * This function prints the given tableau to stdout.
 *
 * @param tableau
 *    tableau to print
 */
void simplex_print_solution(struct Tableau *tableau);

/**
 * @brief Phase 1 of simplex algorithm: find start corner.
 *
 * This function creates a new tableau which extends the given tableau with
 * a new identity matrix and use the extended tableau to calculate a valid
 * corner. This corner can be used for phase 2 of the simplex algorithm, i.e.
 * with the help of "prepare_with_start_corner".
 *
 * @param tableau
 *    tableau to find start corner
 * @return solved, extended tableau
 */
struct Tableau *simplex_find_start_corner(struct Tableau *tab);

/**
 * @brief Prepare phase 2 of simplex algorithm.
 *
 * This function prepares the given tableau "tableau" for the phase 2 of the
 * simplex algorithm with the help of the given tableau "phase1", which have to
 * be a solved, extended tableau.
 *
 * @param phase1
 *    solved extended tableau
 * @param tableau
 *    tableau of optimization problem
 */
void prepare_with_start_corner(struct Tableau *phase1, struct Tableau *tableau);

/**
 * @brief Phase 2 of simplex algorithm.
 *
 * This function solves the optimization problem given as valid tableau.
 *
 * @param tableau
 *    problem to solve
 */
void simplex_find_best_solution(struct Tableau *tableau);

#endif
