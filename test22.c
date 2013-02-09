/*
 * test22.c - diff test
 *
 * $Id: test22.c,v 1.4 2002/12/09 01:55:21 tominaga Exp $
 */

#include <stdio.h>
#include "lambda.h"

char *testcases[][2] = {
  /* same */
  { "1", "1" },
  { "2", "2" },
  { "1000", "1000" },
  { "(L 1.1)", "(L 1.1)" },
  { "(L 1.1)", "(L 2.2)" },
  { "(L 1.(L 1.1))", "(L 1.(L 1.1))" },
  { "(L 1.(L 2.1))", "(L 1.(L 2.1))" },
  { "(L 2.(L 1.1))", "(L 2.(L 1.1))" },
  { "(L 2.(L 1.3))", "(L 2.(L 1.3))" },
  { "(L 2.(L 1.3))", "(L 2.(L 1.3))" },
  { "(1 1)", "(1 1)" },
  { "(2 1)", "(2 1)" },
  { "(2 3)", "(2 3)" },
  { "(L 1.(1 2))", "(L 1.(1 2))" },
  { "(L 1.(1 2))", "(L 3.(3 2))" },
  { "((L 2.2) (L 3.1))", "((L 2.2) (L 3.1))" },
  { "((L 2.2) (L 3.1))", "((L 1.1) (L 3.1))" },
  { "(((L 2.2) (L 3.1)) 5)", "(((L 2.2) (L 3.1)) 5)" },
  { "(((L 1.1) (L 3.1)) 5)", "(((L 2.2) (L 3.1)) 5)" },
  { "(L 1.(1 (L 2.2)))", "(L 1.(1 (L 2.2)))" },
  { "(L 1.(1 (L 2.2)))", "(L 3.(3 (L 2.2)))" },
  { "(L 1.(1 (L 2.2)))", "(L 3.(3 (L 3.3)))" },
  { "(L 1.((L 1.1)2))", "(L 1.((L 1.1)2))" },
  { "(L 1.((L 1.1)2))", "(L 10.((L 1.1)2))" },
  { "(L 1.((L 1.1)2))", "(L 8.((L 3.3)2))" },
  { "(L 1.(L 2.(L 3.((L 2.(3 2))2))))", "(L 1.(L 2.(L 3.((L 2.(3 2))2))))" },
  { "(L 1.(L 2.(L 3.((L 2.(3 2))2))))", "(L 1.(L 5.(L 3.((L 2.(3 2))5))))" },
  { "(L 1.(L 2.(L 3.((L 2.(3 2))2))))", "(L 1.(L 5.(L 9.((L 2.(9 2))5))))" },
  { "(L 1.(L 2.(L 3.((L 2.(3 2))2))))", "(L 1.(L 5.(L 9.((L 13.(9 13))5))))" },
  /* different */
  { "1", "2" },
  { "(L 1.1)", "(L 1.2)" },
  { "(L 2.1)", "(L 1.2)" },
  { "(L 1.(L 1.5))", "(L 1.(L 1.1))" },
  { "(L 2.(L 2.1))", "(L 1.(L 2.1))" },
  { "(L 2.(L 1.2))", "(L 2.(L 1.1))" },
  { "(L 2.(L 1.5))", "(L 2.(L 1.3))" },
  { "(1 1)", "(2 2)" },
  { "(2 3)", "(3 2)" },
  { "(L 1.(1 2))", "(L 2.(1 2))" },
  { "(L 1.(1 2))", "(L 3.(4 2))" },
  { "((L 2.2) (L 3.1))", "((L 2.2) (L 3.2))" },
  { "(((L 2.2) (L 3.9)) 5)", "(((L 2.2) (L 3.1)) 5)" },
  { "(((L 1.1) (L 3.1)) 6)", "(((L 2.2) (L 3.1)) 5)" },
  { "(L 1.(2 (L 2.2)))", "(L 1.(1 (L 2.2)))" },
  { "(L 1.(3 (L 2.2)))", "(L 3.(3 (L 2.2)))" },
  { "(L 1.((L 1.3)2))", "(L 1.((L 1.1)2))" },
  { "(L 1.(L 2.(L 3.((L 2.(2 2))2))))", "(L 1.(L 2.(L 3.((L 2.(3 2))2))))" },
  /* quite different */
  { "(1 2)", "(L 1.(1 2))" },
  { "1", "(L 1.(1 2))" },
  { "((L 1.1)(L 2.2))", "(L 1.(L 1.2))" },
  { "((L 1.1)(L 2.2))", "(L 1.(L 1.(L 2.(1 2))))" },
  { "((L 1.1)((L 1.1)(L 1.1)))", "((L 1.1)(L 1.((L 2.2)(L 2.2))))" },
  { "(L 1.(L 2.2))", "(L 1.(L 2.(1 2)))" },
  { "(L 1.(L 2.(1 2)))", "(L 1.(L 2.(1 (1 2))))" },
  { "(L 1.(L 2.(1 (1 2))))", "(L 1.(L 2.(1 (1 (1 2)))))" },
  { "(L 1.(L 2.(1 2)))", "(L 1.(L 2.(1 (1 (1 2)))))" },
};

int
main() {
  Lexp l1, l2;
  int i, dif;

  setvbuf(stdout, NULL, _IONBF, 0);
  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    l1 = Lstr2Lexp(testcases[i][0]);
    l2 = Lstr2Lexp(testcases[i][1]);
    printlexp(l1);
    printlexp(l2);
    dif = diff(l1, l2);
    printf("diff = %d\n", dif);
  }
  return 0;
}
/* [EOF] */
