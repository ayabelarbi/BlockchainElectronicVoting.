#ifndef _PARTIE3_H
#define _PARTIE3_H
#include "Partie1.h"
#include "Partie2.h"

typedef struct cellKey{
    Key* data;
    struct cellKey* next;
} CellKey; 

typedef struct cellProtected {
    Protected * data ;
    struct cellProtected * next ;
} CellProtected ;

typedef struct hashcell {
    Key * key ;
    int val ;
} HashCell ;

typedef struct hashtable {
    HashCell ** tab ;
    int size  ;
} HashTable ;


CellKey* create_cell_key(Key* key);
CellKey* read_public_keys(char* nom_fichier);
CellKey* addKey(CellKey *ck, Key* key );
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* LCK);
CellProtected * read_protected(char * nomFichier); 
void afficher_liste_dec(CellProtected * ldec);
CellProtected * ajout_en_tete(CellProtected * ldec, Protected * pr); 
void delete_cell_protected(CellProtected* c); 
void delete_chain(CellProtected* ldec); 
HashCell* create_hashcell(Key* key); 
int hash_function(Key* key, int size); 
HashTable* create_hashtable(CellKey* keys, int size); 
void delete_hashtable(HashTable* t);
int find_position(HashTable* t, Key* key); 
int identique(Key* k1, Key *k2);
//Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV); 
CellKey* voters(int sizeC, int sizeV); 
void delete_hashcell(HashCell* hc);

#endif


