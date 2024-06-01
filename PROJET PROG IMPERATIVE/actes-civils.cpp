#include <stdexcept>
/** @file **/
#include <iostream>
#include "tableau-donnees.hpp"
#include "tableau-donnees-avance.hpp"
#include "tableau-lecture-csv.hpp"
using namespace std;

/** Affiche l'année avec le plus de naissances et l'année avec le plus de mariages
 **/
int main() {
    try {
    vector<vector<string>> tab_acte_civil = litTableauCSV("donnees/statistiques_de_creation_d_actes_d_etat_civil_par_arrondissement.csv", 4); // on lit le fichier et on met ses données dans un tableau

    vector<vector<string>> tab_naissances = selectLignes(tab_acte_civil,0,"Naissances");
    vector<vector<string>> tab_mariages = selectLignes(tab_acte_civil,0,"Mariages");


    vector<string> tab_annees_naissances_fichier = distinct(tab_naissances,1);
    vector<int> nb_naissances_annees = groupByInt(tab_naissances,tab_annees_naissances_fichier,1,3);  //on calcule dans un tableau chacune des sommes de naissances de chaque année

    vector<string> tab_annees_mariages_fichier = distinct(tab_mariages,1);
    vector<int> nb_mariages_annees = groupByInt(tab_mariages,tab_annees_mariages_fichier,1,3);  //on calcule dans un tableau chacune des sommes de mariages de chaque année



    cout << "L'année comptant le plus de naissances est l'année " << tab_annees_naissances_fichier[indiceMax(nb_naissances_annees)] << " et l'année dénombrant le plus de mariages est l'année " << tab_annees_mariages_fichier[indiceMax(nb_mariages_annees)] << "." << endl;  // et on renvoie le nombre de naissances/mariages correspondant à l'indice de l'année comptant le plus de naissances/mariages
    } catch (exception & e) {
        cout << "Erreur, les données fournies ne permettent pas l'éxécution normale du programme !";
    }
}

