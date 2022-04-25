#include "Partie3.h"

int main(){
    printf("----- Partie 3 -----\n");
    // --EXO 5  Lecture et stockage des données dans des listes chaînées. 
    //Liste chainee de clés 
    CellKey* cellKey = read_public_keys("key.txt");
    printf("J'affiche la liste des keys\n");
    print_list_keys(cellKey);
    delete_list_keys(cellKey);
    

    //Liste chaine de cles des candidats
    printf("\n\n");
    CellKey* candidates = read_public_keys("candidates.txt");
    printf("J'affiche la liste des candidats\n");
    print_list_keys(candidates);
    delete_list_keys(candidates); 
   
    printf("\n\n");
    
    //Liste chainée de déclaration
    CellProtected* ldec =  read_protected("declaration.txt");
    printf("J'affiche la liste de declaration\n");
    afficher_liste_dec(ldec);
    delete_chain(ldec);
    
    printf("\n\n");

    //--Exo 6-- Determination du gagnant de l'election 
    printf("--exercice 6 : les tables de hachage--\n\n");
    CellKey* cellKey2 = read_public_keys("key.txt");
    print_list_keys(cellKey2);
    Key * pKey2 = malloc( sizeof ( Key ) ) ;
    
    
    // pKey2->val =2797;
    // pKey2->n =27661;

    // printf("\n");

    //creation of hash table : 
    // HashTable* ht  = create_hashtable(cellKey2,2);
    // HashCell** hashtab = ht->tab; 
    // int size = ht->size ; 
    // printf("--Impression du tableau de hachage --\n");
    // //parcours de chaque cellule ; 
    // for(int i = 0; i< size; i++){

    //     printf("la clef en pos : %d\n", i); 
    //     Key* k = (Key*)malloc(sizeof(Key)); 
    //     k =  hashtab[i]->key; 
    //     printf("%ld, %ld \n",k->val, k->n);
    // }

    // int trouve = find_position(ht, pKey2);
    // printf("indice trouvé = %d \n", trouve);
    // Key* ktrouve = hashtab[trouve]->key;

    // printf("voici la clef %ld, %ld \n",ktrouve->val, ktrouve->n);
    // printf("\n");
    //delete_hashtable(ht);

   
    CellKey* cellKeyWinner = read_public_keys("key.txt");
    printf("J'affiche la liste des keys votantes\n");
    print_list_keys(cellKeyWinner);
    printf("\n");

    CellKey* cellKeyCandidat = read_public_keys("candidates.txt");
    printf("J'affiche la liste des candidats\n");
    print_list_keys(cellKeyCandidat);
    printf("\n");
    
    CellProtected* cellprotectedWinner = read_protected("declaration.txt"); 
    printf("J'affiche la liste de declaration\n");
    afficher_liste_dec(cellprotectedWinner);
    printf("\n");
    


    Key * winner = compute_winner(cellprotectedWinner,cellKeyCandidat, cellKeyWinner,8,4); 
    printf("key->val = %ld\n ", winner->val);
    printf("Le gagnant est : %s", key_to_str(winner));
    
   
    printf("\n\n");
    printf("---Exerce 8: manipulation d'un arbre de blocs --- \n");

    return 0;
    
}