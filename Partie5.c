#include "Partie5.h"
#include <dirent.h>

#define TAILLE 256


// VOTE ET CREATION DE BLOCS VALIDES
void submit_vote(Protected* p){
    //on ajoute a la fin avec "a"
    FILE *pending = fopen("Pending_votes.txt", "a"); 
    char* temp = protected_to_str(p); 
    fprintf(pending, "%s", temp); 
    free(temp); 
    fclose(pending);
}

void create_block(CellTree* tree, Key* author, int d){

    //creation de bloc valide
    Block * bloc_valide = (Block*)malloc(sizeof(Block));
    bloc_valide->votes = NULL; 
   
    if(tree == NULL){
        //l'arbre est vide : pas de precedent hash
        bloc_valide->previous_hash = NULL;
        
    }else{
        CellTree* lastNode = last_node(tree); 
        unsigned char* hash_noeud = (unsigned char*)malloc(sizeof(unsigned char));
        hash_noeud = lastNode->block->hash; 
        bloc_valide->previous_hash = hash_noeud; 
    }

    CellProtected* temp = read_protected("Pending_votes.txt"); 
    Key* k =(Key*)malloc(sizeof(Key));
    init_key(k, author->val, author->n); 
    bloc_valide->author = k; 
    bloc_valide->nonce = 0; 

    compute_proof_of_work(bloc_valide, d);
    remove("Pending_votes.txt"); 
    ecriture_bloc(bloc_valide, "Pending_block.txt");
    
    delete_block(bloc_valide);
}

void add_block(int d, char* name){

    char* buffer[256];
    Block* bloc_courant = lecture_bloc("Pending_block.txt");
    char temp[256];
    char path[14];//14 = taille de "./BlockChain/" + "/0"

    if(verify_block(bloc_courant,d)){

        strcpy(temp,"./Blockchain/");
        strcat(temp, name);
        ecriture_bloc(bloc_courant,temp);

    }else{

        printf("Le bloc à ajouter n'est pas valide\n");

    }

    remove("Pending_block.txt");
    delete_block(bloc_courant);
}

// LECTURE DE L’ARBRE ET CALCUL DU GAGNANT

CellTree* read_tree(){


    DIR* rep = opendir("./BlockChain/");
    struct dirent * dir;

    //determination de la taille du tableau
    int sizeofT = 0; 
    if(rep){
         
        while( (dir = readdir(rep))){
            if((strcmp(dir->d_name, ".") !=0) && strcmp(dir->d_name,"..") != 0) {
                //printf("Chemin du fichier : ./BlockChain/%s \n", dir->d_name); 
                sizeofT++; 
            }
        }    
    }
    
    CellTree** T = (CellTree**)malloc(sizeof(CellTree)*sizeofT);
    if(T == NULL){
        printf("Erreur lors de l'allocation de tableau d'arbre\n"); 
        exit(0);
    }

    //on revient au début du répertoire 
    rewinddir(rep);
    CellTree* node = NULL;
    //On creer le tableau de noeud
    Block* block_courant; 
    char path[14]; //14 = taille de "./BlockChain/" + "/0"
    char temp[256];
    strcpy(path, "./BlockChain/");
    int i = 0; 
    if(rep){
        while( (dir = readdir(rep))){
            if((strcmp(dir->d_name, ".") !=0) && strcmp(dir->d_name,"..") != 0){
                strcpy(temp, path);
                strcat(temp, dir->d_name);
                block_courant = lecture_bloc(temp); 
                node = create_node(block_courant);
                T[i] = node; 
                i++; 
            }
        }
    }

    //parcours du tableau 
    CellTree* father; 
    CellTree* son; 
    
    for(int i = 0; i <sizeofT; i++){
        for(int j = 0; j<sizeofT; j++){
            if(i!= j){
                father=T[i]; 
                son = T[j];
                if (strcmp((son->block)->previous_hash, (father->block)->hash) == 0){
                    //Si le father est père de son fils(son)
                    add_child( father, son );
                }
            }
        }
    }
    
    CellTree* racine = (CellTree*)malloc(sizeof(CellTree));

    for(int i = 0; i < sizeofT; i++){
        if(T[i]->father ==NULL){
            racine = T[i];
        }
    }


    closedir(rep);
    return racine; 
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){

    CellTree* r = racine(tree);
    CellProtected* declaration = fusion_blockchain(r);
    supr_sgn_non_valide(declaration);
    Key* winner =  compute_winner(declaration, candidates, voters, sizeC, sizeV); 
    delete_cell_protected(declaration);
    return winner; 
}