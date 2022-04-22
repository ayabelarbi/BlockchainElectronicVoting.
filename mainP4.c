#include "Partie4.h"
#include <openssl/sha.h>

int main(){

//test SHA256

const char * s = "Rosetta code ";
unsigned char * d = SHA256(s , strlen( s ), 0) ;
int i ;

for (i = 0; i<SHA256_DIGEST_LENGTH ; i++)
    printf("%02x" , d[i]) ;
putchar('\n') ;

/*test lecture ecriture
Block * b = ...
ecriture_bloc(b);
Block * 2 = lecture_bloc("bloc.txt"); */

/*test bloc to str
printf("Bloc : \n%s\n", block_to_str(b));  */

//test str_hache
char * s2 = "Rosetta code";
char * hash = str_hache(s2);
printf("%s\n", hash);

    return 0;
}
