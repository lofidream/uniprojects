main.exe: main.o
	g++ main.o -o main.exe 

main.o: main.cpp multiset.h
	g++ -Wall -O0 -c -std=c++0x main.cpp -o main.o

.PHONY: clean
clean:
	rm -f *.o *.exe
