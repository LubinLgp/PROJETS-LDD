#include <stdexcept>
/** @file **/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "tableau-lecture-csv.hpp"


vector<vector<string>> litTableauCSV(string nom_fichier, int nb_colonnes) {
    ifstream fichier(nom_fichier);
    string entete;
    getline(fichier,entete);
    string mot_nombre;
    vector<vector<string>> tableauCSV = {};
    vector<string> ligne;
    while(fichier){ //on parcourt chaque ligne du fichier
        ligne={};
        for(int i=1;i<=nb_colonnes;i++){  //on parcourt chaque colonne
            if(i==nb_colonnes){
                getline(fichier,mot_nombre); //si on est à la dernière colonne, la chaine de caractère se finit par un retour à la ligne
            }else{
                getline(fichier,mot_nombre,';');  //alors que sinon, la chaine de caractère se finit par un ";"
            }
            if ( mot_nombre.size() > 0 and mot_nombre[mot_nombre.length()-1] == '\r' ){ //on supprime le retour à la ligne de la dernière chaine de caractère, s'il y en a un
                mot_nombre.resize(mot_nombre.length() - 1);
            }
            ligne.push_back(mot_nombre);
        }
        tableauCSV.push_back(ligne);
    }
    tableauCSV.pop_back();
    fichier.close();
    return tableauCSV;
}

vector<vector<string>> litTableauCSV_2(string nom_fichier) {  //même code que precedemment mais on calcule ici nb_colonnes nous mêmes
    ifstream fichier(nom_fichier);
    string entete;
    getline(fichier,entete);
    int nb_colonnes=1;
    for(int i=0;i<entete.length();i++){  //on parcourt tous les caractères de l'entete et on regarde combien de fois le caractère ";" apparait. S'il aparrait n fois, cela veut dire que n+1 chaines de caractères ont été séparées et donc il y a n+1 colonnes
        if (entete[i] == ';'){
            nb_colonnes+=1;
        }
    }
    string mot_nombre;
    vector<vector<string>> tableauCSV = {};
    vector<string> ligne;
    while(fichier){ 
        ligne={};
        for(int i=1;i<=nb_colonnes;i++){
            if(i==nb_colonnes){
                getline(fichier,mot_nombre);
            }else{
                getline(fichier,mot_nombre,';');
            }
            if ( mot_nombre.size() > 0 and mot_nombre[mot_nombre.length()-1] == '\r' ){
                mot_nombre.resize(mot_nombre.length() - 1);
            }
            ligne.push_back(mot_nombre);
        }
        tableauCSV.push_back(ligne);
    }
    tableauCSV.pop_back();
    fichier.close();
    return tableauCSV;
}



