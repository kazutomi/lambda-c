/*
 * diff.c - calculate difference between two lambda expressions
 *
 * $Id: diff.c,v 1.4 2002/12/09 01:37:08 tominaga Exp $
 */

#include "lambda.h"

static enum facility debugfacility = F_MISC;

static Var boundvars[MAXABSTDEPTH];

/*
 * numnodes - count number of nodes in specified subtree
 */
static int
numnodes(Cellidx ci) {
  return countcells(ci);
}

/*
 * calcbdist - calculate binding distance for each variable
 */
void
calcbdist_r(Cellidx ci, int depth) {
  int i;

  switch (Ctype(ci)) {
    case VAR:
      /* search the binding lambda */
      for (i = depth - 1; i >= 0; i--)
	if (Cvar(ci) == boundvars[i])
	  break;
      if (i < 0)
	/* free variable */
	Cbdist(ci) = -(depth + 1);	/* +1 to reserve 0 */
      else {
	/* bound variable */
	Cbdist(ci) = depth - i;
      }
      return;
    case ABST:
      if (depth < MAXABSTDEPTH) {
	boundvars[depth] = Cbv(ci);
	calcbdist_r(Cbody(ci), depth + 1);
      } else {
	msg_warning("calcbdist: MAXABSTDEPTH reached; ignoring the subtree\n");
      }
      return;
    case APPL:
      calcbdist_r(Cleft(ci), depth);
      calcbdist_r(Cright(ci), depth);
      return;
    default:
      fatal("calcbdist: invalid cell type %d\n", Ctype(ci));
      /*NOTREACHED*/
  }
}

void
calcbdist(Lexp l) {
  calcbdist_r(l, 0);
}

/*
 * diff - returns difference between two lexps
 */

static int
diff_r(Cellidx c1, Cellidx c2) {
  int i;
  int w1, w2, dif;
  int a1[MAXTREEHEIGHT], a2[MAXTREEHEIGHT];
  int lev1, lev2, bot;

  if (Ctype(c1) == VAR && Ctype(c2) == VAR) {
    w1 = Cbdist(c1);
    w2 = Cbdist(c2);
    if (w1 > 0 && w2 > 0) {
      /* both bound; return difference */
      return DIST(w1, w2);
    } else if (w1 > 0) {
      /* c1 is bound, c2 is free */
      return w1 - w2;
    } else if (w2 > 0) {
      /* c2 is bound, c1 is free */
      return w2 - w1;
    } else {
      /* both free */
      return DIST(Cvar(c1), Cvar(c2));
    }
  } else if (Ctype(c1) == VAR && Ctype(c2) == ABST) {
    return numnodes(c2);
  } else if (Ctype(c1) == ABST && Ctype(c2) == VAR) {
    return numnodes(c1);
  } else if (Ctype(c1) == VAR && Ctype(c2) == APPL) {
    return numnodes(c2);
  } else if (Ctype(c1) == APPL && Ctype(c2) == VAR) {
    return numnodes(c1);
  } else if (Ctype(c1) == ABST && Ctype(c2) == ABST) {
    return diff_r(Cbody(c1), Cbody(c2));
  } else if ((Ctype(c1) == ABST && Ctype(c2) == APPL) ||
             (Ctype(c1) == APPL && Ctype(c2) == ABST)) {
    lev1 = arraynodes(c1, a1, MAXTREEHEIGHT);
    lev2 = arraynodes(c2, a2, MAXTREEHEIGHT);
    bot = max(lev1, lev2);
    dif = 0;
    for (i = 0; i < bot; i++)
      dif += 2 + DIST(a1[i], a2[i]);
    return dif;
  } else if (Ctype(c1) == APPL && Ctype(c2) == APPL) {
    return diff_r(Cleft(c1), Cleft(c2)) + diff_r(Cright(c1), Cright(c2));
  } else {
    fatal("diff_r: unexpected cell type %d and %d\n", Ctype(c1), Ctype(c2));
  }
  fatal("diff_r: cannot come here\n");
  /*NOTREACHED*/
  return 0;
}

int
diff(Lexp l1, Lexp l2) {
  calcbdist(l1);
  calcbdist(l2);
  return diff_r(l1, l2);
}

/*
 * arraynodes - make array of the number of nodes at each level.
 *              returns deepest level reached.
 */

static int
arraynodes_r(Cellidx ci, int curlev, int a[], int asize) {
  int lev1, lev2;

  if (curlev >= asize) {
    msg_warning("arraynodes_r: array size (%d) reached; ignoring the subtree\n", asize);
    return asize;
  }
  a[curlev]++;
  curlev++;
  switch (Ctype(ci)) {
    case VAR:
      return curlev;
    case ABST:
      return arraynodes_r(Cbody(ci), curlev, a, asize);
    case APPL:
      lev1 = arraynodes_r(Cleft(ci), curlev, a, asize);
      lev2 = arraynodes_r(Cright(ci), curlev, a, asize);
      return max(lev1, lev2);
    default:
      fatal("arraynodes_r: unexpected cell type %d\n", Ctype(ci));
  }
  /*NOTREACHED*/
  return curlev;
}

int
arraynodes(Cellidx ci, int a[], int asize) {
  int i;

  for (i = 0; i < asize; i++)
    a[i] = 0;

  return arraynodes_r(ci, 0, a, asize);
}

/* EOF */
