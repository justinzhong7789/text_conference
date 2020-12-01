all:test.c
	gcc -Wall test.c -o test
debug:
	gcc -Wall -g3 test.c -o test
clean:
	rm test