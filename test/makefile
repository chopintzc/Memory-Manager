
CFLAGS= -Wall
LDFLAGS=-lpthread
TESTS=test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14 test15 test16 test17 test18 test19 test20 test21 test22 test23 test24 test25 test26

tests: $(TESTS)

test1: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test1 test1.o support.o my_memory.c $(LDFLAGS)

test2: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test2 test2.o support.o my_memory.c $(LDFLAGS)

test3: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test3 test3.o support.o my_memory.c $(LDFLAGS)

test4: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test4 test4.o support.o my_memory.c $(LDFLAGS)

test5: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test5 test5.o support.o my_memory.c $(LDFLAGS)

test6: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test6 test6.o support.o my_memory.c $(LDFLAGS)

test7: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test7 test7.o support.o my_memory.c $(LDFLAGS)

test8: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test8 test8.o support.o my_memory.c $(LDFLAGS)

test9: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test9 test9.o support.o my_memory.c $(LDFLAGS)

test10: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test10 test10.o support.o my_memory.c $(LDFLAGS)

test11: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test11 test11.o support.o my_memory.c $(LDFLAGS)

test12: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test12 test12.o support.o my_memory.c $(LDFLAGS)

test13: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test13 test13.o support.o my_memory.c $(LDFLAGS)

test14: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test14 test14.o support.o my_memory.c $(LDFLAGS)

test15: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test15 test15.o support.o my_memory.c $(LDFLAGS)

test16: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test16 test16.o support.o my_memory.c $(LDFLAGS)

test17: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test17 test17.o support.o my_memory.c $(LDFLAGS)

test18: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test18 test18.o support.o my_memory.c $(LDFLAGS)

test19: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test19 test19.o support.o my_memory.c $(LDFLAGS)

test20: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test20 test20.o support.o my_memory.c $(LDFLAGS)

test21: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test21 test21.o support.o my_memory.c $(LDFLAGS)

test22: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test22 test22.o support.o my_memory.c $(LDFLAGS)

test23: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test23 test23.o support.o my_memory.c $(LDFLAGS)

test24: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test24 test24.o support.o my_memory.c $(LDFLAGS)

test25: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test25 test25.o support.o my_memory.c $(LDFLAGS)

test26: my_memory.c my_memory.h
	gcc $(CFLAGS) -o test26 test26.o support.o my_memory.c $(LDFLAGS)

test: $(TESTS)
	echo "--------------------------------------"
	echo "Check for malloc calls.  Only one should be reported:"
	grep malloc my_memory.c
	-./test1 8
	-./test1 9
	-./test1 10
	-./test1 11
	-./test1 12
	-./test1 13
	-./test1 14
	-./test1 15
	-./test1 16
	-./test2
	-./test3
	-./test4
	-./test5
	-./test6
	-./test7
	-./test8
	-./test9
	-./test10
	-./test11
	-./test12
	-./test13
	-./test14
	-./test15
	-./test16
	-./test17
	-./test18
	-./test19
	-./test20
	-./test21
	-./test22
	-./test23
	-./test24
	-./test25
	-./test26

clean:
	-rm my_memory.o

spotless: clean
	-rm $(TESTS)

