#ifndef COORD_HPP
#define COORD_HPP
#include <iostream>
#include <ostream>
#include <string>
#include <stdlib.h>
using namespace std;
class Coord{
	int x;
	int y;
	public:
	//constructeurs:
	Coord(int lig, int col);
	
	//méthodes
	int getCol();
	int getLig();
};
ostream& operator<<(ostream& out, Coord &c);
bool operator==(Coord c1, Coord c2);
enum class Direction {nord_ouest, nord, nord_est, est, sud_est, sud, sud_ouest, ouest };
std::ostream& operator<< (std::ostream &out, Direction d);
Direction aGauche(Direction d);                                             // renvoie la direction aGauche de d
Direction aDroite(Direction d);                                             // renvoie la directon aDroite de d 
Coord devantCoord(Direction dir, Coord c);                                  // renvoie la coordonnée devant c en fonction de dir

#endif