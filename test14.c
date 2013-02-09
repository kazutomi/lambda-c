/*
 * test14.c - Church numerals
 *
 * $Id: test14.c,v 1.1 2002/05/14 17:18:40 tominaga Exp tominaga $
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

/*
(define _succ '(lambda . (1 . (lambda . (2 . (lambda . (3 . ((1 . 2) . (2 . 3)))))))))
(define _add '(lambda . (a . (lambda . (b . (lambda . (f . (lambda . (x . ((a . f) . ((b . f) . x)))))))))))
(define _mul '(lambda . (a . (lambda . (b . (lambda . (z . (a . (b . z)))))))))
(define _pred '(lambda 1 ((1 lambda 2 lambda 3 (3 lambda 26 lambda 27 ((2 lambda 7 lambda 8 . 7) . 26) 26 . 27) 2 lambda 9 lambda 10 . 9) lambda 3 3 lambda 33 . 33) lambda 14 lambda 15 . 15))
(define _0 '(lambda . (1 . (lambda . (2 . 2)))))
(define _1 '(lambda . (1 . (lambda . (2 . (1 . 2))))))
(define _2 '(lambda . (1 . (lambda . (2 . (1 . (1 . 2)))))))
(define _3 '(lambda . (1 . (lambda . (2 . (1 . (1 . (1 . 2))))))))
*/

  c0 = Lstr2Lexp(s0);
  c1 = Lstr2Lexp(s1);
  c2 = Lstr2Lexp(s2);
  c3 = Lstr2Lexp(s3);
  printf("0 = "); printlexp(c0);
  printf("1 = "); printlexp(c1);
  printf("2 = "); printlexp(c2);
  printf("3 = "); printlexp(c3);

  cs = Lstr2Lexp(ss);
  printf("succ = "); printlexp(cs);

  c0 = Lstr2Lexp(s0);
  l0 = Lappl(cs, c0);
  printf("succ 0 = "); printlexp(l0);

  cnt = nbeta(l0, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l0);

  c1 = Lstr2Lexp(s1);
  cs = Lstr2Lexp(ss);
  l1 = Lappl(cs, c1);
  printf("succ 1 = "); printlexp(l1);

  cnt = nbeta(l1, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l1);

  c2 = Lstr2Lexp(s2);
  c3 = Lstr2Lexp(s3);
  ca = Lstr2Lexp(sa);
  printf("add = "); printlexp(ca);

  l0 = Lappl(ca, c2);
  l1 = Lappl(l0, c3);
  printf("add 2 3 = "); printlexp(l1);

  cnt = nbeta(l1, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l1);
  
  c2 = Lstr2Lexp(s2);
  c3 = Lstr2Lexp(s3);
  cm = Lstr2Lexp(sm);
  l0 = Lappl(cm, c3);
  l1 = Lappl(l0, c2);
  printf("mul 3 2 = "); printlexp(l1);
  
  cnt = nbeta(l1, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l1);

  l0 =
    Lappl(Lstr2Lexp(ss), 
      Lappl(Lstr2Lexp(ss), 
	Lappl(Lstr2Lexp(ss), 
	  Lappl(Lstr2Lexp(ss), Lstr2Lexp(s0)))));

  printf("s s s s 0 = "); printlexp(l0);
  
  cnt = nbeta(l0, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l0);

  l0 =
    Lappl(Lappl(Lstr2Lexp(sm), Lappl(Lappl(Lstr2Lexp(sa), Lstr2Lexp(s1)), Lstr2Lexp(s2))),
      Lappl(Lappl(Lstr2Lexp(sa), Lstr2Lexp(s2)), Lstr2Lexp(s3)));
  printf("(1 + 2) * (2 + 3) = "); printlexp(l0);

  cnt = nbeta(l0, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l0);

  c3 = Lstr2Lexp(s3);
  cp = Lstr2Lexp(sp);
  l1 = Lappl(cp, c3);
  printf("pred 3 = "); printlexp(l1);

  cnt = nbeta(l1, CANONICAL, 100);
  printf("  -> reduced (%d steps) to ", cnt);
  printlexp(l1);

  l0 = church_num(12);
  printf("12 = "); printlexp(l0);

  for (i = 0; i < 30; i++) {
    l0 = church_num(i);
    cp = Lstr2Lexp(sp);
    printf("pred "); printlexp(l0);
    l1 = Lappl(cp, l0);
    cnt = nbeta(l1, CANONICAL, 1000);
    printf("  -(%d times)-> ", cnt); printlexp(l1);
  }

  epoolinfo();

  return 0;
}
/* [EOF] */
