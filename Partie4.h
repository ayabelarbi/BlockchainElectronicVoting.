#ifndef _PARTIE4_H
#define _PARTIE4_H


#include "Partie3.h"

typedef struct block {
    Key * author ;
    CellProtected * votes ;
    unsigned char * hash ;
    unsigned char * previous_hash ;
    int nonce ;
} Block ;

typedef struct block_tree_cell {
    Block * block ;
    struct block_tree_cell * father ;
    struct block_tree_cell * firstChild ;
    struct block_tree_cell * nextBro ;
    int height ;
} CellTree ;

void ecriture_bloc(Block * block);
Block* lecture_bloc(char * nom_fichier); 
char* block_to_str(Block* block); 
char * hash_to_str(unsigned char * s);
unsigned char * str_to_hash(char * hash_hexa);
void compute_proof_of_work(Block * B, int d);
int verify_block(Block * b, int d);
void delete_block(Block* b);

CellTree* create_node(Block* b); 
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree *t);
void delete_node(CellTree *node);
void delete_tree(CellTree* tree);
CellTree *highest_child(CellTree* cell); 
CellProtected* fusion_declaration(CellProtected* decl1, CellProtected* decl2);
CellProtected* fusion_blockchain(CellTree *t);
#endif


