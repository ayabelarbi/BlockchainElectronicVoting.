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
    CellKey* ajoutcell = (CellKey*)malloc(sizeof(CellKey));
    if(ajoutcell == NULL){
        printf("erreur d'allocation memoire dans addKEY");
    }
    ajoutcell->data = key;
  
    ajoutcell->next = ck;
    return ajoutcell;
}

CellKey* read_public_keys(char* nom_fichier){//ok
    
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
    char buffer[256];
    while(fgets(buffer , 256, fk) != NULL){
        sscanf(buffer,"(%ld,%ld)", &val, &n);
        //fscanf(fk,"(%ld,%ld)", &val, &n); Ne fonctionne pas 
        Key* k = (Key*)malloc(sizeof(Key));
        if(k == NULL){
            printf("Erreur malloc key read_public_key");
            return NULL;
        }
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
        printf("Le liste de cle est supprimée");
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


CellProtected* ajout_en_tete(CellProtected * ldec, Protected * pr){
    CellProtected * cell_courante = create_cell_protected(pr);
    if(cell_courante == NULL){
        printf("erreur d'allocation de memoire dans ajout_en_tete");
        exit(1);
    }
    cell_courante->next = ldec;
    return cell_courante;  
}



CellProtected* read_protected(char * nomFichier){//A revoir 
    
    FILE* f = fopen(nomFichier, "r");

    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        exit(1);
    }

    CellProtected* res = NULL;

    char buffer[TAILLE_MAX];
    while(fgets(buffer, 256, f)){

        Protected* pr_courant= str_to_protected(buffer);
        
        res = ajout_en_tete(res, pr_courant);
        //printf("res ->data = %s\n", protected_to_str(res->data)); 
    }
  
    fclose(f);
    return res;
}


void afficher_liste_dec(CellProtected * ldec){
    CellProtected * lcourant = ldec; 

    while(lcourant != NULL){
        
        char * prtostr = protected_to_str(lcourant->data);
        if (prtostr!=NULL){
            printf("%s\n", prtostr);
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
        printf("La liste de declaration est supprimmée\n");
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


int find_position(HashTable* t, Key* key){
    printf("je rentre dans la fonction\n");
    if(key == NULL){
        printf("erreur, la clef est null\n");
        exit(1); 
    }

    HashCell ** tab = t->tab;
    int ind = hash_function(key, t->size);
    //printf("ind = %d\n", ind);
    int cpt = 0;
    int ind_courant = ind; 


    while((cpt!=t->size)&&(t ->tab[ind_courant] != NULL)){ //tant que le tableau n'est pas parcouru 
        printf("indice  = %d\n", ind);
        if ((tab[ind_courant]->key-> val == key-> val) && (tab[ind_courant]->key -> n == key-> n)){//si la cle est trouvé en ind_courant
            printf("j'ai trouve la cle a l'indice : %d\n", ind_courant);
            return ind_courant; 
        } 
        else {
            printf("je passe if2 et ind_courant incremente\n");
            ind_courant++; //on augmente l'indice jusqu'a trouver une place a la valeur
            cpt ++; 
            if (cpt >= t->size){
                printf("je passe if3 : jarrive a la fin du tableau ind_courant repasse a 0\n");
                ind_courant = 0; 
            }
        }      
    }
    return ind_courant;// on le retourne a la position dans lequel il devrait être
}

// int find_position(HashTable* t, Key* key){
//     //printf("je rentre dans la fonction \n");
//     HashCell ** tab = t->tab;
//     int size = t->size;

//     int ind = hash_function(key, size);
//     //printf("ind = %d\n", ind);
//     int ind_courant = ind;
//     int cpt = 0;

//     while ( (cpt!=size)  ){
//         //printf("je rentre dans le while, ind_courant = %d\n", ind_courant);
//         Key * cle_courante = tab[ind_courant]->key;
//         if (tab[ind_courant]!=NULL){
//             return ind_courant;
//         }
//         if ((cle_courante->val == key->val)&&(cle_courante->n == key->n)){
//             return ind_courant;
//         } else if (ind_courant+1 == size) {
//             ind_courant = 0;
//             cpt++;
//         } else {
//             ind_courant++;
//             cpt++;
//         }
//     } 
//     printf("plus de place\n");
    
//     exit(0);
// }

HashTable* create_hashtable(CellKey* keys, int size) {
    /* crée et initialise une table de hachage de taille size contenant une cellule pour chaque clé de la liste chainée keys */
    if (!keys) return NULL; 
    
    HashTable *hTable = (HashTable*)malloc(sizeof(HashTable)) ; 
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

void delete_hashtable(HashTable* hTable){
   
    HashCell *cell_c; 
    for(int i = 0 ; i <hTable->size; i++){
        cell_c = hTable->tab[i];
        free(cell_c);
        if(cell_c != NULL){
            printf("Problème de free dans delete hashTable\n");
        }
    }
    free(hTable->tab); 
    if(hTable != NULL){
        printf("La table de hashage n'est pas liberé \n");
    }
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    // creation de la table de hachage des candidats 
    // création de la table de hachage des votants 
    HashTable* HTC = create_hashtable(candidates, sizeC); 
    HashTable* HTV = create_hashtable(voters, sizeV);

  
    int verifcondi = 0; //1 = TRUE, 0 = FALSE 

    int positionCandidates, positionVotant; 

    while(decl){

        Protected* DeclaCourante = decl -> data; 
        Key* keyCandidat = str_to_key(decl ->data -> mess);
        //On recupere l'indice de la position du votant et du candidat dans HTV et HTC
        positionCandidates = find_position(HTC, candidates -> data); 
        positionVotant = find_position(HTV, voters -> data); 
        printf("positionCandidate %d", positionCandidates); 
        printf("positionVotant : %d", positionVotant);
        
        Key* keyC = HTC->tab[positionCandidates]->key;
        Key* keyV = HTV->tab[positionVotant]->key;

        //si le votant est présent dans la table de hachage et qu'il peut voter: condi1 ok 
        if((keyC->val == DeclaCourante->pKey->val) && (keyC->n == DeclaCourante->pKey->n)&&(HTV->tab[positionVotant]->val == 0)){
            verifcondi = 1; 
            printf("La condition 1 est verifie : %d", verifcondi);
            HTV->tab[positionVotant]->val = 1; //on comptabilise le vote du votant
            //si le candidat du votant est bien dans HTV
            if((keyCandidat->val == keyV->val) &&(keyCandidat->n == keyV->n)){
                verifcondi = 1;
                printf("La condition est toujours verifie : %d", verifcondi); 
                HTC->tab[positionCandidates]->val += 1;
            } else {
                verifcondi = 0 ;
                printf("Le candidat n'est pas dans HTC"); 
            }
        }else {
            verifcondi = 0; 
            printf("Le votant n'est pas dans HTV");
        }
    decl = decl->next; 
    DeclaCourante = decl->data;
    }

    //on parcours la table de hachage du votant 
    Key * winner = (Key*)malloc(sizeof(Key));
    for(int i = 0; i < sizeV; i ++){
        int nbvoteMAX = 0; 
        if(HTV->tab[i]->val > nbvoteMAX){
            HTV->tab[i]->val = nbvoteMAX; 
            winner =  HTV->tab[i]->key; 
        } 

    }
    
    delete_hashtable(HTC);
    delete_hashtable(HTV);
    
    return winner; 
    
}