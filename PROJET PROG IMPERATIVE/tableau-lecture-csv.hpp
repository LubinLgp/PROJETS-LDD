/** @file **/
#include <vector>
#include <string>
using namespace std;

/** Construction d'un tableau 2D de chaînes lu depuis un fichier CSV
 * @param fichier le nom d'un fichier contenant un nombre fixe
  * d'entiers par ligne, séparés par des espaces
 * @param nb_colonnes le nombre de colonnes du fichier
 * @return un tableau d'entiers à deux dimensions
 **/
vector<vector<string>> litTableauCSV(string fichier, int nb_colonnes);

/** Construction d'un tableau 2D de chaînes lu depuis un fichier CSV
 * @param fichier le nom d'un fichier contenant un nombre fixe
  * d'entiers par ligne, séparés par des espaces
 * @return un tableau d'entiers à deux dimensions
 **/
vector<vector<string>> litTableauCSV_2(string fichier);



