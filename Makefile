CC = g++
CFLAGS = -g -c -std=c++17 

all: parsertestexe

parsertestexe: CocoParser lexer.o parser.o query.o main.o
	$(CC) lexer.o parser.o query.o main.o -o parsertestexe

main.o: main.cpp	
	$(CC) main.cpp $(CFLAGS)

parser.o: Parser.cpp Parser.h
	$(CC) Parser.cpp $(CFLAGS) -o parser.o

lexer.o: Scanner.cpp Scanner.h
	$(CC) Scanner.cpp $(CFLAGS) -o lexer.o

query.o: Query.cpp Query.h
	$(CC) Query.cpp $(CFLAGS) -o query.o

CocoParser: dbgrammar.atg
	./Coco dbgrammar.atg

clean:
	rm -f parsertestexe *.o