/** @file **/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "tableau-lecture.hpp"
#include "tableau-donnees.hpp"
#include "tableau-donnees-avance.hpp"
using namespace std;


/** Affiche :
 * - le nombre total de mariages
 * - le nombre moyen de mariages célébrés par an
 * - l'année où l'on a célébré le plus de mariages (et leur nombre)
 * - le jour de semaine où l'on célèbre le plus de mariages (et leur nombre)
 * - le pourcentage de mariages célébrés un samedi
 **/
int main() {
    vector<vector<string>> data = litTableau("donnees/statistiques-des-jours-des-mariages.txt", 3);

    vector<string> annees = distinct(data,0);
    vector<string> jours = distinct(data,1);

    vector<int> t1 = groupByInt(data, annees, 0, 2);
    vector<int> t2 = groupByInt(data, jours, 1, 2);
    cout << "Le nombre de total de mariages célébrés entre 2010 et 2015 est de " << somme(t1) << endl;
    cout << "Le nombre de mariages célébrés en moyenne par an est de " << moyenne(t1) << endl;
    int m1 = indiceMax(t1);
    int m2 = indiceMax(t2);
    cout << "L'année où l'on a célébré le plus de mariages est " << annees[m1] << " avec " << t1[m1] << " mariages" << endl;
    cout << "Le jour de la semaine où l'on a célébré le plus de mariage est le " << jours[m2] << " avec " << t2[m2] << " mariages" << endl;
    double samedi = t2[chercheIndice(jours,"Samedi")];
    cout << "Le pourcentage de mariages célébrés le samedi est de " << samedi / somme(t2) * 100 << "%" << endl;

    return 0;
}

