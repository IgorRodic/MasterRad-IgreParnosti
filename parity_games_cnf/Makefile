PROGRAM   = parity_games_cnf
CXX       = g++-6
CXXFLAGS  = -g -std=c++1z -Wall

$(PROGRAM): graph.o jurdzinski.o formula.o parity_games_cnf.o
	$(CXX) -o $(PROGRAM) parity_games_cnf.o graph.o jurdzinski.o formula.o

graph.o: graph.cpp graph.h
	$(CXX) -c graph.cpp

jurdzinski.o: jurdzinski.cpp jurdzinski.h
	$(CXX) -c jurdzinski.cpp

formula.o: formula.cpp formula.h
	$(CXX) -c formula.cpp

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)


