#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
#include "coordv2.hpp"
#include "grillev2.hpp"
#include "termitev2.hpp"
#include <cstdlib>
#include <time.h>
#include <vector>

const int dureeSablier = 6;
const float probaTourner = 0.1;
vector<Coord> Nid;
Termite::Termite(int id, Coord c):numero{id}, co{c},d{Direction(rand()%8)}, brindille{false},sablier{6},TourneSurPlace{false},vie{250+rand()%250}{}

Termite::Termite(int id, Coord c,Grille &g):numero{id}, co{c},d{Direction(rand()%8)}, brindille{false},sablier{6},TourneSurPlace{false},vie{250+rand()%250}{
    g.G[c.getLig()][c.getCol()].termite = id;
}

string Termite::toString() const {
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
    out<<t.toString();
    return out;
}

Direction Termite::directionTermite() const{
    return this->d;
}

Coord Termite::devant() const{
    return devantCoord(this->d,this->co);
}

bool Termite::porteBrindille() const{return this->brindille;}

void Termite::tourneADroite(){
    this->d=aDroite(d);
}

void Termite::tourneAGauche(){
    this->d=aGauche(d);
}

void Termite::tourneAleat(){
    int random=rand()%2;
    if(random == 0)
        tourneAGauche();
    else
        tourneADroite();
}

bool Termite::laVoieEstLibre(Grille &g) const{
    Coord c = devant();
    return g.estVide(c);
}

bool Termite::brindilleEnFace(Grille &g)const {
    Coord c = devant();
    return g.contientBrindille(c);
}

int Termite::voisinsLibre(Grille &g) const{
    int compteur=0;
    Termite t2=*this;
    for(int i=0;i<8;i++){
        t2.tourneADroite();
        if(t2.laVoieEstLibre(g)){compteur++;}
    }
    return compteur;
}

void Termite::avance(Grille &g){
    Coord c = devant();
    g.enleveTermite(this->co);
    this->co = c;
    g.poseTermite(this->co,this->numero);

}

void Termite::marcheAleatoire(Grille &g){
    tourneAleat();
    avance(g);
}

void Termite::chargeBrindille(Grille &g){
    if (brindilleEnFace(g) and  not this->brindille){
        Coord c = devant();
        this->brindille = true;
        g.enleveBrindille(c);
    }
}

void Termite::chargeBrindilleEnnemie(Grille &g){
    if (not this->brindille){
        Coord c = devant();
        this->brindille = true;
        g.enleveBrindille(c);
    }
}

void Termite::dechargeBrindille(Grille &g, int idB){
    if (not brindilleEnFace(g) and this->brindille){
        Coord c = devant();
        this->brindille = false;
        g.poseBrindille(c,idB); //attribut un identifiant à la brindille
    }
}

int Termite::brindilleAutour(Grille &g) const{
    int compteur=0;
    Termite t2=*this;
    for(int i=0;i<8;i++){
        t2.tourneADroite();
        if(t2.brindilleEnFace(g)){compteur++;}
    }
    return compteur;
}

int Termite::brindilleTeamAutour(Grille &g, int equipe) const{ //renvoie le nombre de brindilles autour du termite appartenant  déjà à son équipe
    int compteur=0;
    Termite t2=*this;
	Coord c=t2.co;
    for(int i=0;i<8;i++){
        t2.tourneADroite();
		c=devant();
        if(g.G[c.getLig()][c.getCol()].brindille and g.G[c.getLig()][c.getCol()].equipeBrindille == equipe){compteur++;}
    }
    return compteur;
}

int equipe(int numero){
	if(numero >= 0 and numero < 5){return 0;}
	if(numero >= 5 and numero < 10){return 1;}
	if(numero >= 10 and numero < 15){return 2;}
	if(numero >= 15 and numero < 20){return 3;}
}



bool Termite::SonNidEnFace(Grille &g, int idB)const { //renvoie true si la case en face du termite contient son nid
    Coord c = devant();
    return g.contientSonNid(c, idB);
}

int Termite::NidAutour(Grille &g, int idB, Direction &dir) const{ //renvoie le nombre de nids autour du termite
    int compteur=0;
    Termite t2=*this;
    for(int i=0;i<8;i++){
        t2.tourneADroite();
        if(t2.SonNidEnFace(g, idB)){dir=t2.d;return true;}
    }
    return false;
}

bool aCoteCoord(Coord c1, Coord c2){ //renvoie true si deux coordonnées sont séparées de moins d'une case, false sinon
	int lig=(abs(c1.getLig() - c2.getLig()))%TailleGrille;
	int col=(abs(c1.getCol() - c2.getCol()))%TailleGrille;
	if(lig <=1 and col<=1){return true;}
	return false;
}


void Termite::perdVie(){
    if (this->brindille)
        this->vie -=2;
    else 
        this->vie--;
}

bool Termite::brindilleEnnemieEnFace(Grille &g,int equipe) const{ //renvoie true si la case en face contient une brindille ennemie
	Coord c=devant();
    if(g.G[c.getLig()][c.getCol()].brindille and g.G[c.getLig()][c.getCol()].equipeBrindille != equipe){return true;}
	return false;
}


void Termite::vieTermite(Grille &g, int &sablier, int &tempsMort){ //temps mort correspond à la durée depuis laquelle le termite est mort (inutile si le termite est en vie)
	if(vie>0){//si le termite est en vie
		int proba = rand()%100;
		if (proba < probaTourner*100)
			tourneAleat();
		//cout<<(this->d)<<endl;
		if(laVoieEstLibre(g)){
			Coord c1=this->co;
			(*this).avance(g);
			if(brindilleEnFace(g) and not this->brindille and sablier > dureeSablier and vie > 100){ //ne récupère une brindille que si sa vie est superieure à 100
				chargeBrindille(g);
				sablier=0;
			}if(not this->brindille and sablier>dureeSablier and vie>400 and brindilleEnnemieEnFace(g,equipe(numero))){//ne récupère une brindille ennemie que si sa vie est supérieure à 400
				chargeBrindilleEnnemie(g);
				sablier=0;
				vie-=10;//perd 10 points de vie (c'est plus fatiguant que ramasser une brindille neutre)
			}
			if(sablier > dureeSablier and porteBrindille() and voisinsLibre(g) > 0){
				int numequipe=equipe(numero);
				Direction dir;
				if(NidAutour(g,numequipe,dir)){
					if (laVoieEstLibre(g)){
						Coord c2=devant();
						if(aCoteCoord(c2,Nid[numequipe])){ //ne pose la brindille que si le nid est le sien
							dechargeBrindille(g,numequipe);
							sablier=0;
						}else{tourneADroite();}
					}else{
						tourneADroite();}
				}else if(brindilleAutour(g) >=1){ //pose en neutre (dans le cas ou il n'est pas a coté de son nid)
					if (laVoieEstLibre(g)){
						dechargeBrindille(g,-1);
						sablier=0;
					}else
						tourneADroite();
				}
			}
			perdVie();
			sablier++;
		}
	}else{//si il meurt
		if(tempsMort==20){//si il vient de mourir
			if(g.estVide(devant()) and porteBrindille()){g.poseBrindille(devant(),-1);} //si il porte une brindille, il essaye de la poser. Si il ne peut pas la poser il l'emmene dans sa tombe
			g.poseCadavre(this->co); //on crée le cadavre
			tempsMort--;
		}
		else if(tempsMort > 0){
			tempsMort--;
		}
		else{//au bout de 20 tours, son cadavre disparait et fait naitre un nouveau termite
			tempsMort=20;
			g.retireCadavre(this->co);
			int x;int y;
			do{//pose un nouveau termite a des coordonnées valides
				x=rand()%TailleGrille;
				y=rand()%TailleGrille;
			}while(not g.estVide(Coord{x,y}));
			g.enleveTermite(this->co);
			this->co=Coord{x,y};
			g.poseTermite(this->co,this->numero);
			this->d=Direction(rand()%8);
			this->brindille=false;
			sablier=0;
			this->vie=250+rand()%250;
		}
	}
}
