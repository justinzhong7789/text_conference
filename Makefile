all:test.c
	gcc -Wall test.c -o test
clean:
	rm test