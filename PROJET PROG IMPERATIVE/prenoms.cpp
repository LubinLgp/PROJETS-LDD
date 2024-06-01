#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/** Calcule et affiche le prénom le plus utilisé une année donnée
 * ainsi que le nombre de naissances cette année là **/
int main() {
    int annee;
    cout << "Entrez une année entre 2006 et 2021 : ";
    cin >> annee;
    
    //Calcul du nombre de naissances de l'année choisie
    int nombre_naissances=0;
    int nombre_de_fois=0;
    int annee_en_cours,nombre_naissance_prenom;
    string prenom_frequent,prenom_en_cours,sexe_bebe;
    ifstream fichier;
    fichier.open("donnees/liste_des_prenoms.txt");
    while (fichier){ // tant que le fichier n'est pas fini
        fichier >> sexe_bebe;
        fichier >> annee_en_cours;
        if (annee_en_cours==annee){ // si l'année lue depuis le fichier est celle entrée par l'utilisateur, on ajoute le nombre de naissance de cette année au total
            fichier >> prenom_en_cours;
            fichier >> nombre_naissance_prenom;
            nombre_naissances+=nombre_naissance_prenom;
            if (nombre_de_fois<nombre_naissance_prenom) {  // on regarde si le prénom actuel est le plus fréquent, et si oui, on remplace l'ancien par le prénom actuel 
                prenom_frequent=prenom_en_cours;
                nombre_de_fois=nombre_naissance_prenom;
            }
        }
        else {
            fichier >> prenom_en_cours;  // si l'année lue depuis le fichier n'est pas celle entrée par l'utilisateur, on lit quand même les données pour commencer à la ligne suivante au prochain tour de boucle
            fichier >> nombre_naissance_prenom;
        }
    }
    fichier.close();
    cout << "En "<< annee <<", il y a eu " << nombre_naissances << " naissances." << endl;
    cout << "Le prénom le plus donné a été " << prenom_frequent << " donné " << nombre_de_fois <<" fois." << endl;
    return 0;
}

