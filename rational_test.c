#include <stdio.h>
#include <stdlib.h>

#include "rational.h"

int main(void){
  struct Rational *r, *s, *t;
  char **string;

  r = createRational();
  r->n = 12;
  r->d = 18;

  normalize(r);

  printf("r=");
  printRational(r);
  printf("\n");

  s = getRational(1, 5);

  string = toString(s);
  printf("s=%8s\n", *string);
  free(*string);
  free(string);

  printf("r * s = ");
  t = multiply(r, s);
  printRational(t);
  printf("\n");
  free(t);

  printf("r / s = ");
  t = divide(r, s);
  printRational(t);
  printf("\n");
  free(t);

  printf("r + s = ");
  t = add(r, s);
  printRational(t);
  printf("\n");
  free(t);

  printf("r - s = ");
  t = subtract(r, s);
  printRational(t);
  printf("\n");
  free(t);

  printf("r < s? %d\n", is_a_smaller_than_b(r, s));
  printf("s < r? %d\n", is_a_smaller_than_b(s, r));

  printf("ggT(18, 3)=%d\n", ggT(18, 3));
  printf("ggT(5, 15)=%d\n", ggT(5, 15));
  printf("ggT(21, 13)=%d\n", ggT(21, 13));

  free(r);
  free(s);

  return EXIT_SUCCESS;
}
