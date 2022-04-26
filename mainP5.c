#include "Partie5.h"

int main(){
    generate_random_data(1000, 5);

    CellProtected* declarations = read_protected("declaration.txt");
    CellKey* candidates = read_public_keys("candidates.txt");
    CellKey* voters = read_public_keys("key.txt");

    CellProtected* pr_courant = declarations;
    int cpt = 0;
    while(pr_courant){
        if (cpt%10==0){
            //create_block(???, pr_courant->pkey, 3);
            add_block(3,"Blockchain");
        }
        submit_vote(pr_courant->data);
        cpt++;
        pr_courant = pr_courant->next;
    }

    CellTree* tree = read_tree();
    print_tree(tree);

    Key* gagnant = compute_winner_BT(tree, candidates, voters, 5, 1000);
    printf("le gagnant est %s !\n", key_to_str(gagnant));

    return 0;
}