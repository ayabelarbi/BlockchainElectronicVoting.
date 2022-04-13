#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Partie4.h"
#include "Partie2.h"
#include "Partie3.h"
#include "Partie1.h"


#define TAILLE_MAX 10000

CellKey* create_cell_key(Key* key){
    CellKey* CK = (CellKey*)malloc(sizeof(CellKey));
    
    CK->data = key; 
    CK->next = NULL;
    
    return CK; 
}

CellKey* addKey(CellKey *ck, Key* key ){
    CellKey* ajoutcell = create_cell_key(key);
    ajoutcell->next = ck; 
    return ajoutcell;
}

CellKey* read_public_keys(char* nom_fichier){
    char buffer[TAILLE_MAX];
    FILE* fk = fopen(nom_fichier, "r");
    Key* k = (Key*)malloc(sizeof(Key));
    CellKey* LCK = NULL;
    long val;
    long n;
    
    if(fk == NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    while(fgets(buffer, TAILLE_MAX, fk) != NULL){
        Key* k = (Key*)malloc(sizeof(Key));
        fscanf(fk,"(%ld,%ld)", &val, &n);
        init_key(k, val, n);
        LCK = addKey(LCK, k);
    }
    
    fclose(fk);
    return LCK; 
}


void print_list_keys(CellKey* LCK){
    CellKey* tmp = LCK;
    while(tmp){
        printf("(%ld, %ld) \n",tmp->data->val,tmp ->data->n);
        tmp = tmp -> next;
    }
}

void delete_cell_key(CellKey* c){
    if(c == NULL){
        printf("La cellule est déjà vide"); 
    }
    free(c -> data); 
    free(c);
}

void delete_list_keys(CellKey* LCK){
    CellKey *tmp;
    while(LCK){
        tmp = LCK -> next;
        delete_cell_key(LCK);
        LCK = tmp; 
    }
    free(tmp);
    //verification de la suppresion de la cellule 
    if(tmp == NULL){
        printf("Le cellule est supprimé");
    }else{
    printf("Pb\n");
    }
}


CellProtected* create_cell_protected(Protected* pr){
    CellProtected * res = (CellProtected*)malloc(sizeof(CellProtected));
    res->data = pr;
    res->next = NULL;
    return res;
}


CellProtected * ajout_en_tete(CellProtected * ldec, Protected * pr){
    CellProtected * res = create_cell_protected(pr);
    res->next = ldec; 
    return res; 
}


CellProtected* read_protected(char * nomFichier){
    char buffer[TAILLE_MAX];
    FILE* f = fopen(nomFichier, "r");
    CellProtected* ldec = NULL;
    Protected* pr = (Protected*)malloc(sizeof(Protected));
    char* mess =  (char*)malloc(sizeof(char));
    char* pkey = (char*)malloc(sizeof(char));
    char* sgn = (char*)malloc(sizeof(char));
    long val; 
    long n;
    
    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    while(fgets(buffer, TAILLE_MAX, f) != NULL){
        Key *k = (Key*)malloc(sizeof(Key));
        fscanf(f,"(%lx, %lx) %s %s", &val, &n, mess, sgn); 
        init_key(k, val, n);
        Signature* signature = str_to_signature(sgn);
        pr = init_protected(k, mess, signature);
        ldec = ajout_en_tete(ldec, pr);
    }
    fclose(f);
    return ldec;
}


void afficher_liste_dec(CellProtected * ldec){
    CellProtected * lcourant = ldec;
    while(lcourant!=NULL){
        printf("%s\n", protected_to_str(lcourant->data));
        lcourant = lcourant->next;
    }
}


void delete_cell_protected(CellProtected* c){
    free((c->data)->pKey);
    free((c->data)->sgn);
    free(c);
}

void delete_chain(CellProtected* ldec){
    CellProtected * temp = ldec;
    while(ldec!=NULL){
        temp = ldec;
        ldec = ldec->next;
        delete_cell_protected(temp);
    }

    if (ldec==NULL){
        //verification de la suppresion de la cellule 
        printf("supression de la liste ok\n");
    } else {
        printf("erreur dans la supression de la liste\n");
    }  
}


CellProtected* supr_sgn_non_valide(CellProtected * ldec){
    CellProtected * res = ldec;
    CellProtected * temp= NULL;
    while(ldec){
        if (verify(ldec->data)==0){
            temp = ldec;
            ldec = ldec->next;
            delete_cell_protected(temp);
            res = ldec;
        
        } else {
            ldec = ldec->next;
        }
    }
    return res;
}

HashCell* create_hashcell(Key* key){
    HashCell *res = (HashCell*)malloc(sizeof(HashCell));
    res -> key = key; 
    res -> val = 0; 
    return res; 
}

int hash_function(Key* key, int size){
    char * chaine = key_to_str(key);
    long int h = *(encrypt(chaine, key->val, key->n)) % size;
    printf("valeur de hash fonction : %ld", h);
    return (int)h;
}


int identique(Key* k1, Key *k2){
    if((k1->val == k2->val)&&(k1->n == k2->n)){ 
        return 1; 
    }else{
        return 0;
    }
}

int find_position(HashTable* t, Key* key){
    if(key == NULL){
        printf("erreur, la clef est null\n");
        exit(0); 
    }

    HashCell ** tab = t->tab;
    int ind = hash_function(key, t->size);
    printf("ind = %d\n", ind);
    printf("t->size = %d",t->size);
    int ind_courant = ind;

    //tant que l'indice courant est < taille du tableau 
    while(!(identique(tab[ind_courant]->key, key)) && (ind_courant == t->size)){
        //si indentique(tab[ind_courant], key) 
        if(identique(tab[ind_courant]->key, key)){
            return ind_courant;
        }else{
            ind_courant++; 
        }
    }
    return ind; 
}
    // while((tab[ind_courant]->key->n != key->n )&&(tab[ind_courant]->key->val != key->val)&&(ind_courant==t->size)){
    //     if ((tab[ind_courant]->key->n == key->n )&&(tab[ind_courant]->key->val == key->val)){
    //         return ind_courant;
    //     } else {
    //         ind_courant++;
    //     }
    // }
    // return ind;


HashTable* create_hashtable(CellKey* keys, int size){
    //On declare la table de hachage et les cellules
    HashTable * t = (HashTable*)malloc(sizeof(HashTable));
    HashCell ** tab = (HashCell **)malloc(sizeof(HashCell*));
    t->size = size;
    int i = 0; 
    while(keys-> next != NULL){
        // pos = find_position(t, keys ->data);
        // h->tab[pos] = keys-> data; 
        tab[i] = create_hashcell(keys->data); 
        keys = keys-> next;
        i++;   
    }

    while(i<size){
        tab[i] = NULL;
            i++; 
    }
    t->tab = tab;
    return t;
}

void delete_hashtable(HashTable* t){
    HashCell ** tab = t->tab;
    for(int i = 0; i< t->size; i++){
        free(tab[i]);
    }
    free(tab);
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    HashTable 
    // creation de la table de hachage des candidats 
    // création de la table de hachage des votants 
    HashTable* HTC = create_hashtable(candidates, sizeC); 
    HashTable* HTV = create_hashtable(voters, sizeV);
    Key * winner = (Key*)malloc(sizeof(Key));
    bool verifcondi = 0;

    while(decl != NULL){
        Protected* DeclaCourante = decl -> data; 
        int positionCandidates = find_position(HTC, candidates -> data); 
        int positionVotant = find_position(HTV, voters -> data); 
        char* keyVoters = key_to_str

        if((identique(HTV[positionVotant]-> key, DeclaCourante->pKey) &&(DeclaCourante->pKey-> val = 0) && (strcmp(DeclaCourante->mess, key_to_str(HTV[positionVotant]-> key)=0))){
            verifcondi = 1; 

        }verifcondi = 0; 
        DeclaCourante = decl->next; 

 
    }


    
}