#include <stdexcept>
/** @file **/
#include <iostream>
#include "tableau-donnees.hpp"
#include "tableau-donnees-avance.hpp"
#include "tableau-lecture-csv.hpp"
using namespace std;

/** Affiche l'arrondissement parisien ayant eu le plus
 *  de fréquentation dans ses station de métro / RER
 **/
int main() {

    vector<vector<string>> tab_frequentation_transports = litTableauCSV("donnees/trafic-annuel-entrant-par-station-du-reseau-ferre-2021.csv", 11);  // on lit le fichier et on met ses données dans un tableau

    vector<vector<string>> tab_metro = selectLignes(tab_frequentation_transports,1,"Métro");  // on sélectionne uniquement les lignes qui référencent le métro
    int indice_station_metro_plus_frequentee = indiceMax(conversionInt(colonne(tab_metro,3)));  // on cherche l'indice de la ligne (du tableau) qui a le plus de trafic gra^ce à indiceMax


    cout << "La station de métro la plus fréquentée à Paris en 2021 était la station "<<tab_metro[indice_station_metro_plus_frequentee][2]<< " située dans l'arrondissement numéro " << tab_metro[indice_station_metro_plus_frequentee][10] << endl; // on renvoie la valeur du tableau tab_metro à la ligne trouvée avec indiceMax à la colonne correspondant à la station et à la colonne correspondant à l'arrondissement



    vector<vector<string>> tab_RER = selectLignes(tab_frequentation_transports,1,"RER");  // on sélectionne uniquement les lignes qui référencent le RER
    int indice_station_RER_plus_frequentee = indiceMax(conversionInt(colonne(tab_RER,3)));  // on cherche l'indice de la ligne (du tableau) qui a le plus de trafic gra^ce à indiceMax

    cout << "La station de RER la plus fréquentée à Paris en 2021 était la station " << tab_RER[indice_station_RER_plus_frequentee][2] << " située dans l'arrondissement numéro " << tab_RER[indice_station_RER_plus_frequentee][10] << endl;  // on renvoie la valeur du tableau tab_rer à la ligne trouvée avec indiceMax à la colonne correspondant à la station et à la colonne correspondant à l'arrondissement



    return 0;
}
