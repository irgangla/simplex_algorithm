#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rational.h"

#include "simplex.h"


struct Tableau* create_tableau(int equations, int variables){
  int i, j;
  struct Tableau *tableau = NULL;

  tableau = (struct Tableau*)malloc(sizeof(struct Tableau));

  tableau->rows = equations;
  tableau->cols = (variables - equations);

  tableau->A = (struct Rational ***)malloc(equations * sizeof(struct Rational **));
  for(i=0; i<equations; ++i){
    tableau->A[i] = (struct Rational **)malloc((variables - equations) * sizeof(struct Rational *));
    for(j=0; j<(variables - equations); ++j){
      tableau->A[i][j] = createRational();
    }
  }

  tableau->b = (struct Rational **)malloc(equations * sizeof(struct Rational *));
  for(i=0; i<equations; ++i){
    tableau->b[i] = createRational();
  }

  tableau->c = (struct Rational **)malloc((variables - equations) * sizeof(struct Rational *));
  for(i=0; i<(variables - equations); ++i){
    tableau->c[i] = createRational();
  }

  tableau->z = createRational();

  tableau->bvs = (int *)malloc(equations * sizeof(int));
  for(i=0; i<equations; ++i){
    tableau->bvs[i] = 0;
  }

  tableau->nbvs = (int *)malloc((variables-equations) * sizeof(int));
  for(i=0; i<(variables-equations); ++i){
    tableau->nbvs[i] = 0;
  }

  tableau->pivotLine = -1;
  tableau->pivotColumn = -1;

  return tableau;
}

void freeTableau(struct Tableau *tableau){
  int i, j;

  for(i=0; i<tableau->rows; ++i){
    for(j=0; j<tableau->cols; ++j){
      free(tableau->A[i][j]);
    }
    free(tableau->A[i]);
  }
  free(tableau->A);

  for(i=0; i<tableau->rows; ++i){
    free(tableau->b[i]);
  }
  free(tableau->b);

  for(i=0; i<tableau->cols; ++i){
    free(tableau->c[i]);
  }
  free(tableau->c);

  free(tableau->z);

  free(tableau->bvs);

  free(tableau->nbvs);

  free(tableau);
}

void printTableau(struct Tableau *tableau){
  int i,j;
  struct Rational *tmp;
  char **string = NULL;

  printf("Tableau:\n");
  for(i=0; i<tableau->cols; ++i){
    string = toString(tableau->c[i]);
    printf("%8s ", *string);
    free(*string);
    free(string);
  }
  string = toString(tableau->z);
  printf(" |  %8s\n", *string);
  free(*string);
  free(string);
  for(i=0; i<tableau->cols; ++i){
    printf("---------");
  }
  printf("-------------\n");
  for(i=0; i<tableau->rows; ++i){
    for(j=0; j<tableau->cols; ++j){
      string = toString(tableau->A[i][j]);
      printf("%8s ", *string);
      free(*string);
      free(string);
    }
    string = toString(tableau->b[i]);
    printf(" |  %8s\n", *string);
    free(*string);
    free(string);
  }

  printf("Pivot-Line: %d, Pivot-Column: %d\n", tableau->pivotLine, tableau->pivotColumn);

  tmp = invertSign(tableau->z);
  string = toString(tmp);
  printf("aktueller Zielfunktionswert: %s\n", *string);
  free(*string);
  free(string);
  free(tmp);


  printf("aktuelle Basisvariablen: [ ");
  for(i=0; i<tableau->rows; ++i){
    printf("%d", tableau->bvs[i]);
    if(i < tableau->rows-1){
      printf(", ");
    }
  }
  printf(" ]\n");

  printf("aktuelle Nichtbasisvariablen: [ ");
  for(i=0; i<(tableau->cols); ++i){
    printf("%d", tableau->nbvs[i]);
    if(i < (tableau->cols)-1){
      printf(", ");
    }
  }
  printf(" ]\n");
}

struct Rational **getSolution(struct Tableau *tableau){
  struct Rational **solution;
  int i;

  solution = (struct Rational **)malloc(sizeof(struct Rational *));
  *solution = (struct Rational *)malloc((tableau->cols + tableau->rows) * sizeof(struct Rational));
  for(i=0; i<(tableau->cols + tableau->rows); ++i){
    (*solution)[i].n = 0;
    (*solution)[i].d = 1;
  }
  for(i=0; i<tableau->rows; ++i){
        (*solution)[tableau->bvs[i]].n = (tableau->b[i])->n;
        (*solution)[tableau->bvs[i]].d = (tableau->b[i])->d;
  }

  return solution;
}

void printSolution(struct Tableau *tableau){
  int i;
  struct Rational **solution = getSolution(tableau);
  char **string;

  printf("[ ");
  for(i=0; i<(tableau->cols + tableau->rows); ++i){
    string = toString(&((*solution)[i]));
    printf("%s", *string);
    free(*string);
    free(string);

    if(i < (tableau->cols + tableau->rows)-1){
      printf(", ");
    }
  }
  printf(" ]\n");

  free(*solution);
  free(solution);
}

void resizeTableau(struct Tableau *tableau, int equations, int variables){
  int i, j;

  if(tableau->rows > equations){
    for(i=equations; i<tableau->rows; ++i){
      for(j=0; j<tableau->cols; ++j){
        free(tableau->A[i][j]);
      }
      free(tableau->A[i]);
    }
  }

  if(tableau->rows != equations){
    tableau->A = (struct Rational ***)realloc(tableau->A, equations * sizeof(struct Rational **));
  }

  if(tableau->cols != variables){
    for(i=0; i<equations; ++i){
      if(i >= tableau->rows){
        tableau->A[i] = (struct Rational **)malloc(variables * sizeof(struct Rational *));
        for(j=0; j<variables; ++j){
          tableau->A[i][j] = createRational();
        }
      }else{
        for(j=variables; j<tableau->cols; ++j){
          free(tableau->A[i][j]);
        }
        tableau->A[i] = (struct Rational **)realloc(tableau->A[i], variables * sizeof(struct Rational *));
        for(j=tableau->cols; j<variables; ++j){
          tableau->A[i][j] = createRational();
        }
      }
    }
  }

  for(j=variables; j<tableau->cols; ++j){
    free(tableau->c[j]);
  }
  tableau->c = (struct Rational **)realloc(tableau->c, variables * sizeof(struct Rational *));
  for(j=tableau->cols; j<variables; ++j){
    tableau->c[j] = createRational();
  }

  for(j=equations; j<tableau->rows; ++j){
    free(tableau->b[j]);
  }
  tableau->b = (struct Rational **)realloc(tableau->b, equations * sizeof(struct Rational *));
  for(j=tableau->rows; j<equations; ++j){
    tableau->b[j] = createRational();
  }

  tableau->bvs = (int *)realloc(tableau->bvs, variables * sizeof(int));

  tableau->nbvs = (int *)realloc(tableau->nbvs, (variables-equations) * sizeof(int));

  tableau->cols = variables;
  tableau->rows = equations;
}

void updatePivot(struct Tableau *tableau){
  int i;
  struct Rational *min = createRational();
  struct Rational *tmp = NULL;

  tableau->pivotColumn = -1;
  tableau->pivotLine = -1;

  do{
    i=tableau->pivotColumn+1;
    tableau->pivotColumn = -1;
    for(; i<tableau->cols; ++i){
      if((tableau->c[i])->n > 0){
        tableau->pivotColumn = i;
        break;
      }
    }

    if(tableau->pivotColumn >= 0){
      tableau->pivotLine = -1;

      for(i=0; i<tableau->rows; ++i){
        if((tableau->A[i][tableau->pivotColumn])->n > 0){
          tmp = divide(tableau->b[i], tableau->A[i][tableau->pivotColumn]);
          if(1 == is_a_smaller_than_b(tmp, min) || tableau->pivotLine == -1){
            tableau->pivotLine = i;
            free(min);
            min = NULL;
            min = tmp;
          }else{
            free(tmp);
            tmp = NULL;
          }
        }
      }
    }
  }while(tableau->pivotColumn != -1 && tableau->pivotLine == -1);

  free(min);
}


void simplexStep(struct Tableau *tableau){
  int i, j, temp;
  struct Rational *pivotValue, *tmp, *tmp2, *fact;
  struct Rational **bv;
  struct Rational *cb;

  bv = (struct Rational **)malloc(tableau->rows * sizeof(struct Rational *));
  for(i=0; i<tableau->rows; ++i){
    if(i == tableau->pivotLine){
      bv[i] = getRational(1, 1);
    }else{
      bv[i] = getRational(0, 1);
    }
  }
  cb = getRational(0,1);

  pivotValue = cloneRational(tableau->A[tableau->pivotLine][tableau->pivotColumn]);

  if(!(pivotValue->n == 1 && pivotValue->d == 1)){
    for(i=0; i<tableau->cols; ++i){
      tmp = tableau->A[tableau->pivotLine][i];
      tableau->A[tableau->pivotLine][i] = divide(tableau->A[tableau->pivotLine][i], pivotValue);
      free(tmp);
    }

    tmp = bv[tableau->pivotLine];
    bv[tableau->pivotLine] = divide(bv[tableau->pivotLine], pivotValue);
    free(tmp);

    tmp = tableau->b[tableau->pivotLine];
    tableau->b[tableau->pivotLine] = divide(tableau->b[tableau->pivotLine], pivotValue);
    free(tmp);
  }

  fact = cloneRational(tableau->c[tableau->pivotColumn]);
  for(i=0; i<tableau->cols; ++i){
    tmp = tableau->c[i];
    tmp2 = multiply(tableau->A[tableau->pivotLine][i], fact);
    tableau->c[i] = subtract(tableau->c[i], tmp2);
    free(tmp2);
    free(tmp);
  }

  tmp = cb;
  tmp2 = multiply(bv[tableau->pivotLine], fact);
  cb = subtract(cb, tmp2);
  free(tmp2);
  free(tmp);

  tmp = tableau->z;
  tmp2 = multiply(tableau->b[tableau->pivotLine], fact);
  tableau->z = subtract(tableau->z, tmp2);
  free(tmp2);
  free(tmp);
  free(fact);

  for(j=0; j<tableau->rows; ++j){
    if(j == tableau->pivotLine){
      continue;
    }

    fact = cloneRational(tableau->A[j][tableau->pivotColumn]);
    for(i=0; i<tableau->cols; ++i){
      tmp = tableau->A[j][i];
      tmp2 = multiply(tableau->A[tableau->pivotLine][i], fact);
      tableau->A[j][i] = subtract(tableau->A[j][i], tmp2);
      free(tmp2);
      free(tmp);
    }

    tmp = bv[j];
    tmp2 = multiply(bv[tableau->pivotLine], fact);
    bv[j] = subtract(bv[j], tmp2);
    free(tmp2);
    free(tmp);

    tmp = tableau->b[j];
    tmp2 = multiply(tableau->b[tableau->pivotLine], fact);
    tableau->b[j] = subtract(tableau->b[j], tmp2);
    free(tmp2);
    free(tmp);
    free(fact);
  }

  free(tableau->c[tableau->pivotColumn]);
  tableau->c[tableau->pivotColumn] = cb;
  for(i=0; i<tableau->rows; ++i){
    free(tableau->A[i][tableau->pivotColumn]);
    tableau->A[i][tableau->pivotColumn] = bv[i];
  }

  temp = tableau->bvs[tableau->pivotLine];
  tableau->bvs[tableau->pivotLine] = tableau->nbvs[tableau->pivotColumn];
  tableau->nbvs[tableau->pivotColumn] = temp;

  free(bv);
  free(pivotValue);
}

struct Tableau *simplexPhaseI(struct Tableau *tab){
  int i, j;
  struct Tableau *phase1;
  struct Rational *tmp;

  phase1 = create_tableau(tab->rows, (tab->cols + 2*tab->rows));

  for(i=0; i<tab->rows; ++i){
    for(j=0; j<tab->cols; ++j){
      free(phase1->A[i][j]);
      phase1->A[i][j] = getRational((tab->A[i][j])->n, (tab->A[i][j])->d);
    }
  }

  for(i=0; i<phase1->rows; ++i){
    free(phase1->A[phase1->rows-1-i][phase1->cols-1-i]);
    phase1->A[phase1->rows-1-i][phase1->cols-1-i] = getRational(1,1);
  }

  for(i=0; i<phase1->cols; ++i){
    free(phase1->c[i]);
    phase1->c[i] = getRational(0,1);
    if(i < phase1->cols){
      for(j=0; j<phase1->rows; ++j){
        tmp = phase1->c[i];
        phase1->c[i] = add(phase1->c[i], phase1->A[j][i]);
        free(tmp);
      }
    }
  }

  for(j=0; j<tab->rows; ++j){
    free(phase1->b[j]);
    phase1->b[j] = getRational((tab->b[j])->n, (tab->b[j])->d);
  }

  for(j=0; j<phase1->rows; ++j){
    tmp = phase1->z;
    phase1->z = add(phase1->z, phase1->b[j]);
    free(tmp);
  }

    for(i=0; i<phase1->cols; ++i){
      phase1->nbvs[i] = i;
  }

  for(i=0; i<phase1->rows; ++i){
      phase1->bvs[i] = i + phase1->cols;
  }

  updatePivot(phase1);

  printTableau(phase1);

  while(phase1->pivotColumn >= 0 && phase1->pivotLine >= 0){
    simplexStep(phase1);
    updatePivot(phase1);
  }

  return phase1;
}

void prepareTableauForPhaseII(struct Tableau *phase1, struct Tableau *tableau){
  int i, j;

  for(i=0; i<phase1->rows; ++i){
    if(tableau->bvs[i] != phase1->bvs[i]){
      tableau->pivotColumn = -1;
      for(j=0; j<tableau->cols; ++j){
        if(tableau->nbvs[j] == phase1->bvs[i]){
          tableau->pivotColumn = j;
          break;
        }
      }
      tableau->pivotLine = i;
      simplexStep(tableau);
    }
  }
}

void simplexPhaseII(struct Tableau *tableau){

  updatePivot(tableau);

  while(tableau->pivotColumn >= 0 && tableau->pivotLine >= 0){
    simplexStep(tableau);
    updatePivot(tableau);
  }
}
