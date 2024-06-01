#ifndef GRILLE_HPP
#define GRILLE_HPP
#include <iostream>
#include <ostream>
#include <string>
#include <array>
#include "coordv2.hpp"
using namespace std;
const int TailleGrille=20; 


struct Case{
    bool brindille;
	int equipeBrindille;
    int termite;
	int nid;
	bool cadavre;
    //Constructeur:
    Case();
};

struct Grille{
    public:
        array<array<Case,TailleGrille>,TailleGrille> G;
        void poseBrindille(Coord c, int idB);                    //pose une brindille sur la coordonnée c
		void poseNid(Coord c, int idN);                          //pose un nid d'identifiant idN à la coordonnée c
        void enleveBrindille(Coord c);                           //enleve une brindille sur la coordonnée c
		bool contientSonNid(Coord c,int idB);                    //renvoie true si idb est le meme que l'identifiant du nid à c, false sinon
        bool contientBrindille(Coord c) const;                   //renvoie true si il y a une brindille sur c, false sinon
        void poseTermite(Coord c, int idT);                      //pose une termite sur c avec l'identifiant idT
        void enleveTermite(Coord c);                             //enleve la termite presente sur c
        int numeroTermite(Coord c) const;                        //renvoie l'identifaint de la termite presente sur c
        bool estVide(Coord c) const;                             //renvoie true si la case à la coordonnée c est vide, false sinon
		void poseCadavre(Coord c);                               //pose un cadavre sur c
		void retireCadavre(Coord c);                             //retire le cadavre de la coordonnée c

};
std::ostream& operator<< (std::ostream &out, Grille g);
#endif