CFLAGS = -Wall -coverage -fpic -lm

testme:
	gcc -o testme testme.c $(CFLAGS)
	-./testme
	gcov -fb testme.c
		
clean:
	rm -f testme *.o *.gcov *.gcda *.gcno
