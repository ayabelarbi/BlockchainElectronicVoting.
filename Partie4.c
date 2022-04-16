#include "Partie4.h"

//Partie4
//EXO7

void ecriture_bloc(Block * block){
    
}

Block* lecture_bloc(char * nom_fichier){
    
}

char* block_to_str(Block* block){
    char *res = (char *)malloc(sizeof(char)*256);
    strcpy(res,"");
    strcat(res,key_to_str(block->author));
    strcat(res," ");
    strcat(res,block->previous_hash);
    strcat(res," ");
    
    char s[30];
    sprintf(s, "%d", block->nonce);
    strcat(res, s);
    strcat(res, "\n");
    
    CellProtected * votes = block->votes;
    while(votes){
        strcat(res, protected_to_str(votes->data));
        strcat(res," ");
        votes = votes->next;
    }

    return res;
    
}

void compute_proof_of_work(Block *B, int d){
    do {
        
    int i = 0;
        B->nonce = i;
        
        char * s1 = block_to_str(B);
        const char *s = (const char *)s1;
        unsigned char * h  = SHA256 (s , strlen ( s ) , 0);
        
        //la valeur hachee du bloc commence par d 0 successifs si bool = 1 en sortie de boucle
        int bool = 1;
        for(int j = 0; j < d; j++){
            if (d[j]!='0'){
                bool = 0;
                i = h;
            }
        }
    
    } while ( )
}
 
