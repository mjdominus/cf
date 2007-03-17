

#include <stdio.h>
#include "cf.h"
#include "cf_per.h"
#include "cf_arith.h"

int main(void) {
  CF phi, sqrt2p1;
  CF sum;

  phi= new_periodic(1, 0);
  sqrt2p1 = new_periodic(2, 0);
  sum = cadd(sqrt2p1, phi);
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
