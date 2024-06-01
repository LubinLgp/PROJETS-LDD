#include <stdexcept>
/** @file **/
#include <iostream>
#include "tableau-donnees.hpp"
#include "tableau-donnees-avance.hpp"
#include "tableau-lecture-csv.hpp"
using namespace std;

/** Affiche la voiture qui pollue le plus et celle 
 *  qui pollue le moins
 **/
int main() {

    vector<vector<string>> tab_voiture = litTableauCSV("donnees/voiture(2001-2007).csv",3);

    int indice_consommation_plus = indiceMax(conversionInt(colonne(tab_voiture,2)));


    cout << "La voiture la plus polluante vendue entre 2001 et 2007 est le modèle " << tab_voiture[indice_consommation_plus][1] << " de la marque " << tab_voiture[indice_consommation_plus][0] << "." << endl;

    int indice_consommation_moins = indiceMin(conversionInt(colonne(tab_voiture,2)));

    cout << "La voiture la moins polluante vendue entre 2001 et 2007 est le modèle " << tab_voiture[indice_consommation_moins][1] << " de la marque " << tab_voiture[indice_consommation_moins][0] << "." << endl;

    
    
    vector<string> tab_marques = distinct(tab_voiture,0);

    vector<int> moyennes_marques = groupByInt(tab_voiture,tab_marques,0,2);
    int indice_conso_moyenne_plus = indiceMax(moyennes_marques);

    cout << "La marque qui a produit en moyenne les véhicules les plus polluants est " << tab_marques[indice_conso_moyenne_plus] << "." << endl;

    int indice_conso_moyenne_moins = indiceMin(moyennes_marques);

    cout << "La marque qui a produit en moyenne les véhicules les moins polluants est " << tab_marques[indice_conso_moyenne_moins] << "." << endl;



    return 0;

    
    
}