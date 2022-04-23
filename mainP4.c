#include "Partie4.h"
#include <openssl/sha.h>

int main(){

 //test SHA256
    printf("TEST SHA256\n\n");

    const char * s = "Rosetta code ";
    unsigned char * d = SHA256(s , strlen( s ), 0) ;
    int i ;

    printf("hash de rosetta code :\n");
    for (i = 0; i<SHA256_DIGEST_LENGTH ; i++)
        printf("%02x" , d[i]) ;
    putchar('\n') ;
    printf("\n\n");


    //CREATION D'UN BLOC
    printf("CREATION D'UN BLOC\n");
    Block * b = (Block *)malloc(sizeof(Block));
    Key* pkey = (Key*)malloc(sizeof(Key));
    Key* skey = (Key*) malloc(sizeof(Key));
    init_pair_keys(pkey,skey,3,7); 
    b->author = pkey;
    CellProtected * votes = read_protected("declaration.txt");
    b->votes = votes;
    unsigned char * hash = (unsigned char *)malloc(sizeof(unsigned char)*256);
    unsigned char * previous_hash = (unsigned char *)malloc(sizeof(unsigned char)*256);
    b->hash = hash;
    b->previous_hash = previous_hash;
    b->nonce = 0;

    /*

    
    //TEST LECTURE ECRITURE + bloc_to_str
    printf("TEST LECTURE ECRITURE + bloc_to_str\n\n");

    ecriture_bloc(b);
    Block * b2 = lecture_bloc("bloc.txt");

    printf("bloc b :\n%s\n", block_to_str(b));
    printf("Bloc b2 :\n%s\n", block_to_str(b2));

    
    //TEST str_to_hash
    printf("TEST str_to_hash\n\n");

    char * s2 = "Rosetta code";
    unsigned char * hash2 = str_hache(s2);
    printf("%s\n", hash2);

    //TEST compute_prof_of_work + verify
    printf("TEST compute_proof_of_work\\n");

    compute_proof_of_work(b, 17);

    */
    return 0;

}
