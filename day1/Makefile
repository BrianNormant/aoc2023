clean:
	rm ./bin/**

c:
	gcc -Wall -Wextra -Werror -pedantic -g -fsanitize=undefined,address main.c -o ./bin/mainc
	./bin/mainc puzzle.txt
