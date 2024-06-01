#ifndef TERMITE_HPP
#define TERMITE_HPP
#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include "coord.hpp"
#include "grille.hpp"
using namespace std;
struct Termite{
	int numero;
    Coord co;
    Direction d;
    bool brindille;
    int sablier;
	bool TourneSurPlace;
	//constructeurs:
	Termite(int id, Coord c);
	Termite(int id, Coord c,Grille &g);
	//méthodes
	string toString() const; //convertit un termite en un caractère en fonction de son orientation pour l'affichage
	Direction directionTermite() const; //renvoie la direction du termite
    Coord devant() const; //renvoie les coordonnées devant le termite (dans la direction qu'il regarde)
    bool porteBrindille() const; //renvoie true si le termite porte une brindille, false sinon
    void tourneADroite(); //fait tourner le termite à droite à 45 degré
    void tourneAGauche(); //fait tourner le termite à gauche à 45 degré
    void tourneAleat(); //tourne dans une direction aléatoire
    bool laVoieEstLibre(Grille &g) const; //renvoie true si le termite peut avancer ou poser quelque chose dans la case devant lui, false sinon
    bool brindilleEnFace(Grille &g) const; //renvoie true si la case devant le termite contient une brindille
    int voisinsLibre(Grille &g) const; //renvoie le nombre de cases libres autour du termite
    int brindilleAutour(Grille &g) const; //renvoie le nombre de brindilles autour du termite
    void avance(Grille &g); //fait avancer le termite d'une case
    void marcheAleatoire(Grille &g);//marche en tournant aléaotirement
    void chargeBrindille(Grille &g); //prend une brindille sur la case devant lui si il peut
    void dechargeBrindille(Grille &g); //pose une brindille dans la case devant lui si il peut
    void vieTermite(Grille &g, int &sablier);//algorithme de vie du termite, pour un tour
};
#endif