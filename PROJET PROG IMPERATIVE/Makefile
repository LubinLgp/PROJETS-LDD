CXXFLAGS=-Wall -pedantic -std=c++11 -g -Wno-sign-compare
LINK.o = $(LINK.cc)

PROGRAMS= mariage-total mariage-samedi \
    parapluie \
    qualiteair \
	prenoms \
	mariage-complet \
	prenoms-tableau \
	tableau-donnees-test tableau-lecture-test \
	prenoms-tableau-2 mariage-complet-2 \
	tableau-donnees-avance-test mariage-complet-3 \
	prenoms-tableau-avance \
	getline-exemple prenoms-csv \
	tableau-lecture-csv-test \
	arbres-hauteur \
    arbres-platanus \
	actes-civils \
    frequentation-metro-rer \

all: $(PROGRAMS)

tableau-donnees-test:     tableau-donnees.o tableau-donnees-test.o
tableau-lecture-test:     tableau-lecture.o tableau-lecture-test.o
tableau-donnees-avance-test: tableau-donnees-avance.o tableau-donnees.o tableau-donnees-avance-test.o
tableau-lecture-csv-test: tableau-donnees.o tableau-lecture-csv.o tableau-lecture-csv-test.o

prenoms-tableau-2: prenoms-tableau-2.o tableau-donnees.o tableau-lecture.o
mariage-complet-2: mariage-complet-2.o tableau-donnees.o tableau-lecture.o

mariage-complet-3: mariage-complet-3.o tableau-donnees.o tableau-donnees-avance.o tableau-lecture.o  #ici on a rajouté le "tableau-donnees.o" sinon la compilation ne fonctionnait pas
prenoms-tableau-avance: prenoms-tableau-avance.o tableau-donnees.o tableau-donnees-avance.o tableau-lecture.o

arbres-hauteur: tableau-lecture-csv.o tableau-donnees.o arbres-hauteur.o
arbres-platanus: tableau-lecture-csv.o tableau-donnees.o tableau-donnees-avance.o arbres-platanus.o
actes-civils: tableau-lecture-csv.o tableau-donnees.o tableau-donnees-avance.o actes-civils.o

parapluie: tableau-lecture-csv.o
qualiteair: tableau-lecture-csv.o
frequentation-metro-rer: tableau-lecture-csv.o tableau-donnees.o tableau-donnees-avance.o
voitures: tableau-lecture-csv.o tableau-donnees.o tableau-donnees-avance.o

# prenoms et prenoms-csv doivent etre testees à la main

test: all
	@testeq() { echo -n "Cohérence $$1 $$2:\t"; if [ "`$$1`" = "`$$2`" ]; then echo "OK"; else echo "FAILED"; fi }; \
	testeq ./mariage-complet ./mariage-complet-2
	@testeq() { echo -n "Cohérence $$1 $$2:\t"; if [ "`$$1`" = "`$$2`" ]; then echo "OK"; else echo "FAILED"; fi }; \
	testeq ./mariage-complet-2 ./mariage-complet-3
	./tableau-donnees-test
	./tableau-donnees-avance-test
	./tableau-lecture-test
	./tableau-lecture-csv-test

doc:
	doxygen -g
	doxygen

clean:
	-rm *.o
	-rm $(PROGRAMS)
