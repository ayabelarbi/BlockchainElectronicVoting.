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
    char strkey[256];
    while(fgets(buffer , 256, fk) != NULL){
        sscanf(buffer,"%s %*s", strkey);
        Key * k = str_to_key(strkey);
        LCK = addKey(LCK, k);
    }
    fclose(fk);
 
    return LCK; 
}


void print_list_keys(CellKey* LCK){
    CellKey* tmp = LCK;
    while(tmp){
        printf("(%lx, %lx) \n",tmp->data->val,tmp ->data->n);
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

//fonction de hachage 
int hash_function(Key* key, int size){
    int ind = ((key->val) + (key ->n)) % size; 

    return ind;
}

//permet retourner vrai si deux clefs sont identique, faux sinon
int identique(Key* k1, Key *k2){
    if((k1->val == k2->val) && (k1->n == k2->n)){ 
        return 1; 
    }else{
        return 0;
    }
}


int find_position(HashTable* t, Key* key){
  
    if(key == NULL){
        printf("erreur, la clef est null\n");
        exit(1); 
    }
    
    int ind = hash_function(key, t->size);
    int ind_courant = ind; 
    

    while((ind_courant != t->size-1 ) && (t ->tab[ind_courant] != NULL)){ //tant que le tableau n'est pas parcouru 
        //printf("l'indice courant = %d\n", ind_courant);

        if((ind_courant == ind -1) && t->tab[ind_courant] == NULL){
            printf("la table de hashage est pleine dans find_position");
            exit(0);
        }

        if (identique(t->tab[ind_courant]->key, key)){
            //printf("indice trouvé = %d\n", ind_courant);
            return ind_courant; 
        } 
        else {
            ind_courant++; //on augmente l'indice jusqu'a trouver une place a la valeur
            //printf("indice apres incrementation = %d\n", ind_courant);
        }  
        if (ind_courant >= t->size){
                ind_courant = 0; 
        }  
        
    }
    return ind_courant;// on le retourne a la position dans lequel il devrait être
}

//Pour crée la table de hachage il faut que la size soit supérieur à la taille de la cellule de keys 
HashTable* create_hashtable(CellKey* keys, int size) {
    /* crée et initialise une table de hachage de taille size contenant une cellule pour chaque clé de la liste chainée keys */
    if (!keys){
        printf("la cellule de keys est vide");
        exit(1);
    }

    HashTable *hTable = (HashTable*)malloc(sizeof(HashTable));
    hTable->size = size ;
    hTable->tab = (HashCell**)malloc(size*sizeof(HashCell*));

    if (hTable == NULL) {
        printf ("Erreur d'allocation dans create_hashtable\n"); 
        exit(0) ;
    }
    for(int i = 0; i <size; i++){
        hTable->tab[i] = NULL; 
    }

    CellKey *temp = keys; 
    int i = 0; 
    while(temp){
        
        HashCell* cell_courant = create_hashcell(temp->data);
        //i = find_position(hTable, keys->data); 
        hTable->tab[i] = cell_courant ;
        printf("tab[%d] = %ld, %ld\n", i, hTable->tab[i]->key->val, hTable->tab[i]->key->n );
        temp = temp ->next;
        i++; 
    }

    return hTable; 
}



void delete_hashtable(HashTable* hTable){
   
    HashCell *cell_c; 

    for(int i = 0 ; i < hTable->size; i++){
        cell_c = hTable->tab[i];
        free(cell_c);
    }
    free(hTable->tab); 
    free(hTable);

    // if(hTable == NULL){
    //     printf("la table est liberee\n");
    // }else{
    //     printf("la table n'est pas liberee\n");
    // }
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    // creation de la table de hachage des candidats et votant
    
    HashTable* HTV = create_hashtable(voters, sizeV);
    HashTable* HTC = create_hashtable(candidates, sizeC); 

  
     //1 = TRUE, 0 = FALSE 

    int positionCandidates, positionVotant; 
    CellProtected* DeclaCourante = decl;
    int verifcondi = 0;
    while(DeclaCourante){
        
        //On recupère la cle du candidat pour lequel le votant vote
       
        Key* keyVotant = DeclaCourante->data->pKey; 

        //On recupere l'indice de la position du votant et du candidat dans HTV et HTC
        // printf("clef de la liste des votant %ld, %ld\n", voters->data->val, voters->data->n);
        // printf("clef de la liste des candidats %ld, %ld\n", candidates->data->val, candidates->data->n);
        
       
        positionVotant = find_position(HTV, keyVotant); 
        printf("positionVotant : %d\n", positionVotant);
        
        
        Key* keyC = HTC->tab[positionCandidates]->key;
        Key* keyV = HTV->tab[positionVotant]->key;
    
        printf("keyCandidat courante = %s\n", key_to_str(keyC));
        printf("keyVotant Courante = %s\n", key_to_str(keyV));
    
        //si le votant est présent dans la table de hachage et qu'il peut voter: condi1 ok 
        if((HTV->tab[positionVotant])&&(HTV->tab[positionVotant]->val== 0)){
            verifcondi = 1; 
            Key* keyCandidat = str_to_key(decl->data->mess);
            positionCandidates = find_position(HTC, keyCandidat);
            
            
            //si le candidat du votant est bien dans HTV
            if(HTC->tab[positionCandidates]){
                verifcondi = 1;
                //le candidat reçoit un vote 
                HTC->tab[positionCandidates]->val++;
                printf("pas de pb lalala\n");
                //printf(" il y %d vote pour %s \n",HTC->tab[positionCandidates]->val, key_to_str(HTC->tab[positionCandidates]->key) );
                //on comptabilise le vote du votant
                HTV->tab[positionVotant]->val = 1;
            }

        }
        DeclaCourante = DeclaCourante->next;
    }

    //on parcours la table de hachage du votant 
    Key * winner = (Key*)malloc(sizeof(Key));
    if(winner == NULL){
        printf("La clef winner n'est pas allouer\n");
        exit(1);
    }

    //si toute les conditions sont verifie
    if(verifcondi){
        
        for(int i = 0; i < sizeC; i ++){
            //a la recherche du candidat avec le plus vote
            int nbvoteMAX = 0; 
            if(HTC->tab[i]->val > nbvoteMAX){
                printf("%s\n",key_to_str(HTC->tab[i]->key));
                nbvoteMAX = HTC->tab[i]->val ; 
                winner =  HTC->tab[i]->key;    
                printf("nb de vote pour le candidat : %d",  HTC->tab[i]->val);
            }
        }
    }else{
        printf("pas de winner car les conditions ne sont pas satisfaite\n");
        //exit(0);
    }
    
    delete_hashtable(HTC);
    delete_hashtable(HTV);
    
    return winner; 
    
}