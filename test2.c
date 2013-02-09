/*
 * test2.c - abst / apply
 *
 * $Id: test2.c,v 1.4 2002/05/10 04:02:06 tominaga Exp tominaga $
 */

#include "lambda.h"

int
main() {
  Cellidx c1, c2, c3, c4, c5;

  initpool();	/* allocate and initialize the pool */

  c1 = Lnewvar(1L);
  c2 = Labst(1L, c1);

  c3 = Lnewvar(2L);
  c4 = Labst(2L, c3);

  c5 = Lappl(c2, c4);

  printlexp(c5);

  return OK;
}
/* [EOF] */
