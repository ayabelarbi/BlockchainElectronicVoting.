#include "Partie5.h"

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
    
}

