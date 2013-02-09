/*
 * test20.c - Church numerals and some lexp
 *
 * $Id: test20.c,v 1.2 2002/07/16 07:28:50 tominaga Exp $
 */

#include <stdio.h>
#include "lambda.h"

char *s0 = "(L 1.(L 2.2))";
char *s1 = "(L 1.(L 2.(1 2)))";
char *s2 = "(L 1.(L 2.(1 (1 2))))";
char *s3 = "(L 1.(L 2.(1 (1 (1 2)))))";
char *ss = "(L 1.(L 2.(L 3.((1 2)(2 3)))))";
char *sa = "(L 1.(L 2.(L 3.(L 4.((1 3)((2 3) 4))))))";
char *sm = "(L 1.(L 2.(L 3.(1 (2 3)))))";
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

/* char *nisepred = "(L 1.(((L 2.(1 2))(L 2.(2 3)))(L 2.((L 3.2)(4 1)))))"; */
char *nisepred = "(L 1.((1 (L 2.(2 3)))(L 2.2)))";

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
  Lexp c0, c1, c2, c3, cs, ca, cm, cp;
  Lexp l0, l1;
  int i, cnt;

  setvbuf(stdout, NULL, _IONBF, 0);

  initpool();

  for (i = 0; i < 30; i++) {
    l0 = church_num(i);
    cp = Lstr2Lexp(nisepred);
    printf("nisepred "); printlexp(l0);
    l1 = Lappl(cp, l0);
    cnt = nbeta(l1, CANONICAL, 1000);
    printf("  -(%d times)-> ", cnt); printlexp(l1);
  }

  epoolinfo();

  return 0;
}
/* [EOF] */
