
#CFLAGS=-DDEBUG
LIBS=-lm

default: demo

HDRS= cf.h  cf_arith.h  cf_float.h  cf_holo.h  cf_it.h  cf_rat.h
SRCS= cf.c  cf_arith.c  cf_float.c  cf_holo.c  cf_it.c  cf_rat.c  main.c
MANS= demo.1 libcf.3

cf.o: cf.c cf.h
cf_arith.o: cf_arith.c cf_arith.h cf.h
cf_float.o: cf_float.c cf_float.h cf.h
cf_holo.o: cf_holo.c cf_holo.h cf.h
cf_it.o: cf_it.c cf_it.h cf.h
cf_rat.o: cf_rat.c cf_rat.h cf.h

demo: main.o libcf.a
	$(CC) -o demo $^ $(LIBS)


libcf.a: cf.o cf_rat.o cf_float.o cf_holo.o cf_arith.o cf_it.o
	ar cr libcf.a $?

main.o: main.c cf.h cf_rat.h cf_float.h cf_holo.h cf_arith.h cf_it.h

clean: 
	rm -f *.o

veryclean: clean
	rm -f *.a demo cf.tgz


dist: cf.tgz

cf.tgz: $(SRCS) $(HDRS) $(MANS) Makefile
	tar czf cf.tgz *.[ch1-9] Makefile
