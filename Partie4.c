#include "Partie4.h"
#include <openssl/sha.h>

#define MAX 3000


void ecriture_bloc(Block * block){
    FILE *fb = fopen("bloc.txt", "w");
    if(fb == NULL){
        printf("Erreur d'ouvertur du fichier");
        exit(1);
    }
    
    char * str_block = block_to_str(block);
    fprintf(fb, "%s\n",str_block); 
    fclose(fb);
}



Block* lecture_bloc(char * nom_fichier){  
    FILE *f = fopen(nom_fichier, "r");
    if(f == NULL){
        printf("Erreur d'ouverture de fichier");
        exit(1);
    }

    Block * b = (Block *)malloc(sizeof(Block));

    //on recupere la cle
    char str_key[32];
    fgets(str_key, 32, f);
    b->author = str_to_key(str_key);
   
    //on recupere le previous hash
    char hash_hexa[SHA256_DIGEST_LENGTH*2+2];
    fgets(hash_hexa,SHA256_DIGEST_LENGTH*2+2, f);
    b->previous_hash = str_to_hash(hash_hexa);

    //on recupere nonce
    char str_nonce[256];
    fgets(str_nonce,256, f);
    int nonce = 0;
    sscanf(str_nonce, "%d",&nonce);
    b->nonce = nonce;

    //on recupere les votes
    char buffer[256];
    CellProtected * votes = (CellProtected*)malloc(sizeof(CellProtected));
    Protected * vote_courant;
    while(fgets(buffer, 256, f)!=NULL){
        if (buffer[0]!='\0'){
            vote_courant = (Protected*)malloc(sizeof(Protected));
            vote_courant = str_to_protected(buffer);
            votes = ajout_en_tete(votes, vote_courant);
        }
        
    }
    b->votes = votes;

/*
    //on creer le hash
    b->hash = SHA256(block_to_str(b), strlen(block_to_str(b)), 0);
*/

    fclose(f);

    return b;
}


/*retourne la chaine de caractere representant le bloc*/
char* block_to_str(Block* block){

    //calcul de la taille de la chaine 

    int size = strlen(key_to_str(block->author)) + 1;

    size = size + SHA256_DIGEST_LENGTH*2 + 1; 
    
    CellProtected * votes = block->votes;
    while(votes){
        size = size + strlen(protected_to_str(votes->data)) + 1;
        votes = votes->next;     
    }

    double nb_chiffres = 0;
    if (block->nonce == 0){
        nb_chiffres = 1;
    } else {
        nb_chiffres = log10((double)(block->nonce));
    }
    
    size = size + (int)(nb_chiffres) + 2;

    //printf("size = %d\n", size);
   
    //creation de la chaine

    char *res = (char *)malloc(sizeof(char)*size);
    if (res == NULL){
        printf("erreur malloc bloc_to_str\n");
    }

    strcpy(res,"");
    strcat(res,key_to_str(block->author));  

    res[strlen(res)]= '\n';
    res[strlen(res)+1]= '\0';

    strcat(res,hash_to_str(block->previous_hash));
    res[strlen(res)]= '\n';
    res[strlen(res)+1]= '\0';


    votes = block->votes;
    while(votes){
        strcat(res, protected_to_str(votes->data));
        res[strlen(res)]= '\n';
        res[strlen(res)+1]= '\0';
        votes = votes->next;
    }

    char * str_nonce = (char*)malloc(sizeof(char)*round(nb_chiffres) + 2);
    sprintf(str_nonce, "%d", block->nonce);
    strcat(res, str_nonce);

    return res;
}


/* renvoie la valeur hachee retournee par SHA256 sous forme de chaine de cracatère en hexadecimal*/
char * hash_to_str(unsigned char * s){
    char * res = (char*)malloc(sizeof(char)*SHA256_DIGEST_LENGTH*2+1);
    char tmp[3];
    int j = 0;
    int i;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++){
        sprintf(tmp,"%02x", s[i] );                 //conversion en hexadecimal
        res[j]=tmp[0];
        j++;
        res[j]=tmp[1];
        j++;
    }
    res[j]='\0';
    return res;
    
}

unsigned char * str_to_hash(char * hash_hexa){
    unsigned char * hash = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    char * tmp[3];
    for(int i = 0; i < strlen(hash_hexa); i = i + 2){

        int d1, d2;
        char tmp1[2];
        char tmp2[2];
        tmp1[0] = hash_hexa[i];
        tmp1[1] = '\0';
        tmp2[0] = hash_hexa[i+1];
        tmp2[1] = '\0';

        sscanf(tmp1, "%d", &d1);
        sscanf(tmp2, "%d", &d2);

        hash[i/2]= d1 * 16 + d2;
               
        //printf("hash[%d] : %d\n", i/2, hash[i/2] );
    }
    return hash;
}

/* rend le bloc valide */
void compute_proof_of_work(Block * B, int d){
    B->nonce = 0;

    //booleen vrai si la chaine de caracteres en hexadecimal qui correspond a la valeur hachee du bloc commence par d 0 successifs
    //on l'initialise a vrai
    int bool= 1; 
    int i;
    
    do { 
        bool = 1;         
        char * str_block = block_to_str(B);
        B->hash = SHA256(str_block, strlen(str_block), 0);
        char * hash_hexa = hash_to_str(B->hash);
        
        for(i = 0; i < d; i++){
            if (hash_hexa[i] != '0'){
                bool = 0;
                i = d;
                B->nonce = B->nonce + 1;
            }
        }
    } while (bool==0);

}

/* retourne 1 si le bloc est valide car sa valeur hachee commence par d 0, retourne 0 sinon */
int verify_block(Block * b, int d){
    char * hash_hexa = hash_to_str(b->hash);
    
    //on parcourt les d premiers caracteres de la chaine en hexadecimal de la valeur hachee du bloc
    for(int i = 0; i < d; i++){
        //si un des caractere est different de 0 le bloc n'est pas valide et on revoie 0
        if (hash_hexa[i] != '0'){
            return 0;
        }
    }

    //si on est sorti de la boucle c'est que les d premiers caracteres sont 0 alors le bloc est valide et on renvoie 1
    return 1;
}

void delete_block(Block* b){
    delete_chain(b->votes);
    free(b);
}








