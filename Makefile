CC = g++
CFLAGS = -g -std=c++17

all: CocoParser 
	$(CC) *.cpp -o parsertestexe $(CFLAGS) 

CocoParser:
	./Coco dbgrammar.atg

clean:
	rm -f parsertestexe