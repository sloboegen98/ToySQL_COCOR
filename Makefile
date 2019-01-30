all:
	g++ *.cpp -o parsertestexe $(CFLAGS) 

clean:
	rm -f parsertestexe