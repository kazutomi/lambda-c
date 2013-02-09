/*
 * global.h - declarations of global variables
 *
 * $Id: global.h,v 1.6 2002/05/02 08:49:09 tominaga Exp $
 */

#if !defined(_GLOBAL_H_INCLUDED_)
#include "lexp.h"
#include "parser.h"
#include "debug.h"

/* program control */
enum level dlevel[F_NUMFACIL];

/* pool */
extern Lcell *pool;	/* cell pool */
extern Cellidx poolsize;	/* current #cells in the pool */

/* lexp */
extern char *typename[NOTYPE+1];	/* 0 -> "FREE", etc. */
extern char typechar[NOTYPE+2];		/* 0 -> 'F', etc. */

/* token */
char *tokenname[LP_NUMTOK+1];		/* "LP_VAR", etc. */

#define _GLOBAL_H_INCLUDED_
#endif

/* [EOF] */
