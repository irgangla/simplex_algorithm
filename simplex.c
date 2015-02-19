/**
 * @brief Source file for simplex.
 *
 * This file implements the simplex functions.
 *
 * @file simplex.c
 * @author Thomas Irgang
 * @date 17 Feb 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simplex.h"

/*
 * @brief Resize the given tableau.
 *
 * This function resize the given tableau to fit the given size
 *
 * TODO: implement add condition and integer solution, which will require this function
 *
 * @param tableau
 *    tableau to resize
 * @param equations
 *    number of equations of the resized tableau
 * @param variables
 *    number of variables of the resized tableau
static void resize_tableau(struct Tableau *tableau, int equations, int variables); */

/**
 * @brief Update pivot element of tableau.
 *
 * This function implements the rule of Bland to update the pivot column
 * and line of the given tableau.
 *
 * @param tableau
 *    tableau to update pivot
 */
static void update_pivot(struct Tableau *tableau);

/**
 * @brief Implementation of simplex step.
 *
 * This function implements the simplex step. It switch, based on the values
 * of pivotLine and pivotColumn, a basis and a none basis variable.
 *
 * @param tableau
 *    tableau to calculate step
 */
static void simplex_step(struct Tableau *tableau);

struct Tableau* simplex_create_tableau(int equations, int variables)
{
    int i, j;
    struct Tableau *tableau = NULL;

    tableau = (struct Tableau*)malloc(sizeof(struct Tableau));

    tableau->rows = equations;
    tableau->cols = (variables - equations);

    tableau->A = (struct Rational ***)malloc(equations * sizeof(struct Rational **));
    for(i=0; i<equations; ++i)
    {
        tableau->A[i] = (struct Rational **)malloc((variables - equations) * sizeof(struct Rational *));
        for(j=0; j<(variables - equations); ++j)
        {
            tableau->A[i][j] = rational_create();
        }
    }

    tableau->b = (struct Rational **)malloc(equations * sizeof(struct Rational *));
    for(i=0; i<equations; ++i)
    {
        tableau->b[i] = rational_create();
    }

    tableau->c = (struct Rational **)malloc((variables - equations) * sizeof(struct Rational *));
    for(i=0; i<(variables - equations); ++i)
    {
        tableau->c[i] = rational_create();
    }

    tableau->z = rational_create();

    tableau->bvs = (int *)malloc(equations * sizeof(int));
    for(i=0; i<equations; ++i)
    {
        tableau->bvs[i] = 0;
    }

    tableau->nbvs = (int *)malloc((variables-equations) * sizeof(int));
    for(i=0; i<(variables-equations); ++i)
    {
        tableau->nbvs[i] = 0;
    }

    tableau->pivotLine = -1;
    tableau->pivotColumn = -1;

    return tableau;
}

void simplex_free_tableau(struct Tableau *tableau)
{
    int i, j;

    for(i=0; i<tableau->rows; ++i)
    {
        for(j=0; j<tableau->cols; ++j)
        {
            free(tableau->A[i][j]);
        }
        free(tableau->A[i]);
    }
    free(tableau->A);

    for(i=0; i<tableau->rows; ++i)
    {
        free(tableau->b[i]);
    }
    free(tableau->b);

    for(i=0; i<tableau->cols; ++i)
    {
        free(tableau->c[i]);
    }
    free(tableau->c);

    free(tableau->z);

    free(tableau->bvs);

    free(tableau->nbvs);

    free(tableau);
}

void simplex_print_tableau(struct Tableau *tableau)
{
    int i,j;
    struct Rational *tmp;
    char **string = NULL;

    printf("Tableau:\n");
    for(i=0; i<tableau->cols; ++i)
    {
        string = rational_to_string(tableau->c[i]);
        printf("%8s ", *string);
        free(*string);
        free(string);
    }
    string = rational_to_string(tableau->z);
    printf(" |  %8s\n", *string);
    free(*string);
    free(string);
    for(i=0; i<tableau->cols; ++i)
    {
        printf("---------");
    }
    printf("-------------\n");
    for(i=0; i<tableau->rows; ++i)
    {
        for(j=0; j<tableau->cols; ++j)
        {
            string = rational_to_string(tableau->A[i][j]);
            printf("%8s ", *string);
            free(*string);
            free(string);
        }
        string = rational_to_string(tableau->b[i]);
        printf(" |  %8s\n", *string);
        free(*string);
        free(string);
    }

    printf("Pivot-Line: %d, Pivot-Column: %d\n", tableau->pivotLine, tableau->pivotColumn);

    tmp = rational_invert_sign(tableau->z);
    string = rational_to_string(tmp);
    printf("aktueller Zielfunktionswert: %s\n", *string);
    free(*string);
    free(string);
    free(tmp);


    printf("aktuelle Basisvariablen: [ ");
    for(i=0; i<tableau->rows; ++i)
    {
        printf("%d", tableau->bvs[i]);
        if(i < tableau->rows-1)
        {
            printf(", ");
        }
    }
    printf(" ]\n");

    printf("aktuelle Nichtbasisvariablen: [ ");
    for(i=0; i<(tableau->cols); ++i)
    {
        printf("%d", tableau->nbvs[i]);
        if(i < (tableau->cols)-1)
        {
            printf(", ");
        }
    }
    printf(" ]\n");
}

struct Rational **simplex_get_solution(struct Tableau *tableau)
{
    struct Rational **solution;
    int i;

    solution = (struct Rational **)malloc(sizeof(struct Rational *));
    *solution = (struct Rational *)malloc((tableau->cols + tableau->rows) * sizeof(struct Rational));
    for(i=0; i<(tableau->cols + tableau->rows); ++i)
    {
        (*solution)[i].n = 0;
        (*solution)[i].d = 1;
    }
    for(i=0; i<tableau->rows; ++i)
    {
        (*solution)[tableau->bvs[i]].n = (tableau->b[i])->n;
        (*solution)[tableau->bvs[i]].d = (tableau->b[i])->d;
    }

    return solution;
}

void simplex_print_solution(struct Tableau *tableau)
{
    int i;
    struct Rational **solution = simplex_get_solution(tableau);
    char **string;

    printf("[ ");
    for(i=0; i<(tableau->cols + tableau->rows); ++i)
    {
        string = rational_to_string(&((*solution)[i]));
        printf("%s", *string);
        free(*string);
        free(string);

        if(i < (tableau->cols + tableau->rows)-1)
        {
            printf(", ");
        }
    }
    printf(" ]\n");

    free(*solution);
    free(solution);
}

/*static void resize_tableau(struct Tableau *tableau, int equations, int variables)
{
    int i, j;

    if(tableau->rows > equations)
    {
        for(i=equations; i<tableau->rows; ++i)
        {
            for(j=0; j<tableau->cols; ++j)
            {
                free(tableau->A[i][j]);
            }
            free(tableau->A[i]);
        }
    }

    if(tableau->rows != equations)
    {
        tableau->A = (struct Rational ***)realloc(tableau->A, equations * sizeof(struct Rational **));
    }

    if(tableau->cols != variables)
    {
        for(i=0; i<equations; ++i)
        {
            if(i >= tableau->rows)
            {
                tableau->A[i] = (struct Rational **)malloc(variables * sizeof(struct Rational *));
                for(j=0; j<variables; ++j)
                {
                    tableau->A[i][j] = rational_create();
                }
            }
            else
            {
                for(j=variables; j<tableau->cols; ++j)
                {
                    free(tableau->A[i][j]);
                }
                tableau->A[i] = (struct Rational **)realloc(tableau->A[i], variables * sizeof(struct Rational *));
                for(j=tableau->cols; j<variables; ++j)
                {
                    tableau->A[i][j] = rational_create();
                }
            }
        }
    }

    for(j=variables; j<tableau->cols; ++j)
    {
        free(tableau->c[j]);
    }
    tableau->c = (struct Rational **)realloc(tableau->c, variables * sizeof(struct Rational *));
    for(j=tableau->cols; j<variables; ++j)
    {
        tableau->c[j] = rational_create();
    }

    for(j=equations; j<tableau->rows; ++j)
    {
        free(tableau->b[j]);
    }
    tableau->b = (struct Rational **)realloc(tableau->b, equations * sizeof(struct Rational *));
    for(j=tableau->rows; j<equations; ++j)
    {
        tableau->b[j] = rational_create();
    }

    tableau->bvs = (int *)realloc(tableau->bvs, variables * sizeof(int));

    tableau->nbvs = (int *)realloc(tableau->nbvs, (variables-equations) * sizeof(int));

    tableau->cols = variables;
    tableau->rows = equations;
}*/

static void update_pivot(struct Tableau *tableau)
{
    int i;
    struct Rational *min = rational_create();
    struct Rational *tmp = NULL;

    tableau->pivotColumn = -1;
    tableau->pivotLine = -1;

    do
    {
        i=tableau->pivotColumn+1;
        tableau->pivotColumn = -1;
        for(; i<tableau->cols; ++i)
        {
            if((tableau->c[i])->n > 0)
            {
                tableau->pivotColumn = i;
                break;
            }
        }

        if(tableau->pivotColumn >= 0)
        {
            tableau->pivotLine = -1;

            for(i=0; i<tableau->rows; ++i)
            {
                if((tableau->A[i][tableau->pivotColumn])->n > 0)
                {
                    tmp = rational_divide(tableau->b[i], tableau->A[i][tableau->pivotColumn]);
                    if(1 == rational_is_a_smaller_than_b(tmp, min) || tableau->pivotLine == -1)
                    {
                        tableau->pivotLine = i;
                        free(min);
                        min = NULL;
                        min = tmp;
                    }
                    else
                    {
                        free(tmp);
                        tmp = NULL;
                    }
                }
            }
        }
    }
    while(tableau->pivotColumn != -1 && tableau->pivotLine == -1);

    free(min);
}


static void simplex_step(struct Tableau *tableau)
{
    int i, j, temp;
    struct Rational *pivotValue, *tmp, *tmp2, *fact;
    struct Rational **bv;
    struct Rational *cb;

    bv = (struct Rational **)malloc(tableau->rows * sizeof(struct Rational *));
    for(i=0; i<tableau->rows; ++i)
    {
        if(i == tableau->pivotLine)
        {
            bv[i] = rational_get(1, 1);
        }
        else
        {
            bv[i] = rational_get(0, 1);
        }
    }
    cb = rational_get(0,1);

    pivotValue = rational_clone(tableau->A[tableau->pivotLine][tableau->pivotColumn]);

    if(!(pivotValue->n == 1 && pivotValue->d == 1))
    {
        for(i=0; i<tableau->cols; ++i)
        {
            tmp = tableau->A[tableau->pivotLine][i];
            tableau->A[tableau->pivotLine][i] = rational_divide(tableau->A[tableau->pivotLine][i], pivotValue);
            free(tmp);
        }

        tmp = bv[tableau->pivotLine];
        bv[tableau->pivotLine] = rational_divide(bv[tableau->pivotLine], pivotValue);
        free(tmp);

        tmp = tableau->b[tableau->pivotLine];
        tableau->b[tableau->pivotLine] = rational_divide(tableau->b[tableau->pivotLine], pivotValue);
        free(tmp);
    }

    fact = rational_clone(tableau->c[tableau->pivotColumn]);
    for(i=0; i<tableau->cols; ++i)
    {
        tmp = tableau->c[i];
        tmp2 = rational_multiply(tableau->A[tableau->pivotLine][i], fact);
        tableau->c[i] = rational_subtract(tableau->c[i], tmp2);
        free(tmp2);
        free(tmp);
    }

    tmp = cb;
    tmp2 = rational_multiply(bv[tableau->pivotLine], fact);
    cb = rational_subtract(cb, tmp2);
    free(tmp2);
    free(tmp);

    tmp = tableau->z;
    tmp2 = rational_multiply(tableau->b[tableau->pivotLine], fact);
    tableau->z = rational_subtract(tableau->z, tmp2);
    free(tmp2);
    free(tmp);
    free(fact);

    for(j=0; j<tableau->rows; ++j)
    {
        if(j == tableau->pivotLine)
        {
            continue;
        }

        fact = rational_clone(tableau->A[j][tableau->pivotColumn]);
        for(i=0; i<tableau->cols; ++i)
        {
            tmp = tableau->A[j][i];
            tmp2 = rational_multiply(tableau->A[tableau->pivotLine][i], fact);
            tableau->A[j][i] = rational_subtract(tableau->A[j][i], tmp2);
            free(tmp2);
            free(tmp);
        }

        tmp = bv[j];
        tmp2 = rational_multiply(bv[tableau->pivotLine], fact);
        bv[j] = rational_subtract(bv[j], tmp2);
        free(tmp2);
        free(tmp);

        tmp = tableau->b[j];
        tmp2 = rational_multiply(tableau->b[tableau->pivotLine], fact);
        tableau->b[j] = rational_subtract(tableau->b[j], tmp2);
        free(tmp2);
        free(tmp);
        free(fact);
    }

    free(tableau->c[tableau->pivotColumn]);
    tableau->c[tableau->pivotColumn] = cb;
    for(i=0; i<tableau->rows; ++i)
    {
        free(tableau->A[i][tableau->pivotColumn]);
        tableau->A[i][tableau->pivotColumn] = bv[i];
    }

    temp = tableau->bvs[tableau->pivotLine];
    tableau->bvs[tableau->pivotLine] = tableau->nbvs[tableau->pivotColumn];
    tableau->nbvs[tableau->pivotColumn] = temp;

    free(bv);
    free(pivotValue);
}

struct Tableau *simplex_find_start_corner(struct Tableau *tab)
{
    int i, j;
    struct Tableau *phase1;
    struct Rational *tmp;

    phase1 = simplex_create_tableau(tab->rows, (tab->cols + 2*tab->rows));

    for(i=0; i<tab->rows; ++i)
    {
        for(j=0; j<tab->cols; ++j)
        {
            free(phase1->A[i][j]);
            phase1->A[i][j] = rational_get((tab->A[i][j])->n, (tab->A[i][j])->d);
        }
    }

    for(i=0; i<phase1->rows; ++i)
    {
        free(phase1->A[phase1->rows-1-i][phase1->cols-1-i]);
        phase1->A[phase1->rows-1-i][phase1->cols-1-i] = rational_get(1,1);
    }

    for(i=0; i<phase1->cols; ++i)
    {
        free(phase1->c[i]);
        phase1->c[i] = rational_get(0,1);
        if(i < phase1->cols)
        {
            for(j=0; j<phase1->rows; ++j)
            {
                tmp = phase1->c[i];
                phase1->c[i] = rational_add(phase1->c[i], phase1->A[j][i]);
                free(tmp);
            }
        }
    }

    for(j=0; j<tab->rows; ++j)
    {
        free(phase1->b[j]);
        phase1->b[j] = rational_get((tab->b[j])->n, (tab->b[j])->d);
    }

    for(j=0; j<phase1->rows; ++j)
    {
        tmp = phase1->z;
        phase1->z = rational_add(phase1->z, phase1->b[j]);
        free(tmp);
    }

    for(i=0; i<phase1->cols; ++i)
    {
        phase1->nbvs[i] = i;
    }

    for(i=0; i<phase1->rows; ++i)
    {
        phase1->bvs[i] = i + phase1->cols;
    }

    update_pivot(phase1);

    simplex_print_tableau(phase1);

    while(phase1->pivotColumn >= 0 && phase1->pivotLine >= 0)
    {
        simplex_step(phase1);
        update_pivot(phase1);
    }

    return phase1;
}

void prepare_with_start_corner(struct Tableau *phase1, struct Tableau *tableau)
{
    int i, j;

    for(i=0; i<phase1->rows; ++i)
    {
        if(tableau->bvs[i] != phase1->bvs[i])
        {
            tableau->pivotColumn = -1;
            for(j=0; j<tableau->cols; ++j)
            {
                if(tableau->nbvs[j] == phase1->bvs[i])
                {
                    tableau->pivotColumn = j;
                    break;
                }
            }
            tableau->pivotLine = i;
            simplex_step(tableau);
        }
    }
}

void simplex_find_best_solution(struct Tableau *tableau)
{

    update_pivot(tableau);

    while(tableau->pivotColumn >= 0 && tableau->pivotLine >= 0)
    {
        simplex_step(tableau);
        update_pivot(tableau);
    }
}
