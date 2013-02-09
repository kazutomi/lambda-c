/*
 * lexp.h - definitions for lambda expressions
 *
 * $Id: lexp.h,v 1.8 2002/05/01 09:13:10 tominaga Exp $
 */

#if !defined(_LEXP_H_INCLUDED_)
#include <machine/limits.h>

enum {
  /* for struct lcell.type */
  /* must be zero (free) to NOTYPE;
     this range is used for checking of valid type */
  FREE = 0,
  VAR = 1,
  ABST = 2,
  APPL = 3,
  NOTYPE = 4,
  /* size constants */
  INITPOOLSIZE = 1,
  /* beta reduction strategies */
  CANONICAL = 1,
  INNERMOST = 2,
  /*
   * maximum abstraction depth for Lexp canonicalization (canonbvars());
   * requires storage of (MAXABSTDEPTH * sizeof(Var)) bytes
   */
  MAXABSTDEPTH = 1024,
  /*
   * maximum tree height.  used for diff'ing
   */
  MAXTREEHEIGHT = 2048,
};

typedef long int Cellidx;	/* cell pool index */
typedef long int Var;		/* variable */
enum {
  CELLIDX_MAX = LONG_MAX,
  VAR_MAX = LONG_MAX,
};

struct lcell {
  int type;	/* FREE, VAR, ABST or APPL */
  union {
    /* VAR */
    Var var;
    /* ABST */
    struct {
      Var bv;
      Cellidx body;
    } ab;
    /* APPL */
    struct {
      Cellidx left;
      Cellidx right;
    } ap;
  } d;
  Cellidx nextfree;
  Var bdist;	/* binding distance; used to calculate differences */
};

typedef struct lcell Lcell;
typedef Cellidx Lexp;

struct velem {
  Var v;
  struct velem *next;
};

typedef struct velem VarList;

/* aliases for simplicity */
#define Ctype(idx)	(pool[idx].type)
#define Cvar(idx)	(pool[idx].d.var)
#define Cbv(idx)	(pool[idx].d.ab.bv)
#define Cbody(idx)	(pool[idx].d.ab.body)
#define Cleft(idx)	(pool[idx].d.ap.left)
#define Cright(idx)	(pool[idx].d.ap.right)
#define Cbdist(idx)	(pool[idx].bdist)

#define _LEXP_H_INCLUDED_
#endif
/* [EOF] */
