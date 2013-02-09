/*
 * ilambda.c - user (library) interface
 *
 * $Id: ilambda.c,v 1.4 2002/05/10 04:02:06 tominaga Exp tominaga $
 */

#include "lambda.h"

static enum facility debugfacility = F_INTERFACE;

Lexp
Lnewvar(Var v) {
  Cellidx c = newcell(VAR);
  Cvar(c) = v;
  return c;
}

Lexp
Labst(Var bv, Lexp body) {
  Cellidx c = newcell(ABST);
  Cbv(c) = bv;
  Cbody(c) = body;
  return c;
}

Lexp
Lappl(Lexp left, Lexp right) {
  Cellidx c = newcell(APPL);
  Cleft(c) = left;
  Cright(c) = right;
  return c;
}

Lexp
Lcopy(Lexp orig) {
  return deepcopy(orig);
}

void
Lfree(Lexp l) {
  prunecell(l);
}

Lexp
Lstr2Lexp(char *s) {
  return str2lexp(s);
}

int
LLexp2str(Lexp l, char *buf, int len) {
  Cellidx ci;

  ci = l;
  return lexp2str(ci, buf, len);
}

void
Lcanon(Lexp l) {
  /* XXX */
  fatal("Lcanon not implemented yet\n");

  canonbvars(l);
}

int
Leq(Lexp l1, Lexp l2) {
  return isequalLexp(l1, l2);
}

/* [EOF] */
