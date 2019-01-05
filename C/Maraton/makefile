FLAGS=-Wall -Wextra -std=c11 -O2

all: main

main:tree.o main.c
	gcc $(FLAGS) main.c tree.o list.o -o main

tree.o:list.o tree.c tree.h
	gcc $(FLAGS) tree.c -c

list.o: list.c list.h
	gcc $(FLAGS) list.c -c

clean:
	rm list.o tree.o main
