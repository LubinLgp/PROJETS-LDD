 
#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                sucre syntaxique               */
/*                                               */
/*************************************************/

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum { FALSE, TRUE} bool;

/*************************************************/
/*                                               */
/*          definition type liste                */
/*                                               */
/*************************************************/

typedef struct Bloc
{
    int nombre;
    struct Bloc *suivant;
} Bloc;

typedef Bloc *Liste ;

/*************************************************/
/*                                               */
/*                predeclarations                */
/*                                               */
/*************************************************/

/* initialise une Liste � vide */
void initVide(Liste *L);

/* renvoie 1 si la Liste en parametre est vide, 0 sinon */
bool estVide(Liste l);

/* renvoie le premier element de la Liste en parametre */
int premier(Liste l);

/* renvoie une nouvelle Liste correspondant a celle en parametre, avec l'element x ajoute en haut de la pile */
Liste ajoute(int x, Liste l);

/* modifie la Liste en parametre: x est ajoute comme premier element */
void empile(int x, Liste* L);

/* renvoie une nouvelle Liste correspondant a celle en parametre sans son premier element */
Liste suite(Liste l);

/* modifie la Liste en parametre: le premier element est retire */
void depile(Liste* l);

/* affichage simple en recursif et en iteratif */
void affiche_rec(Liste l);
void affiche_iter(Liste l);

/* longueur en recursif et en iteratif */
int longueur_rec (Liste l);
int longueur_iter (Liste l);

/*  Elimination du dernier element en recursif et en iteratif  */
/*  VD est la sousprocedure utilitaire de la version recursive */
void VD (Liste *L);
void VireDernier_rec (Liste *L);
void VireDernier_iter (Liste *L);


/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/


void initVide( Liste *L)
{
    *L = NULL ;
}

bool estVide(Liste l)
{
    return l == NULL ;
}

int premier(Liste l)
{
    return l->nombre ; 
}

Liste ajoute(int x, Liste l)
{
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->nombre = x ;
    tmp->suivant = l ;
    return tmp ;
}

void empile(int x, Liste *L)
{
      *L = ajoute(x,*L) ; 
}

Liste suite(Liste l)
{
    return l->suivant ;
}


void depile(Liste *L)
{
    Liste tmp = *L ;
    *L = suite(*L) ;
    free(tmp) ;
}

/*************************************************/
/*                                               */
/*     Affiche, avec les briques de base         */
/*                                               */
/*************************************************/

void affiche_rec(Liste l)
{
    if(estVide(l))
        printf("\n");
    else
    {
        printf("%d ", premier(l));
        affiche_rec(suite(l));
    }
}


void affiche_iter(Liste l)
{
    Liste L2 = l;
    while(!estVide(L2))
    {
        printf("%d ", premier(L2));
        L2 = suite(L2);
    }
    printf("\n");
}

/*************************************************/
/*                                               */
/*     Longueur, sans les briques de base        */
/*                                               */
/*************************************************/

int longueur_rec (Liste l)
{
    if (l == NULL)
         return 0 ;
    else return (1 + longueur_rec(l->suivant)) ;
}


int longueur_iter (Liste l)
{
    Liste P = l;
    int cpt = 0 ;
    while (P ISNOT NULL)
    {   P = P->suivant ;
        cpt++ ;
    }
    return cpt ;
}

/*************************************************/
/*                                               */
/*       VireDernier,                            */
/*               sans les briques de base,       */
/*               ni le "->"                      */
/*                                               */
/*************************************************/

void VD (Liste *L)
          // *L non NULL ie liste non vide
{
     if ( ((**L).suivant) == NULL )
            depile(L) ;   // moralement : depile(& (*L)) ;
     else VD (& ( (**L).suivant )) ;
}

void VireDernier_rec (Liste *L)
{
     if ( (*L) ISNOT NULL )
          VD(L);        // moralement : VD(& (*L)) ;
}

void VireDernier_iter (Liste *L)
{
    if ( (*L) ISNOT NULL)
    {
        while ( ((**L).suivant) ISNOT NULL )
                 L = & ( (**L).suivant ) ;
        free(*L) ;
        *L = NULL ;
     }
}


/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste *L)
{
    if(NOT(estVide(*L)))
    {
        depile(L);
        VideListe(L);
    }
      
}

/*************************************************/
/*                                               */
/*           Listes-Piles                        */
/*                                               */
/*************************************************/


bool UnPlusDeuxEgalTrois(Liste l){
	int longeur = longueur_iter(l);
	if(longeur>=3)
		return(premier(l) + premier(suite(l)) == premier(suite(suite(l))));
	else if (longeur==2)
		return(premier(l) + premier(suite(l)) == 0);
	else if (longeur==1)
		return(premier(l) == 0);
	else
		return TRUE;
}

bool Croissante(Liste l){
	int a = premier(l);
	while(l != NULL){
		if(a>=premier(suite(l)))
			return FALSE;
		else{
			a=premier(suite(l));
			l=suite(l);
		}
	}
	return TRUE;
}

int NombreMemePosition_iter(Liste l1, Liste l2){
	int compt=0;
	int a1 = premier(l1);
	int a2 = premier(l2);
	while(l1!=NULL && l2!=NULL){
		if(a1==a2)
			compt++;
		a1=premier(suite(l1));
		a2=premier(suite(l2));
	}
	return compt;
}

int NombreMemePosition_rec(Liste l1, Liste l2){
	if(l1!=NULL && l2!=NULL){
		if(premier(l1) == premier(l2))
			return NombreMemePosition_rec(suite(l1),suite(l2)) + 1;
		else
			return NombreMemePosition_rec(suite(l1),suite(l2));
	}
}

int bis1(Liste l1, Liste l2, int acc){
	if(l1==NULL || l2==NULL)
		return acc;
	else{
		if(premier(l1) == premier(l2))
			return bis1(suite(l1),suite(l2),acc+1);
		else
			return bis1(suite(l1),suite(l2),acc);
	}
}

int NombreMemePosition_recterm1(Liste l1, Liste l2){
	return bis1(l1,l2,0);
}

void bis2(Liste l1, Liste l2, int* acc){
	if(l1!=NULL && l2!=NULL){
		if(premier(l1) == premier(l2))
			*acc=*acc+1;
		return bis2(suite(l1),suite(l2),acc);
	}
}

int NombreMemePosition_recterm2(Liste l1, Liste l2){
	int c=0;
	bis2(l1,l2,&c);
	return c;
}

Liste FoncVireDernier_rec(Liste L){
	if (estVide(L))
		return L;
	else if (estVide(suite(L)))
		return suite(L);
	Liste res=FoncVireDernier_rec(suite(L));
	empile(premier(L),&res);
	return res;
}

Liste FonctVireDernier_iter(Liste L){
	if (estVide(L))
		return L;
	else if (estVide(suite(L)))
		return suite(L);
	Liste l2=(Liste) malloc(sizeof(Bloc));
	initVide(*l2);
	empile(0,*l2);
	int a = premier(L);
	while(NOT estVide(suite(suite(L)))){
		l2->suivant=L;
		l2->suivant->suivant=NULL;
		L=suite(L);
	}
	depile(*l2);
	return l2;
}

void AjouteDevantPremierZero(int x, Liste *L){
	if (estVide(*L) || premier(*L) == 0)
		empile(x,L);
	int p=premier(*L);
	AjouteDevantPremierZero(x, L->suivant);
	empile(p,L->suivant);
}

Liste AjouteDevantDernierZero_iter(int x,Liste *L){
	Liste* res=L;
	if(estVide(*L)){
		empile(x,L);*
		return;
	}
	Liste* beforeLastZero = *L;
	while(NOT estVide(suite(*L))){
		if(premier(suite(*L)) == 0)
			beforeLastZero = *L;
		L=L->suivant;
	}
	if(beforeLastZero == res){
		L->suivant=x;
		return;
	}
	Liste* LastPart = beforeLastZero->suivant
	empile(x,LastPart);
	beforeLastZero->suivant = LastPart;
	return *res;
}

void bis3(int x, Liste* L, bool* good, bool* dejavu){
	if(estVide(*L)){
		if(NOT dejavu){
			empile(x,L);
			*good = TRUE;
		}else
			return;
	}
	else{
		if(premier(*L)==0)
			*dejavu=TRUE;
		bis3(x,L->suivant,good,dejavu);
		if(NOT (*good) && premier(*L) == 0){
			empile(x,L);
			*good = TRUE;
		}
	}
}
void AjouteDevantDernierZero_rec(int x, Liste* L){
	bool good=FALSE;
	bool dejavu=FALSE;
	bis3(x,L,&good,&dejavu);
}

void Tic(Liste *L){
	if(NOT estVide(L)){
		if (premier(*L) == 0){
			while(NOT estVide(*L) and premier(*L) == 0)
				depile(L);
		}
		else{
			Tic(L->suivant);
			empile(0,L);
		}
	}
}


/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/

void poup (Liste l)
{
        printf("Double Affichage \n") ;
        affiche_rec(l) ;
        affiche_iter(l) ;

        printf("Longueur en double %d %d \n\n", 
                           longueur_rec(l), 
                           longueur_iter(l) 
               ) ;
}

int main(int argc, char** argv)
{
    Liste l ;

        initVide (&l) ;

          poup(l) ;

             empile(4, &l) ;
 
          poup(l) ;

             empile(5, &l) ;
             empile(6, &l) ;
             empile(7, &l) ;
             empile(8, &l) ;
             empile(9, &l) ;

          poup(l) ;

        VireDernier_rec  (&l) ;
        VireDernier_iter (&l) ;
        depile(& l) ;

          poup(l) ;

    VideListe(&l);
    return 0;
}




