/*
 * test1.c - test routines
 *
 * $Id: test1.c,v 1.3 2002/05/02 08:49:09 tominaga Exp $
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

  /* test */

  c[0] = newcell(VAR);
  Cvar(c[0]) = 1;
  previ = c[0];
  for (i = 1; i < MAXCELLS; i++) {
    c[i] = newcell(ABST);
    Cbv(c[i]) = 1;
    Cbody(c[i]) = previ;
    previ = c[i];
  }
  printlexp(previ);

  return OK;
}
/* [EOF] */
