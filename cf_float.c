

#include "cf_float.h"
#include <math.h>
#define EPSILON 1e-10


typedef struct st_cf_float {
  BASE_CF;
  double d;
} *CFF;

#define cff ((CFF)cf)

static int next_float(CF);

CF
new_float(double d) 
{
  CFF cf = (CFF) new_cf(sizeof(struct st_cf_float));
  if (cf == 0) return 0;
  cf->d = d;
  cf->DONE = 0;
  cf->next = next_float;
  return (CF)cf;
}

static int
next_float(CF cf)
{
  int p;

  if (cff->DONE) return INFINITY;

  p = (int)cff->d;
#if 0
  printf("\n   p=%d d=%lf diff=%.20f\n",
         p, cff->d, fabs(p - cff->d));
#endif
  if (fabs(p - cff->d) <= EPSILON) 
    cff->DONE = 1;
  else 
    cff->d = 1/(cff->d - p);
  
  return p;
}

