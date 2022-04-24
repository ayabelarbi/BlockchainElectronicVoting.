#include "Partie2_4.h"
#include <time.h>

#define DATA_SIZE 256

void generate_random_data(int nv, int nc){
	srand(time(NULL));
	char buffer[DATA_SIZE];
	FILE *fk = fopen("key.txt", "w");
	FILE *fc = fopen("candidates.txt","w");
	FILE *fd = fopen("declaration.txt", "w");
	char mess[256];
	
	Key **tabkp = (Key**)malloc(nv*sizeof(Key*)); 
	Key **tabks = (Key**)malloc(nv*sizeof(Key*)); 
	Key **tabC = (Key**)malloc(nc*sizeof(Key*));
	
	if(fk == NULL || fd == NULL || fc == NULL ){
		printf("Erreur d'ouverture de fichier");
		exit(EXIT_FAILURE);
	}
	//fprintf(fk," \n");
	for(int i = 0; i <nv; i++){
		Key* pkey = (Key*)malloc(sizeof(Key));
		Key* skey = (Key*)malloc(sizeof(Key));
		init_pair_keys(pkey, skey, 3, 7);
		//fprintf(fk ,"Citoyen n° %d" ,i+1);
		fprintf(fk, "(%ld,%ld) (%ld,%ld)\n", skey->val,skey->n,pkey->val,pkey->n);
		tabkp[i]= pkey;
		tabks[i] = skey;

	}
	
	for(int i = 0; i < nc; i++){
		int k= rand()%nv;
		
	    fprintf(fc,"(%ld, %ld)\n", tabkp[k]->val, tabkp[k]->n);
	    Key* pkeyC = (Key*)malloc(sizeof(Key));
		init_key(pkeyC, tabkp[k]->val, tabkp[k]->n);
		tabC[i] = pkeyC;
	}
	
	for(int i = 0; i < nv; i++){
		int indice = rand()%nc;
		Protected *pr; char* mess; Signature* s; 
		mess = key_to_str(tabC[indice]);
		pr = init_protected(tabkp[i], mess , s );
		s = sign(mess, tabks[i]);
		
		fprintf(fd,"%s (%ld,%ld) %s\n",mess, tabkp[i]->val, tabkp[i]->n,signature_to_str(s));

	}
	
	fclose(fk);
	fclose(fc);
	fclose(fd);
}