#include "Partie4.h"


#define MAX 3000

void ecriture_bloc(Block * block){
    FILE *fb = fopen("bloc.txt", "w");
    if(fb == NULL){
        printf("Erreur d'ouvertur du fichier");
        exit(1);
    }
    
    char *auteur = key_to_str(block-> author); 
    CellProtected* cellpr = block-> votes;
    unsigned char* h = block->hash;
    unsigned char* prev_hash = block-> previous_hash; 
    int proof = block-> nonce; 

    fprintf(fb, "%s %s %s %d\n",auteur,h, prev_hash, proof); 
    while(cellpr!=NULL){
        char * str = protected_to_str(cellpr->data);
        fprintf(fb,"%s\n", str);
        cellpr = cellpr->next;
        free(str);
    }
    free(auteur);
    fclose(fb);
}


Block* lecture_bloc(char * nom_fichier){
    char buffer[MAX];
    
    FILE *f = fopen(nom_fichier, "r");

    Block* b = (Block*)malloc(sizeof(Block));
    Key* k = (Key*)malloc(sizeof(Key));
    int val;
    int n; 
    int nonce; 
    unsigned char hash[256];
    unsigned char prev_hash[256];
    CellProtected* votesCourant =NULL; 
    CellProtected * votes = NULL; 
    char cell[258];  
 
    if(f == NULL){
        printf("Erreur d'ouverture de fichier");
        exit(1);
    }

    //PB : comment faire pour parcourir deux lignes !=
    
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

unsigned char * str_hache(char * st){

    /*unsigned int int_tmp;
    unsigned char * str_tmp = (unsigned char*)malloc(sizeof(unsigned char)*4);
    unsigned char * res = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(st)*4));
    strcat(res, "");
    int ind_res = 0;
    int k = 0;
    for(int i = 0; i< strlen(st); i++){
        sscanf(&st[i], "%02x",&int_tmp);
        *str_tmp = int_tmp;
        strcat(res, str_tmp);
        printf("%s\n", res);

    }
    strcat(res, "\0");
    return res;
    
    //OU :

    unsigned char * s = SHA256 (st , strlen ( st ) , 0);
    unsigned char * res = (unsigned char *)malloc(sizeof(unsigned char)*strlen(s)*3);
    unsigned char * res_courant = res;
    char tmp[4];
    int j = 0;
    for ( int i = 0; i < SHA256_DIGEST_LENGTH ; i ++){
        if ((i+1)%4==0){
            sscanf(tmp, "%02x", &res_courant);
            res_courant = res_courant+4;
        } else {
            tmp[i]=s[i];
        }
        
    }
    return res;

    */

    //VERSION DE DANAEL
    unsigned char * res = (unsigned char *) malloc(SHA256_DIGEST_LENGTH*sizeof(unsigned char));
    char buffer_l[4];
    unsigned int stock;
    int j = 0;
    for(int i = 0 ; i < 3*SHA256_DIGEST_LENGTH ; i = i + 3){
        buffer_l[0] = st[i];
        buffer_l[1] = st[i+1];
        buffer_l[2] = st[i+2];
        buffer_l[3] = '\0';


        if(sscanf(buffer_l,"%02x",&stock)!=1){
            printf("Erreur de formatage du hachage hexadécimal\n");
            free(res);
            return NULL;
        }
        res[j] = stock;
        j++;
    }
    return res;
}

void compute_proof_of_work(Block * B, int d){
    B->nonce = 0;
    int bool;
    do {
        bool = 1;                      //booleen vrai si la valeur hachee du bloc commence par d 0, on l'initialise a vrai
        char * str_block = block_to_str(B);
        B->hash = SHA256(str_block, strlen(str_block), 0);
        unsigned char * hash = B->hash;
        for(int i = 0; i < d*4; i++){
            //si on trouve autre chose qu'un 0 on met le booleen a faux, on sort de la boucle et on incremente nonce
            if (hash[i] != 0){
                bool = 0;
                i = d*4;
                B->nonce = B->nonce +1;      
            }
        }
    }  while (bool != 1);
    
}


