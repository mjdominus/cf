
#include "cf.h"
#include "cf_append.h"
#include "cf_per.h"

static int next_append(CF);

typedef struct st_cf_append {
  BASE_CF;
  unsigned pos;
  unsigned n_cfs;
  CF cf[1];			/* one or more */
} *CFP;
#define cfp ((CFP)cf)

static CFP new_cf_append(unsigned n_cfs)
{
  CFP result = (CFP) new_cf(sizeof(struct st_cf_append) 
			    + (n_cfs-1) * sizeof(CF));  
  if (result) {
    result->pos = 0;
    result->n_cfs = n_cfs;
  }
  return result;
}

CF append1(CF a, int t)
{
  CFP result = new_cf_append(2);
  if (result == 0) return 0;

  result->cf[0] = a;
  result->cf[1] = new_array(t);
  result->next = next_append;
  return (CF) result;
}

CF_appendcf(CF a0, ...)
{
  unsigned n;
  va_list va;

  va_start(va, a0);
  n = 1 + count_items(&va);
  va_end(va);

  
}

unsigned
count_items(va_list *ap)
{
  unsigned count = 0;
  va_list ap_save;

  va_copy(ap_save, *ap);

  while (va_arg(ap_save, int) != 0)  count++;
  return count;
}

static int next_append(CF cf) 
{
  int i = C_INF;

  while (cfp->pos < cfp->n_cfs) {
    i = next(cfp->cf[cfp->pos]);
    if (i == C_INF) cfp->pos++;
    else break;
  }

  return i;
}
