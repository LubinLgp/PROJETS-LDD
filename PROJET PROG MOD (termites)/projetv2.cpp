#include "grillev2.hpp"
#include "coordv2.hpp"
#include "coordv2.cpp"
#include "grillev2.cpp"
#include "termitev2.cpp"
#include "termitev2.hpp"
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


std::ostream& operator<< (std::ostream &out, Grille g) {
	out<<"\033[1;1m"<<"_____________________________________________________________"<<"\033[0m"<<endl;
    for (int i = 0; i < TailleGrille; i++){
        out<<"|";
        for (int j = 0; j < TailleGrille; j++){
			if((i==5 and j==5) or (i==15 and j==15)){out <<" " <<"\033[1;1m"<<"O"<<"\033[0m"<<" " ;}//affiche les téléporteurs
			else if(g.G[j][i].cadavre){out<<" x ";}
			else if(g.G[j][i].brindille){
				if(g.G[j][i].equipeBrindille == 0){out <<" " <<"\033[31;1m"<<"*"<<"\033[0m"<<" " ;}//affiche les brindilles dans la couleur de leur équipe si elles en ont une, en blanc sinon
				else if(g.G[j][i].equipeBrindille == 1){out <<" " <<"\033[32;1m"<<"*"<<"\033[0m"<<" " ;}
				else if(g.G[j][i].equipeBrindille == 2){out <<" " <<"\033[33;1m"<<"*"<<"\033[0m"<<" " ;}
				else if(g.G[j][i].equipeBrindille == 3){out <<" " <<"\033[34;1m"<<"*"<<"\033[0m"<<" " ;}
				else{out<<" * ";}
			}
			else if(g.G[j][i].nid != -1){
				if(g.G[j][i].nid == 0){out <<" " <<"\033[31;1m"<<"#"<<"\033[0m"<<" " ;}//même principe pour les nids
				if(g.G[j][i].nid == 1){out <<" " <<"\033[32;1m"<<"#"<<"\033[0m"<<" " ;}
				if(g.G[j][i].nid == 2){out <<" " <<"\033[33;1m"<<"#"<<"\033[0m"<<" " ;}
				if(g.G[j][i].nid == 3){out <<" " <<"\033[34;1m"<<"#"<<"\033[0m"<<" " ;}
			}
            else if(g.G[j][i].termite !=-1){
				int n = T[g.G[j][i].termite].numero;
				if(T[g.G[j][i].termite].brindille){out<<" "<<"\033[40;1m";}
				else{out<<" ";}
				if(n >= 0 and n<5){out <<"\033[31;1m"<<T[g.G[j][i].termite]<<"\033[0m"<<" " ;}//affiche les termites dans la couleur de leur équipe
				else if(n >=5 and n< 10){out <<"\033[32;1m"<<T[g.G[j][i].termite]<<"\033[0m"<<" " ;}
				else if(n>=10 and n < 15){out <<"\033[33;1m"<<T[g.G[j][i].termite]<<"\033[0m"<<" " ;}
				else if(n>=15 and n< 20){out<<"\033[34;1m"<<T[g.G[j][i].termite]<<"\033[0m"<<" " ;}
			}
            else{out<<"   ";}
        }
        out <<"|"<< endl;
    }
	out<<"\033[1;1m"<<"--------------------------------------------------------------"<<"\033[0m"<<endl;
    return out;
}

bool estACote(Coord c,vector<Coord> v){
	int x1=c.getLig();
	int y1=c.getCol();
	for(int i=0;i<v.size();i++){
		int x2=v[i].getLig();
		int y2=v[i].getCol();
		if((abs(x2-x1))%TailleGrille < 4 or (abs(y2-y1))%TailleGrille <4)
			return true;
	}
	return false;
}

Grille initGrille(){
    Grille g;
    for(int i=0;i<TailleGrille;i++){
        for(int j=0;j<TailleGrille;j++){
            g.G[i][j];
            int proba=rand()%100;
            if(proba < densiteBrindille*100){g.poseBrindille(Coord{i,j},-1);}
        }
    }
	for(int t=0;t<4;t++){
        int x;int y;
        do{//crée les nids
            x=rand()%TailleGrille;
            y=rand()%TailleGrille;
        }while(not g.estVide(Coord{x,y}) and estACote(Coord{x,y},Nid));
		g.poseNid(Coord{x,y},t);
		Nid.push_back(Coord{x,y});
	}
    for(int k=0;k<nbTermite;k++){
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
    vector<int> Tsablier={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	vector<int> tempsMort={20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20}; //crée les temps morts (expliqués dans termitev2.cpp) dans un vecteur, comme Tsablier
    int vitesse=1;
    while(true){
        v=getchar();
        if(v=='*'){vitesse=vitesse*2;}
        else if(v=='/' and vitesse > 1){vitesse = vitesse/2;}
        for(int i = 0;i<vitesse;i++){
            for(int j=0;j<nbTermite;j++){
                T[j].vieTermite(g,Tsablier[j],tempsMort[j]);
            }
        }
        cout << "\x1B[2J\x1B[H";
        cout<<g<<endl;
    }
}
