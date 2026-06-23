OBJS = cg.o main.o
BIN = out

all: $(BIN)

$(BIN): cg.o main.o
	gcc main.o cg.o -o $(BIN) -lm

cg.o: cg.c cg.h
	gcc -c cg.c -o cg.o

main.o: main.c cg.h
	gcc -c main.c -o main.o

run: all
	./$(BIN)

clean:
	rm -f *.o $(BIN) *.ppm
