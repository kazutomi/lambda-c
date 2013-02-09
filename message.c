/*
 * messages.c - message printing routines
 *
 * $Id: message.c,v 1.2 2002/05/10 04:02:06 tominaga Exp tominaga $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "lambda.h"

static enum facility debugfacility = F_MISC;

void
fatal(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(FATAL);
}

void
abortwithcore(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  abort();
}

/*
 * message printing
 *
 * (each facility might have macros for convenience)
 */
void
msg_generic(FILE *fp, enum facility f, enum level l, char *fmt, ...) {
  va_list ap;

  if (dlevel[f] >= l) {
    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
  }
}

/* [EOF] */
