
#CFLAGS=-DDEBUG_IO
LIBS=-lm

default: demo

cf.o: cf.c cf.h
cf_rat.o: cf_rat.c cf_rat.h cf.h
cf_float.o: cf_float.c cf_float.h cf.h
cf_holo.o: cf_holo.c cf_holo.h cf.h
cf_arith.o: cf_arith.c cf_arith.h cf.h

demo: main.o libcf.a
	$(CC) -o demo $^ $(LIBS)


libcf.a: cf.o cf_rat.o cf_float.o cf_holo.o cf_arith.o
	ar cr libcf.a $?

main.o: main.c cf.h cf_rat.h cf_float.h cf_holo.h cf_arith.h

clean: 
	rm -f *.o

veryclean: clean
	rm -f *.a demo
