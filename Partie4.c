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
    char buffer[MAX];
    
    FILE *f = fopen(nom_fichier, "r");
     if(f == NULL){
        printf("Erreur d'ouverture de fichier");
        exit(1);
    }

    Block* b = (Block*)malloc(sizeof(Block));
    Key* k = (Key*)malloc(sizeof(Key));
    int val;
    int n; 
    int nonce; 
    unsigned char hash[SHA256_DIGEST_LENGTH+1];
    unsigned char prev_hash[SHA256_DIGEST_LENGTH+1];
    CellProtected* votesCourant =NULL; 
    CellProtected * votes = NULL; 
    char cell[258];  
 
   

       
    while(fgets(buffer, MAX, f) != NULL){
        Key* k = (Key*)malloc(sizeof(Key));
        fscanf(f, "(%d, %d) %s, %s, %d\n", &n, &val, hash, prev_hash, &nonce);
        init_key(k, val, n);
        fscanf(f,"%s", cell); 
        Protected* pr = str_to_protected(cell);
        votesCourant = ajout_en_tete(votesCourant, pr); 
    }
 
    while(votesCourant){
        ajout_en_tete(votes, votesCourant->data);
        votesCourant= votesCourant->next;
    }

    b->author = k; 
    b->hash = hash; 
    b->previous_hash = prev_hash; 
    b->nonce = nonce; 

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








