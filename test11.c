/*
 * test11.c - nbeta test
 *
 * $Id: test11.c,v 1.2 2002/05/10 01:54:24 tominaga Exp $
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
  int i, count;

  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    printf("%s", testcases[i]);
    l = str2lexp(testcases[i]);
    count = nbeta(l, CANONICAL, 10);
    printf(" -(%d times)-> ", count);
    printlexp(l);
  }
  return 0;
}
/* [EOF] */
