

#include "cf_it.h"

typedef struct st_cf_it {
  BASE_CF;
  unsigned n;
  int (*fun)(unsigned);
} *CFI;
#define cfi ((CFI)cf)

static int next_it(CF);

CF
new_it(int (*f)(unsigned)) 
{
  CFI cf = (CFI) new_cf(sizeof(struct st_cf_it));
  if (cf == 0) return 0;

  cf->n = 0;
  cf->fun = f;
  cf->next = next_it;

  return (CF)cf;
}

static int
next_it(CF cf)
{
  return cfi->fun(cfi->n++);
}

static int
ef(unsigned n)
{
  switch (n%3) {
  case 0:
  case 2:
    return 1;
  case 1:
    return 2*(n/3);
  }
}

CF
e(void)
{
  return new_it(ef);
}

