clean:
	rm ./bin/**

range:
	gcc -Wall -Wextra -Wpedantic -g -c range.c -o ./bin/range.o

c: range
	gcc -Wall -Wextra -pedantic -g -fsanitize=undefined,address main.c ./bin/range.o -o ./bin/mainc
	./bin/mainc puzzle.txt
