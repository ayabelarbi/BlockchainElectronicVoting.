#ifndef _PARTIE4_H
#define _PARTIE4_H


typedef struct _block{
    Key* author; 
    CellProtected* votes;
    unsigned char* hash; 
    unsigned char* previous_hash; 
    int nounce;  
}Block;


char* block_to_str(Block* block);
void compute_proof_of_work(Block *B, int d); 

#endif
