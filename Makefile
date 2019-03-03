all: prim

hash: hash.o
	gcc -o hash hash.o

hash.o: hash.c
	gcc -c hash.c

prim: prim.o
	gcc -o prim prim.o

prim.o: prim.c
	gcc -c prim.c

chain: chain.o
	gcc -o chain chain.o

chain.o: chain.c
	gcc -c chain.c
    
test: test.o
	gcc -o test test.o

test.o: test.c
	gcc -c test.c
    
clean:
	rm -rf hash prim chain
	rm -rf *.o
