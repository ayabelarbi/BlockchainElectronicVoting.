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

char * str_hache(char * chaine){

    unsigned int int_tmp;
    unsigned char * str_tmp = (unsigned char*)malloc(sizeof(unsigned char)*4);
    unsigned char * res = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(chaine)*4));
    strcat(res, "");
    int ind_res = 0;
    int k = 0;
    for(int i = 0; i< strlen(chaine); i++){
        sscanf(&chaine[i], "%02x",&int_tmp);
        *str_tmp = int_tmp;
        strcat(res, str_tmp);
        printf("%s\n", res);

    }
    strcat(res, "\0");
    return res;
}


