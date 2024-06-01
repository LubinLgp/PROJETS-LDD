#include <stdexcept>
/** @file **/
#include <iostream>
#include "tableau-donnees.hpp"
#include "tableau-lecture-csv.hpp"
using namespace std;

/** Affiche le genre et l'espece de l'arbre le plus haut de Paris
 * parmi les "arbres remarquables"
 **/
int main() {
    vector<vector<string>> tab_arbres = litTableauCSV("donnees/arbresremarquablesparis.csv", 19); // on lit le fichier et on met ses données dans un tableau
    

    vector<vector<string>> tab_arbres_remarquables = selectLignes(selectLignes(tab_arbres,5,"Arbre"),17,"OUI");  // on sélectionne les lignes qui sont des arbres remarquables (ils le sont tous dans ce fichier mais comme ça on est assuré qu'on a bien seulement les arbres remarquables)
    int nb_arbres_remarquables = tab_arbres_remarquables.size();


    int indice_plus_grand_arbre=0;  
    vector<int> tailles_arbres=conversionInt(colonne(tab_arbres_remarquables,12));

    for (int i=1;i<nb_arbres_remarquables;i++){  // on va chercher l'indice du plus grand arbre
        if (tailles_arbres[i]>tailles_arbres[indice_plus_grand_arbre]){
            indice_plus_grand_arbre=i;
        }
    }
    cout << "Le plus grand arbre de Paris est un " << tab_arbres_remarquables[indice_plus_grand_arbre][1] << ", qui est un arbre de l'espèce " << tab_arbres_remarquables[indice_plus_grand_arbre][3] << " et du genre " << tab_arbres_remarquables[indice_plus_grand_arbre][2] <<" et qui mesure " << tab_arbres_remarquables[indice_plus_grand_arbre][12]<< " mètres." << endl;  // et on renvoie la taille de l'arbre correspondant à l'indice du plus grand arbre trouvé
    
}

