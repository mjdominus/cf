

#include <stdlib.h>
#include <math.h>

#include "cf_arith.h"

typedef struct st_cf_arith {
  BASE_CF;
  int a, b, c, d;
  int e, f, g, h;
  CF x, y;
  char x_done, y_done;
} *CFA;
#define cfa ((CFA)cf)

static int next_arith(CF);
static float bound(int, int);
static void egest(CFA, int);
static void ingest_x(CFA);
static void ingest_y(CFA);

#define MAX(i, j) ((i) > (j) ? (i) : (j))
#define IDIFF(p, q) ((p) == INFINITY ? INFINITY : \
                     (q) == INFINITY ? INFINITY : \
                                       fabs((p)-(q)))

CF
new_arith(int a, int b, int c, int d, 
          int e, int f, int g, int h, 
          CF x, CF y)
{
  CFA cf = (CFA) new_cf(sizeof(struct st_cf_arith));
  if (cf == 0) return 0;

  cf->a = a;
  cf->b = b;
  cf->c = c;
  cf->d = d;
  cf->e = e;
  cf->f = f;
  cf->g = g;
  cf->h = h;

  cf->x = x;
  cf->x_done = 0;
  cf->y = y;
  cf->y_done = 0;

  cf->next = next_arith;

  return (CF)cf;
}

static int
next_arith(CF cf)
{
  int oops = 12;
  while (1) {
#ifdef DEBUG_COEFFS
    fprintf(stderr, "I'm now %d %d %d %d / %d %d %d %d\n", 
            cfa->a, cfa->b, cfa->c, cfa->d,
            cfa->e, cfa->f, cfa->g, cfa->h
            );
#endif
    if (cfa->e == 0 && cfa->f == 0 && cfa->g == 0 && cfa->h == 0) 
        return INFINITY;
    else {
      double
        b11 = bound(cfa->a, cfa->e), 
        b01 = bound(cfa->c, cfa->g),
        b10 = bound(cfa->b, cfa->f),
        b00 = bound(cfa->d, cfa->h);
      int i11 = b11, i10 = b10, i01 = b01, i00 = b00;

      if (i11 == i10 && i10 == i01 && i01 == i00) {
        egest(cfa, i11);
        return i11;
      } else {
        double
          xw = MAX(IDIFF(b11, b01), IDIFF(b10, b00)),
          yw = MAX(IDIFF(b11, b10), IDIFF(b01, b00));
#ifdef DEBUG_BOUNDS
        fprintf(stderr, "bounds: %f %f %f %f\n", b00, b01, b10, b11);
        fprintf(stderr, "x diffs: %f %f\n", IDIFF(b11, b01), IDIFF(b10, b00));
        fprintf(stderr, "y diffs: %f %f\n", IDIFF(b11, b10), IDIFF(b01, b00));
        fprintf(stderr, "x bound = %f, y bound = %f\n", xw, yw);
#endif
        
        if (xw > yw) {
          ingest_x(cfa);
        } else {
          ingest_y(cfa);
        }
      }
    }
  
    if (--oops < 1) {
      fprintf(stderr, "next_arith loop ran too many times\n");
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
egest(CFA cf, int q)
{
  int a = cf->a, b = cf->b, c = cf->c, d = cf->d;

  cf->a = cf->e;
  cf->b = cf->f;
  cf->c = cf->g;
  cf->d = cf->h;

  cf->e = a - q*cf->e;
  cf->f = b - q*cf->f;
  cf->g = c - q*cf->g;
  cf->h = d - q*cf->h;

#ifdef DEBUG_IO
    fprintf(stderr, "Egested %d\n", q);
#endif
}

void
ingest_x(CFA cf)
{
  int p;

  p = next(cf->x);
  if (p == INFINITY) {
#ifdef DEBUG_IO
    fprintf(stderr, "Ingested oo from x\n");
#endif
    cf->x_done = 1;
    cf->c = cf->a;
    cf->d = cf->b;
    cf->g = cf->e;
    cf->h = cf->f;
  } else {
    int a = cf->a, b = cf->b, e = cf->e, f = cf->f;

#ifdef DEBUG_IO
    fprintf(stderr, "Ingested %d from x\n", p);
#endif

    cf->a = a*p + cf->c;
    cf->b = b*p + cf->d;
    cf->c = a;
    cf->d = b;

    cf->e = e*p + cf->g;
    cf->f = f*p + cf->h;
    cf->g = e;
    cf->h = f;
  }
}


void
ingest_y(CFA cf)
{
  int p;

  p = next(cf->y);
  if (p == INFINITY) {
#ifdef DEBUG_IO
    fprintf(stderr, "Ingested oo from y\n");
#endif
    cf->y_done = 1;
    cf->b = cf->a;
    cf->d = cf->c;
    cf->f = cf->e;
    cf->h = cf->g;
  } else {
    int a = cf->a, c = cf->c, e = cf->e, g = cf->g;

#ifdef DEBUG_IO
    fprintf(stderr, "Ingested %d from y\n", p);
#endif

    cf->a = a*p + cf->b;
    cf->b = a;
    cf->c = c*p + cf->d;
    cf->d = c;

    cf->e = e*p + cf->f;
    cf->f = e;
    cf->g = g*p + cf->h;
    cf->h = g;
  }
}


CF
cadd(CF x, CF y) 
{
  return
    new_arith(0, 1, 1, 0, 
              0, 0, 0, 1,
              x, y);
}

CF
csub(CF x, CF y) 
{
  return
    new_arith(0, 1, -1, 0, 
              0, 0,  0, 1,
              x, y);
}

CF
cmul(CF x, CF y) 
{
  return
    new_arith(1, 0, 0, 0, 
              0, 0, 0, 1,
              x, y);
}

CF
cdiv(CF x, CF y) 
{
  return
    new_arith(0, 1, 0, 0, 
              0, 0, 1, 0,
              x, y);
}
