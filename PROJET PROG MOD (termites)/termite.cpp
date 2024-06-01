#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"
#include <cstdlib>
#include <time.h>
const int dureeSablier = 6;
const float probaTourner = 0.1;


Termite::Termite(int id, Coord c):numero{id}, co{c},d{Direction(rand()%8)}, brindille{false},sablier{6},TourneSurPlace{false}{}

Termite::Termite(int id, Coord c,Grille &g):numero{id}, co{c},d{Direction(rand()%8)}, brindille{false},sablier{6},TourneSurPlace{false}{ //même principe que la précédente mais pour actualiser la grille en même temps (utile pour notre algorithme, l'autre sert principalement pour les tests)
    g.G[c.getLig()][c.getCol()].termite = id;
}

string Termite::toString() const { //convertit un termite en un caractère en fonction de son orientation pour l'affichage
    Direction d = this->d;
    string termite = " ";
    switch(d){
        case Direction::nord_ouest : termite = "\\" ; break;
        case Direction::nord : termite = "|"; break;
        case Direction::nord_est : termite = "/"; break;
        case Direction::est : termite = "-"; break;
        case Direction::ouest : termite = "-"; break;
        case Direction::sud : termite = "|"  ; break;
        case Direction::sud_ouest : termite = '/' ; break;
        case Direction::sud_est : termite = "\\" ; break;
        default : throw invalid_argument("Direction n'existe pas"); break;
    }
    return termite;
}

std::ostream& operator<< (std::ostream &out, Termite t) {
    out<<t.toString(); //permet l'affichage
    return out;
}

Direction Termite::directionTermite() const{ //renvoie la direction du termite
    return this->d;
}

Coord Termite::devant() const{ //renvoie les coordonnées devant le termite (dans la direction qu'il regarde)
    return devantCoord(this->d,this->co);
}

bool Termite::porteBrindille() const{return this->brindille;} //renvoie true si le termite porte une brindille, false sinon

void Termite::tourneADroite(){ //fait tourner le termite à droite à 45 degré
    this->d=aDroite(d);
}

void Termite::tourneAGauche(){//fait tourner le termite à gauche à 45 degré
    this->d=aGauche(d);
}

void Termite::tourneAleat(){ //tourne dans une direction aléatoire
    int random=rand()%2;
    if(random == 0)
        tourneAGauche();
    else
        tourneADroite();
}

bool Termite::laVoieEstLibre(Grille &g) const{ //renvoie true si le termite peut avancer ou poser quelque chose dans la case devant lui, false sinon
    Coord c = devant();
    return g.estVide(c);
}

bool Termite::brindilleEnFace(Grille &g)const { //renvoie true si la case devant le termite contient une brindille
    Coord c = devant();
    return g.contientBrindille(c);
}

int Termite::voisinsLibre(Grille &g) const{ //renvoie le nombre de cases libres autour du termite
    int compteur=0;
    Termite t2=*this;
    for(int i=0;i<8;i++){
        t2.tourneADroite();
        if(t2.laVoieEstLibre(g)){compteur++;}
    }
    return compteur;
}

void Termite::avance(Grille &g){ //fait avancer le termite d'une case
    Coord c = devant();
    g.enleveTermite(this->co);
    this->co = c;
    g.poseTermite(this->co,this->numero); //actualiser la grille
    
}

void Termite::marcheAleatoire(Grille &g){//marche en tournant aléaotirement
    tourneAleat();
    avance(g);
}

void Termite::chargeBrindille(Grille &g){ //prend une brindille sur la case devant lui si il peut
    if (brindilleEnFace(g) and  not this->brindille){
        Coord c = devant();
        this->brindille = true;
        g.enleveBrindille(c); //actualiser la grille
    }
}

void Termite::dechargeBrindille(Grille &g){ //pose une brindille dans la case devant lui si il peut
    if (not brindilleEnFace(g) and this->brindille){
        Coord c = devant();
        this->brindille = false;
        g.poseBrindille(c); //actualiser la grille
    }
}

int Termite::brindilleAutour(Grille &g) const{ //renvoie le nombre de brindilles autour du termite
    int compteur=0;
    Termite t2=*this;
    for(int i=0;i<8;i++){ //regarde toutes les directions
        t2.tourneADroite();
        if(t2.brindilleEnFace(g)){compteur++;}
    }
    return compteur;
}

void Termite::vieTermite(Grille &g, int &sablier){ //algorithme de vie du termite, pour un tour
    int proba = rand()%100;
    if (proba < probaTourner*100) //a proba chances de tourner
        tourneAleat();
    //cout<<(this->d)<<endl;
    if(laVoieEstLibre(g)){ //on ne fait bouger le termite que si la voie est libre
        Coord c1=this->co;
        (*this).avance(g); //fait avancer le termite
        if(brindilleEnFace(g) and not this->brindille and sablier > dureeSablier){ //si il peut ramasser une brindille, que la case devant lui en contient une et qu'il n'en a pas récupérée ou posée dans les 6 derniers tours
            chargeBrindille(g);
            sablier=0;
        }
        if(sablier > dureeSablier and porteBrindille() and voisinsLibre(g) > 0 and brindilleAutour(g) >= 1){ //même principe pour les poser
            if (laVoieEstLibre(g)){
                dechargeBrindille(g);
                sablier=0;
            }else
                tourneADroite();//tourne à droite jusqu'à trouver une case libre pour poser la brindille
        }
        sablier++;
    }
}