#include "coord.hpp"
#include "grille.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
//#include "doctest.h"
using namespace std;


Coord::Coord(int posx, int posy):x{posx}, y{posy}{
    if(posx <0){posx=0;}
    if(posy<0){posy=0;}
	if(posx < 0 or posx >= TailleGrille){
		cout<<posx<<" "<<posy<<endl;
        throw runtime_error("La coordonnée x n'est pas dans la grille");
	}
	if(posy < 0 or posy >= TailleGrille){
        cout<<posx<<" "<<posy<<endl;
		throw runtime_error("La coordonnée y n'est pas dans la grille");
	}
}

/*
la methode getLig retrouner x de la coordonnée
*/
int Coord::getLig(){
	return this->x;
}
/*
la methode getCol retrouner y de la coordonnée
*/
int Coord::getCol(){
	return this->y;
}



ostream& operator<<(ostream& out, Coord c) {
	out<<"( "<<c.getLig()<<", "<<c.getCol()<<" )";
	return out;
}

/*
Verifie que deux coordonnées sont egales 
*/
bool operator==(Coord c1, Coord c2) {
	if(c1.getLig() == c2.getLig() and c1.getCol() == c2.getCol()){return true;}
	return false;
}




 

 
std::ostream& operator<< (std::ostream &out, Direction d) {
	switch(d) {
		case Direction::nord_ouest : out << "nord_ouest" ; break;
		case Direction::nord : out << "nord" ; break ;
		case Direction::nord_est : out << "nord_est" ; break;
		case Direction::est : out << "est" ; break;
		case Direction::ouest : out << "ouest" ; break;
		case Direction::sud : out << "sud" ; break;
		case Direction::sud_ouest : out << "sud_ouest" ; break;
		case Direction::sud_est : out << "sud_est" ; break;
		default : throw invalid_argument("Direction n'existe pas"); break; 
	}		
	return out; 
}


/*
fonction aGauche renvoie la direction a gauche d'une direction
@param une direction d
retrun la directrion à gauche de d 
*/
Direction aGauche(Direction d){
	return (Direction((int(d)+7)%8));
}
 

/*
fonction aDroite renvoie la direction à droite d'une direction
@param une direction d
retrun la directrion à droite de d 
*/
Direction aDroite(Direction d){
	return (Direction((int(d)+1)%8));
}

/*
la fonciton devantCoord
@param une direction dir
@param une coordonée c
retrunr les coordonnées de la case devant en fonction de la direction 
*/
Coord devantCoord(Direction dir, Coord c){
    int x; int y;
	switch(dir){
		case Direction::nord: x=c.getLig();y=c.getCol()-1;break;
		case Direction::sud:  x=c.getLig();y=c.getCol()+1;break;
		case Direction::est: x=c.getLig()+1;y=c.getCol();break;
		case Direction::ouest:  x=c.getLig()-1;y=c.getCol();break;
		case Direction::sud_est:  x=c.getLig()+1;y=c.getCol()+1;break;
		case Direction::sud_ouest:  x=c.getLig()-1;y=c.getCol()+1;break;
		case Direction::nord_est:  x=c.getLig()+1;y=c.getCol()-1;break;
		case Direction::nord_ouest:  x=c.getLig()-1;y=c.getCol()-1;break;
        //default : return c;
	}
    if(x<0)x=TailleGrille+x;
    if(y<0)y=TailleGrille+y;
    x=x%TailleGrille;                                                       //les modulos nous permettent que les termites ne sortent pas du 
    y=y%TailleGrille;                                                       //tableau, si une termite se situe en {19, 5} et avance vers l'est 
    return Coord{x,y};                                                      //elle se retrouvera en {0, 5}.
}


