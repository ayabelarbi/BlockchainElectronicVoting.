#include "Partie3.h"

int main(){
    printf("----- Partie 3 -----\n");
    //Liste chainee de clés 
    CellKey* cellKey = read_public_keys("key.txt");
    print_list_keys(cellKey);
    delete_list_keys(cellKey);
    printf("\n");

    //Liste chainée de déclaration

    //CellProtected* ldec =  read_protected("declaration.txt");
    //afficher_liste_dec(ldec);
    //delete_chain(ldec);


    printf("\n\n------EXO6 ---- \n\n");

    Key * pKey2 = malloc( sizeof ( Key ) ) ;
    Key * sKey2 = malloc( sizeof ( Key ) ) ;
    pKey2 -> val = 18439; 
    pKey2 -> n = 26167; 
    sKey2 -> val = 14039; 
    sKey2 -> n = 26167; 

    CellKey* cellKey2 = read_public_keys("key6.txt");
    print_list_keys(cellKey2);
    HashTable *ht  = create_hashtable(cellKey2,7);
    int trouve = find_position(ht, pKey2);
    printf(" indice trouvé = %d \n", trouve);
   
    // delete_cell_key(cellKey2);
    // delete_hashtable(ht);

    return 0;
}