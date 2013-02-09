#
# Makefile - makefile for lambda-c library
#
# $Id: Makefile,v 1.12 2002/05/10 04:02:06 tominaga Exp tominaga $
#

SHELL = /bin/sh
CC = gcc
#CFLAGS = -g -Wall
#LDFLAGS = -g
CFLAGS = -g -Wall -pg
LDFLAGS = -g -pg

#
# files
#

SA = liblambda.a
SO = liblambda.so

TOOLS = regress do-test compare-test-results

HDR = lambda.h \
	lexp.h \
	const.h \
	global.h \
	misc.h \
	parser.h \
	proto.h \
	debug.h \

SRC = global.c \
	message.c \
	pool.c \
	ilambda.c \
	strlexp.c \
	parser.c \
	lambops.c \
	diff.c \

OBJ = $(SRC:.c=.o)

TESTSRC = test1.c \
	test2.c \
	test3.c \
	test4.c \
	test5.c \
	test6.c \
	test7.c \
	test8.c \
	test9.c \
	test10.c \
	test11.c \
	test12.c \
	test13.c \
	test14.c \
	test15.c \
	test16.c \
	test17.c \
	test18.c \
	test19.c \
	test20.c \
	test21.c \
	test22.c \
	test23.c \

TESTOBJ = $(TESTSRC:.c=.o)

TESTOUTS = $(TESTSRC:.c=.out)

TESTDS = $(TESTSRC:.c=)
TESTSS = $(TESTSRC:.c=-s)

ALLSRC = Makefile $(TOOLS) $(HDR) $(SRC) $(TESTSRC)

#
# dependencies and commands
#

all: $(SA) $(SO) $(TESTDS) $(TESTSS)

static: $(TESTSS)

$(SA): $(OBJ)
	ar r $(SA) $(OBJ)

$(SO): $(OBJ)
	ld -o $(SO) $(OBJ) -shared

$(TESTDS): $(SO) $$@.o
	$(CC) $(LDFLAGS) $@.o -o $@ -L. -llambda

$(TESTSS): $(SA) $$(@:-s=).o
	$(CC) -static $(LDFLAGS) $(@:-s=).o -o $@ -L. -llambda

# traditional FRC entries; to force make, do 'make FRC=FRC'
$(OBJ): $(FRC)
$(FRC):

# individual test
$(TESTOUTS): $$(@:.out=)
	LD_LIBRARY_PATH=. ./regress $*

# do all tests
reg: $(TESTDS)
	LD_LIBRARY_PATH=. ./regress $(TESTDS)

sreg: $(TESTSS)
	./regress $(TESTSS)

ci:
	ci -l $(ALLSRC)

diff:
	- rcsdiff -c $(ALLSRC)

clean:
	rm -f $(OBJ) $(TESTOBJ)

checkmacros:
	@ echo '$$(TESTDS)' is $(TESTDS)

tags: $(HDR) $(SRC)
	ctags $(HDR) $(SRC)

depend:
	makedepend $(HDR) $(SRC) $(TESTSRC)

# DO NOT DELETE

lambda.o: const.h misc.h global.h lexp.h /usr/include/machine/limits.h
lambda.o: parser.h debug.h proto.h /usr/include/stdio.h
lambda.o: /usr/include/sys/cdefs.h /usr/include/machine/ansi.h
lambda.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
lambda.o: /usr/include/machine/types.h /usr/include/machine/endian.h
lexp.o: /usr/include/machine/limits.h
global.o: lexp.h /usr/include/machine/limits.h parser.h debug.h
global.o: lexp.h /usr/include/machine/limits.h parser.h debug.h
message.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
message.o: /usr/include/machine/ansi.h /usr/include/stdlib.h
message.o: /usr/include/stdarg.h lambda.h const.h misc.h global.h lexp.h
message.o: /usr/include/machine/limits.h parser.h debug.h proto.h
message.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
message.o: /usr/include/machine/types.h /usr/include/machine/endian.h
pool.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
pool.o: /usr/include/machine/ansi.h /usr/include/stdlib.h
pool.o: /usr/include/string.h /usr/include/assert.h lambda.h const.h misc.h
pool.o: global.h lexp.h /usr/include/machine/limits.h parser.h debug.h
pool.o: proto.h /usr/include/sys/types.h /usr/include/sys/inttypes.h
pool.o: /usr/include/machine/types.h /usr/include/machine/endian.h
ilambda.o: lambda.h const.h misc.h global.h lexp.h
ilambda.o: /usr/include/machine/limits.h parser.h debug.h proto.h
ilambda.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
ilambda.o: /usr/include/machine/ansi.h /usr/include/sys/types.h
ilambda.o: /usr/include/sys/inttypes.h /usr/include/machine/types.h
ilambda.o: /usr/include/machine/endian.h
strlexp.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
strlexp.o: /usr/include/machine/ansi.h /usr/include/string.h
strlexp.o: /usr/include/assert.h lambda.h const.h misc.h global.h lexp.h
strlexp.o: /usr/include/machine/limits.h parser.h debug.h proto.h
strlexp.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
strlexp.o: /usr/include/machine/types.h /usr/include/machine/endian.h
parser.o: /usr/include/stdlib.h /usr/include/sys/cdefs.h
parser.o: /usr/include/machine/ansi.h /usr/include/limits.h
parser.o: /usr/include/sys/_posix.h /usr/include/machine/limits.h
parser.o: /usr/include/sys/syslimits.h /usr/include/assert.h
parser.o: /usr/include/ctype.h /usr/include/runetype.h lambda.h const.h
parser.o: misc.h global.h lexp.h parser.h debug.h proto.h
parser.o: /usr/include/stdio.h /usr/include/sys/types.h
parser.o: /usr/include/sys/inttypes.h /usr/include/machine/types.h
parser.o: /usr/include/machine/endian.h
lambops.o: /usr/include/assert.h /usr/include/sys/cdefs.h lambda.h const.h
lambops.o: misc.h global.h lexp.h /usr/include/machine/limits.h parser.h
lambops.o: debug.h proto.h /usr/include/stdio.h /usr/include/machine/ansi.h
lambops.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
lambops.o: /usr/include/machine/types.h /usr/include/machine/endian.h
diff.o: lambda.h const.h misc.h global.h lexp.h /usr/include/machine/limits.h
diff.o: parser.h debug.h proto.h /usr/include/stdio.h
diff.o: /usr/include/sys/cdefs.h /usr/include/machine/ansi.h
diff.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
diff.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test1.o: lambda.h const.h misc.h global.h lexp.h
test1.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test1.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test1.o: /usr/include/machine/ansi.h /usr/include/sys/types.h
test1.o: /usr/include/sys/inttypes.h /usr/include/machine/types.h
test1.o: /usr/include/machine/endian.h
test2.o: lambda.h const.h misc.h global.h lexp.h
test2.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test2.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test2.o: /usr/include/machine/ansi.h /usr/include/sys/types.h
test2.o: /usr/include/sys/inttypes.h /usr/include/machine/types.h
test2.o: /usr/include/machine/endian.h
test3.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test3.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test3.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test3.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test3.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test4.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test4.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test4.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test4.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test4.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test5.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test5.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test5.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test5.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test5.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test6.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test6.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test6.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test6.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test6.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test7.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test7.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test7.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test7.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test7.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test8.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test8.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test8.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test8.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test8.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test9.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test9.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test9.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test9.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test9.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test10.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test10.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test10.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test10.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test10.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test11.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test11.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test11.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test11.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test11.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test12.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test12.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test12.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test12.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test12.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test13.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test13.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test13.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test13.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test13.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test14.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test14.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test14.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test14.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test14.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test15.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test15.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test15.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test15.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test15.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test16.o: lambda.h const.h misc.h global.h lexp.h
test16.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test16.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test16.o: /usr/include/machine/ansi.h /usr/include/sys/types.h
test16.o: /usr/include/sys/inttypes.h /usr/include/machine/types.h
test16.o: /usr/include/machine/endian.h
test17.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test17.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test17.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test17.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test17.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test18.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test18.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test18.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test18.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test18.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test19.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test19.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test19.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test19.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test19.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test20.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test20.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test20.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test20.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test20.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test21.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test21.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test21.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test21.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test21.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test22.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test22.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test22.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test22.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test22.o: /usr/include/machine/types.h /usr/include/machine/endian.h
test23.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
test23.o: /usr/include/machine/ansi.h lambda.h const.h misc.h global.h lexp.h
test23.o: /usr/include/machine/limits.h parser.h debug.h proto.h
test23.o: /usr/include/sys/types.h /usr/include/sys/inttypes.h
test23.o: /usr/include/machine/types.h /usr/include/machine/endian.h
