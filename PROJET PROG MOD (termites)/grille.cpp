#include "grille.hpp"
#include "coord.hpp"
#include "termite.hpp"
#include <iostream>
#include<ostream>
#include<string>
#include <stdlib.h>
//include "doctest.h"

using namespace std;


Case::Case():termite{-1},brindille{false}{}
/*
la methode poseBrindille de la struct Grille 
@param une coordonnée c
la methode pose une brindille sur la coordonnée c, si il y a deja une brindille sur la case, la methode renvoie un message d'erreur
*/
void Grille::poseBrindille(Coord c){
    if(not this->G[c.getLig()][c.getCol()].brindille){this->G[c.getLig()][c.getCol()].brindille=true;}
    else{throw runtime_error("Il y a déjà une brindille sur cette case");}
}
/*
methode enleveBrindille de la struct Grille 
@param une coordonnée c
la methode enleve une brindille sur la coordonnée c, si il n'y a pas de brindille sur c, la methode renvoie un message d'erreur 
*/
void Grille::enleveBrindille(Coord c){
    if(this->G[c.getLig()][c.getCol()].brindille){this->G[c.getLig()][c.getCol()].brindille=false;}
    else{throw runtime_error("Il n'y a pas de brindille sur cette case");}
}

/*
la methode contientBrindille de la struct Grille 
@param une coordonnée c
return true si il y a une brindille sur c, false sinon
*/
bool Grille::contientBrindille(Coord c) const{
    return(this->G[c.getLig()][c.getCol()].brindille);
}

/*
la methode poseTermite de la struct Grille 
@param une coordonnée c
@param un entier idT correspondant à un identifiant 
la methode pose, s'il n'y a pas de termite, une termite idT sur c, un message d'erreur sinon 
*/
void Grille::poseTermite(Coord c, int idT){
    if(this->G[c.getLig()][c.getCol()].termite == -1){this->G[c.getLig()][c.getCol()].termite=idT;}
    else{throw runtime_error("Il y a déjà une termite sur cette case");}
}


/*
la methode enleveTermite de la struct Grille
@param une coordonnée c
la methode enleve la termite sur c, s'il n'y en a pas la methode affiche un message d'erreur 
*/
void Grille::enleveTermite(Coord c){
    int x = c.getLig();
    int y = c.getCol();
    if (this->G[x][y].termite == -1)
        throw runtime_error("Pas de termite dans cette case");
    else
        this->G[x][y].termite = -1;
}

/*
la methode numeroTermite de la struct Grille 
@param une coordonnée c 
retrun l'identifiant du termite à la coordonnée c, s'il n'y a pas de termite, la methode renvoie un message d'erreur
*/
int Grille::numeroTermite(Coord c) const {
    int x =c.getLig();
    int y= c.getCol();
        if (G[x][y].termite == -1)
    throw runtime_error("Pas de termite dans cette case");
        else
    return G[x][y].termite;
}

/*
la methode estVide de la struct Grille 
@param une coordonnée c 
return true si la case ne possede ni termite ni brindille, false sinon
*/
bool Grille::estVide(Coord c) const{
    if(this->G[c.getLig()][c.getCol()].termite == -1 and not contientBrindille(c)){return true;}
    else{return false;}
}



