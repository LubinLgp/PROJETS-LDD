#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

typedef struct BlocZ {
    int valeur;
    struct BlocZ *next;
    struct BlocZ *prev;
} BlocZ;

typedef struct ListeZ {
    BlocZ *acces;
} ListeZ;

void ZAjouteDevant(int x, ListeZ *liste) { //on ne comprends pas pourquoi on devrait utiliser un quadruple pointeur. Peut-être pour que l'allocation en memoire ne se fasse pas comme ça? Ce code fonctionne et on ne remarque pas de perte de mémoire
    BlocZ *res= (BlocZ *)malloc(sizeof(BlocZ));
    res->valeur = x;

    if (liste->acces == NULL) { //cas où la liste est vide
        res->next = res;
        res->prev = res;
        liste->acces = res;
    } else {
        res->next = liste->acces;
        res->prev = liste->acces->prev;
        liste->acces->prev->next = res;
        liste->acces->prev = res;
        liste->acces = res;
    }
}


void afficherListeAvecLiens(ListeZ *liste) {
    if (liste->acces == NULL) {
        printf("Liste vide.\n");
        return;
    }

    BlocZ *ptr = liste->acces;
    printf("Affichage de la liste avec liens :\n");

    do {
        BlocZ *precedent = ptr->prev;
        BlocZ *suivant = ptr->next;
        printf("prev = %d, actuel = %d, suivant = %d\n", precedent->valeur, ptr->valeur, suivant->valeur);
        ptr = ptr->next;
    } while (ptr != liste->acces);
}

int main() {
    ListeZ maListe = {NULL};

    ZAjouteDevant(42, &maListe);
    ZAjouteDevant(93, &maListe);
	ZAjouteDevant(17, &maListe);
    ZAjouteDevant(23, &maListe);

    afficherListeAvecLiens(&maListe);

    return 0;
}
