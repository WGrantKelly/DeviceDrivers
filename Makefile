default: library.c driver.c fix.c
	gcc -g library.c driver.c -o driver
	gcc library.c fix.c -o fix
clean :
	rm driver fix driver.o fix.o library.o

	