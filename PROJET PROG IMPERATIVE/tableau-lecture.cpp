#include <stdexcept>
/** @file **/
#include <fstream>
#include <sstream>
#include "tableau-lecture.hpp"


vector<vector<string>> litTableau(string nom_fichier, int nb_colonnes) {
    ifstream fichier;
    string donnee;
    fichier.open(nom_fichier);
    vector<vector<string>> tableau;
    vector<string> t;
    t= vector<string>(nb_colonnes);  // on sait que t aura exactement nb_colonnes colonnes, puisque on lit le fichier tel quel, sans sélectionner des données
    int j=0;
    int i=1;
    while (fichier >> donnee){ //tant qu'on peut lire dans le fichier
        tableau.push_back(t); // en revanche, on ne sait pas combien tableau aura de lignes ( on ne le précise pas avec un argument) donc on doit recourir à des push_back de tableaux t avec le nombre de colonnes approprié
        tableau[j][0]=donnee;
        i=1;
        while (i<nb_colonnes){
            fichier >> donnee ;
            tableau[j][i]=donnee; // on écrit les données de la ligne en cours dans le tableau
            i+=1;
        }
        j+=1;
    }
    fichier.close();
    return tableau;
}
