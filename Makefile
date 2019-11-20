file : hw2.o
	gcc -o file hw2.o
hw2.o : hw2.c
	gcc -c -o hw2.o hw2
clean :
	rm *.o file
