/** @file **/
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    ifstream fichier("donnees/liste_des_prenoms.csv");
    string entete;
    getline(fichier, entete);
    cout << "Entête: " << entete << endl;

    string nombre1;
    string MF;
    string annee;
    string prenom;
    string nombre2;
    int nombre;
    getline(fichier, nombre1, ';');
    getline(fichier, MF, ';');
    getline(fichier, annee, ';');
    getline(fichier, prenom, ';');
    getline(fichier, nombre2);

    // Sous windows, les fichiers texte utilisent deux
    // caractères pour chaque fin de ligne:
    // - '\r': retour au début de la ligne
    // - '\n': saut de ligne
    //
    // Apparemment le fichier de donnée a été créé par/pour Windows.
    // Si on l'utilise sous Linux, il faut supprimer le '\r' qui n'est pas
    // géré automatiquement par getline.
    if ( nombre2.size() > 0 and nombre2[nombre2.length()-1] == '\r' ) {
        nombre2.resize(nombre2.length() - 1);
    }
    // Conversion du nombre en entier
        istringstream(nombre2) >> nombre;

    cout << "Nombre 1: «" << nombre1 << "»" << endl;
    cout << "Sexe: «" << MF << "»" << endl;
    cout << "Année: «" << annee << "»" << endl;
    cout << "Nombre 2 (str) : «" << nombre2 << "»" << endl;
    cout << "Nombre (entier): " << nombre << endl;

    fichier.close();
    return 0;
}

