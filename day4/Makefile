clean:
	rm ./bin/**

c:
	gcc -Wall -Wextra -Werror -g -fanalyzer -fsanitize=undefined,address main.c -o ./bin/mainc
	./bin/mainc puzzle.txt
