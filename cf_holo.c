

#include <math.h>
#include "cf_holo.h"

typedef struct st_cf_holo {
  BASE_CF;
  int a, b, c, d;
  CF x;
  char x_done;
} *CFH;

#define cfh ((CFH)cf)

static float bound(int, int);
static void egest(CFH, int);
static void ingest(CFH);
static int next_holo(CF);

CF
new_holo(int a, int b, int c, int d, CF x)
{
  CFH cf = (CFH) new_cf(sizeof(struct st_cf_holo));
  if (cf == 0) return 0;

  cf->a = a;
  cf->b = b;
  cf->c = c;
  cf->d = d;

  cf->x = x;
  cf->x_done = 0;

  cf->next = next_holo;

  return (CF)cf;
}


static int
next_holo(CF cf)
{
  int oops = 12;
  while (1) {
#ifdef DEBUG
    fprintf(stderr, "I'm now %d %d / %d %d\n", cf->a, cf->b, cf->c, cf->d);
#endif
    if (cfh->c == 0 && cfh->d == 0) 
        return INFINITY;
    else {
      float ub = bound(cfh->a, cfh->c), lb = bound(cfh->b, cfh->d);
      if ((int)ub == (int)lb) {
        egest(cfh, (int)ub);
        return (int)ub;
      } else {
        ingest(cfh);
      }
    }

    if (--oops < 1) {
      fprintf(stderr, "next_holo loop ran too many times\n");
      exit(1);
    }
  }
}

float
bound(int n, int d) 
{
  return d == 0 ? INFINITY : (float)n/(float)d;
}

void
egest(CFH cf, int q)
{
  int a = cf->a, b = cf->b;
  cf->a = cf->c;
  cf->b = cf->d;
  cf->c = a - q*cf->c;
  cf->d = b - q*cf->d;
#ifdef DEBUG
    fprintf(stderr, "Egested %d\n", q);
#endif
}

void
ingest(CFH cf)
{
  int p;

  if (cf->x_done) return;        /* err? */

  p = next(cf->x);
  if (p == INFINITY) {
#ifdef DEBUG
    fprintf(stderr, "Ingested oo\n");
#endif
    cf->x_done = 1;
    cf->b = cf->a;
    cf->d = cf->c;
  } else {
    int a = cf->a, c = cf->c;

#ifdef DEBUG
    fprintf(stderr, "Ingested %d\n", p);
#endif

    cf->a = a*p + cf->b;
    cf->b = a;
    cf->c = c*p + cf->d;
    cf->d = c;
  }
}

