#include <stdexcept>
/** @file **/
#include <iostream>
#include "tableau-donnees.hpp"
#include "tableau-donnees-avance.hpp"
#include "tableau-lecture-csv.hpp"
using namespace std;

/** Affiche le nombre d'arbres du genre Platanus, et le nombre d'espèces
 *  différentes pour ce genre Platanus, parmi les "arbres remarquables"
 **/
int main() {
    vector<vector<string>> tab_arbres = litTableauCSV("donnees/arbresremarquablesparis.csv", 19);  // on lit le fichier et on met ses données dans un tableau
    

    vector<vector<string>> tab_platanus_remarquables = selectLignes(selectLignes(selectLignes(tab_arbres,5,"Arbre"),17,"OUI"),2,"Platanus");  // on sélectionne les lignes qui sont des arbres remarquables (ils le sont tous dans ce fichier mais comme ça on est assuré qu'on a bien seulement les arbres remarquables) du genre "Platanus"
    int nb_platanus_remarquables = tab_platanus_remarquables.size();

    vector<string> especes_differentes_platanus = distinct(tab_platanus_remarquables,3);  // un appel à distinct nous permet de récupérer toutes les espèces différentes de "platanus"

    cout << "Il y a " << nb_platanus_remarquables << " arbres du genre Platanus à Paris, et on compte " << especes_differentes_platanus.size() << " espèces différentes de Platanus dans Paris." << endl; //on a alors seulement à renvoyer la taille du tableau obtenu avec distinct

    return 0;
}

