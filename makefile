default:
	g++ -std=c++11 -Wall -O2 -o SuperSnek main.cpp board.cpp

debug:
	g++ -std=c++11 -Wall -g -o SuperSnek main.cpp board.cpp

profile:
	g++ -std=c++11 -Wall -O2 -pg -o SuperSnek main.cpp board.cpp

clean: SuperSnek
	rm SuperSnek gmon.out profile.txt
