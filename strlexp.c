/*
 * strlexp.c - string and internal data conversion routines
 *
 * $Id: strlexp.c,v 1.5 2002/05/10 04:02:06 tominaga Exp tominaga $
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lambda.h"

static enum facility debugfacility = F_STRLEXP;
static int proceedwith(char *, char *, int);

/*
 * just to print
 *
 * XXX: might be better to use lexp2str, but don't want to be bothered with buffer
 */
void
fprintlexp_n(FILE *fp, Lexp ci) {
  switch (Ctype(ci)) {
    case FREE:
      fprintf(fp, "-");
      break;
    case VAR:
      fprintf(fp, "%ld", Cvar(ci));
      break;
    case ABST:
      fprintf(fp, "(L %ld.", Cbv(ci));
      fprintlexp_n(fp, Cbody(ci));
      fprintf(fp, ")");
      break;
    case APPL:
      fprintf(fp, "(");
      fprintlexp_n(fp, Cleft(ci));
      fprintf(fp, " ");
      fprintlexp_n(fp, Cright(ci));
      fprintf(fp, ")");
      break;
    default:
      fprintf(fp, "X");
      break;
  }
  return;
}

void
fprintlexp(FILE *fp, Lexp ci) {
  fprintlexp_n(fp, ci);
  fputc('\n', fp);
}

void
printlexp(Lexp ci) {
  fprintlexp(stdout, ci);
}

void
eprintlexp(Lexp ci) {
  fprintlexp(stderr, ci);
}

/*
 * proceedwith - proceed in buf with str.  always null terminate buf.
 *
 * return value: > 0 proceed, -1 overrun
 */
static int
proceedwith(char *buf, char *str, int bytesleft) {
  int proceed, n, overrun = 0;

  msg_debug("proceedwith: \"%s\", left %d bytes\n", str, bytesleft);

  assert(bytesleft > 0);

  n = strlen(str);
  if (n == 0)
    msg_warning("proceedwith: called with null string; bad logic in lexp2str?\n");

  /* if strncpy(3) works with n=0, the following if statement is not necessary */
  if (bytesleft == 1) {
    *buf = '\0';
    if (n > 0)
      return -1;	/* overrun */
    else
      return 1;		/* proceeded normally */
  }

  if (n > bytesleft - 1) {
    overrun = 1;
    n = bytesleft - 1;
  }
  strncpy(buf, str, n);
  proceed = n;

  buf[n] = '\0';

  msg_debug("proceedwith: returning with buf \"%s\", proceed %d bytes\n", buf, proceed);

  if (overrun)
    return -1;
  else
    return proceed;
}

/*
 * lexp2str - convert Lexp to string
 *
 * return value: >= 0 result length, -1 = failure
 */

int
lexp2str(Cellidx ci, char *buf, int len) {
  int proceed, n;
  static char tmp[MISCSTRBUFLEN];
  int shouldbe;
  
  msg_debug("lexp2str: called for type %s with len = %d\n", typename[Ctype(ci)], len);
  assert(len > 0);

  switch (Ctype(ci)) {
    case FREE:
      proceed = proceedwith(buf, "-", len);
      return proceed;
    case VAR:
      shouldbe = snprintf(tmp, sizeof(tmp), "%ld", Cvar(ci));
      if (shouldbe > strlen(tmp))
	fatal("lexp2str: too short buffer len %d to get var\n", sizeof(tmp));
      proceed = proceedwith(buf, tmp, len);
      return proceed;
    case ABST:
      proceed = 0;

      shouldbe = snprintf(tmp, sizeof(tmp), "(L %ld.", Cbv(ci));
      if (shouldbe > strlen(tmp))
	fatal("lexp2str: too short buffer len %d to get bv\n", sizeof(tmp));
      n = proceedwith(buf+proceed, tmp, len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      n = lexp2str(Cbody(ci), buf+proceed, len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      n = proceedwith(buf+proceed, ")", len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      return proceed;
    case APPL:
      proceed = 0;
      
      n = proceedwith(buf+proceed, "(", len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      n = lexp2str(Cleft(ci), buf+proceed, len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      n = proceedwith(buf+proceed, " ", len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      n = lexp2str(Cright(ci), buf+proceed, len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      n = proceedwith(buf+proceed, ")", len);
      if (n < 0)
	return -1;
      proceed += n;
      len -= n;

      return proceed;
    default:
      proceed = proceedwith(buf, "X", len);
      return proceed;
  }
}

/*
 * parse lambda expression string, make internal data and return the cell index
 */
Lexp
str2lexp(char *str) {
  return create_lexp(str);
}

/* [EOF] */
