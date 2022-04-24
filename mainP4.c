#include "Partie4.h"
#include <openssl/sha.h>

int main(){

    //TEST SHA256
    printf("TEST SHA256\n");

    const char * s = "Rosetta code";
    unsigned char * d = SHA256(s , strlen( s ), 0) ;
    int i ;

    printf("hash de rosetta code :\n");
    for (i = 0; i<SHA256_DIGEST_LENGTH ; i++){
        printf("%02x" , d[i]) ;
    }
        
    putchar('\n') ;
    printf("\n\n");


    //CREATION D'UN BLOC
    Block * b = (Block *)malloc(sizeof(Block));
    Key* pkey = (Key*)malloc(sizeof(Key));
    Key* skey = (Key*) malloc(sizeof(Key));
    init_pair_keys(pkey,skey,3,7); 
    b->author = pkey;
    CellProtected * votes = read_protected("declaration.txt");
    b->votes = votes;
    
    unsigned char * previous_hash = (unsigned char *)malloc(sizeof(unsigned char)*256);
    //on prend un previous hash au hasrad
    previous_hash = SHA256(d, strlen(d),0);
    b->previous_hash = previous_hash;
    b->nonce = 0; 

    unsigned char * hash = (unsigned char *)malloc(sizeof(unsigned char)*256);
    hash = SHA256(block_to_str(b), strlen(block_to_str(b)), 0);
    b->hash = hash;
    
    //TEST HASH_TO_STR
    printf("TEST HASH_TO_STR\n");
    unsigned char * hash_test = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        hash_test[i]=0;
    }

    printf("%s\n", hash_to_str(hash_test));
    
    //TEST BLOC_TO_STR
    printf("\nTEST BLOC_TO_STR\n");
    printf("bloc b :\n%s\n\n", block_to_str(b));


    //TEST VERIFY_BLOCK + COMPUTE_PROOF_OF_WORK

    
    printf("hash du bloc avant de le rendre valide : %s\n", hash_to_str(b->hash));

    if (verify_block(b,3)==1){
        printf("le bloc est valide !\n");
    } else {
        printf("le bloc n'est pas valide\n");
    }

    compute_proof_of_work(b, 3);
   
    printf("hash du bloc apres l'avoir rendu valide : %s\n", hash_to_str(b->hash));

    if (verify_block(b,3)==1){
        printf("le bloc est valide !\n");
    } else {
        printf("le bloc n'est pas valide\n");
    }


    /*
    //TEST LECTURE ECRITURE
    printf("TEST LECTURE ECRITURE\n\n");

    ecriture_bloc(b);
    Block * b2 = lecture_bloc("bloc.txt");
    
    printf("On ecrit le bloc precedent dans un fichier, on creer un bloc a partir de la lecture de ce fichier et on obtient :\n");
    printf("%s\n", block_to_str(b2));

    */
    return 0;
}


