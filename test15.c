/*
 * test15.c - performance test by pred 20
 *
 * $Id: test15.c,v 1.1 2002/05/17 02:24:36 tominaga Exp $
 */

#include <stdio.h>
#include "lambda.h"

char *s0 = "(L 1.(L 2.2))";
char *ss = "(L 1.(L 2.(L 3.((1 2)(2 3)))))";
/* pred by Revesz */
char *sp = "
(L 14.
  (
    (
      (
	14
	(L 16.(L 26.
	  (
	    (
	      26
	      (
		(L 1.(L 2.(L 3.((1 2)(2 3)))))
		(16 (L 1.(L 2.1)))
	      )
	    )
	    (16 (L 1.(L 2.1)))
	  )
	))
      )
      (L 26.((26 (L 1.(L 2.2))) (L 1.(L 2.2))))
    )
    (L 24.(L 25.25))
  )
)
";

Lexp
church_num(int n) {
  Lexp l0, ltmp, cs;

  l0 = Lstr2Lexp(s0);
  while (--n >= 0) {
    cs = Lstr2Lexp(ss);
    ltmp = Lappl(cs, l0);
    nbeta(ltmp, CANONICAL, 4);	/* succ reduces in 3 steps */
    l0 = ltmp;
  }
  return l0;
}

int
main() {
  char s20[1024];
  Lexp l0, l1;
  int i, cnt;

  setvbuf(stdout, NULL, _IONBF, 0);

  initpool();

  l0 = church_num(20);
  if (LLexp2str(l0, s20, sizeof(s20)) < 0)
    fatal("buffer too small\n");

  for (i = 0; i < 10; i++) {
    l0 = Lstr2Lexp(s20);
    l1 = Lappl(Lstr2Lexp(sp), l0);
    printf("pred "); printlexp(l0);
    cnt = nbeta(l1, CANONICAL, 1000);
    printf("  -(%d times)-> ", cnt); printlexp(l1);
  }

  epoolinfo();

  return 0;
}
/* [EOF] */
