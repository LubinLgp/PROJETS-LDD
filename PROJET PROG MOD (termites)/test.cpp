#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <ostream>
using namespace std;
Grille g1; //pour les tests
TEST_CASE("Constructeur + get"){
	CHECK_THROWS_AS((Coord{-1,2}),runtime_error);
	Coord c{5,3};
	CHECK(c.getLig() == 5);
	CHECK(c.getCol() == 3);
}
TEST_CASE("Operateur =="){
	Coord c1{5,1};
	Coord c2{1,5};
	Coord c3{5,1};
	CHECK(c1==c3);
	CHECK_FALSE(c1==c2);
}
TEST_CASE("a Gauche"){
	CHECK(aGauche(Direction::nord)==Direction::nord_ouest); 
	CHECK(aGauche(Direction::nord_est)==Direction::nord);
	CHECK(aGauche(Direction::est)==Direction::nord_est);
	CHECK(aGauche(Direction::sud_est)==Direction::est);
	CHECK(aGauche(Direction::sud)==Direction::sud_est);
	CHECK(aGauche(Direction::sud_ouest)==Direction::sud);
	CHECK(aGauche(Direction::ouest)==Direction::sud_ouest);
	CHECK(aGauche(Direction::nord_ouest)==Direction::ouest);
}

TEST_CASE("a Droite"){
	CHECK(aDroite(Direction::nord)==Direction::nord_est); 
	CHECK(aDroite(Direction::nord_est)==Direction::est);
	CHECK(aDroite(Direction::est)==Direction::sud_est);
	CHECK(aDroite(Direction::sud_est)==Direction::sud);
	CHECK(aDroite(Direction::sud)==Direction::sud_ouest);
	CHECK(aDroite(Direction::sud_ouest)==Direction::ouest);
	CHECK(aDroite(Direction::ouest)==Direction::nord_ouest);
	CHECK(aDroite(Direction::nord_ouest)==Direction::nord);
}
 
TEST_CASE("Droite puis Gauche ou Gauche puis Droite"){
	Direction n = Direction::nord; 
	Direction interm1 = aGauche(Direction::nord);
	Direction interm2 = aDroite(interm1); 
	CHECK(interm2 == n);
	Direction s = Direction::sud;
	Direction interm3 = aDroite(Direction::sud); 
	Direction interm4 = aGauche(interm3); 
	CHECK(interm4 == s); 
	Direction e = Direction::est; 
	for (int i = 0; i < 8; i++)
		e = aGauche(e);
	CHECK(e == Direction::est);
	Direction o = Direction::ouest;
	for (int j = 0; j < 8; j++)
		o = aDroite(o);
	CHECK(o == Direction::ouest);
}
TEST_CASE("Devant coord"){
    Coord c = devantCoord(Direction::nord, Coord{5,6});
    Coord d{5,5};
    CHECK(c == d);
    c=devantCoord(Direction::sud_ouest, Coord{6,4});
    CHECK(c == d);
    c=devantCoord(Direction::sud, Coord{5,4});
    CHECK(c == d);
    c=devantCoord(Direction::sud_est, Coord{4,4});
    CHECK(c == d);
    c=devantCoord(Direction::ouest, Coord{6,5});
    CHECK(c == d);
    c=devantCoord(Direction::est, Coord{4,5});
    CHECK(c == d);
    c=devantCoord(Direction::nord_est, Coord{4,6});
    CHECK(c == d);
    c=devantCoord(Direction::nord_ouest, Coord{6,6});
    CHECK(c == d);
    c=devantCoord(Direction::sud,devantCoord(Direction::nord, Coord{5,5}));
    CHECK(c== d);
    c=devantCoord(Direction::sud_est,devantCoord(Direction::nord_ouest, Coord{5,5}));
    CHECK(c== d);
    c=devantCoord(Direction::sud_est,devantCoord(Direction::sud, Coord{4,3}));
    CHECK(c == d);
    //CHECK_THROWS_AS(devantCoord(Direction::nord,Coord{0,0}),runtime_error);
    //CHECK_THROWS_AS(devantCoord(Direction::nord_est,Coord{1,TailleGrille}),runtime_error);
    for(int i =0;i<8;i++){
        int x = rand()%(TailleGrille-2)+1; //pour ne pas risquer de sortir de la grille
        int y= rand()%(TailleGrille-2)+1;
        Coord posinit{x,y};
        c=devantCoord(Direction(i),Coord{x,y});
        Direction dfinal = aDroite(aDroite(aDroite(aDroite(Direction(i)))));
        c=devantCoord(dfinal,c);
        CHECK(c == posinit);
    }
}

TEST_CASE("pose brindille"){
    g1.G[6][5];
    CHECK_FALSE(g1.G[5][5].brindille);
    g1.poseBrindille(Coord{5,5});
    CHECK(g1.G[5][5].brindille);
    CHECK_THROWS_AS(g1.poseBrindille(Coord{5,5}),runtime_error);
}
TEST_CASE("enlève brindille"){
    CHECK(g1.G[5][5].brindille);
    g1.enleveBrindille(Coord{5,5});
    CHECK_FALSE(g1.G[5][5].brindille);
    CHECK_THROWS_AS(g1.enleveBrindille(Coord{5,5}),runtime_error);
}
TEST_CASE("contientBrindille"){
    g1.G[7][5];
    g1.poseBrindille(Coord{7,5});
    CHECK(g1.contientBrindille(Coord{7,5}));
    CHECK_FALSE(g1.contientBrindille(Coord{5,5}));
}
TEST_CASE("pose Termite"){
    g1.G[1][2];
    g1.G[3][2];
    g1.G[1][1];
    g1.poseTermite(Coord{1, 2}, 2);
    CHECK(g1.G[1][2].termite == 2);
    g1.poseTermite(Coord{3, 2}, 1);
    CHECK(g1.G[3][2].termite == 1);
    CHECK_FALSE(g1.G[1][1].termite == 4);
    CHECK_THROWS_AS(g1.poseTermite(Coord{3, 2}, 1), runtime_error);
}
TEST_CASE("enleveTermite"){
    Grille g;
    g.G[1][2];
    g.G[3][2];
    g.G[1][1];
    g.G[3][5];
    g.G[2][3];
    g.poseTermite(Coord{1, 2},1);
    g.poseTermite(Coord{2, 3},2);
    CHECK(g.G[1][2].termite == 1);
    g.enleveTermite(Coord{1, 2});
    CHECK(g.G[1][2].termite == -1);
    /**CHECK_THROWS_AS(g.enleveTermite(Coord{1,2}), runtime_error);
    g.enleveTermite({2, 3});
    CHECK(g.G[2][3].termite == -1);**/
}
TEST_CASE("numero Termite"){
    Grille g;
    g.G[1][3];
    g.G[2][2];
    g.G[5][3];
    g.G[3][5];
    g.G[2][3];
    g.poseTermite({1, 3}, 2);
    g.poseTermite({2, 2}, 4);
    CHECK(g.numeroTermite(Coord{1, 3}) == 2);
    CHECK(g.numeroTermite(Coord{2, 2}) == 4);
    CHECK_THROWS_AS(g.numeroTermite({5, 3}), runtime_error);
}
TEST_CASE("Est Vide"){
    Grille g;
    g.G[1][2];
    g.G[2][2];
    g.G[3][3];
    g.poseBrindille(Coord{1, 2});
    g.poseTermite(Coord{1, 2},5);
    g.enleveTermite(Coord{1, 2});
    CHECK_FALSE(g.estVide(Coord{1, 2}));
    g.poseBrindille(Coord{2, 2});
    g.poseTermite(Coord{2, 2},9);
    g.enleveTermite(Coord{2, 2});
    g.enleveBrindille(Coord{2, 2});
    CHECK(g.estVide(Coord{2, 2}));
    CHECK(g.estVide(Coord{3, 3}));
}


//TEST TERMITE

TEST_CASE("toString"){
    Termite t{0,Coord{5,5}};
    t.d=Direction::nord;
    CHECK(t.toString()=="|");
    t.d=Direction::sud;
    CHECK(t.toString()=="|");
    t.d=Direction::ouest;
    CHECK(t.toString()=="-");
    t.d=Direction::est;
    CHECK(t.toString()=="-");
    t.d=Direction::sud_est;
    CHECK(t.toString()=="\\");
    t.d=Direction::sud_ouest;
    CHECK(t.toString()=="/");
    t.d=Direction::nord_est;
    CHECK(t.toString()=="/");
    t.d=Direction::nord_ouest;
    CHECK(t.toString()=="\\");
}

TEST_CASE("direction Termite"){
    Termite t{0, Coord{5, 5}};
    t.d = Direction::nord;
    CHECK(t.directionTermite() == Direction::nord);
    t.d = Direction::nord_ouest;
    CHECK(t.directionTermite() == Direction::nord_ouest);
    t.d = Direction::nord_est;
    CHECK(t.directionTermite() == Direction::nord_est);
    t.d = Direction::ouest;
    CHECK(t.directionTermite() == Direction::ouest);
    t.d = Direction::est;
    CHECK(t.directionTermite() == Direction::est);
    t.d = Direction::sud_ouest;
    CHECK(t.directionTermite() == Direction::sud_ouest);
    t.d = Direction::sud_est;
    CHECK(t.directionTermite() == Direction::sud_est);
}

TEST_CASE("Devant"){
    Termite t{0,Coord{5,5}};
    Coord c{0,0};
    t.d=Direction::nord;
    c=t.devant();
    CHECK(c == Coord{5,4});
    t.d=Direction::sud;
    c=t.devant();
    CHECK(c == Coord{5,6});
    t.d=Direction::est;
    c=t.devant();
    CHECK(c == Coord{6,5});
    t.d=Direction::ouest;
    c=t.devant();
    CHECK(c == Coord{4,5});
    t.d=Direction::sud_est;
    c=t.devant();
    CHECK(c == Coord{6,6});
    t.d=Direction::sud_ouest;
    c=t.devant();
    CHECK(c == Coord{4,6});
    t.d=Direction::nord_est;
    c=t.devant();
    CHECK(c == Coord{6,4});
    t.d=Direction::nord_ouest;
    c=t.devant();
    CHECK(c == Coord{4,4});
}

TEST_CASE("Porte Brindille"){
    Termite t{0,Coord{5,5}};
    CHECK(not t.porteBrindille());
    t.brindille=true;
    CHECK(t.porteBrindille());
}

TEST_CASE("Tourne à droite"){
    Termite t{0,Coord{5,5}};
    t.d=Direction::ouest;
    for(int i=0;i<8;i++){
        t.tourneADroite();
        CHECK(t.d == Direction(i));
    }
}

TEST_CASE("Tourne à gauche"){
    Termite t{0,Coord{5,5}};
    t.d=Direction::nord_ouest;
    for(int i=0;i<8;i++){
        t.tourneAGauche();
        CHECK(t.d == Direction(7-i));
    }
}

TEST_CASE("Voie est libre"){
    Grille g;
    Termite t{0, Coord{5, 5}};
    t.d=Direction::sud;
    g.G[5][6];
    CHECK(t.laVoieEstLibre(g));
    Termite t3{1, Coord{1, 2}};
    t3.d=Direction::sud;
    g.G[1][3];
    g.G[1][3].brindille = true;
    CHECK_FALSE(t3.laVoieEstLibre(g));
}


TEST_CASE("Brindille en face"){
    Grille g;
    Termite t{0, Coord{2, 2}};
    t.d=Direction::sud;
    g.G[2][3];
    g.G[2][3].brindille = false;
    CHECK_FALSE(t.brindilleEnFace(g));
    Termite t2{3, Coord{3, 4}};
    t2.d=Direction::sud;
    g.G[3][5];
    g.G[3][5].brindille = true;
    CHECK(t2.brindilleEnFace(g));
}

TEST_CASE("Voisins libres"){
    Grille g;
    for(int i=0;i<TailleGrille;i++){
        for(int j=0;j<TailleGrille;j++){
            g.G[i][j];
        }
    }
    Termite t{0,Coord{5,5}};
    CHECK(t.voisinsLibre(g) == 8);
    g.poseBrindille(Coord{6,6});
    g.poseTermite(Coord{4,4},2);
    g.poseBrindille(Coord{6,5});
    g.poseTermite(Coord{6,5},1);
    CHECK(t.voisinsLibre(g) == 5);
}

TEST_CASE("Charge Brindille"){
    Grille g;
    Termite t{0, Coord{2, 2}};
    t.d=Direction::sud;
    g.G[2][3];
    g.G[2][3].brindille = true;
    t.chargeBrindille(g);
    CHECK(t.brindille);
    CHECK_FALSE(g.G[2][3].brindille);
    Termite t2{1, Coord{1, 3}};
    t2.d=Direction::sud;
    g.G[1][4];
    t2.brindille = true;
    t2.chargeBrindille(g);
    CHECK_FALSE(g.G[1][4].brindille);
    CHECK(t2.brindille);
}
 
TEST_CASE("decharge Brindille"){
    Grille g;
    Termite t{0, Coord{2, 2}};
    t.d=Direction::est;
    t.brindille = true;
    g.G[3][2];
    g.G[3][2].brindille = false;
    t.dechargeBrindille(g);
    CHECK_FALSE(t.brindille);
    CHECK(g.G[3][2].brindille);
    Termite t2{2, Coord{2, 5}};
    t2.d=Direction::sud;
    g.G[2][6];
    g.G[2][6].brindille = true;
    t2.brindille = true;
    t2.dechargeBrindille(g);
    CHECK(t2.brindille);
    CHECK(g.G[2][6].brindille);
}


TEST_CASE("Avance"){
    Grille g;
    for(int i=0;i<TailleGrille;i++){
        for(int j=0;j<TailleGrille;j++){
            g.G[i][j];
        }
    }
    Termite t{0,Coord{5,5},g};
    t.d=Direction::nord;
    CHECK(t.co == Coord{5,5});
    CHECK(g.G[5][5].termite == 0);
    CHECK(g.G[5][4].termite == -1);
    t.avance(g);
    CHECK(g.G[5][5].termite == -1);
    CHECK(g.G[5][4].termite == 0);
    CHECK(t.co == Coord{5,4});
    t.avance(g);
    t.tourneADroite();
    t.avance(g);
    CHECK(t.co == Coord{6,2});
    
}

TEST_CASE("Marche aléatoire"){
    Grille g;
    for(int i=0;i<TailleGrille;i++){
        for(int j=0;j<TailleGrille;j++){
            g.G[i][j];
        }
    }
    Termite t{0,Coord{5,5},g};
    t.d=Direction::nord;
    Coord c=t.co;
    CHECK(g.G[5][5].termite == 0);
    t.marcheAleatoire(g);
    CHECK(g.G[5][5].termite == -1);
    CHECK_FALSE(c == t.co);
}