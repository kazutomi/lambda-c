/*
 * proto.h - prototype declarations of functions
 *
 * $Id: proto.h,v 1.11 2002/05/02 08:49:09 tominaga Exp tominaga $
 */

#if !defined(_PROTO_H_INCLUDED_)
#include <stdio.h>
#include <sys/types.h>
#include "lexp.h"

/*
 * main.c
 */
int main(void);

/*
 * ilambda.c
 */
Lexp Lnewvar(Var);
Lexp Labst(Var, Lexp);
Lexp Lappl(Lexp, Lexp);
Lexp Lcopy(Lexp);
void Lfree(Lexp);
Lexp Lstr2Lexp(char *);
int LLexp2str(Lexp, char *, int);
void Lcanon(Lexp);
int Leq(Lexp, Lexp);

/*
 * strlexp.c
 */
void fprintlexp_n(FILE *, Lexp);
void fprintlexp(FILE *, Lexp);
void printlexp(Lexp);
void eprintlexp(Lexp);
int lexp2str(Cellidx, char *, int);
Lexp str2lexp(char *);

/*
 * pool.c
 */
void *emalloc(size_t);
void fprintfreelist(FILE *);
void printfreelist(void);
void eprintfreelist(void);
void fprintcell(FILE *, Cellidx);
void printcell(Cellidx);
void eprintcell(Cellidx);
void initpool(void);
Cellidx newcell(int);
void enlargepool(void);
void fprintpool(FILE *);
void printpool(void);
void eprintpool(void);
void fpoolinfo(FILE *);
void poolinfo(void);
void epoolinfo(void);
void freecell(Cellidx);
void prunecell(Cellidx);
Cellidx deepcopy(Cellidx);
void copycell(Cellidx, Cellidx);
void canonbvars(Lexp);
int isequalLexp(Lexp, Lexp);
void dfsLexp_rec(Cellidx, int (*)(Cellidx, int));
void dfsLexp(Lexp, int (*)(Cellidx, int));

/*
 * message.c
 */
void fatal(char *, ...);
void abortwithcore(char *, ...);
void msg_generic(FILE *, enum facility, enum level, char *, ...);

/*
 * parser.c
 */
Cellidx create_lexp(char *);
enum token peeknext(void);
long int getlong(void);
void getnext(void);
Cellidx do_lexp(void);
Cellidx do_abst(void);
Cellidx do_appl(void);

/*
 * lambops.c
 */
Lexp alpha(Lexp, Var, Lexp);
Var findmaxvar(Lexp);
Lexp subst(Lexp, Var, Lexp, Var);
int betaat(Cellidx);
Cellidx canon_findredex(Cellidx);
Cellidx findredex(Lexp, int);
int betastep(Lexp, int);
int nbeta(Lexp, int, int);
int countcells(Lexp);

/*
 * diff.c
 */
void calcbdist_r(Cellidx, int);
void calcbdist(Cellidx);
int diff_r(Lexp, Lexp);
int diff(Lexp, Lexp);
int arraynodes(Cellidx, int [], int);

#define _PROTO_H_INCLUDED_
#endif
/* [EOF] */
