/*
 * test21.c - calc bdist test
 *
 * $Id: test21.c,v 1.1 2002/12/07 08:04:00 tominaga Exp $
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
func(Cellidx ci, int descending) {
  if (!descending)
    return 0;

  printf("cell #%ld:\n", ci);
  printf("  type %d (%s)\n", Ctype(ci), typename[Ctype(ci)]);
  switch (Ctype(ci)) {
    case VAR:
      printf("    var = %ld (bdist = %ld)\n", Cvar(ci), Cbdist(ci));
      break;
    case ABST:
      printf("    bv = %ld, body cell=#%ld\n", Cbv(ci), Cbody(ci));
      break;
    case APPL:
      printf("    left cell=#%ld, right cell=#%ld\n", Cleft(ci), Cright(ci));
      break;
    default:
      printf("    unknown\n");
      break;
  }
  return 1;	/* always continue (expect to bounce at VARs) */
}

int
main() {
  Lexp l;
  int i;

  setvbuf(stdout, NULL, _IONBF, 0);
  initpool();

  for (i = 0; i < sizeof(testcases)/sizeof(testcases[0]); i++) {
    l = Lstr2Lexp(testcases[i]);
    calcbdist(l);
    printf("*** "); printlexp(l);
    dfsLexp(l, func);
  }
  return 0;
}
/* [EOF] */
