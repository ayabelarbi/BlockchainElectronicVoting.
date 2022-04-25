#include "Partie2.h"
#include "Partie1.h"

// LES CLEFS 

/*initialisation de cle*/
void init_key(Key* key, long val, long n){
    key->val = val; 
    key->n = n ; 
}

/*initialisation d'une paire de cles*/
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);
    while ( p == q ) {
        q = random_prime_number (low_size ,up_size , 5000) ;
    }
    long n , s , u, t ;
    generate_key_values (p ,q ,&n ,&s ,&u ) ;
    
    //Pour avoir des cles positives :
    if (u < 0) {
        t = (p -1) *( q -1) ;
        u = u + t ; //on aura toujours s*u mod t = 1
    }
    init_key(pKey,s,n);
    init_key(sKey,u,n);
}

/* revoie la chaine de caractere qui represente la cle*/
char* key_to_str(Key* key){
    char* str = (char*)malloc(sizeof(char)*256);
    if(str == NULL){
        printf("la chaine est vide dans key_to_str");
        exit(1);
    }

    if(key == NULL){
        printf("Clef vide dans key_to_str");
        exit(1);
    }

    char buffer[256];
    str = strdup(buffer);
    sprintf(str,"(%lx,%lx)", key->val, key->n);
    return str;
}

/* renvoie la cle representee par la chaine de caractere */
Key* str_to_key(char* str){
    Key* res = (Key*)malloc(sizeof(Key));
    if (res == NULL){
        printf("erreur malloc str_to_key\n");
    }
    long val; 
    long n; 
   
    if (sscanf(str, "(%lx, %lx)", &val, &n)!=2){
        printf("erreur dans sscanf de str_to_key\n");
    }
    init_key(res, val, n); 
    return res;
}

//LA SIGNATURE

/*alloue et initialise une signature avec le tableau de long content*/
Signature* init_signature(long* content, int size){
    Signature* res = (Signature*)malloc(sizeof(Signature));
    if (res == NULL){
        printf("erreur malloc init_signature\n");   
    }
    res->content = content;
    res->size = size; 
    return res; 
}

/* renvoie signature : le message mess crypté avec la cle secrete du votant */
Signature* sign(char* mess, Key* sKey){
    long* content = encrypt(mess, sKey->val, sKey-> n); 
    Signature* res = init_signature(content, strlen(mess));
    return res; 
}

/* renvoie la chaine de caractere qui represente la signature */
char* signature_to_str ( Signature * sgn ) {
    char* result = malloc(10*sgn->size*sizeof(char)) ;
    result [0]= '#' ;
    int pos = 1;
    char buffer[156];
    for ( int i = 0; i < sgn -> size ; i ++) {
        sprintf(buffer , "%lx" , sgn -> content [ i ]) ;
        for ( int j =0; j < strlen ( buffer ) ; j ++) {
            result [ pos ] = buffer [ j ];
            pos = pos +1;
        }
        result [ pos ] = '#' ;
        pos = pos +1;
    }
    result [ pos ] = '\0' ;
    result = realloc ( result , ( pos +1) * sizeof ( char ) ) ;
    return result ;
}

/* renvoie la signature representee par la chaine de caractere */
Signature* str_to_signature( char * str ) {
    int len = strlen(str) ;
    long* content = ( long *)malloc(sizeof(long)*len ) ;
    int num = 0;
    char buffer[256];
    int pos = 0;
    for ( int i =0; i < len ; i ++) {
        if ( str[i] != '#' ) {
            buffer[pos] = str[i];
            pos = pos +1;
        } else {
            if ( pos != 0) {
                buffer[pos] = '\0' ;
                sscanf ( buffer , "%lx" , &(content [num ]) ) ;
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc( content , num * sizeof ( long ) ) ;
    return init_signature(content , num ) ;
}

//LES DECLARATIONS

/* alloue et initialise une declaration */
Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    Protected* res = (Protected*)malloc(sizeof(Protected));
    if (res == NULL){
        printf("erreur malloc init_protected\n");
    }
    res->pKey = pKey; 
    res->mess = mess; 
    res->sgn = sgn;
    return res; 
}

/*retourne 1 si la Signature de pr correspond bien au message et à la personne contenue dans pr, 0 sinon*/
int verify(Protected* pr){
    long * tab = (pr->sgn)->content;
    char * m = decrypt(tab, (pr->sgn)->size , (pr->pKey)->val, (pr->pKey)->n);
    if (m==NULL){
        printf("erreur dans l'allocation de m dans verify\n");
        exit(0);
    }
    if (strcmp(pr->mess, m)==0){
        return 1;
    } else {
        printf("Erreur dans verify!! \n");
        return 0;      
    }
}

/* renvoie la chaine de caractere representant la declaration */
char* protected_to_str(Protected* pr){
    
    char * key = key_to_str(pr->pKey); 
    if(key == NULL){
        printf("erreur clef vide dans protected_to_str");
    }
    char * s = signature_to_str(pr->sgn);
    if(s == NULL){
        printf("erreur signature vide dans protected_to_str");
    }

    int size =  strlen(key)+strlen(pr->mess)+strlen(s)+3;
    char *res = (char *)malloc(size*sizeof(char));
    if (res == NULL){
        printf("erreur malloc protected_to_str\n");   
    }

    // printf("key = %s\n", key); 
    // printf("mess = %s\n", pr->mess);
    // printf("signature = %s\n", s);
    strcpy(res,"");
    strcat(res,key);
    strcat(res," ");
    strcat(res,pr->mess);
    strcat(res," ");
    strcat(res, s);
    strcat(res,"");
    free(key);
    free(s);
    return res;
}


/*renvoie la declaration representee par la chaine de caractere */
Protected* str_to_protected(char* chaine){

    char key[256];
    char * mess = (char *)malloc(sizeof(char)*256);
    char sgn[256];

    if (sscanf(chaine, "%s %s %s", key, mess, sgn)!=3){
        printf("erreur dans le sscanf de str_to_protected\n");
        printf("nb arg : %d\n",sscanf(chaine, "%s %s %s", key, mess, sgn) );
    }

    //printf("affichage de ce qui a été scanné : %s %s %s\n", key, mess, sgn);
    Key * k = str_to_key(key);
    
    Signature * s = str_to_signature(sgn);
    
    Protected * pr = init_protected(k,mess,s);
    
    return pr;

}