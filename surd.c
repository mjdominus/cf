

#include <stdio.h>
#include "cf.h"
#include "cf_per.h"
#include "cf_arith.h"

int main(void) {
  CF sqrt2, sqrt3;
  CF sum;

  sqrt2 = new_periodic2(1, 0, 2, 0);
  sqrt3 = new_periodic2(1, 0, 1, 2, 0);
  sum = cadd(sqrt2, sqrt3);
  while (1) {
    unsigned i;
    int c;

    for (i=0; i < 20; i++) {
      int n;
      n = next(sum);
      if (n == C_INF) return;
      printf("%d%c", n, i==19 ? '\n' : ' ');
    }
    c = getchar();
    if (c == EOF) break;
  }
  return;
}
