#include "rational.h"
#include <stdlib.h>
#include <stdio.h>

struct Rational *createRational(){
  return getRational(0, 1);
}

struct Rational *getRational(int nominator, int denominator){
  struct Rational *r = NULL;

  r = (struct Rational *)malloc(sizeof(struct Rational));
  if(r == NULL){
    fprintf(stderr, "Memory error in getRational() of rational. Emergeny stop program!\n");
    exit(EXIT_FAILURE);
  }

  r->n = nominator;
  r->d = denominator;

  return r;
}


void normalize(struct Rational *r){
  int div = ggT(r->n , r->d);

  r->n = (r->n)/div;
  r->d = (r->d)/div;

  if(r->d < 0){
    r->n = -(r->n);
    r->d = -(r->d);
  }
}


struct Rational *multiply(struct Rational *a, struct Rational *b){
  int n, d;
  struct Rational *r;

  n = (a->n) * (b->n);
  d = (a->d) * (b->d);

  r = getRational(n, d);
  normalize(r);

  return r;
}

struct Rational *divide(struct Rational *a, struct Rational *b){
  int n, d;
  struct Rational *r = NULL;

  n = (a->n) * (b->d);
  d = (a->d) * (b->n);

  r = getRational(n, d);
  normalize(r);

  return r;
}

struct Rational *add(struct Rational *a, struct Rational *b){
  int n, d;
  struct Rational *r;

  n = (a->n) * (b->d) + (b->n) * (a->d);
  d = (a->d) * (b->d);

  r = getRational(n, d);
  normalize(r);

  return r;
}


struct Rational *subtract(struct Rational *a, struct Rational *b){
  int n, d;
  struct Rational *r;

  n = (a->n) * (b->d) - (b->n) * (a->d);
  d = (a->d) * (b->d);

  r = getRational(n, d);
  normalize(r);

  return r;
}

void printRational(struct Rational *a){
  if(a->d == 1){
    printf("%d", a->n);
  }else{
    printf("%d/%d", a->n, a->d);
  }
}

char **toString(struct Rational *a){
  char **string = NULL;
  int n;

  string = (char **)malloc(sizeof(char *));
  *string = (char *)malloc(BUFFER * sizeof(char));
  if(string == NULL){
    fprintf(stderr, "Not enough memory!\n");
    exit(EXIT_FAILURE);
  }

  if(a->d == 1){
    n = sprintf(*string, "%d", a->n);
  }else{
    n = sprintf(*string, "%d/%d", a->n, a->d);
  }

  if(n>(BUFFER - 1)){
    fprintf(stderr, "Memory error in toString() method of rational. Emergeny stop program!\n");
    exit(EXIT_FAILURE);
  }

  *string = (char *)realloc(*string, (n+1) * sizeof(char));

  return string;
}


int ggT(int a, int b){
  int tmp;

  a = (a<0)?-a:a;
  b = (b<0)?-b:b;

  do{
    if(b > a){
      tmp = a;
      a = b;
      b = tmp;
    }
    a -= b;
  }while(b > 0);

  return a;
}

int is_a_smaller_than_b(struct Rational *a, struct Rational *b){
  int va, vb;

  va = (a->n) * (b->d);
  vb = (b->n) * (a->d);

  if(va < vb){
    return 1;
  }else{
    return 0;
  }
}

struct Rational *invertSign(struct Rational *a){
  return getRational(-a->n, a->d);
}

struct Rational *cloneRational(struct Rational *a){
  return getRational(a->n, a->d);
}
