#ifndef RATIONAL_H
#define RATIONAL_H RATIONAL_H

#define BUFFER 40

struct Rational {
  int n;
  int d;
};

struct Rational *createRational();
struct Rational *getRational(int nominator, int denominator);
struct Rational *cloneRational(struct Rational *a);

void normalize(struct Rational *r);

struct Rational *multiply(struct Rational *a, struct Rational *b);
struct Rational *divide(struct Rational *a, struct Rational *b);
struct Rational *add(struct Rational *a, struct Rational *b);
struct Rational *subtract(struct Rational *a, struct Rational *b);
struct Rational *invertSign(struct Rational *a);

int is_a_smaller_than_b(struct Rational *a, struct Rational *b);

void printRational(struct Rational *a);
char **toString(struct Rational *a);

int ggT(int a, int b);

#endif
