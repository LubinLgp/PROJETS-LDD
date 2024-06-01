#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
#include "tableau-donnees.hpp"
#include "tableau-lecture.hpp"


/**
 * Demande à l'utilisateur un prénom et calcule
 * - le nombre de garçons et filles à qui l'on a donné ce prénom
 * - la meilleure année pour ce prénom pour les garçons et filles
 **/
int main() {
    string prenom;
    cout << "Entrez un prénom :" << endl;
    cin >> prenom;
    
    vector<vector<string>> tab_naissances = litTableau("donnees/liste_des_prenoms.txt", 4);
    int somme_naissances = somme(conversionInt(colonne(tab_naissances,3)));
    cout << "Il y eu " << somme_naissances << " naissances entre 2006 et 2021." << endl;
    
    vector<vector<string>> tab_enfant = selectLignes(litTableau("donnees/liste_des_prenoms.txt", 4),2,prenom);  //on lit le fichier en entier (sans sélectionner de données), puis on sélectionne les lignes contenant le prénom entré par l'utilisateur
    if (selectLignes(tab_enfant, 0, "M").size()!=0){
        cout << "Ce prénom a été donné à "<< somme(conversionInt(colonne(selectLignes(tab_enfant, 0, "M"),3))) << " garçon(s) entre 2006 et 2021." << endl;  //on sélectionne les naissances de garçons du tableau, puis la colonne contenant le nombre d'enfant nés quelques soit l'année et on en fait la somme ( après avoir convertit le tableau de chaînes de caractères en tableau d'entiers)
        vector<vector<string>>tab_annee;
        vector<string> colonne_nb_prenom;
        ostringstream lect_annee;
        string annee;
        int max_annee=0;
        int max_prenom=0;
        int nb_prenom_annee=0;
        for (int i=2006;i<=2021;i++){  // pour chaque année, on va sélectionner les données de manière similaire à la ligne 200, mais on sélectionne en plus selon l'année en cours
            lect_annee << i;
            annee=lect_annee.str();  // ces deux lignes servent à convetir l'année i en chaînes de caractères pour pouvoir sélectionner l'année dans le tableau de chaînes de caractères
            nb_prenom_annee=somme(conversionInt(colonne(selectLignes(selectLignes(tab_enfant, 0, "M"),1,annee),3)));
            if (nb_prenom_annee>max_prenom){  // si pour l'année en cours, ce prénom a été donné plus de fois qu'une autre année, on remplace l'année où ce prénom a été donné le plus de fois en l'année en cours
                max_prenom=nb_prenom_annee;
                max_annee=i;
            }
            lect_annee.str("");
        }
        cout << "L'année la plus forte est " << max_annee << " où ce prénom a été donné à " << max_prenom << " garçons." << endl;
    }else{
        cout << "Ce prénom n'a été donné à aucun garçon entre 2006 et 2021." << endl;
    }
    
    
    // idem que le bloc précedent, excepté qu'on sélectionne le tableau selon les naissances de filles
    if (selectLignes(tab_enfant, 0, "F").size()!=0){
        cout<< "Ce prénom a été donné à "<< somme(conversionInt(colonne(selectLignes(tab_enfant, 0, "F"),3))) << " fille(s) entre 2006 et 2021." << endl;

        vector<vector<string>>tab_annee_fille;
        vector<string> colonne_nb_prenom_fille;
        ostringstream lect_annee_fille;
        string annee_fille;
        int max_annee_fille=0;
        int max_prenom_fille=0;
        int nb_prenom_annee_fille=0;
        for (int i=2006;i<=2021;i++){
            lect_annee_fille << i;
            annee_fille=lect_annee_fille.str();
            nb_prenom_annee_fille=somme(conversionInt(colonne(selectLignes(selectLignes(tab_enfant, 0, "F"),1,annee_fille),3)));
            if (nb_prenom_annee_fille>max_prenom_fille){
                max_prenom_fille=nb_prenom_annee_fille;
                max_annee_fille=i;
            }
            lect_annee_fille.str("");
        }
        cout << "L'année la plus forte est " << max_annee_fille << " où ce prénom a été donné à " << max_prenom_fille << " filles." << endl;
    }else{
        cout<<"Ce prénom n'a été donné à aucune fille entre 2006 et 2021." << endl;
    }
      
    
    return 0;
}