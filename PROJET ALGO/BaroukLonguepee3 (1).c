#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
//LONGUEPEE Lubin
//BAROUK Aidan

typedef struct bloc_image{ 
	bool quatre ;
	struct bloc_image *hg, *hd, *bg, *bd ;
} bloc_image ;
typedef bloc_image *image ;


image Blanc() {
    image img = (image)malloc(sizeof(bloc_image));
    img->quatre = false;
    img->hg = NULL;
    img->hd = NULL;
    img->bg = NULL;
    img->bd = NULL;
    return img;
}


image Noir() {
    image img = (image)malloc(sizeof(bloc_image));
	img=NULL;
    return img;
}

image Compose(image i0, image i1, image i2, image i3) {
    image img = (image)malloc(sizeof(bloc_image));
    img->quatre = true;
    img->hg = i0;
    img->hd = i1;
    img->bg = i2;
    img->bd = i3;
    return img;
}


void Affichage(image img){
    if (img==NULL) printf("%s","X");
    else{
        if (!img->quatre) printf("%s","o");
        else{
            printf("%s","(");
            Affichage(img->hg);
            Affichage(img->hd);
            Affichage(img->bg);
            Affichage(img->bd);
            printf("%s",")");
        }
    }
}

void AffichageProfondeur(image img, int profondeur){
    if (img==NULL) printf("%s%d","X",profondeur);
    else{
        if (!img->quatre) printf("%s%d","o",profondeur);
        else{
            printf("%s","(");
            AffichageProfondeur(img->hg,profondeur+1);
            AffichageProfondeur(img->hd,profondeur+1);
            AffichageProfondeur(img->bg,profondeur+1);
            AffichageProfondeur(img->bd,profondeur+1);
            printf("%s",")");
        }
    }
}


image Lecture() {//On a pas trouvé d'autres moyens de le faire que en passant par un tableau qui sert de pile
    char c;
    image stack[100]; // Pile pour garder une trace des images
    int top = -1; // Indice supérieur de la pile

    while ((c = getchar()) != '!') {
        if (c == 'X') {
            stack[++top] = Noir();
        } else if (c == 'o') {
            stack[++top] = Blanc();
        } else if (c == '*') {
            image i3 = stack[top--];
            image i2 = stack[top--];
            image i1 = stack[top--];
            image i0 = stack[top--];
            stack[++top] = Compose(i0, i1, i2, i3);
        } 
    }
    return stack[top];
} 

bool EstNoire(image a) {
    if (a == NULL)
        return true ;
    else if (!a->quatre )
        return false;
    else 
        return EstNoire(a->hg) && EstNoire(a->hd) && EstNoire(a->bg) && EstNoire(a->bd);
}

bool EstBlanc(image a) {
    if (a == NULL)
        return false ;
    else if (! a->quatre)
        return true ;
    else 
        return EstBlanc(a->hd) && EstBlanc(a->hg) && EstBlanc(a->bd) && EstBlanc(a->bg);
}
float Aire(image a){
	if(a==NULL)
		return 1.0;
	else if(! a->quatre)
		return 0.0;
	else
		return (Aire(a->hg)+Aire(a->hd)+Aire(a->bg)+Aire(a->bd))/4.0;
}

image bis(int p, bool endessous) {
	if(p==0){
		return Blanc();
	}
	else{
		if(endessous){
			return Compose(bis(p-1,false),bis(p-1,true),bis(p-1,true),Noir());
		}
		else{
			return Blanc();
		}
	}

}
image TriangleBD(int p){
	return bis(p, true);
}

void Arrondit(image* i, int p){
	if(p==0 ){
		if (Aire((*i))>=0.5){
			*i=Noir();
		}
		else{
			*i=Blanc();
		}	
	}
	else if(*i!=NULL && &((*i)->quatre)){
		Arrondit(&((*i)->hg),p-1);
		Arrondit(&((*i)->hd),p-1);
		Arrondit(&((*i)->bg),p-1);
		Arrondit(&((*i)->bd),p-1);
	}
	else{
		return;
	}
}

bool MemeDessin(image i1, image i2){
	if(i1 != NULL && i2!=NULL){
		if (i1->quatre && i2->quatre)
			return MemeDessin(i1->hg,i2->hg) && MemeDessin(i1->hd,i2->hd) && MemeDessin(i1->bg,i2->bg) && MemeDessin(i1->bd,i2->bd);
		else{
			return EstBlanc(i1) && EstBlanc(i2);
		}
	}else
		return (EstNoire(i1) && EstNoire(i2));
}

void InterUnion(image* i1, image* i2){
	image tmp = *i1;
	if((*i1) != NULL){
		if((*i2) != NULL){
			if((*i1)->quatre){
				if((*i2)->quatre){
					InterUnion(&((*i1)->hg),&((*i2)->hg));
					InterUnion(&((*i1)->hd),&((*i2)->hd));
					InterUnion(&((*i1)->bg),&((*i2)->bg));
					InterUnion(&((*i1)->bd),&((*i2)->bd));
				}else{
					*i2=*i1;
					*i1=Blanc();
				}
			}else{
				if((*i2)->quatre){
					*i1=Blanc();
				}
			}
		}else{
			*i2=Noir();
		}
	}else{
		if((*i2) != NULL){
			image i3 = *i1;
			*i1=*i2;
			*i2=i3;
		}
	}
}

bool EstDamier1(image img){
    return((EstBlanc(img->hg))&&(EstNoire(img->hd))&&((EstNoire(img->bg)))&&(EstBlanc(img->bd)));
}

bool EstDamier(image img){
    if (EstNoire(img) || EstBlanc(img)) return false;
    else if (EstDamier1(img)) return true;
    else return EstDamier(img->hg)&&EstDamier(img->hd)&&EstDamier(img->bg)&&EstDamier(img->bd);
}

int CompteDamiers(image img){
    if (EstNoire(img) || EstBlanc(img)) return 0;
    else if (EstDamier1(img)) return 1;
    else if (!EstDamier1(img)&&EstDamier(img)) {
        int cpt=1;
        if (EstDamier(img->hg)) cpt+=CompteDamiers(img->hg);
        if (EstDamier(img->hd)) cpt+=CompteDamiers(img->hd);
        if (EstDamier(img->bg)) cpt+=CompteDamiers(img->bg);
        if (EstDamier(img->bd)) cpt+=CompteDamiers(img->bd);
        return cpt;
    }
    else if (!EstDamier(img)) {
        int cpt=0;
        if (EstDamier(img->hg)) cpt+=CompteDamiers(img->hg);
        if (EstDamier(img->hd)) cpt+=CompteDamiers(img->hd);
        if (EstDamier(img->bg)) cpt+=CompteDamiers(img->bg);
        if (EstDamier(img->bd)) cpt+=CompteDamiers(img->bd);
        return cpt;
    }
    else return 0;
}

//POUR LA QUESTION 12 !! :
/*
Nous n'avons pas réussi à implémenter les deux fonctions demandées mais nous avons eu une vague idée de comment faire : 
Nous avons pensé à stocker toutes les sous-images daus une nouvelle structure de donnée, comme un tableau qui contient des pointeurs vers toutes les sous-images. Il aurait ensuite fallu parcourir toute notre image en comparant chaque sous-arbre à notre tableau, et si un élément identique est trouvé, on remplace le sous-arbre par le pointeur de la case du tableau.
Pour la fonction dilate, il aurait fallu faire l'inverse : dès qu'on tombe sur un pointeur vers un autre sous-arbre, on recopie ce sous-arbre à la place du pointeur.
*/

int main() {
	
	//TEST EstBlanc et EstNoire
	/**
	image a= Compose(Noir(),Noir(),Compose(Noir(),Noir(),Noir(),Noir()),Noir());
	image a2= Compose(Noir(),Noir(),Compose(Noir(),Blanc(),Noir(),Noir()),Noir());
	image a3 = Compose(Blanc(),Blanc(),Compose(Blanc(),Blanc(),Blanc(),Blanc()),Blanc());
	assert(EstNoire(a));
	assert(! EstNoire(a2));
	assert(EstBlanc(a3));
	assert(! EstBlanc(a2));
	**/
	
	//TEST Aire
	/**
	image testAire=Compose(Noir(),Compose(Blanc(),Noir(),Blanc(),Blanc()),Noir(),Compose(Noir(),Noir(),Noir(),Blanc()));
	image testAire2=Compose(Noir(),Blanc(),Blanc(),Noir());
	assert(Aire(testAire) == 0.75);
	assert(Aire(testAire2) == 0.50);
	**/

	//TEST TriangleBD
	/**
	Affichage(TriangleBD(0));
	printf("\n");
	Affichage(TriangleBD(1));
	printf("\n");
	Affichage(TriangleBD(3));
	printf("\n");
	**/
	
	//TEST Arrondit
	/**
	image i=Compose(Noir(),Compose(Compose(Noir(),Compose(Noir(),Blanc(),Blanc(),Noir()),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Blanc(),Blanc(),Noir())),Blanc(),Noir(),Blanc()),Blanc(),Compose(Noir(),Compose(Noir(),Blanc(),Blanc(),Compose(Blanc(),Noir(),Noir(),Noir())),Blanc(),Noir()));
	Arrondit(&i,2);
	Affichage(i);
	printf("\n");
	**/
	
	//TEST MemeDessin
	/**
	image i3=Compose(Compose(Noir(),Noir(),Noir(),Compose(Noir(),Noir(),Noir(),Compose(Noir(),Noir(),Noir(),Noir()))),Compose(Blanc(),Blanc(),Blanc(),Blanc()),Noir(),Compose(Blanc(),Compose(Blanc(),Blanc(),Blanc(),Blanc()),Blanc(),Noir()));
	//Affichage(i3);
	printf("\n");
	image i4=Compose(Noir(), Compose(Blanc(),Compose(Compose(Blanc(), Blanc(), Blanc(), Blanc()), Blanc(), Blanc(), Blanc()),Compose(Blanc(), Blanc(), Blanc(), Blanc()), Blanc()), Noir(), Compose(Blanc(), Blanc(), Blanc(), Compose(Noir(), Noir(), Noir(), Noir())));
	//Affichage(i4);
	printf("\n");
	assert(MemeDessin(i3,i4));
	**/
	
	//TEST INTERUNION
	/**
	image testIU1=Compose(Blanc(),Noir(),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Compose(Blanc(),Noir(),Blanc(),Noir()),Blanc(),Noir(),Noir()));
	image testIU2=Compose(Noir(),Blanc(),Noir(),Compose(Blanc(),Compose(Noir(),Blanc(),Blanc(),Noir()),Compose(Blanc(),Blanc(),Noir(),Noir()),Noir()));
	Affichage(testIU1);
	printf("\n");
	Affichage(testIU2);
	printf("\n");
	printf("\n");
	InterUnion(&testIU1,&testIU2);
	Affichage(testIU1);
	printf("\n");
	Affichage(testIU2);
	printf("\n");
    return 0;
	**/
	
	//TEST CompteDamiers
	/**
	image testCD = Compose(Compose(Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc())),Compose(Compose(Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc())),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc())),Compose(Compose(Blanc(),Noir(),Noir(),Blanc()),Compose(Blanc(),Noir(),Noir(),Blanc()),Noir(),Noir()),Compose(Compose(Blanc(),Blanc(),Blanc(),Blanc()),Noir(),Noir(),Compose(Blanc(),Blanc(),Compose(Blanc(),Blanc(),Blanc(),Blanc()),Blanc())));
	assert(CompteDamiers(testCD) == 15);
	**/
	return 0;
}
