/*
 * global.c - global variables
 *
 * $Id: global.c,v 1.6 2002/05/02 08:49:09 tominaga Exp $
 */

#include "lexp.h"
#include "parser.h"
#include "debug.h"

enum level dlevel[F_NUMFACIL] = {
  /* must be in order of enum facility; see debug.h */
  L_PRODUCT,	/* F_MAIN */
  L_PRODUCT,	/* F_POOL */
  L_PRODUCT,	/* F_PARSER */
  L_PRODUCT,	/* F_LAMBOPS */
  L_PRODUCT,	/* F_STRLEXP */
  L_PRODUCT,	/* F_INTERFACE */
  L_PRODUCT,	/* F_MISC */
};

Lcell *pool;	/* cell pool */
Cellidx poolsize = 0;	/* current #cells in the pool */

char *typename[NOTYPE+1] = {
  "FREE",
  "VAR",
  "ABST",
  "APPL",
  "NOTYPE",
};

char typechar[NOTYPE+2] = {	/* last entry means 'unknown' */
  '-',
  'V',
  'B',
  'P',
  'N',
  'X',
};

char *tokenname[LP_NUMTOK+1] = {
  "LP_ERROR",
  "LP_NULL",
  "LP_VAR",
  "LP_LAMBDA",
  "LP_LPAREN",
  "LP_RPAREN",
  "LP_DOT",
  "LP_NUMTOK",
};

/* [EOF] */
