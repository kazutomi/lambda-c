/*
 * test23.c - arraynodes test
 *
 * $Id: test23.c,v 1.1 2002/12/09 01:56:47 tominaga Exp $
 */

#include <stdio.h>
#include "lambda.h"

char *testcases[] = {
  /* variable */
  "1",
  "2",
  "1000",
  /* abstraction */
  "(L 1.1)",
  "(L 2.5)",
  "(L 1.(L 1.1))",
  "(L 1.(L 2.1))",
  /* application */
  "(1 1)",
  "(2 1)",
  /* mixture */
  "(L 1.(1 2))",
  "(1 (L 3.1))",
  "((L 2.2) (L 3.1))",
  "(((L 2.2) (L 3.1)) 5)",
  "(L 1.(1 (L 2.2)))",
  "(L 1.((L 1.1)2))",
  "(L 1.(L 2.(L 3.((1 2) 3))))",
  "(L 1.(L 2.(L 3.((L 2.(3 2))2))))",
  "(L 1.(L 2.((L 3.((L 2.(3 2))2))1)))",
  "(L 1.((L 2.((L 3.((L 2.(3 2))2))1))1))",
  /* other practical tests */
  "((L 1.(2 1))(2 3))",
  "((L 1.(2 1))(L 2.3))",
  "((L 1.(L 2.(3 (1 2))))(L 2.(2 3)))",
  "((L 1.(1 1))((1 1) 1))",
  "((L 2.(L 1.(1 2)))(1 2))",
  "((L 1.(L 2.(1 2)))(1 2))",
};

enum {
  LEV = 6,
};

int
main() {
  Lexp l;
  int i, j, lev;
  int a[LEV];

  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    l = str2lexp(testcases[i]);
    lev = arraynodes(l, a, LEV);
    printf("%s: level %d; ", testcases[i], lev);
    for (j = 0; j < LEV; j++) {
      printf(" %d", a[j]);
    }
    putchar('\n');
  }
  return 0;
}
/* [EOF] */
