# makefile

program: main.o
	mpicc -std=c99 main.o -o main
	mpirun -n 5 ./main

main.o: main.c
	mpicc -lcrypt -std=c99 -c main.c
val:
	valgrind mpirun -n 5 ./main

clean:
	rm -f *.o
	rm -f main
	rm main
	clear

