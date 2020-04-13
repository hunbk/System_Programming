long : main.o copy.o mygets.o
	gcc -o long main.o copy.o mygets.o

main.o : main.c longest.h
	gcc -c main.c

copy.o : copy.c longest.h
	gcc -c copy.c

mygets.o : mygets.c longest.h
	gcc -c mygets.c
