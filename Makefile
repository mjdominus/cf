
# CFLAGS=-DDEBUG_COEFFS  -DDEBUG_VARARGS
# CFLAGS=-g
# LDFLAGS=-g
LIBS=-lm

default: cf-demo between surd surd2 t/appendtest t/arraytest doc

HDRS= cf.h  cf_append.h cf_arith.h cf_between.h cf_float.h  cf_holo.h  cf_it.h  cf_rat.h cf_per.h
SRCS= cf.c  cf_append.c cf_arith.c cf_between.c cf_float.c  cf_holo.c  cf_it.c  cf_rat.c cf_per.c main.c
MANS= man/demo.1 man/libcf.3

doc: man/demo.ps man/demo.pdf

cf.o: cf.c cf.h
cf_append.o: cf_append.c cf_append.h cf.h
cf_arith.o: cf_arith.c cf_arith.h cf.h
cf_between.o: cf_between.c cf_between.h cf.h
cf_float.o: cf_float.c cf_float.h cf.h
cf_holo.o: cf_holo.c cf_holo.h cf.h
cf_it.o: cf_it.c cf_it.h cf.h
cf_per.o: cf_per.c cf_per.h cf.h
cf_rat.o: cf_rat.c cf_rat.h cf.h

cf-demo: main.o libcf.a
	$(CC) $(LDFLAGS) -o cf-demo $^ $(LIBS)

between: between.o libcf.a
	$(CC) $(LDFLAGS) -o between $^ $(LIBS)

t/arraytest: t/arraytest.o libcf.a
	$(CC) $(LDFLAGS) -o t/arraytest $^ $(LIBS)

surd: surd.o libcf.a
	$(CC) $(LDFLAGS) -o surd $^ $(LIBS)

surd2: surd2.o libcf.a
	$(CC) $(LDFLAGS) -o surd2 $^ $(LIBS)

t/appendtest: t/appendtest.o libcf.a
	$(CC) $(LDFLAGS) -o t/appendtest $^ $(LIBS)

libcf.a: cf.o cf_append.o cf_arith.o cf_between.o cf_rat.o cf_float.o cf_holo.o cf_arith.o cf_it.o cf_per.o
	ar cr libcf.a $?
	ranlib libcf.a

main.o: main.c cf.h cf_rat.h cf_float.h cf_holo.h cf_arith.h cf_it.h cf_per.h
between.o: between.c cf_between.h cf_rat.h cf.h
arraytest.o: arraytest.c cf_per.h

clean: 
	rm -f *.o

veryclean: clean
	rm -f *.a demo cf.tgz


dist: cf.tgz

cf.tgz: $(SRCS) $(HDRS) $(MANS) Makefile
	tar czf cf.tgz *.[ch1-9] Makefile

man/demo.ps: man/demo.1
	groff -man man/demo.1 > man/demo.ps

man/demo.pdf: man/demo.ps
	ps2pdf man/demo.ps && mv demo.pdf man

