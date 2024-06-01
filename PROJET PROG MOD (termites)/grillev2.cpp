// -*- coding: utf-8 -*-
#include "grillev2.hpp"
#include "coordv2.hpp"
#include "termitev2.hpp"
#include <iostream>
#include<ostream>
#include<string>
#include <stdlib.h>
//include "doctest.h"

using namespace std;


Case::Case():termite{-1},brindille{false},nid{-1},equipeBrindille{-1},cadavre{false}{}

/*
la methode poseBrindille de la struct Grille 
@param une coordonnée c
@param un entier idB correspondant à l'identifiant de la termite qui l'a poséé
la methode pose une brindille sur la coordonnée c, si il y a deja une brindille sur la case, la methode renvoie un message d'erreur
la brindille va ensuite prendre un identifiant idB 
*/
void Grille::poseBrindille(Coord c, int idB){
    if(not this->G[c.getLig()][c.getCol()].brindille){this->G[c.getLig()][c.getCol()].brindille=true;this->G[c.getLig()][c.getCol()].equipeBrindille=idB;}
    else{throw runtime_error("Il y a déjà une brindille sur cette case");}
}

/*
la methode contientSonNid de la struct Grille 
@param une coordonnée c 
@param un entier idB correspondant à un identifiant 
retrun true si le nid correspond à idB, false sinon
*/
bool Grille::contientSonNid(Coord c,int idB){
    return (this->G[c.getLig()][c.getCol()].nid == idB); 
}

/*
la methode poseNid de la struct Grille 
@param une coordonnée c 
@param un entier idN correspondant à un identifiant 
la methode pose un nid idN à la coordonnée c
*/
void Grille::poseNid(Coord c, int idN){
	if(this->G[c.getLig()][c.getCol()].nid == -1){this->G[c.getLig()][c.getCol()].nid=idN;}
    else{throw runtime_error("Il y a déjà un nid sur cette case");}
}

/*
methode enleveBrindille de la struct Grille 
@param une coordonnée c
la methode enleve une brindille sur la coordonnée c, si il n'y a pas de brindille sur c, la methode renvoie un message d'erreur 
*/
void Grille::enleveBrindille(Coord c){
    if(this->G[c.getLig()][c.getCol()].brindille){this->G[c.getLig()][c.getCol()].brindille=false; this->G[c.getLig()][c.getCol()].equipeBrindille=-1;}
    else{throw runtime_error("Il n'y a pas de brindille sur cette case");}
}

/*
la methode contientBrindille de la struct Grille 
@param une coordonnée c
return true si il y a une brindille sur c, false sinon
*/
bool Grille::contientBrindille(Coord c) const{
    if(this->G[c.getLig()][c.getCol()].brindille and this->G[c.getLig()][c.getCol()].equipeBrindille == -1){return true;}
	return false;
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
la methode poseCadavre de la struc Grille 
@param une coordonnée c
la methode pose un cadavre sur la coordonnée c 
*/
void Grille::poseCadavre(Coord c){
	this->G[c.getLig()][c.getCol()].cadavre=true;
}


/* 
la methode retireCadavre de la struc Grille 
@param une coordonnée c
la methode retire un cadavre sur la coordonnée c 
*/
void Grille::retireCadavre(Coord c){
	this->G[c.getLig()][c.getCol()].cadavre=false;
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
return true si la case ne possede ni termite ni brindille ni nid ni cadavre, false sinon
*/
bool Grille::estVide(Coord c) const{
    return (this->G[c.getLig()][c.getCol()].termite == -1 and not this->G[c.getLig()][c.getCol()].brindille and G[c.getLig()][c.getCol()].nid == -1 and not G[c.getLig()][c.getCol()].cadavre);
}



