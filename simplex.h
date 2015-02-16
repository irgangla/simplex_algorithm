/**
 * @brief Header file for simplex.
 *
 * This file describes the simplex methods and the simplex tableau, which is the
 * data structrue used for the algorithm.
 *
 * @file simplex.h
 * @author Thomas Irgang
 * @date 16 Feb 2015
 */

#ifndef SIMPLEX_H
#define SIMPLEX_H SIMPLEX_H

/**
 * @brief Data structure for simplex algorithm.
 *
 * This structure groups all data used from the simplex algorithm.
 */
struct Tableau{
  int rows; /**< Number of equations = number of rows of A. */
  int cols; /**< Number of variables = number of columns of A. */
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
struct Tableau* create_tableau(int equations, int variables);
/**
 * @brief Free memory of given tableau.
 *
 * This function frees the memory which is associated with the given tableau.
 *
 * @param tableau
 *    pointer to tableau to free
 */

void freeTableau(struct Tableau *tableau);
void printTableau(struct Tableau *tableau);
void resizeTableau(struct Tableau *tableau, int equations, int variables);
void updatePivot(struct Tableau *tableau);
void simplexStep(struct Tableau *tableau);
struct Rational **getSolution(struct Tableau *tableau);
void printSolution(struct Tableau *tableau);

struct Tableau *simplexPhaseI(struct Tableau *tab);
void simplexPhaseII(struct Tableau *tableau);
void prepareTableauForPhaseII(struct Tableau *phase1, struct Tableau *tableau);

int ggT(int a, int b);
void minimizeDivisor(struct Tableau *tableau);

#endif
