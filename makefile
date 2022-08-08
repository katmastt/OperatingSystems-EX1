ask1: ask1.o creation.o pr_co.o

creation.o : creation.c mytypes.h
	cc -c creation.c
pr_co.o : pr_co.c mytypes.h
	cc -c pr_co.c
ask1.o : ask1.c mytypes.h
	cc -c ask1.c
clean :
	rm ask1 creation.o pr_co.o ask1.o

