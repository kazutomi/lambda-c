/*
 * test4.c - deep copy test
 *
 * $Id: test4.c,v 1.3 2002/05/05 05:27:42 tominaga Exp $
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
  NULL,
};

int
main() {
  char **p;
  Lexp l, newl;

  initpool();

  for (p = testcases; *p != NULL; p++) {
    printf("Lambda expression string \"%s\" -> ", *p);
    l = str2lexp(*p);
    printlexp(l);
    newl = deepcopy(l);
    printf("Original Lexp: ");
    printlexp(l);
    printf("Copied Lexp: ");
    printlexp(newl);
  }

  return 0;
}
/* [EOF] */
