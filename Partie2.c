#include "Partie2.h"
#include "Partie1.h"

// LES CLEFS 
void init_key(Key* key, long val, long n){
    key -> val = val; 
    key -> n = n ; 
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);
    while ( p == q ) {
        q = random_prime_number (low_size ,up_size , 5000) ;
    }
    long n , s , u ;
    generate_key_values (p ,q ,&n ,&s ,&u ) ;
    //Pour avoir des cles positives :
    if (u < 0) {
        long t = (p -1) *( q -1) ;
        u = u + t ; //on aura toujours s*u mod t = 1
    }
    init_key(pKey,s,n);
    init_key(sKey,u,n);
}

//LA SIGNATURE 
char* key_to_str(Key* key){
    char* buffer = (char*)malloc(sizeof(char)*256); 
    sprintf(buffer,"(%ld,%ld)", key-> val, key -> n); 
    return buffer;
}

Key* str_to_key(char* str){
    Key* res = (Key*)malloc(sizeof(Key));
    long val; 
    long n; 
    sscanf(str, "(%lx, %lx)", &val, &n); 
    init_key(res, val, n); 
    return res;
}


Signature* init_signature(long* content, int size){
    Signature* res = (Signature*)malloc(sizeof(Signature));
    res -> content = content;
    res -> size = size; 
    return res; 
}

Signature* sign(char* mess, Key* sKey){
    long* content = encrypt(mess, sKey ->val, sKey -> n); 
    Signature* res = init_signature(content, strlen(mess)); // mess ou content ?? 
    return res; 
}

char* signature_to_str ( Signature * sgn ) {
    char* result = malloc(10*sgn-> size*sizeof(char)) ;
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

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    Protected* res = (Protected*)malloc(sizeof(Protected));
    res -> pKey = pKey; 
    res -> mess = mess; 
    res -> sgn = sgn;
    return res; 
}

int verify(Protected* pr){
 /*retourne 1 si la Signature de pr correspond bien au message et à la personne contenue dans pr, 0 sinon*/
    long * tab = (pr->sgn)->content;
    char * m = decrypt(tab, (pr->sgn)->size , (pr->pKey)->val, (pr->pKey)-> n);
    if (strcmp(pr->mess, m)==0){
        return 1;
    } else {
        printf("Erreur dans verify!! \n");
        return 0;      
    }
}

char* protected_to_str(Protected* pr){
    char *res = (char *)malloc(sizeof(char)*256);
    strcpy(res,"");
    strcat(res,key_to_str(pr->pKey));
    strcat(res," ");
    strcat(res,pr->mess);
    strcat(res," ");
    strcat(res,signature_to_str(pr->sgn));
    return res;
}



Protected* str_to_protected(char* chaine){
    Signature * sgn = (Signature*)malloc(sizeof(Signature));
    char * mess = (char*)malloc(sizeof(char));
    Key * pKey = (Key*)malloc(sizeof(Key));
    
    int i = 0;
    char * chaine_courante = "";
    while(chaine[i] != ' '){
        chaine_courante+= chaine[i];
        i++;
    }
    chaine_courante+='\0';
    pKey = str_to_key(chaine_courante);
    
    chaine_courante = "";
    while(chaine[i] != ' '){
        chaine_courante+= chaine[i];
        i++;
    }
    
    chaine_courante+='\0';
    mess = chaine_courante;
    
    chaine_courante = "";
    while(chaine[i] != '\0'){
        chaine_courante+= chaine[i];
        i++;
    }
    
    chaine_courante+='\0';
    sgn = str_to_signature(chaine_courante);
    
    Protected * res = init_protected(pKey, mess, sgn);
    free(pKey);
    return res;

}