#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simplex.h"
#include "rational.h"

struct Tableau *createTestdata(void);

int main(void){
  struct Tableau *tableau = NULL, *phase1 = NULL;

  tableau = createTestdata();
  printf("aktualisiere Pivot-Element ...\n");
  updatePivot(tableau);
  printf("schreibe Tableau ...\n");
  printTableau(tableau);


  printf("Simplex Phase I ...\n");
  phase1 = simplexPhaseI(tableau);
  printTableau(phase1);
  printf("Lösung von Phase I:\n");
  printSolution(phase1);

  if((phase1->z)->n == 0){
    printf("Phase I hat erfolgreiche eine Startlösung gefunden.\n");
    printf("Phase II vorbereiten ...\n");
    prepareTableauForPhaseII(phase1, tableau);
    printf("Tableau für Phase II:\n");
    printTableau(tableau);

    printf("Simplex Phase II ...\n");
    simplexPhaseII(tableau);
    printTableau(tableau);
    printf("Lösung von Phase II: ");
    printSolution(tableau);
  }

  printf("gebe Tableau Speicher frei ...\n");
  freeTableau(phase1);
  freeTableau(tableau);

  return EXIT_SUCCESS;
}


struct Tableau *createTestdata(void){
  struct Tableau *tableau;

  printf("erzeuge Tableau ...\n");
  tableau = create_tableau(4,6);
  printf("fülle Tableau mit Testdaten...\n");

  int i, j;
  int t[5][7] = {
    {300,500,0,0,0,0,0},
    {1,2,1,0,0,0,170},
    {1,1,0,1,0,0,150},
    {0,3,0,0,1,0,180},
    {0,-1,0,0,0,1,-1}
  };
  int basis[4] = {2,3,4,5};
  int nbvs[2] = {0,1};

  for(i=0; i<6; i++){
    (tableau->c[i])->n = t[0][i];
  }

  (tableau->z)->n = 0;

  for(i=0; i<4; i++){
    (tableau->b[i])->n = t[i+1][6];
  }

  for(i=0; i<4; ++i){
    for(j=0; j<6; ++j){
      (tableau->A[i][j])->n = t[i+1][j];
    }
  }

  memcpy(tableau->bvs, basis, 4*sizeof(int));
  memcpy(tableau->nbvs, nbvs, 2*sizeof(int));

  return tableau;
}
