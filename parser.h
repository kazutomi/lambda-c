/*
 * parser.h - parser definitions
 *
 * $Id: parser.h,v 1.1 2002/04/21 09:36:47 tominaga Exp $
 */

#if !defined(_PARSER_H_INCLUDED_)

/* terminal / nonterminal symbols */
enum token {
  LP_ERROR = 0,
  LP_NULL,	/* '\0' */
  LP_VAR,	/* digits */
  LP_LAMBDA,	/* L */
  LP_LPAREN,	/* ( */
  LP_RPAREN,	/* ) */
  LP_DOT,	/* . */
  LP_NUMTOK,	/* number of kinds of tokens */
};

#define _PARSER_H_INCLUDED_
#endif
/* [EOF] */
