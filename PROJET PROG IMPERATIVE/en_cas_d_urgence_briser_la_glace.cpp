vector<vector<string>> litTableauCSV(string nom_fichier, int nb_colonnes) { // Auteurs: les profs!
    ifstream fichier(nom_fichier);
    vector<vector<string>> tableau;
    string ligne;
    // Lecture de l'entête
    getline(fichier, ligne);
    while (fichier) {
        vector<string> ligne_tableau;
        string v;

        // Lecture des valeurs de la ligne sauf la dernière
        for ( int i = 0; i < nb_colonnes-1; i++ ) {
            getline(fichier, v, ';');
            ligne_tableau.push_back(v);
        }

        // Lecture de la dernière valeur
        getline(fichier, v);
        // Gestion des fichier Windows sous Linux
        if ( v.size() > 0 and v[v.length()-1] == '\r' )
            v.resize(v.length() - 1);
        ligne_tableau.push_back(v);

        if ( fichier )
            tableau.push_back(ligne_tableau);
    }
    return tableau;
}

vector<vector<string>> litTableauCSV(string nom_fichier) { // Auteurs: les profs!
    ifstream fichier(nom_fichier);
    vector<vector<string>> tableau;
    string ligne;
    // Lecture de l'entête
    getline(fichier, ligne);
    while ( getline(fichier,ligne) ) {
        istringstream sl(ligne);
        vector<string> ligne_tableau;
        string v;
        while ( getline(sl, v, ';' )) {
            // Gestion des fichier Windows sous Linux
            if ( v.size() > 0 and v[v.length()-1] == '\r' )
                v.resize(v.length() - 1);
            ligne_tableau.push_back(v);
        }
        tableau.push_back(ligne_tableau);
    }
    return tableau;
}

