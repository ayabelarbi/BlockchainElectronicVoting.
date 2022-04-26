#include "Partie5.h"
#include "Partie2_4.h"

int main(){

    Protected *pr = str_to_protected("(55f7,6749) (350b,5405) #f5#5bd5#2cc4#5170#32b9#25ec#2cc4#3e6a#5170#2cc4#ef2#"); 
    submit_vote(pr);
    generate_random_data(10, 5);

    CellProtected* declarations = read_protected("declaration.txt");
    CellKey* candidates = read_public_keys("candidates.txt");
    CellKey* voters = read_public_keys("key.txt");
    printf("\nJ'affiche la liste des keys votantes\n");
    print_list_keys(voters);
    printf("\n");
    printf("J'affiche la liste des candidats\n");
    print_list_keys(candidates);


    printf("\n");
    printf("J'affiche la liste de declaration\n");
    afficher_liste_dec(declarations);
    printf("\n");


    char path[256]; 
    strcpy(path, "./BlockChain/");

    int j = 0; 
    

    CellTree* t = NULL;
 
    CellProtected* pr_courant = declarations;
    int cpt = 0;
    CellTree* nodetree = NULL;
    CellKey* author = voters;

    char nom[256];

    while(pr_courant){
        submit_vote(pr_courant->data);

        if (cpt%10==0){
            
            create_block(t, pr_courant->data->pKey, 3);
            sprintf(nom, "Bloc%d.txt",j);
            
            add_block(3,nom);
            j++; 
        }
        cpt++;
        pr_courant = pr_courant->next;
    }

    //Lecture de l'arbre
    CellTree* tree = read_tree();
    print_tree(tree);

    Key* gagnant = compute_winner_BT(tree, candidates, voters, 5, 10);
    printf("le gagnant est %s !\n", key_to_str(gagnant));

    return 0;
}
