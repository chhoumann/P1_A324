CFLAGS = -ansi -Wall 

Madplan.exe: randomizer.o utility/utility.o utility/utility.h database/database.o database/database.h weekplan/weekplan.o weekplan/weekplan.h user_prefs/user_prefs.o user_prefs/user_prefs.h main.c
	gcc main.c utility/utility.o randomizer.o database/database.o weekplan/weekplan.o user_prefs/user_prefs.o $(CFLAGS)\
		-o Madplan.exe

randomizer.o: randomizer.c utility/utility.h
	gcc -c randomizer.c

utility/utility.o: utility/utility.c utility/utility.h
	gcc -c utility/utility.c \
		-o ./utility/utility.o

database/database.o: database/database.c database/database.h
	gcc -c database/database.c \
		-o ./database/database.o

weekplan/weekplan.o: weekplan/weekplan.c weekplan/weekplan.h utility/utility.h utility/utility.o
	gcc -c weekplan/weekplan.c utility/utility.o \
		-o ./weekplan/weekplan.o

user_prefs/user_prefs.o: user_prefs/user_prefs.c user_prefs/user_prefs.h utility/utility.h utility/utility.o
	gcc -c user_prefs/user_prefs.c utility/utility.o \
		-o user_prefs/user_prefs.o