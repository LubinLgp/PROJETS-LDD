#include <stdexcept>
/** @file **/
#include <fstream>
#include <sstream>
#include "tableau-donnees-avance.hpp"
#include "tableau-donnees.hpp"

int chercheIndice(vector<string> t, string valeur) {
    int indice=-1;   // si l'indice n'est pas là, la valeur d'indice ne changera pas dans la fonction et sera donc égale à -1
    for (int i=0;i<t.size();i++){
        if (t[i]==valeur){  // sinon, si la valeur est trouvée dans le tableau, on renvoie son indice tout de suite
            indice=i;
            return indice;
        }
    }
    return indice; // on pourrait aussi directement faire un return -1, c'est uniquement un choix d'implentation
    
}

vector<string> distinct(vector<vector<string>> data, int j) {
    vector<string> colonne_distinct;
    colonne_distinct.push_back(data[0][j]); // cette ligne sert à mettre déjà la première valeur, puisque forcément elle n'y est pas encore vu que le tableau colonne_distinct est vide
    for (int i=1;i<data.size();i++){
            if (chercheIndice(colonne_distinct,data[i][j])==-1){  // on utilise chercheIndice pour savoir si cette valeur a déjà été ajoutée dans le tableau, autrement dit si elle renvoie -1 ou non (-1 signifie qu'elle n'y est pas encore)
                colonne_distinct.push_back(data[i][j]);  // si elle n'y est pas on la rajoute à l'aide d'un push_back (car on ne sait pas quelle sera la taille de colonne_distinct à la fin 
            }
    }
    return colonne_distinct;
}

vector<double> conversionDouble(vector<string> t) {  //même code que conversionInt mais on remplace les int par double
    vector<double> resultat = vector<double>(t.size());
    for(int i =0; i < t.size(); i++) {
        istringstream(t[i]) >> resultat[i];
    }
    return resultat;
}

vector<int> groupByInt(vector<vector<string>> data, vector<string> valeurs, int j1, int j2) {
    vector<int> tab_groupByInt;
    tab_groupByInt=vector<int>(valeurs.size());
    for (int k=0;k<valeurs.size();k++){  // pour chaque valeur dans le tableau valeurs
        for (int i=0;i<data.size();i++){  // pour chaque ligne dans le tableau data
            if (data[i][j1]==valeurs[k]){  // si la valeur de la colonne j1 de la ligne en cours est celle souhaitée (la valeur du tableau valeurs à l'indice k)
                tab_groupByInt[k]+=conversionInt({data[i][j2]})[0];  // on ajoute à la somme déjà calculée la valeur du tableau data à la ligne en cours à la colonne j2
            }
        }
    }
    return tab_groupByInt;
}

vector<double> groupByDouble(vector<vector<string>> data, vector<string> valeurs, int j1, int j2) {   //même code que groupByInt mais on remplace les int par double
    vector<double> tab_groupByDouble;
    tab_groupByDouble=vector<double>(valeurs.size());
    for (int k=0;k<valeurs.size();k++){
        for (int i=0;i<data.size();i++){
            if (data[i][j1]==valeurs[k]){
                tab_groupByDouble[k]+=conversionDouble({data[i][j2]})[0];
            }
        }
    }
    return tab_groupByDouble;
}

template<class T>
vector<T> conversion(vector<string> t) {  //même code que conversionInt mais on remplace les int par T
    vector<T> resultat = vector<T>(t.size());
    for(int i =0; i < t.size(); i++) {
        istringstream(t[i]) >> resultat[i];
    }
    return resultat;
}

template<class T> 
vector<T> groupBy(vector<vector<string>> data, vector<string> valeurs, int j1, int j2) {  //même code que groupByInt mais on remplace les int par T
    vector<T> tab_groupBy;
    tab_groupBy=vector<T>(valeurs.size());
    for (int k=0;k<valeurs.size();k++){
        for (int i=0;i<data.size();i++){
            if (data[i][j1]==valeurs[k]){
                tab_groupBy[k]+=conversion<T>({data[i][j2]})[0];
            }
        }
    }
    return tab_groupBy;
}

// Force l'instanciation des templates
template vector<int> conversion<int>(vector<string> t);
template vector<double> conversion<double>(vector<string> t);
template vector<int> groupBy<int>(vector<vector<string>> data, vector<string> valeurs, int j1, int j2);
template vector<double> groupBy<double>(vector<vector<string>> data, vector<string> valeurs, int j1, int j2);

