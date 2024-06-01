/** @file **/
#include <vector>
#include <string>
#include <iostream>
using namespace std;

/** Affiche le contenu d'un tableau de chaînes à deux dimensions
 * @param tableau un tableau à deux dimensions
 **/
void afficheTableau(vector<vector<string>> tableau);

/** Moyenne d'un tableau
 * @param t un tableau d'entiers
 * @result la moyenne de ces entiers
 **/
int moyenne(vector<int> t);

/** Somme d'un tableau
 * @param t un tableau d'entiers
 * @result la somme de ces entiers
 **/
int somme(vector<int> t);

/** Renvoie l'indice de la valeur maximale du tableau
 * @param t, un tableau d'entiers
 * @return l'indice de la valeur maximale ou -1 si le tableau est vide
 **/
int indiceMax(vector<int> t);

/** Renvoie l'indice de la valeur minimale du tableau
 * @param t, un tableau d'entiers
 * @return l'indice de la valeur minimale ou -1 si le tableau est vide
 **/
int indiceMin(vector<int> t);

/** Sélection des lignes d'un tableau de données
 * Sélectionne un ensemble de lignes en fonction d'un critère donné
 * @param t, un tableau contenant des lignes de données
 * @param j, l'indice de la colonne à vérifier
 * @param valeur, une chaine de caractères correspondant au critère
 * @return un tableau de données ne contenant que les lignes de t telles
 *  que la colonne j ait la valeur correspondant au critère
 **/
vector<vector<string>> selectLignes(vector<vector<string>> t, int j, string valeur);

/** Extraction d'une colonne d'un tableau de données
 * @param t un tableau 2D de chaines de caractères
 * @param i un numéro de colonne
 * @return la colonne j, représentée par un vecteur de chaines de caractères
 **/
vector<string> colonne(vector<vector<string>> t, int j);

/** Convertit un tableau de chaines de caractères en un tableau d'entiers
 * @param t, un vecteur de chaines de caractères
 * @return un vecteur d'entiers
 **/
vector<int> conversionInt(vector<string> t);
