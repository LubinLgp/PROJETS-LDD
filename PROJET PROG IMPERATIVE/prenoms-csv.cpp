#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

/** Calcule et affiche le prénom le plus donné une année donnée
 * ainsi que le nombre de naissance cette année là **/
int main() {
    ifstream fichier("donnees/liste_des_prenoms.csv");
    string entete;
    getline(fichier, entete);
    int annee_choisie;
    cout<<"Veuillez saisir une année entre 2006 et 2021: ";
    cin>>annee_choisie;
    cout<<endl;
    string nombre1;
    string MF;
    string annee;
    string prenom;
    string nombre2;
    int nombre;
    int annee_int;
    int somme_naissance=0;
    int nombre_max=0;
    string prenom_max;
    while(fichier){  //on parcourt toutes les lignes du fichier et on stocke les différentes valeurs, séparées par des ";" dans les variables correspondantes
        getline(fichier, nombre1, ';');
        getline(fichier, MF, ';');
        getline(fichier, annee, ';');
        getline(fichier, prenom, ';');
        getline(fichier, nombre2);
        if ( nombre2.size() > 0 and nombre2[nombre2.length()-1] == '\r' ) {  //si le dernier caractère de la ligne (donc de la variable nombre2) est un retour à la ligne, on le supprime. Ces fichiers csv ont en effet été crée par et pour des systemes d'exploitation Windows. Nous devons donc les traiter manuellement.
            nombre2.resize(nombre2.length() - 1); //on supprime le dernier caractère de la chaine de caractère
        }
            istringstream(nombre2) >> nombre;
            istringstream(annee)>>annee_int;
        if(annee_int==annee_choisie){
            somme_naissance+=nombre;
            if(nombre>=nombre_max){
                nombre_max=nombre;
                prenom_max=prenom;
            }
        }
}

    cout << "Il y a eu "<<somme_naissance<<" naissances cette année là"<<endl;
    cout<< "Le prénom le plus donné cette année là est: "<<prenom_max<<" (avec "<<nombre_max<<" naissances)"<<endl;

    fichier.close();
    return 0;
}
