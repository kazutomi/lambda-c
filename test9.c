/*
 * test9.c - findredex test
 *
 * $Id: test9.c,v 1.1 2002/05/09 14:53:15 tominaga Exp $
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
  /* omega */
  "((L 1.(1 1)) (L 1.(1 1)))",
  "(((L 1.(1 1)) (L 1.(1 1)))(L 2.2))",
  /* Y combinator */
  "(L 2.((L 1.(2 (1 1)))(L 1.(2 (1 1)))))",
  "((L 2.((L 1.(2 (1 1)))(L 1.(2 (1 1)))))(L 3.3))",
  /* other practical tests */
  "((L 1.(2 1))(2 3))",
  "((L 1.(2 1))(L 2.3))",
  "((L 1.(L 2.(3 (1 2))))(L 2.(2 3)))",
  "((L 1.(1 1))((1 1) 1))",
  "((L 2.(L 1.(1 2)))(1 2))",
  "((L 1.(L 2.(1 2)))(1 2))",
};

int
main() {
  Lexp l;
  int i;
  Cellidx redex;

  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    printf("%s:", testcases[i]);
    l = str2lexp(testcases[i]);
    redex = findredex(l, CANONICAL);
    if (redex < 0) {
      printf(" no redex\n");
    } else {
      printf(" redex ");
      printlexp(redex);
    }
  }
  return 0;
}
/* [EOF] */
