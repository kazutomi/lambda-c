/*
 * lambops.c - lambda expression operations (interface)
 *
 * $Id: lambops.c,v 1.10 2002/05/10 04:02:06 tominaga Exp tominaga $
 */

#include <assert.h>
#include "lambda.h"

static enum facility debugfacility = F_LAMBOPS;

/*
 * alpha - substitution M[x:=N]
 *
 * substitute all occurrences of M's free variable x with N
 */
Lexp
alpha(Lexp m, Var x, Lexp n) {
  Cellidx ci;

  ci = m;	/* top cell */
  switch (Ctype(ci)) {
    case VAR: {
      if (Cvar(ci) == x) {
	freecell(ci);
	return deepcopy(n);
      } else
	return ci;
    }
    case APPL: {
      Cellidx cleft, cright;

      cleft = alpha(Cleft(ci), x, n);
      cright = alpha(Cright(ci), x, n);
      Cleft(ci) = cleft;
      Cright(ci) = cright;
      return ci;
    }
    case ABST: {
      Cellidx body;

      if (Cbv(ci) == x)
	return ci;
      else {
	body = alpha(Cbody(ci), x, n);
	Cbody(ci) = body;
	return ci;
      }
    }
    default:
      msg_warning("alpha: unknown cell type %s\n", typename[Ctype(ci)]);
      return ci;
  }
}

/* findmaxvar - finds max var in Lexp */
Var
findmaxvar(Lexp l) {
  switch (Ctype(l)) {
    case VAR:
      return Cvar(l);
    case APPL: {
      Var maxleft, maxright;

      maxleft = findmaxvar(Cleft(l));
      maxright = findmaxvar(Cright(l));
      return max(maxleft, maxright);
    }
    case ABST: {
      Var maxbody;

      maxbody = findmaxvar(Cbody(l));
      return max(Cbv(l), maxbody);
    }
    default:
      fatal("findmaxvar: unknown cell type %d\n", Ctype(l));
      /*NOTREACHED*/
      return -1;	/* dummy for cc -Wall */
  }
}

/*
 * subst - substitute x in m with n, avoiding binding free variables
 *
 * maxvar is maximum Var used, i.e., (maxvar + 1) is a new variable
 *
 * call with maxvar = -1 at the top level
 */
Lexp
subst(Lexp m, Var x, Lexp n, Var maxvar) {
  Lexp lret;
  Var maxinm, maxinn;

  if (maxvar < 0) {
    /* toplevel */
    maxinm = findmaxvar(m);
    maxinn = findmaxvar(n);
    maxvar = max(max(maxinm, maxinn), x);
  }

  switch (Ctype(m)) {
    case VAR: {
	if (Cvar(m) == x) {
	  freecell(m);
	  lret = deepcopy(n);
	} else
	  lret = m;
      }
      break;
    case APPL: {
	Cellidx cleft, cright;

	cleft = subst(Cleft(m), x, n, maxvar);
	cright = subst(Cright(m), x, n, maxvar);
	Cleft(m) = cleft;
	Cright(m) = cright;
	lret = m;
      }
      break;
    case ABST: {
	Var bv, newbv;
	Cellidx body, newbody, tmpv;

	/* create new binding variable */
	bv = Cbv(m);
	maxvar = max(maxvar, bv);
	newbv = maxvar + 1;
	maxvar = newbv;

	/* replace old bv with newbv to avoid collision */
	tmpv = newcell(VAR);
	Cvar(tmpv) = newbv;

	body = alpha(Cbody(m), bv, tmpv);
	freecell(tmpv);

	/* go further */
	newbody = subst(body, x, n, maxvar);
	Cbv(m) = newbv;
	Cbody(m) = newbody;
	lret = m;
      }
      break;
    default:
      msg_warning("subst: unknown cell type %s\n", typename[Ctype(m)]);
      lret = m;
      break;
  }
  return lret;
}

/*
 * betaat - beta reduction at specified redex
 * Returns 1 if reduced, 0 if not.
 */
int
betaat(Cellidx redex) {
  Cellidx left, right, newci;
  Var bv;

  /* reducible? */
  if (Ctype(redex) != APPL) {
    return 0;
  }
  left = Cleft(redex);
  right = Cright(redex);
  if (Ctype(left) != ABST) {
    return 0;
  }

  /* reduction */
  bv = Cbv(left);
  newci = subst(Cbody(left), bv, right, -1);

  /*
   * Link the result to parent, and free cells which are no longer necessary.
   *
   * XXX: Because we do not know which is the parent,
   *      we overwrite redex with the result.
   */
  copycell(newci, redex);
  freecell(newci);
  freecell(left);
  prunecell(right);
  
  return 1;
}

/*
 * canon_findredex - find a redex for canonical reduction
 *
 * returns Cellidx when found, -1 if not found
 */
Cellidx
canon_findredex(Cellidx c) {
  assert(Ctype(c)==VAR||Ctype(c)==ABST||Ctype(c)==APPL);

  switch (Ctype(c)) {
    case VAR:
      return -1;
    case ABST:
      return canon_findredex(Cbody(c));
    case APPL: {
	Cellidx left, right, redex;

	left = Cleft(c);
	right = Cright(c);

	if (Ctype(left) == ABST) {
	  /* found */
	  return c;
	}

	/* search in leftmost manner (and outermost = topdown recursive) */
	if ((redex = canon_findredex(left)) >= 0) {
	  /* found in left */
	  return redex;
	}
	/* not found; try right */
	return canon_findredex(right);
      }
    default:
      /* assert prevents control coming here */
      /*NOTREACHED*/
      return -1;	/* dummy for cc -Wall */
  }
}

/*
 * findredex - find a redex according to the specified strategy
 *
 * returns Cellidx when found, -1 if not found (i.e., l is already canonical)
 */
Cellidx
findredex(Lexp l, int strategy) {
  static int warned = 0;	/* to warn once */

  switch (strategy) {
    case INNERMOST:
      if (!warned) {
	msg_warning("findredex: strategy INNERMOST not yet implemented; using CANONICAL hereafter\n");
	warned = 1;
      }
      /* FALLTHROUGH */
    case CANONICAL:
      return canon_findredex(l);
    default:
      fatal("findredex: unknown strategy %d\n", strategy);
      /*NOTREACHED*/
      return -1;	/* dummy for cc -Wall */
  }
}

/*
 * betastep - one step beta reduction
 *
 * reduce Lexp l with strategy, and overwrite l with the result.
 * return value: 1 = reduced (but possibly same as l), 0 = no redex
 */
int
betastep(Lexp l, int strategy) {
  Cellidx redex;
  int reduced;

  if (deblev(L_DEBUG)) {
    msg_debug("betastep: on ");
    eprintlexp(l);
  }
  if ((redex = findredex(l, strategy)) < 0) {
    /* no redex */
    return 0;
  }
  reduced = betaat(redex);

  assert(reduced);	/* must be reduced because findredex must have found a redex */

  /* now 'redex' points reduced expression */
  /* l points 'redex' so nothing has to be done to link them */
  return 1;
}

/*
 * nbeta - do beta reductions
 *
 * times specifies max #times of beta reductions, 0 = as many times as possible.
 * returns #times done.
 */
int
nbeta(Lexp l, int strategy, int times) {
  int i;

  if (times > 0) {
    for (i = 0; i < times; i++)
      if (!betastep(l, strategy))
	break;
  } else {
    for (i = 0; ; i++)
      if (!betastep(l, strategy))
	break;
  }
  if (deblev(L_DEBUG)) {
    msg_debug("nbeta: %d steps, returns ", i);
    eprintlexp(l);
  }
  return i;
}

/*
 * countcells - count #cells that the lexp possesses
 */

static int ncells;

static int
countcells_handler(Cellidx ci, int descending) {
  if (!descending)
    return 1;	/* ignore; carry on */
  ncells++;
  return 1;	/* go deeper */
}

int
countcells(Lexp l) {
  ncells = 0;
  dfsLexp(l, countcells_handler);
  return ncells;
}

/* EOF */
