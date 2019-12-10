CFLAGS = -ansi -Wall

Madplan.exe: randomizer.o utility/utility.o utility/utility.h main.c
	gcc randomizer.o main.c utility/utility.o $(CFLAGS)\
		-o Madplan.exe

randomizer.o: randomizer.c utility/utility.h
	gcc -c randomizer.c

utility/utility.o: utility/utility.c utility/utility.h
	gcc -c utility/utility.c