#Eloy Salinas
#eloys@ucsc.edu | 1243517
#Pa2
# Makefile for Programming Assignment 2
Shuffle : List.o Shuffle.o
	gcc -o Shuffle Shuffle.o List.o
Shuffle.o : List.h Shuffle.c
	gcc -c -ansi -std=c99 -Wall Shuffle.c
ListTest: List.o ListTest.o
	gcc -o ListTest ListTest.o List.o
ListTest.o : List.h ListTest.c
	gcc -c -ansi -std=c99 -Wall ListTest.c
List.o : List.h List.c
	gcc -c -ansi -std=c99 -Wall List.c
clean :
	rm -f Shuffle ListTest Shuffle.o ListTest.o List.o
