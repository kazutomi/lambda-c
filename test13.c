/*
 * test13.c - misc test
 *
 * $Id: test13.c,v 1.1 2002/05/14 16:41:49 tominaga Exp tominaga $
 */

#include <stdio.h>
#include "lambda.h"

int
main() {
  Lexp c1, c2, c3, c4, c5;
  int i;
  char buf[1024];

  setvbuf(stdout, NULL, _IONBF, 0);

  initpool();

  c1 = Lstr2Lexp("(L 1.(L 2.(2 1)))");
  c2 = Lstr2Lexp("(L 1.(L 2.1))");
  c3 = Lstr2Lexp("(L 1.(L 2.2))");
  c4 = Lappl(c1, c2);
  c5 = Lappl(c4, c3);
  printlexp(c5);
  
  i = nbeta(c5, CANONICAL, 10);
  LLexp2str(c5, buf, sizeof(buf));
  printf(" -> reduced to %s in %d times\n", buf, i);

  c1 = Lnewvar(1L);
  c2 = Lnewvar(2L);
  c3 = Lappl(c1, c2);
  c4 = Labst(1L, c3);
  c5 = Labst(2L, c4);
  printf("first: ");
  printlexp(c5);

  c1 = Lstr2Lexp("(4 (5 6))");
  c2 = Lappl(c5, c1);
  printf("next: ");
  printlexp(c2);

  c3 = Lstr2Lexp("(L 7.(7 7))");
  c4 = Lappl(c2, c3);
  printf("last: ");
  printlexp(c4);

  i = nbeta(c4, CANONICAL, 20);
  LLexp2str(c4, buf, sizeof(buf));
  printf(" -> reduced to %s in %d times\n", buf, i);

  return 0;
}
/* [EOF] */
