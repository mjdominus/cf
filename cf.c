
#include <stdio.h>
#include <stdlib.h>
#include "cf.h"

static int stub(CF);

CF
new_cf(size_t s)
{
  CF cf = malloc(s);
  if (cf == 0) return 0;
  cf->DONE = 0;
  cf->next = stub;
  return cf;
}

static int
stub(CF cf)
{
  fprintf(stderr, "cf: no behavior defined\n");
  exit(1);
}

int
next(CF cf) {
  int t;

  if (cf->DONE) return C_INF;

  t = (cf->next)(cf);

  if (t > TOOBIG) {
    cf->DONE = 1;
    t = C_INF;
  }

  return t;
}

void
print_cf(CF cf) 
{
  fprint_cf(stdout, cf);
}

#define STOP 30

void
fprint_cf(FILE *f, CF cf) 
{
  int t;
  unsigned stop = STOP;
  while ((t = next(cf)) != C_INF) {
    fprintf(f, "%d ", t);
    if (--stop == 0) break;
  }
  fputs("\n", f);
}
