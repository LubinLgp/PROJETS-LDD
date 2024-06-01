#include <stdexcept>
/** @file **/
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

/** Infrastructure minimale de test **/
#define CHECK(test) if (!(test)) cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

vector<vector<string>> tableauTest = {
    { "M", "2011", "Bubulle", "3"},
    { "F", "2012", "Bichette", "4"},
    { "F", "2011", "Babouche", "7"},
    { "F", "2011", "Ziboulette", "1"}
};

/** Affiche le contenu d'un tableau de chaînes à deux dimensions
 * @param tableau un tableau à deux dimensions
 **/
void afficheTableau(vector<vector<string>> tableau) {
    for (int i=0; i<tableau.size();i++){ // pour chaque ligne dans le tableau 
        for (int j=0; j<tableau[i].size();j++){ // pour chaque colonne dans le tableau 
            cout << tableau[i][j] << " ";  // on affiche la valuer de la "case"
            if (j==(tableau[i].size())-1) cout << "" << endl; // à la fin de la ligne on revient à la ligne
        }
    }
}

void testAfficheTableau() {
    afficheTableau(tableauTest);
}


/** Construction d'un tableau 2D de chaines de caractères lu depuis un fichier
 * @param fichier le nom d'un fichier contenant un nombre fixe
  * de colonnes séparées par des espaces
 * @param nb_colonnes le nombre de colonnes du fichier
 * @return un tableau de chaines de caractères à deux dimensions
 **/
vector<vector<string>> litTableau(string nom_fichier, int nb_colonnes) {
    ifstream fichier;
    string donnee;
    fichier.open(nom_fichier);
    vector<vector<string>> tableau;
    vector<string> t;
    t= vector<string>(nb_colonnes); // on sait que t aura exactement nb_colonnes colonnes, puisque on lit le fichier tel quel, sans sélectionner des données
    int j=0;
    int i=1;
    while (fichier >> donnee){  //tant qu'on peut lire dans le fichier
        tableau.push_back(t);  // en revanche, on ne sait pas combien tableau aura de lignes ( on ne le précise pas avec un argument) donc on doit recourir à des push_back de tableaux t avec le nombre de colonnes approprié
        tableau[j][0]=donnee; 
        i=1;
        while (i<nb_colonnes){  // on écrit les données de la ligne en cours dans le tableau
            fichier >> donnee ;
            tableau[j][i]=donnee;
            i+=1;
        }
        j+=1;
    }
    fichier.close();
    return tableau;
}

/** Test de la fonction litTableau **/
void testLitTableau() {
    CHECK(litTableau("donnees/donnees-test.txt", 4) == tableauTest);
    vector<vector<string>> t = litTableau("donnees/liste_des_prenoms.txt", 4);
    CHECK(t.size() == 22994);
    CHECK(t[0][0] == "F");
    CHECK(t[4][2] == "Astrid");
    CHECK(t[5][3] == "12");
    CHECK(t[22993][3] == "32");
}

/** Extraction d'une colonne d'un tableau de données
 * @param t un tableau 2D de chaines de caractères
 * @param i un numéro de colonne
 * @return la colonne j, représentée par un vecteur de chaines de caractères
 **/
vector<string> colonne(vector<vector<string>> t, int j) {
    vector<string> colonne;
    colonne = vector<string>(t.size()); // on sait que colonne à la même taille que t donc on peut faire l'allocation
    for (int i=0;i<t.size();i++){
        colonne[i]=t[i][j]; // on met dans le tableau colonne chaque valeur de chaque ligne à la colonne j
    }
    return colonne;
}

/** Test de la fonction colonne **/
void testColonne() {
    CHECK(colonne(tableauTest, 0) == vector<string>({"M","F","F","F"}));
    CHECK(colonne(tableauTest, 1) == vector<string>({"2011","2012","2011","2011"}));
    CHECK(colonne(tableauTest, 2) == vector<string>({"Bubulle","Bichette","Babouche","Ziboulette"}));
    CHECK(colonne(tableauTest, 3) == vector<string>({"3","4","7","1"}));
}

/** Convertit un tableau de chaines de caractères en un tableau d'entiers
 * @param t, un vecteur de chaines de caractères
 * @return un vecteur d'entiers
 **/
vector<int> conversionInt(vector<string> t) {
    vector<int> resultat = vector<int>(t.size());
    for(int i =0; i < t.size(); i++) {
        istringstream(t[i]) >> resultat[i];
    }
    return resultat;
}

void testConversionInt() {
    CHECK(conversionInt({}) == vector<int>({}));
    CHECK(conversionInt({"1","2","3"}) == vector<int>({1,2,3}));
}

/** copier la fonction somme déjà écrite **/
int somme(vector<int> t) {
    int somme=0;
    for (int i=0;i<t.size();i++){
        somme+=t[i];
        }
    return somme;
}

/** copier la fonction moyenne déjà écrite **/
int moyenne(vector<int> t) {
    int compt=0;
    int somme=0;
    for (int i=0;i<t.size();i++){
        somme+=t[i];
        compt+=1;
    }
    auto moyenne=somme/compt;
    return moyenne;
}

/** copier la fonction indiceMax déjà écrite **/
int indiceMax(vector<int> t) {
    int indiceMax=-1;
    int max=0;
    for (int i=0;i<t.size();i++){
        if (max<t[i]){
            max=t[i];
            indiceMax=i;
        }
    }
    return indiceMax;
}

/** Sélection des lignes d'un tableau de données
 * Sélectionne un ensemble de lignes en fonction d'un critère donné
 * @param t, un tableau contenant des lignes de données
 * @param j, l'indice de la colonne à vérifier
 * @param valeur, une chaine de caractères correspondant au critère
 * @return un tableau de données ne contenant que les lignes de t telles
 *  que la colonne j ait la valeur correspondant au critère
 **/
vector<vector<string>> selectLignes(vector<vector<string>> t, int j, string valeur) {
    vector<vector<string>> lignes;
    vector<string> tableau_intermediaire; // l'utilisation du tableau intermédiaire permet de push_back dans le tableau de tableaux avec les valeurs déjà insérées. Il est possible de faire sans, mais je trouvais personnellement cette utilisation d'un tableau intermédiare plus claire.
    tableau_intermediaire=vector<string>(t[0].size());
    for (int i=0;i<t.size();i++){
        if (t[i][j]==valeur){
            for (int k=0;k<t[i].size();k++){
                tableau_intermediaire[k]=t[i][k];
            }
            lignes.push_back(tableau_intermediaire);
        }
    }
    return lignes;
}

/** Test de la fonction selectLignes **/
void testSelectLignes() {
    CHECK(selectLignes(tableauTest, 0, "M") == vector<vector<string>>({{ "M", "2011", "Bubulle", "3"}}));
    CHECK(selectLignes(tableauTest, 1, "2011") == vector<vector<string>>({{ "M", "2011", "Bubulle", "3"}, { "F", "2011", "Babouche", "7"}, { "F", "2011", "Ziboulette", "1"}}));
    CHECK(selectLignes(tableauTest, 2, "Bubulle") == vector<vector<string>>({{ "M", "2011", "Bubulle", "3"}}));
}

/** Lance les tests de litTableau, selectLignes et colonne puis
 * affiche le nombre total de naissances puis
 * demande à l'utilisateur un prénom et calcule et affiche
 * - le nombre de garçons et filles à qui l'on a donné ce prénom
 * - la meilleure année pour ce prénom pour les garçons et filles
 **/
int main() {
    testLitTableau();
    testAfficheTableau();
    testColonne();
    testSelectLignes();

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