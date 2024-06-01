#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

/** Infrastructure minimale de test **/
#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

vector<string> jours = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};


/** Construction du tableau des mariages par années
 * On suppose que le fichier contient sur chaque ligne une année entre 2010 et 2015
 * suivi d'un jour, puis d'un nombre de mariages
 * @param nom_fichier, le nom du fichier de données
 * @return un tableau d'entiers tel que dans chaque
 * case d'indice i, on trouve le nombre total de mariages de l'année 2010+i
 **/
vector<int> litTableauAnnee(string nom_fichier) {
    ifstream fichier;
    fichier.open(nom_fichier);
    vector<int> nbMariageAnnee;
    nbMariageAnnee = vector<int>(6);
    int annee,nbMariage;
    string jour;
    while(fichier>>annee>>jour>>nbMariage){  //On parcourt chaque ligne du fichier en sotckant les 3 valeurs de chaque lignes dans les variables correspondantes
        nbMariageAnnee[annee-2010] += nbMariage;
    }
    fichier.close();
    return nbMariageAnnee;
}

/** Test de la fonction litTableauAnnee **/
void testLitTableauAnnee() {
    vector<int> t = litTableauAnnee("donnees/statistiques-des-jours-des-mariages.txt");
    CHECK(t[0] == 9195); // Nombre de mariages en 2010
    CHECK(t[3] == 9443); // Nombre de mariages en 2013
    CHECK(t[5] == 9046); // Nombre de mariages en 2015
}

/** Renvoie le numéro du jour correspondant à la chaîne de caractères
 * @param jour une chaine de caractère contenant un jour de la semaine (première lettre en majuscule)
 * @return un numéro entre 0 et 6 correspondant au jour de la semaine ou
 * -1 si le jour n'est pas valide
 **/
int indiceJour(string jour) {
    for(int i=0; i < jours.size(); i++) {
        if (jour == jours[i]) {
            return i;
        }
    }
    return -1;
}

void testIndiceJour() {
    CHECK(indiceJour("Lundi") == 0);
    CHECK(indiceJour("Samedi") == 5);
    CHECK(indiceJour("Dimanche") == 6);
    CHECK(indiceJour("samedi") == -1);
}

/** Construction du tableau des mariages par jour
 * On suppose que le fichier contient sur chaque ligne une année entre 2010 et 2015
 * suivie d'un jour, puis d'un nombre de mariages
 * @param nom_fichier, le nom du fichier de données
 * @return un tableau d'entiers tel que dans chaque
 * case d'indice i, on trouve le nombre total de mariages célébrés le jour i
 **/
vector<int> litTableauJours(string nom_fichier) {
    ifstream fichier;
    fichier.open(nom_fichier);
    vector<int> nbMariageJour;
    nbMariageJour = vector<int>(7);
    int annee,nbMariage;
    string jour;
    while(fichier>>annee>>jour>>nbMariage){ //on parcourt encore une fois chaque ligne du fichier
        nbMariageJour[indiceJour(jour)] += nbMariage;  //on convertit la chaine de caractère du jour en question à sa position dans le tableau grâce à la fonction indiceJour
    }
    fichier.close();
    return nbMariageJour;
}

/** Test de la fonction litTableauJours **/
void testLitTableauJours() {
    vector<int> t = litTableauJours("donnees/statistiques-des-jours-des-mariages.txt");
    CHECK(t[0] == 4171); // Nombre de mariages le lundi
    CHECK(t[3] == 4393); // Nombre de mariages le jeudi
    CHECK(t[6] == 2); // Nombre de mariages le dimanche
}

/** Renvoie la somme d'un tableau d'entiers
 * @param t, un tableau d'entier
 * @return la somme des valeurs du tableau
 **/
int somme(vector<int> t) {
    int resultat=0;
    for(int i=0; i<t.size();i++){
        resultat+=t[i];
    }
    return resultat;
}

/** Test de la fonction somme **/
void testSomme() {
    CHECK(somme({1,2,3}) == 6);
    CHECK(somme({}) == 0);
    CHECK(somme({2,2,4,8}) == 16);
}

/** Renvoie la moyenne d'un tableau d'entier
 * @param t, un tableau d'entiers
 * @return la moyenne (entière) des valeurs
 * (on arrondit à l'entier inférieur)
 **/
int moyenne(vector<int> t) {
    return( int(somme(t)/t.size()));  //pour éviter de réécrire du code inutile, on apelle directement la fonction somme
}

/** Test de la fonction moyenne **/
void testMoyenne() {
    CHECK(moyenne({1,2,3}) == 2);
    CHECK(moyenne({1}) == 1);
    CHECK(moyenne({2,2,4,8}) == 4);
}


/** Renvoie l'indice de la valeur maximale du tableau
 * @param t, un tableau d'entier
 * @return l'indice de la valeur maximale ou -1 si le tableau est vide
 **/
int indiceMax(vector<int> t) {
    if(t.size() == 0){
        return -1;
    }
    int indiceMaximum = 0;
    int valeurMaximum = t[0];
    for(int i=1; i<t.size() ; i++){
        if( t[i] > valeurMaximum){
            valeurMaximum = t[i];
            indiceMaximum = i;
        }
    }
    return indiceMaximum;
}

/** Test de la fonction IndiceMax **/
void testIndiceMax() {
    CHECK(indiceMax({}) == -1);
    CHECK(indiceMax({1,2,3}) == 2);
    CHECK(indiceMax({1,3,2}) == 1);
    CHECK(indiceMax({1}) == 0);
    CHECK(indiceMax({2,8,4,4}) == 1);
}

/** Lance les fonctions de test puis affiche :
 * - le nombre total de mariages
 * - le nombre moyen de mariages célébrés par an
 * - l'année où l'on a célébré le plus de mariages (et leur nombre)
 * - le jour de la semaine où l'on célèbre le plus de mariages (et leur nombre)
 * - le pourcentage de mariages célébrés un samedi
 **/
int main() {
    testLitTableauAnnee();
    testIndiceJour();
    testLitTableauJours();
    testSomme();
    testMoyenne();
    testIndiceMax();
    vector<int> tableauAnnee = litTableauAnnee("donnees/statistiques-des-jours-des-mariages.txt");
    vector<int> tableauJours = litTableauJours("donnees/statistiques-des-jours-des-mariages.txt");
    int nbMariageTotal = somme(tableauAnnee);
    int moyenneMariage = moyenne(tableauAnnee);
    int anneeMariageMax = 2010 + indiceMax(tableauAnnee);
    string jourMariageMax = jours[indiceMax(tableauJours)];
    float pourcentageMariageSamedi = float(tableauJours[indiceJour("Samedi")]) / somme(tableauJours)*100;
    cout<<"Le nombre total de mariages célbrés entre 2010 et 2015 est de "<<nbMariageTotal<<endl;
    cout<<"Le nombre de mariages célbrés en moyenne par an est de "<<moyenneMariage<<endl;
    cout<<"L'année où l'on a célébré le plus de mariage est "<<anneeMariageMax<<" avec "<<tableauAnnee[anneeMariageMax - 2010]<<" mariages"<<endl;
    cout<<"Le jour de la semaine où l'on a célébré le plus de mariages est le "<<jourMariageMax<<" avec "<<tableauJours[indiceJour(jourMariageMax)]<<" mariages"<<endl;
    cout<<"Le pourcentage de mariages célébrés le samedi est de "<<pourcentageMariageSamedi<<"%"<<endl;
}
