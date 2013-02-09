/*
 * test12.c - unparser (lexp2str) test
 *
 * $Id: test12.c,v 1.1 2002/05/10 04:02:06 tominaga Exp tominaga $
 */

#include <stdio.h>
#include "lambda.h"

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
  char buf[1024];
  int nc;

  if (setvbuf(stdout, NULL, _IONBF, 0) == EOF)
    fprintf(stderr, "*** setvbuf failed ***\n");

  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    printf("%s ***\n", testcases[i]);
    c = str2lexp(testcases[i]);
    printf("  printlexp: ");
    printlexp(c);
    nc = lexp2str(c, buf, sizeof(buf));
    printf("  lexp2str:  ");
    printf("%s (code %d)\n", buf, nc);
  }
  return 0;
}
/* [EOF] */
