/*
 * test7.c - substitution test
 *
 * $Id: test7.c,v 1.1 2002/05/02 08:49:09 tominaga Exp $
 */

#include <stdio.h>
#include "lambda.h"

#define NELEMS(a) (sizeof(a)/sizeof(a[0]))

struct testcase {
  char *mstr;
  Var  var;
  char *nstr;
} testcases[] = {
  { "1", 1, "2" },
  { "1", 1, "1" },
  { "2", 2, "3" },
  { "2", 1, "3" },
  { "1", 2, "5" },
  { "1", 1, "(10 20)" },
  { "2", 1, "(10 20)" },
  { "5", 5, "(5 10)" },
  { "(1 2)", 2, "(3 4)" },
  { "(2 2)", 2, "(3 4)" },
  { "(L 1.2)", 2, "(3 4)" },
  { "(L 1.2)", 1, "(3 4)" },
  { "(L 2.1)", 1, "(3 4)" },
  { "(L 1.1)", 1, "(3 4)" },
  { "(L 1.(2 3))", 3, "(4 5)" },
  { "(L 1.(2 3))", 3, "(3 3)" },
  { "(L 1.(L 2.1))", 1, "(3 3)" },
  { "((L 1.(L 2.1)) (L 2.(L 1.2)))", 1, "(3 3)" },
  { "((L 1.(L 2.1)) (L 2.(L 1.2)))", 2, "(3 3)" },
  { "((L 1.(L 2.1)) (L 2.(L 2.1)))", 1, "(3 3)" },
  { "(1 (L 2.(L 3.((1 2)(3 4)))))", 1, "(2 3)" },
  { "(1 (L 2.(L 3.((1 2)(3 4)))))", 1, "(3 4)" },
};

int
main() {
  int i;
  struct testcase *p;
  Lexp m, n, l;
  Var v;

  initpool();

  for (i = 0, p = testcases; i < NELEMS(testcases); i++, p++) {
    printf("case %d: m=\"%s\", v=%ld, n=\"%s\"\n", i, p->mstr, p->var, p->nstr);
    m = str2lexp(p->mstr);
    printf("  created lexp for m: ");
    printlexp(m);
    n = str2lexp(p->nstr);
    printf("  created lexp for n: ");
    printlexp(n);
    v = p->var;
    l = subst(m, v, n, -1);
    printf("    after substitution: ");
    printlexp(l);
  }

  return 0;
}
/* [EOF] */
