CFLAGS = -ansi -Wall

Madplan.exe: randomizer.o utility/utility.o utility/utility.h database/database.o database/database.h main.c
	gcc main.c utility/utility.o randomizer.o database/database.o $(CFLAGS)\
		-o Madplan.exe

randomizer.o: randomizer.c utility/utility.h
	gcc -c randomizer.c

utility/utility.o: utility/utility.c utility/utility.h
	gcc -c utility/utility.c -o ./utility/utility.o

database/database.o: database/database.c database/database.h
	gcc -c database/database.c -o ./database/database.o

weekplan/weekplan.o: weekplan/weekplan.c weekplan/weekplan.h
	gcc -c weekplan/weekplan.c -o ./weekplan/weekplan.o

clean:
	rm -f weekplan/weekplan.o database/database.o utility/utility.o randomizer.o