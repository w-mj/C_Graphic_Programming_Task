main: calculator.o operator.o queue.o stack.o main.o node.o tree.o debug.o
	gcc -o main.run calculator.o operator.o queue.o stack.o main.o node.o tree.o debug.o -lm -g -lSDL2 -lSDL2_ttf

tree.o:tree.c tree.h
	gcc -c tree.c

node.o:node.c node.h
	gcc -c node.c

debug.o:debug.c debug.h
	gcc -c debug.c

calculator.o: calculator.c calculator.h operator.h queue.h stack.h tree.h
	gcc -c calculator.c -lm

operator.o: operator.c operator.h 
	gcc -c operator.c

queue.o: queue.c queue.h
	gcc -c queue.c

stack.o: stack.c stack.h
	gcc -c stack.c

main.o: main.c
	gcc -c main.c -lSDL2 -lSDL2_ttf -lm 

clean:
	rm *.o
