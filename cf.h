
#ifndef CF_H
#define CF_H

#define BASE_CF   int (*next)(struct st_cf *); char DONE

typedef struct st_cf {
  BASE_CF;
} *CF;

#include <limits.h>
#define C_INF INT_MAX
#define TOOBIG 65535

#ifdef DEBUG
#define DEBUG_BOUNDS
#define DEBUG_COEFFS
#define DEBUG_IO
#endif

#include <stdio.h>
int next(CF);
void print_cf(CF);
void fprint_cf(FILE *, CF);

#endif
