/**
 * @brief Little program using simplex algorithm.
 *
 * This file implements a little example using the simplex algorithm.
 *
 * @file rational_test.c
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simplex.h"

/**
 * @brief Create tableau with example problem.
 *
 * This function creates a new (invalid) simplex tableau filled with a
 * small test optimization problem.
 *
 * Maximize 300x + 500y
 * s.t.: 1) x + 2y <= 170
 *       2) x +  y <= 150
 *       3)     3y <= 180
 *       4)      y >= 1
 *
 * @return tableau for problem
 */
struct Tableau *create_testproblem_tableau(void);

/**
 * @brief Solve example problem.
 *
 * This function solves the little example problem using
 * phase 1 and 2 of simplex algorithm.
 */
int main(void)
{
    struct Tableau *tableau = NULL, *phase1 = NULL; /* variables for tableaus */
    clock_t start, s_p1, e_p1, s_prep = 0, e_prep = 0, s_p2 = 0, e_p2 = 0, end, calc; /* variables for time */

    start = clock();

    tableau = create_testproblem_tableau(); /* Create test problem. */
    printf("Print tableau ...\n");
    simplex_print_tableau(tableau); /* Print test problem to stdout. */


    printf("Run simplex phase 1 ...\n");
    s_p1 = clock();
    phase1 = simplex_find_start_corner(tableau); /* Calculate start corner for phase 2 of simplex algorithm. */
    e_p1 = clock();
    simplex_print_tableau(phase1); /* Print solved, extended tableau. */
    printf("Solution of phase 1:\n");
    simplex_print_solution(phase1); /* Print solution of phase 1. */

    if((phase1->z)->n == 0)
    {
        printf("Phase 1 found a start corner for phase 2.\n");
        printf("Prepare tableau for phase 2 ...\n");
        s_prep = clock();
        prepare_with_start_corner(phase1, tableau); /* Update tableau with found solution of phase 1. */
        e_prep = clock();
        printf("Tableau updated for phase 2:\n");
        simplex_print_tableau(tableau); /* Print tableau used for phase 2 of simplex algorithm. */

        printf("Run simplex phase 2 ...\n");
        s_p2 = clock();
        simplex_find_best_solution(tableau); /* Phase 2: Find best solution for problem. */
        e_p2 = clock();
        simplex_print_tableau(tableau); /* Print final tableau of phase 2. */
        printf("Best solution of problem: ");
        simplex_print_solution(tableau); /* Print best solution of problem. */
    }

    end = clock();

    calc = (double)(end - start); /* Calculate and print times. */
    printf("Total time: %f ms\n", (double)(calc*1000)/CLOCKS_PER_SEC);
    calc = (double)(e_p1 - s_p1);

    if((phase1->z)->n == 0)
    {
      printf("Simplex phase 1: %f ms\n", (double)(calc*1000)/CLOCKS_PER_SEC);
      calc = (double)(e_prep - s_prep);
      printf("Prepare phase 2: %f ms\n", (double)(calc*1000)/CLOCKS_PER_SEC);
      calc = (double)(e_p2 - s_p2);
      printf("Simplex phase 2: %f ms\n", (double)(calc*1000)/CLOCKS_PER_SEC);
    }

    printf("Free tableau memory ...\n");
    simplex_free_tableau(phase1); /* Free memory of tableaus. */
    simplex_free_tableau(tableau);

    return EXIT_SUCCESS;
}

struct Tableau *create_testproblem_tableau(void)
{
    struct Tableau *tableau;
    int i, j;
    int t[5][3] =
    {
        {300,500,0},
        {1,2,170},
        {1,1,150},
        {0,3,180},
        {0,-1,-1}
    };
    int basis[4] = {2,3,4,5};
    int nbvs[2] = {0,1};

    printf("Create a new tableau ...\n");
    tableau = simplex_create_tableau(4,6); /* Create a new tableau for 4 conditions and 6 variables (x, y and slack variables for the 4 inequalities) */
    printf("Fill tableau with test problem data ...\n");

    for(i=0; i<2; i++) /* Target function. */
    {
        (tableau->c[i])->n = t[0][i];
    }

    (tableau->z)->n = 0; /* Target function value. */

    for(i=0; i<4; i++) /* Limits of inequalities. */
    {
        (tableau->b[i])->n = t[i+1][2];
    }

    for(i=0; i<4; ++i) /* Tableau data (only none basis variable columns, (invalid) basis is s1, s2, s3, s4). */
    {
        for(j=0; j<2; ++j)
        {
            (tableau->A[i][j])->n = t[i+1][j];
        }
    }

    memcpy(tableau->bvs, basis, 4*sizeof(int)); /* Initialze basis variables. */
    memcpy(tableau->nbvs, nbvs, 2*sizeof(int)); /* Initialize none basis variables. */

    return tableau;
}
