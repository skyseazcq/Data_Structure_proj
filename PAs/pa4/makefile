
all: test

test: main.o boggleplayer.o
	g++ -std=c++0x main.o boggleplayer.o -o test

main.o: main.cpp boggleplayer.h
	g++ -c -g -std=c++0x main.cpp -o main.o

boggleplayer.o: boggleplayer.cpp boggleplayer.h
	g++ -std=c++0x -c -g boggleplayer.cpp -o boggleplayer.o

boggleplayer.hpp: baseboggleplayer.h boggleutil.h
	
clean:
	rm *.o test
