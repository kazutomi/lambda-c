/*
 * test16.c - printfreelist test
 *
 * $Id: test16.c,v 1.1 2002/05/18 16:37:31 tominaga Exp $
 */

#include "lambda.h"

enum {
  MAXCELLS = 100,
};

int
main() {
  Cellidx c[MAXCELLS];
  Cellidx i, previ;

  initpool();	/* allocate and initialize the pool */
  printfreelist();

  /* test */

  c[0] = newcell(VAR);
  Cvar(c[0]) = 1;
  previ = c[0];
  for (i = 1; i < MAXCELLS; i++) {
    c[i] = newcell(ABST);
    Cbv(c[i]) = 1;
    Cbody(c[i]) = previ;
    previ = c[i];
    printfreelist();
  }
  printlexp(previ);
  printfreelist();

  return OK;
}
/* [EOF] */
