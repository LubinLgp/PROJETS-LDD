#include "grille.hpp"
#include "coord.hpp"
#include "coord.cpp"
#include "grille.cpp"
#include "termite.cpp"
#include "termite.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include <unistd.h>

using namespace std;
const float densiteBrindille = 0.05;
const int nbTermite=20;

vector<Termite> T;


std::ostream& operator<< (std::ostream &out, Grille g) { //opérateur d'affichage de la grille
    for (int i = 0; i < TailleGrille; i++){//parcourt toutes les lignes
        out<<"|";
        for (int j = 0; j < TailleGrille; j++){//parcourt les colonnes
            if(g.G[j][i].brindille){out<<" * ";}
            else if(g.G[j][i].termite !=-1){out <<" " <<"\033[31;1m"<<T[g.G[j][i].termite]<<"\033[0m"<<" " ;}//affiche les termites en rouge
            else{out<<"   ";}
        }
        out <<"|"<< endl;
    }
    return out;
}

Grille initGrille(){//initialisation de la grille
    Grille g;
    for(int i=0;i<TailleGrille;i++){
        for(int j=0;j<TailleGrille;j++){
            g.G[i][j];
            int proba=rand()%100;
            if(proba < densiteBrindille*100){g.poseBrindille(Coord{i,j});}//pose les brindilles aléatoirement
        }
    }
    for(int k=0;k<nbTermite;k++){//pose tous les termites et les ajoute au vecteur T
        int x;int y;
        do{
            x=rand()%TailleGrille;
            y=rand()%TailleGrille;
        }while(not g.estVide(Coord{x,y}));
        Termite t=Termite{k,Coord{x,y},g};
        T.push_back(t);
    }
    return g;
}


int main(){
    srand(time(NULL)); 
    Grille g=initGrille();
    cout<<g<<endl;
    char v;
    vector<int> Tsablier={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //sablier de chaque termite
    int vitesse=1;
    while(true){
        v=getchar();
        if(v=='*'){vitesse=vitesse*2;} //affiche deux fois plus de tour en une pression de la touche si "*" entré
        else if(v=='/' and vitesse > 1){vitesse = vitesse/2;} //pareil mais pour diminuer
        for(int i = 0;i<vitesse;i++){
            for(int i =0;i<nbTermite;i++){
                T[i].vieTermite(g,Tsablier[i]);//fait vivre les termites un par un
            }
        }
        cout << "\x1B[2J\x1B[H";//clear l'ancienne grille de l'ancien tour
        cout<<g<<endl;
    }
}