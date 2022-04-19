// #include "Partie4.h"
#include "Partie2.h"
#include "Partie3.h"
#include "Partie1.h"

#include <time.h>

#define TAILLE_MAX 256

CellKey* create_cell_key(Key* key){
    CellKey* CK = (CellKey*)malloc(sizeof(CellKey));
    if(CK == NULL){
        printf("Erreur malloc create_cell_key");
    }
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
    
    if(k == NULL){
        printf("Erreur malloc read_public_key");
    }
    if(fk == NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    while(fgets(buffer, TAILLE_MAX, fk) != NULL){
        Key* k = (Key*)malloc(sizeof(Key));

        if(k == NULL){
            printf("Erreur malloc key reas_public_key");
            return NULL;
        }
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
    if(res == NULL){
        printf("Erreur malloc create_cell_protected");
    }
    res->data = pr;
    res->next = NULL;
    return res;
}


CellProtected * ajout_en_tete(CellProtected * ldec, Protected * pr){
    CellProtected * res = create_cell_protected(pr);
    res->next = ldec; 
    return res; 
}


CellProtected* read_protected(char * nomFichier){//A revoir 
    char buffer[TAILLE_MAX];
    FILE* f = fopen(nomFichier, "r");

    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

      

    CellProtected* res = (CellProtected*)malloc(sizeof(CellProtected));
    if (res==NULL){
        printf("probleme d'allocation res dans read_protected\n");
    }

    while(fgets(buffer, TAILLE_MAX, f) != NULL){
        char* chaine =  (char*)malloc(sizeof(char)*256);    
        fscanf(f,"%s", chaine);   
        res = ajout_en_tete(res, str_to_protected(chaine));
    }
    fclose(f);
    return res;
}


void afficher_liste_dec(CellProtected * ldec){
    CellProtected * lcourant = ldec;
    while(lcourant!=NULL){
        if (protected_to_str(lcourant->data)!=NULL){
            printf("%s\n", protected_to_str(lcourant->data));
            lcourant = lcourant->next;
        } else {
            printf("erreur dans afficher_liste_dec\n");
        }       
        
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
    if(res == NULL){
        printf("Erreur de malloc create_hashcell");
    }
    res -> key = key; 
    res -> val = 0; 
    return res; 
}

int hash_function(Key* key, int size){
    int ind = ((key->val) + (key ->n)) % size; 
    return ind;
}


int identique(Key* k1, Key *k2){
    if((k1->val == k2->val) && (k1->n == k2->n)){ 
        return 1; 
    }else{
        return 0;
    }
}

// int find_position(HashTable*h , Key * key){
    
//     HashCell** tab = h ->tab; 
//     int pos = hash_function(key, h->size);
//     int i = 0;
//     //si l'element a ete trouvé on retourne la position 
//     if ((tab[pos]->key->val == key->val) &&(tab[pos]->key->n == key->n)){
//         return pos; 
//     }

//     else { //sinon il retourne la position dans lequel il devrait être
        
//         while((tab[i] != NULL)){
//             i++; 
//         }
//     }
//     return i; 
// }


int find_position(HashTable* t, Key* key){

    if(key == NULL){
        printf("erreur, la clef est null\n");
        exit(1); 
    }

    HashCell ** tab = t->tab;
    int ind = hash_function(key, t->size);
    if(t ->tab[ind] != NULL){
        if ((tab[ind]->key-> val == key-> val) && (tab[ind]->key -> n == key-> n)){
            return ind; 
        } else {
            for(int i = 0; i < t->size; i++){
                if(t-> tab[(ind + i) % t->size] == NULL){
                    return (ind + i) % t-> size; 
                }
            }
        return -1; 
        }
    }else {
        return ind; 
    }
}

HashTable* create_hashtable(CellKey* keys, int size) {
    /* crée et initialise une table de hachage de taille size contenant une cellule pour chaque clé de la liste chainée keys */
    if (!keys) return NULL; 
    
    HashTable *hTable = (HashTable *) malloc (sizeof(HashTable)) ; 
    hTable ->size = size ; 
    CellKey *temp = keys; 
    if (!hTable) {
        printf ("Erreur d'allocation \n"); 
        return NULL ;
    }
  
    // on cherche et insere chaque clé à sa position
    do {
        int i = find_position(hTable, keys->data);
        hTable ->tab[i] = create_hashcell(keys ->data) ;  
        temp = temp->next ; 
    } while (temp) ; 

    return hTable; 
}


// HashTable* create_hashtable(CellKey* keys, int size){
//     //On declare la table de hachage et les cellules
//     HashTable * t = (HashTable*)malloc(sizeof(HashTable));
//     HashCell **tab = (HashCell **)malloc(sizeof(HashCell*));
//     t->tab = tab; 
//     t->size = size;
//     int i = 0; 
//     if(t == NULL && tab == NULL){
//         printf("Erreur malloc ");
//     }

//     for(int j = 0 ; j< size; j++){
//         tab[i] = NULL; 
//     }

//     //insertion des cles de la cellule keys
//     while(keys){
//         int pos = find_position(t, keys ->data);// position pour ajouter les clefs
//         tab[pos] = create_hashcell(keys->data); // rajout des clefs dans la table
//         keys = keys-> next;
//         i++;
//     }

//     return t;
// }

void delete_hashcell(HashCell* hc){
    free(hc->key); 
    free(hc);
}

void delete_hashtable(HashTable* t){
    for(int i = 0 ; i< t->size; i++){
        delete_hashcell(t->tab[i]);
    }
    free(t);
}


// Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
//     HashTable 
//     // creation de la table de hachage des candidats 
//     // création de la table de hachage des votants 
//     HashTable *HTC = create_hashtable(candidates, sizeC); 
//     HashTable *HTV = create_hashtable(voters, sizeV);
//     Key * winner = (Key*)malloc(sizeof(Key));
//     int verifcondi = 0;
//     int positionCandidates, positionVotant; 

//     while(decl){

//         Protected* DeclaCourante = decl -> data; 
//         positionCandidates = find_position(HTC, candidates -> data); 
//         positionVotant = find_position(HTV, voters -> data); 
//         char* keyVoters = str_to_key(decl ->data -> mess); 
//         // a finir 
//         if(HTV -> ta)

//         //vérification de toute les conditions 
//         if((identique(HTV[positionVotant]-> key, DeclaCourante->pKey) &&(DeclaCourante->pKey-> val = 0) && (strcmp(DeclaCourante->mess, key_to_str(HTV[positionVotant]-> key)=0))){
//             verifcondi = 1; 

//         }verifcondi = 0; 
//         DeclaCourante = decl->next; 

 
//     }
    
// }