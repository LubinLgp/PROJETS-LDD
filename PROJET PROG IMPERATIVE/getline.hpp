/** @file **/
/** Lit une ligne d'un flux et la stocke dans 'resultat'
 * @param f un flux entrant
 * @param resultat une chaîne de caractères
 * @return le flux f
 **/
istream& getline(istream &f, string &resultat);

/** Lit une chaîne de caractères depuis un flux jusqu'au séparateur
    et la stocke dans 'resultat'
 * @param f un flux entrant
 * @param resultat une chaîne de caractères
 * @param separateur un caractère
 * @return le flux f
 **/
istream& getline(istream &f, string &resultat, char separateur);
