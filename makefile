default:
	g++ -std=c++11 -Wall -o SuperSnek main.cpp board.cpp

clean: SuperSnek
	rm SuperSnek
