#include <stdexcept>
/** @file **/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "tableau-lecture-csv.hpp"
//source : https://ressources.data.sncf.com/


int main(){
    vector<vector<string>> tableau = litTableauCSV("donnees/objets-trouves-restitution.csv",8); //transforme le fichier csv en tableau de 8 colonnes
    map<string,int> stats; //création d'un dictionnaire avec comme clé une chaine de caractère et comme valeur un entier
    for(int i=0;i<tableau.size();i++){
        stats[tableau[i][2]]+=1;  //création du dictionnaire qui associe à chaque ville, un nombre de parapluies retrouvés
    }
    int max=0;
    string villemax;
    for(auto item : stats){
        if(item.second > max){ //si le deuxième élément, donc la valeur correspondant au nombre de parapluies retrouvés, est suppérieur au nombre maximum stocké jusqu'à présent:
            max = item.second; //on sauvegarde la nouvelle valeur dans la variable maximum en stockant aussi la ville correspondante dans la variable villemax
            villemax = item.first;
        }
    }
    cout<<"La ville où l'on a retrouvé le plus de parapluies est: "<<villemax<<" avec "<<max<<" parapluies retrouvés (sur la période 2013-2022)"<<endl;
    return 0; 
}
