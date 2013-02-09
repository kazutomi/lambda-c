/*
 * parser.c - LL(1) recursive descent parser for my lambda expressions
 *
 * $Id: parser.c,v 1.7 2002/05/10 04:02:06 tominaga Exp tominaga $
 */
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>
#include "lambda.h"

static enum facility debugfacility = F_PARSER;

static char *cur;	/* where lex looks at */
static enum token next;	/* token got */
static Var tokdata;	/* token itself; only Var needs this data */
static int error;

/*
 * error message.  sets error flag for convenience.
 */
void
syntax_error(char *expected) {
  msg_warning("parser.c: syntax error: next %s (value %d), expected %s, tokdata %ld, followed by \"%s\"\n",
      (0<=next&&next<=LP_NUMTOK)?tokenname[next]:"---", next, expected, tokdata, cur);
  error = 1;
}

/*
 * create lambda expression list from lexp buf
 */
Cellidx
create_lexp(char *buf) {
  Cellidx c;

  error = 0;
  msg_debug("create_lexp: called \"%s\"\n", buf);
  cur = buf;
  c = do_lexp();
  getnext();
  if (error) {
    msg_warning("create_lexp: failed to create list for bad lexp \"%s\"\n", buf);
    return -1;
  }
  if (next != LP_NULL) {
    syntax_error("end of line");
    return -1;
  }
  return c;
}

/*
 * peek next token
 */
enum token
peeknext() {
  char *p;

  for (p = cur; isspace(*p); p++) ;
  switch (*p) {
    case '\0':
      return LP_NULL;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return LP_VAR;
    case 'L':
      return LP_LAMBDA;
    case '(':
      return LP_LPAREN;
    case ')':
      return LP_RPAREN;
    case '.':
      return LP_DOT;
    default:
      msg_warning("peeknext: unknown token met \"%s\"\n", p);
      return LP_ERROR;
  }
  /* NOTREACHED */
}

/*
 * get next long int string
 */
long int
getlong() {
  long int val = 0L;

  assert('0'+1=='1'&&'1'+1=='2'&&'2'+1=='3'&&'3'+1=='4'&&
      '4'+1=='5'&&'5'+1=='6'&&'6'+1=='7'&&'7'+1=='8'&&'8'+1=='9');
  /* or use strtol(3) for each digit, but take efficiency */

  while (isdigit(*cur)) {
    if (val >= LONG_MAX / 10) {
      msg_warning("getlong: value would exceed the limit (%ld -> ...)", val);
    }
    val *= 10;
    val += *cur - '0';
    cur++;
  }
  return val;
}

/*
 * get next token to next and tokdata
 */
void
getnext() {
  tokdata = -1;	/* for sanity */
  for (; isspace(*cur); cur++) ;
  switch (*cur) {
    case '\0':
      next = LP_NULL;
      break;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      tokdata = getlong();
      next = LP_VAR;
      break;
    case 'L':
      cur++;
      next = LP_LAMBDA;
      break;
    case '(':
      cur++;
      next = LP_LPAREN;
      break;
    case ')':
      cur++;
      next = LP_RPAREN;
      break;
    case '.':
      cur++;
      next = LP_DOT;
      break;
    default:
      msg_warning("getnext: unknown token met \"%s\"\n", cur);
      next = LP_ERROR;
      error = 1;
      break;
  }
  msg_debug("getnext: got %s, tokdata %ld\n", tokenname[next], tokdata);
}

Cellidx
do_lexp() {

  msg_debug("do_lexp invoked\n");
  
  if (error) {
    msg_warning("do_lexp: error has occurred; bailing out\n");
    return -1;
  }

  /* get next token */
  getnext();
  switch (next) {
    case LP_ERROR:
      syntax_error("nothing (fatal)");
      return -1;
    case LP_NULL:
      msg_warning("do_lexp: nothing to parse\n");
      error = 1;
      return -1;
    case LP_VAR: {
      Var var;
      Cellidx c;

      var = tokdata;
      c = newcell(VAR);
      Cvar(c) = var;
      msg_debug("allocated var %ld\n", var);
      return c;
    }
    case LP_LPAREN: {	/* ABST or APPL */
      Cellidx c;
      enum token peek;

      peek = peeknext();
      switch (peek) {
	case LP_LAMBDA:	/* ABST */
	  c = do_abst();
	  return c;
	case LP_VAR:
	case LP_LPAREN:	/* APPL */
	  c = do_appl();
	  return c;
	default:
	  getnext();	/* set next for error message */
	  syntax_error("L, var or lparen");
	  return -1;
      }
    }
    default:
      syntax_error("start of lexp");
      return -1;
  }
  /* NOTREACHED */
}

Cellidx
do_abst() {
  Var bv;
  Cellidx c, body;

  msg_debug("do_abst invoked\n");

  getnext();	/* next token must be L */
  if (next != LP_LAMBDA) {
    syntax_error("L");
    return -1;
  }

  getnext();	/* must be variable */
  if (next != LP_VAR) {
    syntax_error("variable");
    return -1;
  }
  bv = tokdata;

  getnext();	/* must be a dot */
  if (next != LP_DOT) {
    syntax_error("dot");
    return -1;
  }
  body = do_lexp();
  
  c = newcell(ABST);
  Cbv(c) = bv;
  Cbody(c) = body;

  getnext();	/* skip rparen */
  if (next != LP_RPAREN) {
    syntax_error("rparen");
    return -1;
  }
  return c;
}

Cellidx
do_appl() {
  Cellidx c, left, right;

  msg_debug("do_appl invoked\n");

  left = do_lexp();
  right = do_lexp();

  c = newcell(APPL);
  Cleft(c) = left;
  Cright(c) = right;

  getnext();	/* skip rparen */
  if (next != LP_RPAREN) {
    syntax_error("rparen");
    return -1;
  }
  return c;
}

/* [EOF] */
