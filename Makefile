clean:
	rm ./bin/**

c:
	gcc -Wall -Wextra -pedantic -lm -g -fsanitize=undefined,address main.c -o ./bin/mainc
	./bin/mainc puzzle.txt

elixir:
	elixirc main.ex -o ./bin
