

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

typedef enum { false, true} bool;

/*************************************************/
/*                                               */
/*            factorielle                        */
/*                                               */
/*************************************************/

long fact (int n) 
{ if (n==0) return 1 ;
  else return n * fact(n-1) ; 
}

// itou avec un arg out => passage par adresse

void bisfact(int n, long * r) 
{ if (n==0) 
         *r=1.0 ;
  else { bisfact(n-1,r) ;
         *r = *r *n ;
       }
}

long fact2 (int n)
{ long r ;
  bisfact(n,&r) ;
  return r ;
}

/*************************************************/
/*                                               */
/*            Calcul de e                        */
/*                                               */
/*************************************************/

  // d'après google,
  // e = 2,7182818284 5904523536 0287471352 6624977572 4709369995 
  //       9574966967 6277240766 3035354759 4571382178 5251664274

     // Il est proposé de faire 3 versions pour observer des phénomènes de précision.
     // C'est le même code, seul le type change.

/*************************************************/
/**
Pourquoi resfact passe à 0 dès la première entrée dans la boucle while ???????????????? le code ne fonctionne pas
**/
float Efloat() {
    float e = 1.0;
    float resfact = 1.0;
    int n = 1;
    while (resfact > 1.0e-10) {
        resfact /= n;
        e += resfact;
        n++;
    }

    return e;
}
/*************************************************/

double Edouble() {
    double e = 1.0;
    double resfact = 1.0;
    int n = 1;
    while (resfact > 1.0e-10) {
        resfact /= n;
        e += resfact;
        n++;
    }

    return e;
}
/*************************************************/

long double Elongdouble() {
    long double e = 1.0;
    long double resfact = 1.0;
    int n = 1;
    while (resfact > 1.0e-10) {
        resfact /= n;
        e += resfact;
        n++;
    }

    return e;
}

/*************************************************/
/*                                               */
/*            Suite Y                            */
/*                                               */
/*************************************************/

void afficheYfloat (int n) {
	float y = Efloat() -1;
	for(int i=1;i<=30;i++){
		printf("n = %d ; yn = %.60f \n",i,y);
		y=i * y - 1;
	}
}

/*************************************************/

void afficheYdouble (int n) {
	double y = Edouble() -1;
	for(int i=1;i<=30;i++){
		printf("n = %d ; yn = %.60lf \n",i,y);
		y=i * y - 1;
	}
}
/*************************************************/

void afficheYlongdouble (int n) {
	long double y = Elongdouble() -1;
	for(int i=1;i<=30;i++){
		printf("n = %d ; yn = %.60Lf \n",i,y);
		y=i * y - 1;
	}
}

//On remarque que pour les 3 suites, la fonction semble converger vers 0 pour les environs 10 premiers termes, puis tend vers +infini pour la version avec des float, et -infini pour les deux autres.
/*************************************************/
/*                                               */
/*            Puissance                          */
/*                                               */
/*************************************************/
double bispos(double x,long n){
	if (n==0)
		return 1.0; 
	else
		return bispos(x,n-1) * x;
}


double power1 (double x, long n) {
	if (n==0)
		return 1; 
	else if (n > 0) 
		return bispos(x,n);
	else 
		return 1/bispos(x,-1*n);
}
/*************************************************/

double power2a (double x, long n) {
	bool neg = false;
	if(n<0){
		n=-1*n;
		neg=true;
	}
	double r=1.0;
	for(int i=1; i<=n;i++){
		r=r*x;
	}
	if(neg)
		return 1/r;
	else return r;
}

/*************************************************/

double power2b (double x, long n) {
	double r=1.0;
	while(n>0){
		r=r*x;
		n--;
	}
	return r;
}

/*************************************************/
void pow3(double x,long n,double *r){
	if (n!=0){
		*r=*r*x;
		pow3(x,n-1,r);
	}
}

double power3 (double x, long n) {
	double r=1.0;
	pow3(x,n,&r);
	return r;
}

/*************************************************/
double pow4(double x,long n,long r){
	if(n==0){return r;}
	return pow4(x,n-1,r*x);
}

double power4 (double x, long n) {
	return pow4(x,n,1);
}

/*************************************************/

     //   Observation (1+1/10^k)^(10^k) : rame : 8 en 1/2s, 9 en qqs s, 10 en 1m

double power5 (double x, long n)
{
    if (n == 0)
         return 1 ;
    else if (n % 2 == 0)
              return (power5(x,n/2)*power5(x,n/2)   ) ;
         else return (power5(x,n/2)*power5(x,n/2) *x) ;   
}

/*************************************************/

double power6 (double x, long n) {
	if(n==0){return 1;}
	else{
		int y=power6(x,n/2);
		if(n%2 == 0)
			return y*y;
		else return y*y*x;
	}
}

/*************************************************/

double power7 (double x, long n) {
	if(n == 0)
		return 1;
	else{
		if(n%2==0)
			return power7(x*x,n/2);
		else return power7(x*x,n/2)*x;
	}
}

/*************************************************/
double pow8(double x,long n,double r){
	if(n==0)
		return r;
	else{
		if(n%2 ==0)
			return pow8(x*x,n/2,r);
		else return pow8(x*x,n/2,r*x);
	}
}

double power8 (double x, long n) {
	return pow8(x,n,1);
}

/*************************************************/
void pow9(double x,int n, double *r){
	if(n!=0){
		if(n%2==1)
			*r=(*r) *x;
		pow9(x*x,n/2,r);
	}
}

double power9 (double x, long n) {
	double r=1.0;
	pow9(x,n,&r);
	return r;
}

/*************************************************/

     //   Observation (1+1/10^k)^(10^k) : calcul immédiat

double power10 (double x, long n)
{
    double r = 1.0 ;
    while (n ISNOT 0) 
      { if (n % 2 == 1) then r = r*x ; // no else
        n = n / 2 ; 
        x = x * x ;
       }
    return r ;
}

/*************************************************/

double power (double x, long n, int i)
{ switch (i)  
   {
   case 1 : return power1(x,n) ; 
   case 2 : return power2a(x,n) ;   // 2 pour 2a
   case 0 : return power2b(x,n) ;  // 0 pour 2b
   case 3 : return power3(x,n) ; 
   case 4 : return power4(x,n) ; 
   case 5 : return power5(x,n) ; 
   case 6 : return power6(x,n) ; 
   case 7 : return power7(x,n) ; 
   case 8 : return power8(x,n) ; 
   case 9 : return power9(x,n) ; 
  case 10 : return power10(x,n) ; 
     default : return 0 ; 
   }
}

   // remarque : les break sont ici inutiles car les returns font déjà des break

/*************************************************/

    // memes versions que la 10 mais avec des long double, puis des floats
    // but du jeu : observer des variations de precision

   // observation :
   //   imprécision à partir de k~5 (float), k~12 (double), k~14 (long double)
   //   rend 1 à partir de k=8 (float), k=16 (double)
   //   rend 1 non observé sur les long double, maxint atteint avant

long double power11 (long double x, long n)
{
    long double r = 1.0 ;
    while (n ISNOT 0) 
      { if (n % 2 == 1) then r = r*x ; // no else
        n = n / 2 ; 
        x = x * x ;
       }
    return r ;
}

/*************************************************/

float power12 (float x, long n)
{
    float r = 1.0 ;
    while (n ISNOT 0) 
      { if (n % 2 == 1) then r = r*x ; // no else
        n = n / 2 ; 
        x = x * x ;
       }
    return r ;
}


/*************************************************/
/*                                               */
/*             Ackermann                         */
/*                                               */
/*************************************************/


int Ackermann1(int m, int n) {
	if(m==0)
		return n+1;
	else{
		if(n==0)
			return Ackermann1(m-1,1);
		else return Ackermann1(m-1,Ackermann1(m,n-1));
	}
}

/*************************************************/

int Ackermann2(int m, int n) {
	if(m==0)
		return n+1;
	else{
		int r=1;
		for(int i=1;i<=n+1;i++){
			r=Ackermann2(m-1,r);
		}
		return r;
	}
}


/*************************************************/

int Ackermann3(int m, int n) {
    while (m != 0) {
        if (n == 0) {
            n = 1;
            m--;
        } else {
            int tempN = n - 1;
            n = Ackermann3(m, tempN);
            m--;
        }
    }
    return n + 1;
}
/*************************************************/

int Ackermann4(int m, int n) {
return 0;
}
/*************************************************/
int Ackermann5 (int m) { return 0 ; }
/*************************************************/
/*
int Ackermann (int m, int i)
{ switch (i)  
   {
   case 1 : return Ackermann1 (m) ; 
   case 2 : return Ackermann2 (m) ; 
   case 3 : return Ackermann3 (m) ;
   case 4 : return Ackermann4 (m) ; 
   case 5 : return Ackermann5 (m) ; 
   default : return 0 ; 
   }
}
*/
/*************************************************/
/*                                               */
/*             Xn                                */
/*                                               */
/*************************************************/

/*************************************************/
int LE(int n,int cpt){
	if(n==1)
		return cpt;
	else
		return LE(n/2,cpt+1);
}

int LogEntier(int n){
	return LE(n,0);
}
//version itérative
int X(long n){
	int res = 2;
	while (n > 0 ) { 
		res = res + LogEntier(res); 
		n--;
	} 
	return res;
}
//version récursive

int X2(long n){
	if (n==0) 
		return 2;
	else{
		int res=X2(n-1);
		return res+LogEntier(res);	
	}
}

//verison récursive terminale avec sous fonction
int bis(long n, long acc) { 
	if (n == 0) 
		return acc; 
	else return bis(n-1, acc+LogEntier(acc));
}

int X3(long n){
	return bis(n,2);
}

//version recursive terminale avec sous procedure
void bis2(long n, long * acc) { 
	if (n !=0){
		*acc=(*acc)+LogEntier(*acc);
		bis2(n-1, acc);
	}
}

int X4(long n){
	long r = 2;
	bis2(n,&r);
	return r;
}
/**
void affiche_Xn(){
	int n; 
	printf("Entrez une valeur \n"); 
	cin >> n; 
	for (int i = 0; i <=n; i++){
		printf("Premiere Version n = %d ; xn = %.20f \n", i, X(i)); 
	for (int i = 0; i <=n; i++){
		printf("Deuxieme Version n = %d ; xn = %.20f \n", i, X2(i));
	for (int i = 0; i <=n; i++){
		printf("Troisieme Version n = %d ; xn = %.20f \n", i, X3(i));
	for (int i = 0; i <=n; i++){
		printf("Quatrieme Version n = %d ; xn = %.20f \n", i, X4(i));
}
**/

/*************************************************/
/*                                               */
/*               main                            */
/*                                               */
/*************************************************/


int main(int argc, char** argv)
{

       double x ;
       long double y ;
       float z ;
       
       int cptx ;
  
       long nx ;
       
       int i,j ; 
       
/******************************************************************************/
 
    // mettre "if true" vs "if false" selon que vous voulez les tests ou non
  
/****************  petit test sur le fonctionnement du switch  ************/

if (false) {

 switch (2)  
   {
   case 1 : printf("toc\n") ;  break ;
   case 2 : printf("pata") ; 
   case 3 : printf("pouf\n") ;  break ;
   case 4 : printf("youpla") ; 
   default : printf("boum\n") ; 
   }


 switch (4)  
   {
   case 1 : printf("toc\n") ;  break ;
   case 2 : printf("pata") ; 
   case 3 : printf("pouf\n") ;  break ;
   case 4 : printf("youpla") ; 
   default : printf("boum\n") ; 
   }

   printf("\n") ; 

}
      
      
/************************  taille des nombres  *************************/
      
if (false) {     
       
       printf("ce programme suppose que les long font 8 octets\n") ;
       printf("S'ils n'en font que 4, il faudra utiliser long long\n") ;

       printf("short : %d octets\n", (int) sizeof(short));
       printf("int : %d octets\n", (int) sizeof(int));
       printf("long : %d octets\n", (int) sizeof(long));
       printf("long long : %d octets\n", (int) sizeof(long long));
       printf("float : %d octets\n", (int) sizeof(float));
       printf("double : %d octets\n", (int) sizeof(double));
       printf("long double : %d octets\n", (int) sizeof(long double));
       printf("\n") ;
 
       x = 1.0 ;
       cptx = 0 ;
       while ( (1.0 + x) - 1.0 != 0 )
          { x = x/10 ; cptx ++ ; }
       printf("1+1/10^c devient 1 a partir de c=%d pour les double\n", cptx) ; 
       printf("et 1+1/10^%d vaut en fait 1+%lf\n", cptx-1, (1.0 + 10*x) - 1.0) ;
        
       printf("ce programme suppose que les doubles font au moins 8 octets\n") ;
       printf("et que ((double) 1+1/10^-15) n'est pas 1 \n") ;       
       printf("Si ce n'est pas le cas, utiliser des long double \n") ;
       
       printf("\n") ;      
}

/************************  factorielle  *************************/

if (false) {

     printf("%ld \n",fact(5)) ;
     printf("%ld \n",fact2(5)) ;
     printf("\n") ;
}


/******************    Autour de e      *******************************/

  // d'après google,
  // e = 2,7182818284 5904523536 0287471352 6624977572 4709369995 
  //       9574966967 6277240766 3035354759 4571382178 5251664274

if (false) {  
       

        printf(" e1 = %.40f \n", Efloat()) ;
        printf(" e3 = %.60lf \n", Edouble()) ; 
        printf(" e3 = %.60Lf \n", Elongdouble()) ; 
        
}

if (false) {  
            afficheYfloat(30) ;
            afficheYdouble(30) ;
            afficheYlongdouble(30) ;
}
        
/******************    power     *******************************/

if (false) {  

        //   test simplet, vérifie le B.A. BA, test de 2^10 pour toutes les versions
        
        for(i=0 ; i<=10 ; i++)
        printf("Power %d dit que power(2,10) vaut %.0lf \n", i, power(2,-3,i) ) ;
        
        printf("\n") ;
      
}

        
if (false) {  
	/**
	On remarque que (1+1/N)^N tend vers e quand N devient grand.
	Les versions 1,3,4 produisent une segmentation fault
	Les versions 0,2,5 calculent le resultat pour n<=9, mais à partir de n=10, le temps devient assez long
	Le calcul renvoie 1 pour la version 6 (?????), quelque soit la valeur de N
	Pour les versions 7 et 8,le calcul jusqu'à N=10^15 fonctionne mais pour N=10^15, la valeur renvoyée est supérieure à 3
	Pour les versions 9 et 10, à partir de N=10, la valeur renvoyée revient à 1 (surement un dépassement de la capacité de stockage du nombre en mémoire)
	On remarque que des optimisations pour les versions récursives terminales ont bien été faites puisque le calcul fonctionne jusqu'à N=15, sans contrainte de temps
	**/

        i=7 ;  // numéro de la version que l'on teste

        // test de la résistance de power version i, calcul de (1+1/N)^N
        // pour des N puissances de 10 de plus en plus grosses
     
        x = 1.0 ;
        nx = 1 ;
        for(j=0 ; j<=15 ; j++)
        {
        printf("power %d ((1+10^-%2d)^(10^%2d)) rend %.12lf\n", i, j, j, power(1+x,nx,i)) ;
        x = x/10 ;
        nx = nx * 10 ;
        }
        printf("\n") ;
       
}
     
if (false) {

        // tests sur la précision que l'on obtient suivant que
        // l'on utilise float, double, long double

        printf("VERSION 12 avec float \n") ;
        z = 1.0 ;
        nx = 1 ;
        for(j=0 ; j<=18 ; j++)
        {
        printf("power((1+10^-%2d)^(10^%2d)) rend %.12f\n", j, j, power12(1+z,nx)) ;
        z = z/10 ;
        nx = nx * 10 ;
        }
        printf("\n") ;


        printf("VERSION 10 avec double \n") ;
        x = 1.0 ;
        nx = 1 ;
        for(j=0 ; j<=18 ; j++)
        {
        printf("power((1+10^-%2d)^(10^%2d)) rend %.12lf\n", j, j, power10(1+x,nx)) ;
        x = x/10 ;
        nx = nx * 10 ;
        }
        printf("\n") ;


        printf("VERSION 11 avec long double \n") ;
        y = 1.0 ;
        nx = 1 ;
        for(j=0 ; j<=18 ; j++)
        {
        printf("power((1+10^-%2d)^(10^%2d)) rend %.12LF\n", j, j, power11(1+y,nx)) ;
        y = y/10 ;
        nx = nx * 10 ;
        }
        printf("\n") ;



}

/******************    Ackermann    *******************************/
              
if (true) { 
 
        for(i=1 ; i<=3 ; i++)  // numéro de version
        
        for(j=0 ; j<=2 ; j++)  // test de A(j,0) pour j de 0 à 5
        
        printf("Ack%d(%d) = %d \n", i, j, Ackermann4(j,i)) ;
}

/***********************************************************************/
/******************    Xn    *******************************/
                
if (false) { 
 
    for(i=100 ; i<=100 ; i++){  // numéro de version
        printf("X(%d) = %d \n", i, X2(i)) ;
	}
}

/***********************************************************************/
	
    return 0;
}






