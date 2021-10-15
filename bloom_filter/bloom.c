//
// Created by Shiyu Zhang on 2021/9/25.
//

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <printf.h>
#include "bloom.h"
#include "MurmurHash2.h"

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

BLOOM *bloom_create(size_t size, size_t funcs){
    BLOOM *bloom;
    if(!(bloom = malloc(sizeof(BLOOM)))) return NULL;
    if(! (bloom->a = calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char)))){
        free(bloom);
        return NULL;
    }
    bloom->nfuncs = funcs;
    bloom->_range = (size+CHAR_BIT-1)/CHAR_BIT;
    bloom->_size = size;
    return bloom;
}

void bloom_add(BLOOM *bloom, KeyValue s){
    size_t n;
    for(n = 0; n < bloom->nfuncs; n ++){
        SETBIT(bloom->a,MurmurHash2(s,strlen(s),n) % bloom->_size);
    }
}

bool bloom_check(BLOOM *bloom, KeyValue s){
    int n;
    bool isIn = true;
    for(n = 0; n < bloom->nfuncs; n ++){
        isIn &= (GETBIT(bloom->a,MurmurHash2(s,strlen(s),n) % bloom->_size) != 0);
    }
    return isIn;
}

void bloom_destroy(BLOOM *bloom){
    free(bloom->a);
    free(bloom);
}






