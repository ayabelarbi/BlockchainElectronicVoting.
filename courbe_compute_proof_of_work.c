#include <stdio.h>
#include <stdlib.h>
#include "Partie4.h"
#include <openssl/sha.h>
#include <time.h>

int main(){

    const char * s = "Rosetta code";
    unsigned char * d = SHA256(s , strlen( s ), 0);

    //CREATION D'UN BLOC
        //cle de l'auteur
    Block * b = (Block *)malloc(sizeof(Block));
    Key* pkey = (Key*)malloc(sizeof(Key));
    Key* skey = (Key*) malloc(sizeof(Key));
    init_pair_keys(pkey,skey,3,7); 
    b->author = pkey;

        //votes
    CellProtected * votes = read_protected("declaration.txt");
    b->votes = votes;

        //un previous_hash au hasard
    unsigned char * tab_ph = SHA256(d, strlen(d),0);
    b->previous_hash = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        b->previous_hash[i] = tab_ph[i];
    }
        //preuve de travail
    b->nonce = 0; 
   
        //hash
    char * str_b = block_to_str(b);
    unsigned char * tab_h = SHA256(str_b, strlen(str_b), 0);
    b->hash = (unsigned char *)malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++){
        b->hash[i] = tab_h[i];
    }

    //calcul du temps mis par compute_proof_of_work et ecriture dans un fichier

    FILE *f = fopen("proof_of_work.dat", "w");
    if(f == NULL){
        printf("Erreur d'ouvertur du fichier");
        exit(1);
    }

	clock_t temps_initial; 
	clock_t temps_final;

    int nb_0;
    for(nb_0 = 0; nb_0 < 5; nb_0++){
        b->nonce = 0;
        temps_initial = clock();
        compute_proof_of_work(b, nb_0);
        temps_final = clock();
        float temps = 1000*((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f, "%d %.4f\n",nb_0, temps); 

    }
	
    fclose(f);
	return 0; 
	
}