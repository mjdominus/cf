#include <stdarg.h>
#include <stdlib.h>
#include "cf_per.h"

struct st_iarray {
  unsigned len;
  int a[1];                     /* one or more */
};

static int next_periodic(CF);
static void count_terms(va_list *, unsigned, struct st_iarray **);

typedef struct st_cf_per {
  BASE_CF;
  struct st_iarray *iterms;	/* initial terms */
  struct st_iarray *terms;	/* repeated terms */
  unsigned pos;
} *CFP;
#define cfp ((CFP)cf)

CF
new_array(int a0,...)
{
  va_list ap;
  struct st_iarray *inf = malloc(sizeof(struct st_iarray)), *a;
  CFP cf = (CFP) new_cf(sizeof(struct st_cf_per));

  if (inf == 0 || cf == 0) return 0;

  va_start(ap, a0);
  count_terms(&ap, 1, &a);
  va_end(ap);
  a->a[0] = a0;

  cf->iterms = a;

  /* after the initial terms, produce infinity */
  cf->terms = inf;
  cf->terms->len = 1;
  cf->terms->a[0] = C_INF;

  cf->pos = 0;
  cf->next = next_periodic;
  return (CF)cf;
}

CF
new_periodic(int a0,...)
{
  va_list ap;
  struct st_iarray *ia = malloc(sizeof(struct st_iarray)), *a;
  CFP cf = (CFP) new_cf(sizeof(struct st_cf_per));

  if (ia == 0 || cf == 0) return 0;

  va_start(ap, a0);
  count_terms(&ap, 1, &a);
  va_end(ap);
  a->a[0] = a0;

  ia->len = 0;
  cf->iterms = ia;
  cf->terms = a;
  cf->pos = 0;
  cf->next = next_periodic;
  return (CF)cf;
}

CF
new_periodic2(int a0,...)
{
  va_list ap;
  struct st_iarray *ia, *a;
  CFP cf = (CFP) new_cf(sizeof(struct st_cf_per));

  if (cf == 0) return 0;
  va_start(ap, a0);
  count_terms(&ap, 1, &ia);
  count_terms(&ap, 0, &a);
  va_end(ap);
  ia->a[0] = a0;

  cf->iterms = ia;
  cf->terms = a;
  cf->pos = 0;
  cf->next = next_periodic;
  return (CF)cf;
}

static void
count_terms(va_list *ap, unsigned extra, struct st_iarray **ia)
{
  unsigned count = 0, i;
  va_list ap_save;
  int nelts;

  va_copy(ap_save, *ap);

  while (va_arg(ap_save, int) != 0)  count++;
  nelts = count+extra-1;
  if (nelts < 0) nelts = 0;
#ifdef DEBUG_VARARGS
  fprintf(stderr, "count_terms counts %u plus %u extra - 1 = %d\n", 
          count, extra, nelts);
#endif

  *ia = malloc(sizeof(struct st_iarray) + nelts*sizeof(int));
  if (*ia == 0) return;
  (*ia)->len = count+extra;

  count = extra;
  while ((i = va_arg(*ap, int)) != 0) {
    (*ia)->a[count++] = i;
#ifdef DEBUG_VARARGS
  fprintf(stderr, "* copied %d\n", i);
#endif
  }
}


static int
next_periodic(CF cf)
{
  unsigned n = cfp->pos++;
  if (cfp->pos == cfp->terms->len + cfp->iterms->len) 
    cfp->pos -= cfp->terms->len;
  if (n >= cfp->iterms->len)
    return cfp->terms->a[n - cfp->iterms->len];
  else
    return cfp->iterms->a[n];
}
