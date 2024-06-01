#include <stdexcept>
/** @file **/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "tableau-lecture-csv.hpp"
//source : https://data.ratp.fr/

void split(string str, char delim,vector<string> &out){  //le "&" sert à dire qu'on ne copie pas le tableau out dans une variable locale, mais on le modifie directement là où il est
    stringstream ss(str);
    string s;
    while (getline(ss, s, delim)) {
        out.push_back(s);
    }
}

vector<float> moyennequaliteair(string fichier){
    vector<vector<string>> tableau = litTableauCSV(fichier,8);
    vector<float> moyenne;
    moyenne=vector<float>(10);
    int nbligne=0;
    for (int annee=2022;annee>=2013;annee-=1){
        string anneestring = to_string(annee);  //conversion de l'année en une chaine de caractère pour pouvoir la comparer à l'anneau de chaque ligne du fichier
        int division=0;
        int somme=0;
        for(int i=nbligne;i<tableau.size();i++){
            vector<string> date;
            split(tableau[i][0],'-',date);  //on sépare la première case du tableau correspondant à la date et l'heure, afin de pouvoir stocker uniquement l'année dans une variable
            if(date[0] == anneestring){
                int nombre;  //ces 3 lignes servent à convertir le valeur en PM2.5 d'une chaine de caractère en un entier
                istringstream iss(tableau[i][3]);
                iss >> nombre;
                nbligne+=1;
                somme+=nombre;
                division+=1;
            }
            else{
                break; //on sort de la boucle for i 
            }
        }
        moyenne[2022-annee]=somme/division; 
    }
    return moyenne;
}

    
    
int main(){
    cout<<endl;
    vector<float> moyenne= moyennequaliteair("donnees/qualite-de-lair-mesuree-dans-la-station-auber.csv");
    cout<<"La qualité de l'air dans la station de métro auber en 2013 est d'en moyenne :"<<moyenne[9]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro auber en 2014 est d'en moyenne :"<<moyenne[8]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro auber en 2015 est d'en moyenne :"<<moyenne[7]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro auber en 2016 est d'en moyenne :"<<moyenne[6]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro auber en 2017 est d'en moyenne :"<<moyenne[5]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro auber en 2018 est d'en moyenne :"<<moyenne[4]<<" PM10"<<endl;
    cout<<"Aucune données ne sont fournies pour les années 2019,2020 et 2021"<<endl;
    cout<<"La qualité de l'air dans la station de métro auber en 2022 est d'en moyenne :"<<moyenne[0]<<" PM10"<<endl;
    cout<<endl;
    cout<<endl;
    vector<float> moyenne2= moyennequaliteair("donnees/qualite-de-lair-mesuree-dans-la-station-chatelet.csv");
    cout<<"La qualité de l'air dans la station de métro chatelet en 2013 est d'en moyenne :"<<moyenne2[9]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2014 est d'en moyenne :"<<moyenne2[8]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2015 est d'en moyenne :"<<moyenne2[7]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2016 est d'en moyenne :"<<moyenne2[6]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2017 est d'en moyenne :"<<moyenne2[5]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2018 est d'en moyenne :"<<moyenne2[4]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2019 est d'en moyenne :"<<moyenne2[3]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2020 est d'en moyenne :"<<moyenne2[2]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2021 est d'en moyenne :"<<moyenne2[1]<<" PM10"<<endl;
    cout<<"La qualité de l'air dans la station de métro chatelet en 2022 est d'en moyenne :"<<moyenne2[0]<<" PM10"<<endl;
    return 0; 
}