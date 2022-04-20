#include "Partie1.h"

/* renvoie 1 si p (impair) est premier, 0 sinon */
int is_primitive_naive(long int p){
	for(int i = 3 ; i < p - 1 ; i++){
		if(p % i == 0){
		    printf("Le nombre %ld n'est pas premier \n", p);
		    return 0; 
		}
	}
	printf("Le nombre %ld est premier \n", p)
	return 1; 
}

/* retourne a (puissance m) mod n */
long modpow_naive(long a, long m, long n){
	long res ;
	long vcm = 1; 
	
	for(int i = 0; i < m; i++){
		vcm = vcm*a ;
		//printf("valeure courante = %ld, pour i =  %d \n", vcm, i);
		res = vcm % n; 
	}
	return res;
}

/* retourne a (puissance m) mod n */
int modpow(long a, long m, long n){ 
	if (m == 0)
		return 1; 
	if(a == 0 )
		return 0; 
	
	long newa;
	/* si m est pair */
	if(m % 2 == 0 ){
		newa = modpow(a, m / 2, n) % n;
		newa = (newa * newa) % n;
	}
	else
	{
		newa = a % n; 
		newa = (newa * modpow(a, m - 1, n) % n) % n;
	}
	return (int)((newa + n )% n);
}

/* renvoie 1 si a est un temoin de miller pour p, 0 sinon */
int witness ( long a , long b , long d , long p ) {
	long x = modpow (a ,d , p ) ;
	if ( x == 1) {
		return 0;
	}
	for ( long i = 0; i < b ; i ++) {
		if ( x == p -1) {
			return 0;
		}
		x = modpow (x ,2 , p ) ;
	}
	return 1;
}

/* renvoie un entier long aleatoire entre low et up inclus */
long rand_long ( long low , long up ) {
	return rand () % ( up - low +1) + low ;
}

/* renvoie 1 si p est premier, 0 sinon */
int is_prime_miller ( long p , int k ) {
	if ( p == 2) {
		return 1;
	}
	if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
		return 0;
	}
	//on determine b et d :
	long b = 0;
	long d = p - 1;
	while (!( d & 1) ) { //tant que d n’est pas impair
		d = d /2;
		b = b +1;
	}
	// On genere k valeurs pour a, et on teste si c’est un temoin :
	long a ;
	int i ;
	for ( i = 0; i < k ; i ++) {
		a = rand_long (2 , p -1) ;
		if ( witness (a ,b ,d , p ) ) {
			return 0;
		}
	}
	return 1;
}

/* retourne un nombre premier de taille comprise entre low_size et up_size */:
long random_prime_number(int lowSize, int upSize, int k){
	long max = pow(2, lowSize);
	long min = pow(2,upSize);
	long p ; 
	
	//on genere un nouveau p tant qu'il ne passe pas le test de Miller-Rabin
	
	do{
		p = rand_long(min, max);
	} while (is_prime_miller(p, k) == 0);
	
	return p;
}

/* retourne le PGCD (s,t) */
long extended_gcd ( long s , long t , long *u , long * v ) {
	if ( s == 0) {
		*u = 0;
		*v = 1;
		return t ;
	}
	long uPrim , vPrim ;
	long gcd = extended_gcd(t%s , s , &uPrim , &vPrim ) ;
	*u = vPrim -( t / s ) * uPrim ;
	*v = uPrim ;
	return gcd ;
}

/* genere les valeurs pour la cle publique pkey (s, n) et la cle secrete skey (u, n) */
void generate_key_values(long p, long q, long *n, long *s,long *u){
	*n = p * q;
	long t = (p - 1) * (q - 1);
	long v;
	long pgcd = extended_gcd(*s,t,u,&v);
	
	/* on genere aleatoirement des entiers s inferieurs a t jusqu'a ce que PGCD(s,t) = 1*/
	while (pgcd != 1){
		*s = rand()% t;  
		pgcd = extended_gcd(*s,t,u,&v);
	}
	if (((*s) * (*u) + t * v) != 1){
		printf("Erreur de génération de clef");
	}
}

/* chiffre la chaine avec la cle publique (s,n), retourne un tableau de long */
long *encrypt(char* chaine, long s, long n){
	int lc = strlen(chaine);
    	long * res = (long *)malloc(sizeof(long)*lc);
	if (res == NULL){
		printf("Erreur malloc encrypt\n");
	}

    int i = 0;
    while(chaine[i]!='\0'){
        res[i] = modpow(chaine[i], s, n);
        i++;
    }
    return res;
}

/* retourne le message a l'origine du tableau de long decrypte avec la cle secrete (u,n) */
char* decrypt(long* crypted, int size, long u, long n ){
	char* dechiffre = (char*)malloc(sizeof(char)*(size+1));
	if(dechiffre == NULL){
		printf("Erreur malloc decrypt");
	}
 	int i;
	for (i = 0; i < size; i++){
		dechiffre[i] = modpow(crypted[i], u, n); 
	}
	dechiffre[size+1] = '\0';
	return dechiffre ;
}
