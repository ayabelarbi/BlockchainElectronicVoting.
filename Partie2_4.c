#include "Partie2_4.h"
#include <time.h>

#define DATA_SIZE 10000

void generate_random_data(int nv, int nc){
	srand(time(NULL));
	
	char buffer[DATA_SIZE];
	FILE *fk = fopen("key.txt", "w");
	FILE *fc = fopen("candidates.txt","w");
	FILE *fd = fopen("declaration.txt", "w");
	
	if(fk == NULL || fd == NULL || fc == NULL ){
		printf("Erreur d'ouverture de fichier");
		exit(EXIT_FAILURE);
	}
	
	char mess[256];
	
	Key **tabkp = (Key**)malloc(nv*sizeof(Key*)); //tableau de pointeurs sur les cles publiques
	Key **tabks = (Key**)malloc(nv*sizeof(Key*));  //tableau de pointeurs sur les cles secretes
	Key **tabC = (Key**)malloc(nc*sizeof(Key*));  // tableau de pointeurs sur les cles publiques des citoyens
	if ((tabkp == NULL)||(tabks==NULL)||(tabC==NULL)) {
		printf("erreur malloc tab pointeurs sur cles dans generate_random_data\n");
	}
	
	  
	for(int i = 0; i <nv; i++){
		Key* pkey = (Key*)malloc(sizeof(Key));
		Key* skey = (Key*)malloc(sizeof(Key));
		if ((pkey==NULL)||(skey==NULL)){
			printf("erreur malloc cles dans generate_random_data\n"); 	
		}
		
		init_pair_keys(pkey, skey, 3, 7);
		
		if (fprintf(fk, "(%ld,%ld), (%ld,%ld)\n", skey->val,skey->n,pkey->val,pkey->n)!=4){
			printf("erreur dans le fprintf de generate_random_data\n");
		}
		
		tabkp[i]= pkey;
		tabks[i] = skey;
	}
	
	for(int i = 0; i < nc; i++){
		int k= rand()%nv;
		
	    fprintf(fc,"Cle publique du candidat (%ld,%ld)\n", tabkp[k]->val, tabkp[k]->n);
	    Key* pkeyC = (Key*)malloc(sizeof(Key));
		init_key(pkeyC, tabkp[k]->val, tabkp[k]->n); 
		tabC[i] = pkeyC; 
	}
	char * keyd; 
	for(int i = 0; i < nv; i++){
		int indice = rand()%nc;
		Protected *pr; char* mess; Signature* s; 
		mess = key_to_str(tabC[indice]);
		pr = init_protected(tabkp[i], mess , s );
		s = sign(mess, tabks[i]);
		
		fprintf(fd,"(%ld,%ld) %s %s\n", tabkp[i]->val, tabkp[i]->n, mess, signature_to_str(s));
	}
	free(tabkp); 
	free(tabks); 
	free(tabC); 
	fclose(fk);
	fclose(fc);
	fclose(fd);

} 
