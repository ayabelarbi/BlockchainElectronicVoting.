#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Partie1.h"
#include "Partie2.h"
#include "Partie2_4.h"
#include "Partie3.h"
#include "Partie4.h"

#include <math.h>
#include <time.h>

void print_long_vector ( long * result , int size ) {
	printf ("Vector : [" ) ;
	for ( int i = 0; i < size ; i ++) {
		printf ( "%ld \t" , result [ i ]) ;
	}
	printf ( "] \n" ) ;
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
	long n;
	long u;
	long s;
	generate_key_values(p ,q ,&n ,&s ,&u ) ;
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
 
 
    printf("----- Partie 2 -----\n");
    //Testing Init Keys
    Key * pKey = malloc( sizeof ( Key ) ) ;
    Key * sKey = malloc( sizeof ( Key ) ) ;
    init_pair_keys( pKey , sKey ,3 ,7) ;
    printf ( "pKey : %lx , %lx \n" , pKey -> val , pKey -> n ) ;
    printf ( "sKey : %lx , %lx \n" , sKey -> val , sKey -> n ) ;
    
    //Testing Key Serialization
    char * chaine = key_to_str(pKey ) ;
    printf("Key to str : %s \n" , chaine ) ;
    Key * k = str_to_key(chaine) ;
    printf ("Str to key : %lx , %lx \n" , k -> val , k -> n ) ;
    
	//Candidate keys
    Key* pKeyC = malloc(sizeof(Key)) ;
    Key* sKeyC = malloc(sizeof(Key)) ;
    init_pair_keys(pKeyC , sKeyC ,3,7) ;
    
    //Declaration:
    char * mess = key_to_str(pKeyC ) ;
    printf ( "%s vote pour %s \n" , key_to_str(pKey), mess);
    Signature* sgn = sign( mess , sKey) ;
    printf ( "Signature : ") ;
    print_long_vector( sgn -> content , sgn -> size );
    chaine = signature_to_str(sgn ) ;
    printf ( "signature to str : %s \n" , chaine );
    sgn = str_to_signature( chaine ) ;
    printf ("str to signature :") ;
    print_long_vector(sgn -> content , sgn -> size ) ;
    
    //Testing protected:
    Protected * pr = init_protected(pKey , mess , sgn ) ;
    //Verification:
    if (verify(pr)) {
        printf ("Signature valide \n" ) ;
    }else {
        printf( "Signature non valide \n" ) ; 
    }
    
    chaine = protected_to_str(pr) ;
    printf("protected to str : %s \n" , chaine ) ;
    pr = str_to_protected(chaine) ;
    printf ("str to protected : %s %s %s \n" , key_to_str( pr -> pKey ) ,pr -> mess, signature_to_str(pr -> sgn )) ;
    
    
    printf("---- FIN DE LA PARTIE 2 ----\n\n");
    generate_random_data(5, 3);
    printf("----- Partie 3 -----\n");
    //Liste chainee de clés 
    CellKey* cellKey = read_public_keys("key.txt");
    print_list_keys(cellKey);
    delete_list_keys(cellKey);
    printf("\n");
    
    //Liste chainée de déclaration
    //CellProtected* ldec =  read_protected("declaration.txt");
    //afficher_liste_dec(ldec);
    //delete_chain(ldec);

    printf("\n\n------EXO6 ---- \n\n");
    Key * pKey2 = malloc( sizeof ( Key ) ) ;
    Key * sKey2 = malloc( sizeof ( Key ) ) ;

    Key * pKey3 = malloc( sizeof ( Key ) ) ;
    Key * sKey3 = malloc( sizeof ( Key ) ) ;
    init_pair_keys( pKey2 , sKey2 ,3 ,7) ;
    init_pair_keys( pKey3 , sKey3 ,3 ,7) ;
    printf ( "pKey2 : %ld , %ld \n" , pKey2 -> val , pKey2 -> n ) ;
    printf ( "sKey2 : %ld , %ld \n" , sKey2 -> val , sKey2 -> n ) ;

    printf ( "pKey3 : %ld , %ld \n" , pKey3 -> val , pKey3 -> n ) ;
    printf ( "sKey3 : %ld , %ld \n" , sKey3 -> val , sKey3 -> n ) ;

    CellKey* cellKey2 = create_cell_key(pKey2);
    cellKey2 = addKey(cellKey2, pKey3);
    HashTable *ht  = create_hashtable(cellKey2, 4);
    int trouve = find_position(ht, pKey2);
    printf(" indice trouvé = %d \n", trouve);

    // free(cellKey2);
    // free(cellKey),
    // free(pKey2);
    // free(sKey2);
    // free(pKeyC);
    // free(sKeyC);
    // free(pKey);
    // free(sKey);
	return 0; 
}
