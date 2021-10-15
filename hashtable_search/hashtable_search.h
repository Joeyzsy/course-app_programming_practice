//
// Created by Shiyu Zhang on 2021/10/9.
//

#ifndef HASHTABLE_SEARCH_REWRITE_HASHTABLE_SEARCH_H
#define HASHTABLE_SEARCH_REWRITE_HASHTABLE_SEARCH_H
#include <stdlib.h>

typedef const char* KeyValue;
typedef size_t (*Hash_Func)(KeyValue str);

typedef struct Node{
    KeyValue keyValue;
    struct Node *next;
}ValueNode;

typedef struct Table{
    Hash_Func hashFunc;
    size_t table_size;
    ValueNode* buckets[0];
}HashTable;

HashTable* hashtable_init(size_t size,float loadFactor,size_t* memory);
void hashtable_insert(HashTable* table,KeyValue str,size_t* memory);
int hashtable_search(HashTable* table,KeyValue str,int* search_count);
void hashtable_destroy(HashTable* table);

#endif //HASHTABLE_SEARCH_REWRITE_HASHTABLE_SEARCH_H
