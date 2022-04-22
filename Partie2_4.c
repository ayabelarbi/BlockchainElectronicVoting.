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
	  
	for(int i = 0; i <nv; i++){
		Key* pkey = (Key*)malloc(sizeof(Key));
		Key* skey = (Key*)malloc(sizeof(Key));
		init_pair_keys(pkey, skey, 3, 7);
		
		//fprintf(fk ,"Citoyen n° %d" ,i+1);
		fprintf(fk, "(%ld,%ld) (%ld, %ld)\n", skey->val,skey->n,pkey->val,pkey->n);
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
// void generate_random_data(int nv, int nc){
// 	srand(time(NULL));
	
// 	char buffer[DATA_SIZE];
// 	FILE *fk = fopen("key.txt", "w");
// 	FILE *fc = fopen("candidates.txt","w");
// 	FILE *fd = fopen("declaration.txt", "w");
	
// 	if(fk == NULL || fd == NULL || fc == NULL ){
// 		printf("Erreur d'ouverture de fichier");
// 		exit(EXIT_FAILURE);
// 	}
	
// 	char mess[256];
	
// 	Key **tabkp = (Key**)malloc(nv*sizeof(Key*)); //tableau de pointeurs sur les cles publiques
// 	Key **tabks = (Key**)malloc(nv*sizeof(Key*));  //tableau de pointeurs sur les cles secretes
// 	Key **tabC = (Key**)malloc(nc*sizeof(Key*));  // tableau de pointeurs sur les cles publiques des candidats
// 	if ((tabkp == NULL)||(tabks==NULL)||(tabC==NULL)) {
// 		printf("erreur malloc tab pointeurs sur cles dans generate_random_data\n");
// 	}
	
	 
// 	//ecriture des cles dans key.txt et remplissage des tableaux de pointeurs de cles
// 	for(int i = 0; i <nv; i++){
// 		Key* pkey = (Key*)malloc(sizeof(Key));
// 		Key* skey = (Key*)malloc(sizeof(Key));
// 		if ((pkey==NULL)||(skey==NULL)){
// 			printf("erreur malloc cles dans generate_random_data\n"); 
			
// 		}
		
		
// 		init_pair_keys(pkey, skey, 3, 7);
		
// 		if (fprintf(fk, "pkey : %s, sKey : %s\n", key_to_str(pkey),key_to_str(skey))!=2){
// 			printf("erreur dans le fprintf de generate_random_data\n");
// 			printf("nb args %d\n",fprintf(fk, "pkey : %s, sKey : %s\n", key_to_str(pkey),key_to_str(skey)));
// 		}
		
// 		tabkp[i]= pkey;
// 		tabks[i] = skey;
// 		free(pkey);
// 		free(skey);
// 	}
	
// 	//ecriture des candidats dans candidate.txt et remplissage de tabC
// 	for(int i = 0; i < nc; i++){
// 		int k= rand()%nv;
		
// 		if( fprintf(fc, "%s", key_to_str(tabkp[k])) !=1){
// 			printf("erreur fprintf dans generate_random_data\n");
// 			printf("nb args %d\n",fprintf(fc, "%s", key_to_str(tabkp[k])));
// 		}

// 		Key* pkeyC = (Key*)malloc(sizeof(Key));
// 		if (pkeyC==NULL){
// 			printf("erreur malloc cle de candidat dans generate_random_data\n");
// 		}
// 		init_key(pkeyC, tabkp[k]->val, tabkp[k]->n); 
// 		tabC[i] = pkeyC; 
// 		free(pkeyC);
// 	}
	
// 	//ecriture des declarations dans declaration.txt
// 	for(int i = 0; i < nv; i++){
// 		int indice = rand()%nc;
// 		Protected *pr; 
// 		char* mess; 
// 		Signature* s; 
// 		mess = key_to_str(tabC[indice]);
// 		pr = init_protected(tabkp[i], mess , s);
// 		s = sign(mess, tabks[i]);
		
// 		if (fprintf(fd,"(%lx,%lx) %s %s\n", tabkp[i]->val, tabkp[i]->n, mess, signature_to_str(s))!=4){
// 			printf("erreur fprintf dans generate_random_data\n");
// 		}
// 		free(s);
// 		free(pr);
// 	}
// 	free(tabkp); 
// 	free(tabks); 
// 	free(tabC); 
// 	fclose(fk);
// 	fclose(fc);
// 	fclose(fd);

// } 