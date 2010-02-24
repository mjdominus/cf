
#include "cf.h"
#include "cf_between.h"
#include "cf_rat.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void usage(char *argv0) {
  fprintf(stderr, "Usage: %s a/b c/d\n\tFind simplest fraction between a/b and c/d\n", argv0);
  exit(0);
}

unsigned parserat(char *s, int *n, int *d) {
  *n = atoi(s);

  while (*s != '\0' && *s != '/') s++;

  if (*s == '\0') {
    *d = 1;
    return 1;
  } 

  s++;
  *d = atoi(s);

  return 1;
}

int main(int argc, char **argv) {
  char *prog = argv[0];
  char **arg = argv+1;
  int n, d;
  CF a[2], r;

  if (argc < 2) usage(prog);

  {
    unsigned i;
    for (i=0; i<2; i++) 
      if (parserat(arg[i], &n, &d)) 
	a[i] = new_rat(n, d);
      else 
	usage(prog);
  }

  r = between(a[0], a[1]);
  print_cf(r);

  return 0;
}

