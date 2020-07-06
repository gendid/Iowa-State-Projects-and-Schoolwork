objects = shell.o

shell : $(objects)
	cc -o shell $(objects)

shell.o : shell.c
	cc -c shell.c
clean :
	rm shell $(objects)
