CXX=g++
CXXFLAGS= -Wall -std=c++11 -g -O3
EXEC_FILES= coord

#all: Ensemble-main
# Regle generique
%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
coord: coord.o
	$(CXX) -o $@ $^ $(LDFLAGS)
coordv2: coordv2.o
	$(CXX) -o $@ $^ $(LDFLAGS)
grille: grille.o
	$(CXX) -o $@ $^ $(LDFLAGS)
grillev2: grillev2.o
	$(CXX) -o $@ $^ $(LDFLAGS)
termite: termite.o
	$(CXX) -o $@ $^ $(LDFLAGS)
termitev2: termitev2.o
	$(CXX) -o $@ $^ $(LDFLAGS)

projet: projet.o
	$(CXX) -o $@ $^ $(LDFLAGS)
projetv2: projetv2.o
	$(CXX) -o $@ $^ $(LDFLAGS)

tests: test.o coord.o grille.o termite.o
	g++ -o tests test.o coord.o grille.o termite.o

coord.o : coord.cpp coord.hpp
coordv2.o : coordv2.cpp coordv2.hpp
grille.o : grille.cpp grille.hpp coord.cpp coord.hpp
grillev2.o : grillev2.cpp grillev2.hpp coordv2.cpp coordv2.hpp
termite.o : termite.cpp termite.hpp coord.cpp coord.hpp grille.cpp grille.hpp
#termitev2.o : termitev2.cpp termitev2.hpp coordv2.cpp coordv2.hpp grillev2.cpp grillev2.hpp
projet.o : grille.cpp grille.hpp coord.cpp coord.hpp
projetv2.o : grillev2.cpp grillev2.hpp coordv2.cpp coordv2.hpp
check : tests
	./tests
clean:
	rm -f *.o $(EXEC_FILES)