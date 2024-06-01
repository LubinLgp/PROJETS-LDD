#ifndef TERMITE_HPP
#define TERMITE_HPP
#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include "coordv2.hpp"
#include "grillev2.hpp"
using namespace std;
struct Termite{
	int numero;
    Coord co;
    Direction d;
    bool brindille;
    int sablier;
	bool TourneSurPlace;
	int vie;
	//constructeurs:
	Termite(int id, Coord c);
	Termite(int id, Coord c,Grille &g);
	//méthodes
	//méthodes non commentées = déjà présentées dans la v1
	string toString() const;
	Direction directionTermite() const;
    Coord devant() const;
    bool porteBrindille() const;
    void tourneADroite();
    void tourneAGauche();
    void tourneAleat();
    bool laVoieEstLibre(Grille &g) const;
    bool brindilleEnFace(Grille &g) const;
    int voisinsLibre(Grille &g) const;
    int brindilleAutour(Grille &g) const;
    void avance(Grille &g);
    void marcheAleatoire(Grille &g);
    void chargeBrindille(Grille &g);
	void chargeBrindilleEnnemie(Grille &g);
    void dechargeBrindille(Grille &g, int idB);
    void vieTermite(Grille &g, int &sablier, int &tempsMort);
	int NidAutour(Grille &g, int idB, Direction &d) const; //renvoie le nombre de nids autour du termite
	bool SonNidEnFace(Grille &g, int idB)const; //renvoie true si le termite a son nid en face de lui, false sinon
	int brindilleTeamAutour(Grille &g, int equipe) const; //renvoie le nombre de brindilles appartenant à l'equipe du termite présentes autour de lui
	void perdVie(); //enlève la vie au termite à chaque tour
	bool brindilleEnnemieEnFace(Grille &g,int equipe) const; //renvoie true si la case devant le termite contient une brindille ennemie, false sinon
};
#endif