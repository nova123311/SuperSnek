default:
	g++ -std=c++11 -Wall -o SuperSnek main.cpp board.cpp

debug:
	g++ -std=c++11 -Wall -g -o SuperSnek main.cpp board.cpp

clean: SuperSnek
	rm SuperSnek
