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


    //on prend un previous hash au hasrad
    //unsigned char * previous_hash = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    //previous_hash = SHA256(d, strlen(d),0);
    b->previous_hash = SHA256(d, strlen(d),0);
    b->nonce = 0; 
    

    //unsigned char * hash = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);  
    //hash = SHA256(block_to_str(b), strlen(block_to_str(b)), 0);
    b->hash = SHA256(block_to_str(b), strlen(block_to_str(b)), 0); 
    
    
    //Bloc n°2
    Block *b2= (Block *)malloc(sizeof(Block));
    Key* keyb2 = (Key*)malloc(sizeof(Key));
    Key* skeyb2 = (Key*)malloc(sizeof(Key));
    init_pair_keys(keyb2,skeyb2,3,7);
    b2->author = keyb2;
    CellProtected * cellb2 = read_protected("declaration.txt");
    b2->votes = cellb2;
    b2->previous_hash = b->hash;
    b2->nonce = 0;  
    b2->hash = SHA256(block_to_str(b2), strlen(block_to_str(b2)), 0); 
    
    //Bloc n°3
    Block *b3= (Block *)malloc(sizeof(Block));
    Key* keyb3 = (Key*)malloc(sizeof(Key));
    Key* skeyb3 = (Key*)malloc(sizeof(Key));
    init_pair_keys(keyb3,skeyb3,3,7); 
    b3->author = keyb3; 
    CellProtected *cellb3 = read_protected("declaration.txt");
    b3->votes = cellb3;
    b3->previous_hash = b2->hash;
    b3->hash = SHA256(block_to_str(b3), strlen(block_to_str(b3)), 0);
    b3->nonce = 0; 


    Block *b4= (Block *)malloc(sizeof(Block));
    Key* keyb4 = (Key*)malloc(sizeof(Key));
    Key* skeyb4 = (Key*)malloc(sizeof(Key));
    init_pair_keys(keyb4,skeyb4,3,7); 
    b4->author = keyb4; 
    CellProtected *cellb4 = read_protected("declaration.txt");
    b4->votes = cellb4;
    b4->previous_hash = b3->hash;
    b4->hash = SHA256(block_to_str(b4), strlen(block_to_str(b4)), 0);
    b4->nonce = 0; 


    //TEST HASH_TO_STR + STR_TO_HASH
    printf("TEST HASH_TO_STR\n");
    unsigned char * hash_test = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        hash_test[i]=18;
    }

    char * hash_hexa = hash_to_str(hash_test);
    printf("hash_hexa : %s\n", hash_hexa);

    unsigned char * hash_test2 = str_to_hash(hash_hexa);
    char * hash_hexa2 = hash_to_str(hash_test2);
    printf("hash_hexa2 : %s\n", hash_hexa2);

  
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



    //TEST LECTURE ECRITURE
    printf("TEST LECTURE ECRITURE\n\n");

    ecriture_bloc(b, "bloc.txt");


    Block * b2 = lecture_bloc("bloc.txt");
    /*

    printf("On ecrit le bloc precedent dans un fichier, on creer un bloc a partir de la lecture de ce fichier et on obtient :\n");
    printf("%s\n", block_to_str(b2));

    */

    delete_block(b);
    return 0;
printf("\n---Exercice 8 : Manipulation d'un arbre de blocs---\n");
    //Test des fonction de gestion de l'arbre 
    CellTree* newTree = create_node(b); 
    CellTree* child = create_node(b2);
    CellTree* nextchild = create_node(b3);
    add_child(newTree, child);
    add_child(child, nextchild);

    printf("Voici le premier tree du bloc B \n");
    print_tree(newTree);
    
    printf("\n");


//LIBERATION DE MEMOIRE 
delete_cell_protected(votes);
delete_cell_protected(cellb2);
delete_cell_protected(cellb3);
delete_cell_protected(cellb4); 

return 0; 
}
    
}





