CFLAGS = -g 
LDFLAGS = -lm

test: memory_lib.c 
	gcc $(CFLAGS) -o test 

clean:
	-rm test