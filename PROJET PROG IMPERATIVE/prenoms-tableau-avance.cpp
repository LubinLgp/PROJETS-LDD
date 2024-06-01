#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include "tableau-lecture.hpp"
#include "tableau-donnees.hpp"
#include "tableau-donnees-avance.hpp"
using namespace std;


/**
 * Calcule et affiche :
 * - le nombre total de naissances de garçons et de naissances de filles
 * - l'année qui a eu le plus de naissances (garçons et filles confondus), avec leur nombre
 * - le nombre de naissances en moyenne par an
 * - les nombres de prénoms féminins et masculins différents
 * - le prénom masculin et le prénom féminin le plus populaire
 **/

int main() {
    vector<vector<string>> tab_enfant = litTableau("donnees/liste_des_prenoms.txt", 4);

    //nombre total de naissances de garçons et de naissances de filles

    
    cout << "Il y a eu "<< somme(conversionInt(colonne(selectLignes(tab_enfant, 0, "M"),3))) << " naissances de garçons et " << somme(conversionInt(colonne(selectLignes(tab_enfant, 0, "F"),3))) <<" naissances de filles entre 2006 et 2021." << endl;

    
    //l'année qui a eu le plus de naissances (garçons et filles confondus), avec leur nombre


    vector<string> tab_annees_repertoriees = distinct(tab_enfant,1);
    string max_annee=tab_annees_repertoriees[indiceMax(groupByInt(tab_enfant,tab_annees_repertoriees,1,3))];
    
    cout << "L'année qui a eu le plus de naissances est " << max_annee << " avec " << somme(conversionInt(colonne(selectLignes(tab_enfant,1,max_annee),3))) << " naissances." << endl;

    
    //le nombre de naissances en moyenne par an

    cout << "Il y a en moyenne " << moyenne(groupByInt(tab_enfant,tab_annees_repertoriees,1,3)) << " naissances par an." << endl;


    //les nombres de prénoms féminins et masculins différents

    
    vector<vector<string>> tab_enfant_garcons = selectLignes(tab_enfant,0,"M");
    vector<vector<string>> tab_enfant_filles = selectLignes(tab_enfant,0,"F");
    vector<string> liste_prenoms_garcons=distinct(tab_enfant_garcons,2);
    vector<string> liste_prenoms_filles=distinct(tab_enfant_filles,2);

    
    cout << "Il y a eu " << liste_prenoms_filles.size() << " prénoms différents donnés à des filles et " << liste_prenoms_garcons.size() << " prénoms différents donnés à des garçons." << endl;


    //le prénom masculin et le prénom féminin le plus populaire


    vector<int> tab_nb_prenom_garcons_toutes_annees = groupByInt(tab_enfant_garcons,liste_prenoms_garcons,2,3);
    vector<int> tab_nb_prenom_filles_toutes_annees = groupByInt(tab_enfant_filles,liste_prenoms_filles,2,3);
    string prenom_garcon;
    string prenom_fille;

    for (int i=0;i<liste_prenoms_filles.size();i++){
        if (somme(conversionInt(colonne(selectLignes(tab_enfant_filles,2,liste_prenoms_filles[i]),3)))==tab_nb_prenom_filles_toutes_annees[indiceMax(tab_nb_prenom_filles_toutes_annees)]){
            prenom_fille=liste_prenoms_filles[i];
            break;
        }
    }
    for (int i=0;i<liste_prenoms_garcons.size();i++){
        if (somme(conversionInt(colonne(selectLignes(tab_enfant_garcons,2,liste_prenoms_garcons[i]),3)))==tab_nb_prenom_garcons_toutes_annees[indiceMax(tab_nb_prenom_garcons_toutes_annees)]){
            prenom_garcon=liste_prenoms_garcons[i];
            break;
        }
    }

    
    cout << "Le prénom féminin le plus populaire est " << prenom_fille << " donné " << tab_nb_prenom_filles_toutes_annees[indiceMax(tab_nb_prenom_filles_toutes_annees)] << " fois." << endl;
    cout << "Le prénom masculin le plus populaire est " << prenom_garcon << " donné " << tab_nb_prenom_garcons_toutes_annees[indiceMax(tab_nb_prenom_garcons_toutes_annees)] << " fois." << endl;
    
    return 0;
}

