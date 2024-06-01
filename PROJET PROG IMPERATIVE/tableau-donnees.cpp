#include <stdexcept>
/** @file **/
#include <fstream>
#include <sstream>
#include "tableau-donnees.hpp"

void afficheTableau(vector<vector<string>> tableau) {
    for (int i=0; i<tableau.size();i++){ // pour chaque ligne dans le tableau 
        for (int j=0; j<tableau[i].size();j++){ // pour chaque colonne dans le tableau 
            cout << tableau[i][j] << " "; // on affiche la valuer de la "case"
            if (j==(tableau[i].size())-1) cout << "" << endl; // à la fin de la ligne on revient à la ligne
        }
    }
}

int moyenne(vector<int> t) {
    int compt=0;
    int somme=0;
    for (int i=0;i<t.size();i++){
        somme+=t[i];
        compt+=1;
    }
    auto moyenne=somme/compt;
    return moyenne;
}

int somme(vector<int> t) {
     int somme=0;
    for (int i=0;i<t.size();i++){
        somme+=t[i];
        }
    return somme;
}


int indiceMax(vector<int> t) {
    int indiceMax=-1;
    int max=0;
    for (int i=0;i<t.size();i++){
        if (max<t[i]){
            max=t[i];
            indiceMax=i;
        }
    }
    return indiceMax;
}

int indiceMin(vector<int> t) {
    int indiceMin=-1;
    int min=t[0];
    for (int i=0;i<t.size();i++){
        if (min>t[i]){
            min=t[i];
            indiceMin=i;
        }
    }
    return indiceMin;
}

vector<string> colonne(vector<vector<string>> t, int j) {
    vector<string> colonne;
    colonne = vector<string>(t.size()); // on sait que colonne à la même taille que t donc on peut faire l'allocation
    for (int i=0;i<t.size();i++){
        colonne[i]=t[i][j]; // on met dans le tableau colonne chaque valeur de chaque ligne à la colonne j
    }
    return colonne;
}



vector<int> conversionInt(vector<string> t) {
    vector<int> resultat = vector<int>(t.size());
    for(int i =0; i < t.size(); i++) {
        istringstream(t[i]) >> resultat[i];
    }
    return resultat;
}

vector<vector<string>> selectLignes(vector<vector<string>> t, int j, string valeur) {
    vector<vector<string>> lignes;
    vector<string> tableau_intermediaire; // l'utilisation du tableau intermédiaire permet de push_back dans le tableau de tableaux avec les valeurs déjà insérées. Il est possible de faire sans, mais je trouvais personnellement cette utilisation d'un tableau intermédiare plus claire.
    tableau_intermediaire=vector<string>(t[0].size());
    for (int i=0;i<t.size();i++){
        if (t[i][j]==valeur){
            for (int k=0;k<t[i].size();k++){
                tableau_intermediaire[k]=t[i][k];
            }
            lignes.push_back(tableau_intermediaire);
        }
    }
    return lignes;
}
