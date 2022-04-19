#include "Partie1.h"
#include <time.h>


void print_long_vector(long * result , int size){
	printf ("Vector : [" ) ;
	for ( int i = 0; i < size ; i ++) {
		printf ( "%ld \t" , result[i]) ;
	}
	printf ( "]\n" ) ;
}

int main(){
    srand(time(NULL));
    
    printf(" ----- Partie 1 -----\n");
    long p = random_prime_number(3, 7, 5000);
	long q = random_prime_number(3, 7, 5000);
	while(p==q){
		q = random_prime_number(3,7, 5000);
	}
	printf("Nombre premier %ld %ld\n", p, q); 
	long n, u, s; 
	generate_key_values(p ,q ,&n ,&s ,&u );
	//Pour avoir des cles positives :
	if (u <0) {
		long t = (p -1) *( q -1) ;
		u = u + t ; //on aura toujours s*u mod t = 1
	}
	
	//Afichage des cles en hexadecimal
	printf ("cle publique = (%ld , %ld)\n", s , n ) ;
	printf ("cle privee = (%ld, %ld) \n", u, n); 
	//Chiffrement
	char mess1 [50] = "Hello World !";
	int len = strlen(mess1);
	long* crypted = encrypt(mess1 , s, n);
	
	printf("Initial message : %s \n", mess1);
	printf("Encoded representation : \n");
	print_long_vector(crypted , len);

	//Dechiffrement
	char* decoded = decrypt(crypted , len , u, n);
	printf("Decoded : %s \n", decoded);
    printf("---- FIN DE LA PARTIE 1 ----\n\n");

	free(crypted);
	free(decoded);
	return 0; 
}
