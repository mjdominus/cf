
#include "cf_rat.h"
#include "cf_float.h"
#include "cf_holo.h"
#include "cf_arith.h"
#include <stdio.h>
#include <stdlib.h>

int 
main(int argc, char **argv) 
{
  int n, d;
  double f;
  CF cf;

  if (argc == 3) {
    n = atoi(argv[1]);
    d = atoi(argv[2]);
    cf = new_rat(n, d);
  } else if (argc == 2) {
    f = atof(argv[1]);
    cf = new_rat_from_float(f);
  } else if (argc == 6) {
    CF cfx;
    f = atof(argv[5]);
    cfx = new_rat_from_float(f);
    cf = new_holo(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), cfx);
  } else if (argc == 4) {
    CF
      cfx = new_rat_from_float(atof(argv[1])),
      cfy = new_rat_from_float(atof(argv[3]));
    
    switch (argv[2][0]) {
    case '+': cf = cadd(cfx, cfy); break;
    case '-': cf = csub(cfx, cfy); break;
    case '*':
    case 'x': cf = cmul(cfx, cfy); break;
    case '/': cf = cdiv(cfx, cfy); break;
    default: 
      fprintf(stderr, "Unknown operator '%c'; use + - x /\n", argv[2][0]);
      return 1;
    }
  } else if (argc == 11) {
    CF
      cfx = new_rat_from_float(atof(argv[9])),
      cfy = new_rat_from_float(atof(argv[10]));
    cf = new_arith(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]),
                   atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]),
                   cfx, cfy);
  } else {
    fprintf(stderr, "Usage:\t%1$s num den\n\t%1$s real\n\t%1$s a b c d real\n\t%1$s a b c d e f g h real real\n\t%1$s real +-x/ real\n", argv[0]);
    return 1;
  }

  print_cf(cf);
  return 0;
}
