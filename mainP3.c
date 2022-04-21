#include "Partie3.h"

int main(){
    printf("----- Partie 3 -----\n");
    //Liste chainee de clés 
    CellKey* cellKey = read_public_keys("key.txt");
    print_list_keys(cellKey);
    delete_list_keys(cellKey);
    printf("\n\n");
    
    //Liste chainée de déclaration
    // CellProtected* ldec =  read_protected("declaration.txt");
    // afficher_liste_dec(ldec);
    // delete_chain(ldec);


    printf("\n\n------EXO6 ---- \n\n");

    Key * pKey2 = malloc( sizeof ( Key ) ) ;
    Key * sKey2 = malloc( sizeof ( Key ) ) ;
    pKey2 -> val =30181; 
    pKey2 -> n = 50851; 


    CellKey* cellKey2 = read_public_keys("key.txt");
    print_list_keys(cellKey2);
    HashTable *ht  = create_hashtable(cellKey2,7);
    int trouve = find_position(ht, pKey2);
    printf("indice trouvé = %d \n", trouve);
   
    // HashCell**hc = ht->tab; 
    // printf("La taille de la table de hachage = %d\n", ht->size);
    // for(int i = 0; i< ht->size; i++){
    //     printf("la clef en pos : %d = (%ld,%ld)\n ", i, hc[i]->key->val, hc[i]->key->n );
    // }

    //delete_hashtable(ht);

    CellKey* cellKeyWinner = read_public_keys("key.txt");
    CellKey* cellKeyCandidat = read_public_keys("candidates.txt");
    print_list_keys(cellKeyWinner);
    printf("\n");
    print_list_keys(cellKeyCandidat);
    printf("\n");
    CellProtected* cellprotectedWinner = read_protected("declaration.txt"); 
    afficher_liste_dec(cellprotectedWinner);
    // printf("\n");
    
    // Key * winner = compute_winner(cellprotectedWinner,cellKeyCandidat, cellKeyWinner,2,4); 
    // printf("Le gagnant est : %s", key_to_str(winner));
    
    return 0;
    
}