#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/** Calcule et affiche le nombre de mariages un samedi entre 2010 et 2015 **/
int main() {
    ifstream lecture;
    lecture.open("donnees/statistiques-des-jours-des-mariages.txt");  
    int annee,nbMariage;
    string jour;
    int sommeMariageTotal = 0;
    while(lecture>>annee>>jour>>nbMariage){ //on parcourt chaque ligne du fichier en stockant la première valeur dans année, la deuxième dans jour et la troisieme dans nbMariage
        if(jour == "Samedi"){
            sommeMariageTotal+=nbMariage;
        }
    }
    lecture.close();
    cout<<"Le nombre de mariages célébrés à Paris un samedi entre 2010 et 2015 est de "<<sommeMariageTotal<<endl;
}
