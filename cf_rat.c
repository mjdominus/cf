

#include "cf_rat.h"
#include <math.h>

typedef struct st_cf_rat {
  BASE_CF;
  int n, d;
} *CFR;
#define cfr ((CFR)cf)

static int next_rat(CF);

CF
new_rat(int n, int d) 
{
  CFR cf = (CFR) new_cf(sizeof(struct st_cf_rat));
  if (cf == 0) return 0;
  cf->n = n;
  cf->d = d;
  cf->next = next_rat;
  return (CF)cf;
}

CF
new_rat_from_float(double f) 
{
  int d = 1;
  double of = f;

  while (f != rint(f) && (int)(f*2) >= 0 && d < 2*d) {
    f *= 2;
    d <<= 1;
#ifdef DEBUG
    printf("%f => %f / %d\n", of, f, d);
#endif
  }
#ifdef DEBUG
  printf("%f => %d / %d\n", of, (int)(f+0.5), d);
#endif
  return new_rat((int)(f+0.5), d);
}

static int
next_rat(CF cf)
{
  int p, den = cfr->d;

  if (den == 0) return INFINITY;

  p = cfr->n / den;            /* integer division */
  cfr->d = cfr->n - p * den;
  cfr->n = den;
  return p;
}

