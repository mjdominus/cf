

#include <math.h>
#include "cf_holo.h"

typedef struct st_cf_holo {
  BASE_CF;
  int a, b, c, d;
  CF x;
  char x_done;
  void (*ingest)(struct st_cf_holo *);
  void (*egest)(struct st_cf_holo *, int);
} *CFH;

#define cfh ((CFH)cf)

static float bound(int, int);
static void egest(CFH, int);
static void egest_digit(CFH, int);
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
  cf->ingest = ingest;
  cf->egest = egest;

  return (CF)cf;
}

CF
new_dec(CF x)
{
  CF cf = new_holo(1, 0, 0, 1, x);
  if (cf == 0) return 0;

  cfh->egest = egest_digit;

  return (CF)cf;
}

static int
next_holo(CF cf)
{
  int oops = 12;
  while (1) {
#ifdef DEBUG
    fprintf(stderr, "I'm now %d %d / %d %d\n", cfh->a, cfh->b, cfh->c, cfh->d);
#endif
    if (cfh->c == 0 && cfh->d == 0) 
        return INFINITY;
    else {
      float ub = bound(cfh->a, cfh->c), lb = bound(cfh->b, cfh->d);
      if ((int)ub == (int)lb) {
        cfh->egest(cfh, (int)ub);
        return (int)ub;
      } else {
        cfh->ingest(cfh);
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

static void
egest(CFH cf, int q)
{
  int a = cf->a, b = cf->b;
  cf->a = cf->c;
  cf->b = cf->d;
  cf->c = a - q*cf->c;
  cf->d = b - q*cf->d;
#ifdef DEBUG_IO
    fprintf(stderr, "Egested %d\n", q);
#endif
}

static void
egest_digit(CFH cf, int t)
{
  cf->a = 10*(cf->a - cf->c * t);
  cf->b = 10*(cf->b - cf->d * t);
#ifdef DEBUG_IO
    fprintf(stderr, "Egested %d\n", t);
#endif
}

static void
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

