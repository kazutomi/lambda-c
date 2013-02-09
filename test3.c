/*
 * test3.c - parser test
 *
 * $Id: test3.c,v 1.3 2002/05/01 08:23:18 tominaga Exp $
 */

#include <stdio.h>
#include "lambda.h"

/*
 * nb: This test messes the pool with unlinked cells
 */

char *testcases[] = {
  /*
   * normal cases
   */
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
  /*
   * error cases
   */
  "1 1",
  "(",
  "L",
  ".",
  ")",
  "(L)",
  "(3)",
  "(L 1.1",
  "(L 1.1))",
  "(L 1..1)",
  ")3 2(",
  "(L 1.1)(L 2.2)",
  "(L 3.(L 1.1)(L 2.2))",
};

int
main() {
  Lexp c;
  int i;

  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    printf("%s ===> ", testcases[i]);
    c = str2lexp(testcases[i]);
    printlexp(c);
  }
  return 0;
}
/* [EOF] */
