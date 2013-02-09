/*
 * testz.c
 *
 * $Id: testz.c,v 1.1 2002/04/21 06:57:37 tominaga Exp $
 */

#include "lambda.h"

int
main() {
  lexp l;
  char str[1024];

  initpool();	/* allocate and initialize the pool */

  l = str2lexp("((L 1.1)(L 2.(2 3)))");
  reduce(l, LEFTOUT, 0);	/* 0 means "until stop" */
  lexp2str(l, str, sizeof(str));
  printf("result = %s\n", str);
  freelexp(l);
  
  return 0;
}
/* [EOF] */
