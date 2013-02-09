/*
 * pool.c - manages the cell pool
 *
 * $Id: pool.c,v 1.11 2002/05/14 17:26:11 tominaga Exp tominaga $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lambda.h"

static enum facility debugfacility = F_POOL;
static unsigned long int count_allocated = 0, count_freed = 0;
static Cellidx freehead;	/* head of the free list */

static int isequalLexp_r(Cellidx, VarList *, Cellidx, VarList *);

void *
emalloc(size_t size) {
  void *p;

  if ((p = malloc(size)) == NULL)
    fatal("emalloc: cannot allocate %d bytes\n", size);
  return p;
}

void
fprintfreelist(FILE *fp) {
  Cellidx i;

  fprintf(fp, "Free list: ");
  for (i = freehead; i >= 0; i = pool[i].nextfree)
    fprintf(fp, "%ld ", i);
  fprintf(fp, "\n");
}

void
printfreelist() {
  fprintfreelist(stdout);
}

void
eprintfreelist() {
  fprintfreelist(stderr);
}

void
fprintcell(FILE *fp, Cellidx c) {
  fprintf(fp, "[cell#%ld]", c);
  if (poolsize <= c) {
    fprintf(fp, " out of range (poolsize %ld)\n", poolsize);
  }
  if (Ctype(c) <= NOTYPE) {
    /* nb. NOTYPE is curious but it has typename entry */
    fprintf(fp, " type = %d (%s)\n", Ctype(c), typename[Ctype(c)]);
  } else {
    fprintf(fp, " unknown type %d\n", Ctype(c));
  } 
  switch (Ctype(c)) {
    case FREE:
      break;
    case VAR:
      fprintf(fp, " var = %ld\n", Cvar(c));
      break;
    case ABST:
      fprintf(fp, " bv = %ld, body = %ld\n", Cbv(c), Cbody(c));
      break;
    case APPL:
      fprintf(fp, " left = %ld, right = %ld\n", Cleft(c), Cright(c));
      break;
    case NOTYPE:
      break;
  }
}

void
printcell(Cellidx c) {
  fprintcell(stdout, c);
}

void
eprintcell(Cellidx c) {
  fprintcell(stderr, c);
}

void
initpool() {
  Cellidx i;

  /* canonbvars() requires this, and should be done once for efficiency */
  assert(MAXABSTDEPTH <= ULONG_MAX);

  /* allocate the pool */
  /* nb. The area returned by calloc is initialized by zero,
     and Lcell.type = 0 means FREE cell */
  if ((pool = calloc(INITPOOLSIZE, sizeof(Lcell))) == NULL) {
    fatal("cannot allocate the pool; size=%ld\n", INITPOOLSIZE);
  }

  /* make free list */
  pool[0].nextfree = -1;	/* means tail */
  for (i = 1; i < INITPOOLSIZE; i++)
    pool[i].nextfree = i - 1;
  freehead = INITPOOLSIZE - 1;

  poolsize = INITPOOLSIZE;
  msg_info("initpool: allocated %ld cells\n", poolsize);
  return;
}

Cellidx
newcell(int type) {
  Cellidx i;

  msg_debug("newcell: requested one cell of type %d\n", type);
  /* valid type? */
  if (type <= FREE || NOTYPE <= type)
    fatal("newcell: unknown type %d requested\n", type);

  if (freehead >= 0) {
    /* get one cell from the free list */
    i = freehead;
    /* link next */
    freehead = pool[i].nextfree;
    /* set up the cell */
    msg_debug("newcell: get one cell from free list [index %ld]\n", i);
    Ctype(i) = type;
    count_allocated++;
    return i;
  }

  /* free list empty; allocate new space */
  msg_debug("newcell: space ran out; enlarging\n");
  enlargepool();

  /* Enlargepool assures that it allocates at least one new cell space,
     and the free list must be set up */

  /* integrity check */
  if (freehead < 0)
    abortwithcore("newcell: internal error: poolsize = %ld, freehead = %ld\n", poolsize, freehead);

  i = freehead;
  /* link next */
  freehead = pool[i].nextfree;
  /* set up the cell */
  msg_debug("newcell: returns a cell from new space; [index %ld]\n", i);
  Ctype(i) = type;
  count_allocated++;
  return i;
}

/*
 * enlargepool - enlarge the pool
 */
void
enlargepool() {
  Lcell *newp;
  Cellidx oldsize, newsize;
  Cellidx i;

  oldsize = poolsize;

  if (poolsize >= CELLIDX_MAX)	/* actually == */
    fatal("enlargepool: already reached limit (%ld cells)\n", poolsize);
  
  if (poolsize >= CELLIDX_MAX / 2)
    newsize = CELLIDX_MAX;
  else
    newsize = poolsize * 2;

  msg_debug("enlargepool: enlarging the pool: %ld -> %ld\n", poolsize, newsize);
  msg_debug(" current pool: ");
  if (deblev(L_DEBUG)) eprintpool();
  newp = realloc(pool, sizeof(Lcell) * newsize);
  if (newp == NULL)
    /*
     * XXX: should show some information?
     */
    fatal("enlargepool: cannot enlarge pool (%ld cells) to have %ld cells (size = %ld)\n",
	poolsize, newsize, sizeof(Lcell) * newsize);

  /* clear newly allocated area */
  /*
   * XXX: for speeding: clear only .type field
   */
  memset(newp + poolsize, 0, (newsize - poolsize) * sizeof(Lcell));

  /* set new pool pointer */
  pool = newp;
  poolsize = newsize;
  msg_debug("     new pool: ");
  if (deblev(L_DEBUG)) eprintpool();

  /* link new cells to the free list */
  pool[oldsize].nextfree = freehead;
  for (i = oldsize + 1; i < newsize; i++) {
    pool[i].nextfree = i - 1;
  }
  freehead = newsize - 1;
}

/*
 * fprintpool - print map of current pool
 */
void
fprintpool(FILE *fp) {
  Cellidx i;

  fprintf(fp, "*** pool (%ld slots, %ld bytes) ***\n", poolsize, poolsize*sizeof(pool[0]));
  for (i = 0; i < poolsize; i++)
    if (pool[i].type <= NOTYPE)
      fprintf(fp, "%c ", typechar[pool[i].type]);
    else
      fprintf(fp, "%c ", typechar[NOTYPE+1]);	/* holds a char for 'unknown' */
  fprintf(fp, "\n");
}

void
printpool() {
  fprintpool(stdout);
}

void
eprintpool() {
  fprintpool(stderr);
}

/*
 * fpoolinfo - show summary info of the pool
 */
void fpoolinfo(FILE *fp) {
  Cellidx i;
  Cellidx nfree = 0;

  for (i = 0; i < poolsize; i++)
    if (pool[i].type == FREE) nfree++;
  fprintf(fp, "pool info: %ld slots (%ld bytes), %ld used, %ld free: "
	      "cumulative %lu allocated, %lu freed, %lu leaked\n",
      poolsize, poolsize*sizeof(pool[0]), poolsize-nfree, nfree,
      count_allocated, count_freed, (poolsize-nfree) - (count_allocated-count_freed));
}

void poolinfo() {
  fpoolinfo(stdout);
}

void epoolinfo() {
  fpoolinfo(stderr);
}


/*
 * freecell - free a cell
 * nb. to be in harmony with free(3), freeing an unallocated cell is allowed
 */
void
freecell(Cellidx ci) {
  msg_debug("freecell: requested freeing cell #%ld of type %d\n", ci, Ctype(ci));

  /* check cell index specified */
  if (poolsize <= ci) {
    msg_warning("freecell: tried to free nonexisting cell, Cellidx = %ld, poolsize = %ld (but did nothing)\n", ci, poolsize);
  }

  /* link to the free list */
  pool[ci].nextfree = freehead;
  freehead = ci;

  Ctype(ci) = FREE;
  count_freed++;
  return; 
}

/*
 * prunecell - free all cells linked from the specified cell
 */
void
prunecell(Cellidx ci) {
  msg_debug("prunecell: requested freeing cells from #%ld of type %d\n", ci, Ctype(ci));

  /* check cell index specified */
  if (poolsize <= ci) {
    msg_warning("prunecell: tried to free from nonexisting cell, Cellidx = %ld, poolsize = %ld (but did nothing)\n", ci, poolsize);
  }

  switch (Ctype(ci)) {
    case FREE:
      msg_notice("prunecell: free an already free cell? %ld\n", ci);
      return;
    case VAR:
      freecell(ci);
      return;
    case ABST:
      prunecell(Cbody(ci));
      freecell(ci);
      return;
    case APPL:
      prunecell(Cleft(ci));
      prunecell(Cright(ci));
      freecell(ci);
      return;
    default:
      msg_warning("prunecell: strange type cell encountered; "); if (deblev(L_WARNING)) eprintcell(ci);
      return;
  }
}

/*
 * deepcopy - create entirely new cell structure that denotes specified lexp
 */
Cellidx
deepcopy(Cellidx ci) {
  Cellidx newci;
  Cellidx t;

  switch (Ctype(ci)) {
    case VAR:
      newci = newcell(VAR);
      Cvar(newci) = Cvar(ci);
      return newci;
    case ABST:
      newci = newcell(ABST);
      Cbv(newci) = Cbv(ci);
      /*
       * deepcopy has side effect on pool, which is in Cbody; we need the temporary variable here
       */
      t = deepcopy(Cbody(ci));
      Cbody(newci) = t;
      return newci;
    case APPL:
      newci = newcell(APPL);
      /*
       * deepcopy has side effect on pool, which is in Cleft and Cright; we need the temporary variable here
       */
      t = deepcopy(Cleft(ci));
      Cleft(newci) = t;
      t = deepcopy(Cright(ci));
      Cright(newci) = t;
      return newci;
    default:
      abortwithcore("deepcopy: unknown cell type %s, Cellidx = %ld\n", Ctype(ci), ci);
      /* NOTREACHED */
      return ci;
  }
}

/*
 * copycell - copy a cell by bcopy
 */
void
copycell(Cellidx src, Cellidx dst) {
  bcopy(&pool[src], &pool[dst], sizeof(pool[dst]));
}

/*
 * canonbvars - canonicalize binding variables' ids
 */

int
canonbvarhandler(Cellidx ci, int descending) {
  static Var bvstack[MAXABSTDEPTH];
  static unsigned long int sp = 0;	/* stack pointer */
  int i;

  /* XXX: doesn't work yet; must handle free variables correctly */
  fatal("XXX: canonbvarhandler not implemented correctly (so far)\n");

  msg_curdebug("canonbvarhander: ci=%ld, %s\n", ci, descending?"descending":"ascending");
  switch (Ctype(ci)) {
    case APPL:
      return 1;		/* simply continue dfs */
    case ABST:
      if (descending) {
	if (sp >= MAXABSTDEPTH) {
	  /*
	   * XXX: might be saved but we might have rewritten some vars...
	   */
	  abortwithcore("canonbvarhandler: stack overflow."
	      " stacksize (MAXABSTDEPTH) = %lu."
	      " increase MAXABSTDEPTH in lexp.h\n", MAXABSTDEPTH);
	}
	/* push bv */
	bvstack[sp++] = Cbv(ci);
	return 1;		/* go deeper */
      } else {
	/* ascending */
	sp--;
	Cbv(ci) = sp + 1;	/* canonicalized on 1-origin */
	return 1;
      }
    case VAR:	/* always bottom so called once; no need to check descending */
      /* search the frame */
      for (i = sp - 1; i >= 0; i--)
	if (Cvar(ci) == bvstack[i])
	  break;
      if (i < 0) {
	/* not found = free variable.  leave it as it is */
      } else {
	/* bound var */
	Cvar(ci) = i + 1;	/* canonicalize on 1-origin */
      }
      return 1;
    default:
      abortwithcore("canonbvarhandler: specified non-Lexp or incomplete Lexp; met type %d\n", Ctype(ci));
      /*NOTREACHED*/
      return 0;
  }
}

void
canonbvars(Lexp l) {
  /* XXX */
  fatal("canonbvars not implemented correctly\n");

  dfsLexp(l, canonbvarhandler);
  return;
}

static int
isequalLexp_r(Cellidx c1, VarList *bv1, Cellidx c2, VarList *bv2) {
  VarList *p1, *p2;
  Var v1, v2;
  int res;

  if (Ctype(c1) != Ctype(c2))
    return 0;
  if (Ctype(c1) == APPL)
    return isequalLexp_r(Cleft(c1), bv1, Cleft(c2), bv2) &&
           isequalLexp_r(Cright(c1), bv1, Cright(c2), bv2);
  if (Ctype(c1) == ABST) {
    p1 = emalloc(sizeof(VarList));
    p1->v = Cbv(c1);
    p1->next = bv1;
    p2 = emalloc(sizeof(VarList));
    p2->v = Cbv(c2);
    p2->next = bv2;
    res = isequalLexp_r(Cbody(c1), p1, Cbody(c2), p2);
    free(p1);
    free(p2);
    return res;
  }
  if (Ctype(c1) != VAR) {
    msg_warning("isequalLexp_r: comparing bad type (%d)\n", Ctype(c1));
    return 0;	/* XXX: should abort? */
  }
  /* comparing variables */
  v1 = Cvar(c1);
  v2 = Cvar(c2);
  /* are those bound variables? */
  for (p1 = bv1, p2 = bv2; p1 != NULL && p2 != NULL; p1 = p1->next, p2 = p2->next) {
    if (p1->v == v1) {
      if (p2->v == v2) {
	/* same bound variable */
	return 1;
      } else {
	/* v1 is bound but v2 is not the same bound var */
	return 0;
      }
    } else if (p2->v == v2) {
      /* v2 is bound here but v1 not */
      return 0;
    } else {
      /* both not bound; continue */
      ;
    }
  }
  /* both are free variables */
  if (v1 == v2)
    return 1;
  else
    return 0;
}

int
isequalLexp(Lexp l1, Lexp l2) {	
  return isequalLexp_r(l1, NULL, l2, NULL);
}

/*
 * dfsLexp - generic dfs routine for lexp
 *
 * does depth-first search on specified Lexp and call func on the node of Cellidx
 * second parameter for func: 1 = descending, 0 = ascending.
 *
 * go deeper when func returns 1, terminates (and returns) when func returns 0.
 */
void
dfsLexp_rec(Cellidx ci, int (*func)(Cellidx, int)) {
  int g;	/* go deeper? */

  switch (Ctype(ci)) {
    case VAR:
      (void)((*func)(ci, 1));
      return;
    case ABST:
      g = (*func)(ci, 1);
      if (g) {
	dfsLexp_rec(Cbody(ci), func);
	(void)((*func)(ci, 0));
      }
      return;
    case APPL:
      g = (*func)(ci, 1);
      if (g) {
	dfsLexp_rec(Cleft(ci), func);
	dfsLexp_rec(Cright(ci), func);
	(void)((*func)(ci, 0));
      }
      return;
    default:
      abortwithcore("dfsLexp_rec: specified non-Lexp or incomplete Lexp: type %d\n", Ctype(ci));
      /*NOTREACHED*/
      return;
  }
}

void
dfsLexp(Lexp l, int (*func)(Cellidx, int)) {
  Cellidx rootci = l;

  dfsLexp_rec(rootci, func);
}

/* [EOF] */
