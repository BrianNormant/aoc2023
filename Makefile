clean:
	rm ./bin/**
	rm ./build/**

c:
	gcc -Wall -Wextra -pedantic -lm -g -fsanitize=undefined,address main.c -o ./bin/mainc
	./bin/mainc puzzle.txt

elixir:
	elixirc main.ex -o ./bin

idris:
	idris2 main.idr -o ./a.out
	./build/exec/a.out
