/*
 * test6.c - findmaxvar test
 *
 * $Id: test6.c,v 1.1 2002/05/01 10:35:03 tominaga Exp $
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
  /* omega */
  "((L 1.(1 1)) (L 1.(1 1)))",
  /* Y combinator */
  "(L 2.((L 1.(2 (1 1)))(L 1.(2 (1 1)))))",
};

int
main() {
  Lexp c;
  int i;
  Var v;

  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    printf("%s ===> ", testcases[i]);
    c = str2lexp(testcases[i]);
    printlexp(c);
    v = findmaxvar(c);
    printf("  max var is %ld\n", v);
  }
  return 0;
}
/* [EOF] */
