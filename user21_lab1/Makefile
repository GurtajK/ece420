demo: matrixmult.o lab1_IO.o timer.h
	gcc -o demo matrixmult.o lab1_IO.o timer.h -lpthread -lm

matrixmult.o: matrixmult.c
	gcc -c matrixmult.c

lab1_IO.o: lab1_IO.c
	gcc -c lab1_IO.c

clean:
	rm -f demo matrixmult.o lab1_IO.o