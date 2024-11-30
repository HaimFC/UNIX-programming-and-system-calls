clean: OS
	rm *.o
OS: a.o
	gcc -o OS a.o
a.o: main.c
	gcc -c main.c -o a.o
