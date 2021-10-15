# makefile

program: main.o
	mpicc -std=c99 -lcrypt main.o -o main
	mpirun -n 15 ./main

main.o: main.c
	mpicc  -std=c99 -c -lcrypt main.c
val:
	valgrind mpirun -n 5 ./main

clean:
	rm -f *.o
	rm -f main
	rm main
	clear

