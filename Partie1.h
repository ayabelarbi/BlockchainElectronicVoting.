#ifndef _PARTIE1_H
#define _PARTIE1_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_primitive_naive(long int p);
long modpow_naive(long a, long m, long n);
int modpow(long a, long m, long n);
int is_prime_miller ( long p , int k ); 
long rand_long ( long low , long up );
int witness ( long a , long b , long d , long p );
long random_prime_number(int low_size, int up_size, int k);
long extended_gcd ( long s , long t , long *u , long * v ); 
void generate_key_values(long p, long q, long* n, long *s,long *u); 
void print_long_vector ( long * result , int size ); 
long * encrypt(char* chaine, long s, long n); 
char* decrypt(long* crypted, int size, long u, long n ); 

#endif
