//
// Created by Shiyu Zhang on 2021/10/9.
//
#include <string.h>
#include <stdio.h>
#include "hashtable_search.h"

size_t BKDRHash(KeyValue str)
{
    size_t seed = 131;  // 31 131 1313 13131 131313
    size_t hash = 0;
    while (*str )
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

HashTable* hashtable_init(size_t size,float loadFactor,size_t* memory){
    HashTable* table;
    int max_size = (int) (size / loadFactor);
    table = malloc(sizeof (HashTable) + max_size * sizeof (ValueNode*));//释放内存
    *memory += sizeof (HashTable) + max_size * sizeof (ValueNode*);
    table->hashFunc = BKDRHash;
    table->table_size = max_size;
    return table;
}

//think:为什么存指针数组，不直接存ValueNode数组？

void hashtable_insert(HashTable* table,KeyValue str,size_t* memory){
    Hash_Func hashFunc = table->hashFunc;
    size_t pos = (*hashFunc)(str) % table->table_size;
    ValueNode* pHead = table->buckets[pos];
    while (pHead != NULL){
        if(strcmp(pHead->keyValue, str) == 0)
        {
            return;
        }
        pHead = pHead->next;
    }
    ValueNode* pNewNode = (ValueNode*)malloc(sizeof(ValueNode));//释放内存
    *memory += sizeof(ValueNode);
    pNewNode->keyValue = (char*)malloc(sizeof(char) * (strlen(str) + 1));//释放内存
    *memory += sizeof(char) * (strlen(str) + 1);
    strcpy(pNewNode->keyValue, str);
    pNewNode->next = table->buckets[pos];
    table->buckets[pos] = pNewNode;
}

int hashtable_search(HashTable* table,KeyValue str,int* search_count){
    Hash_Func hashFunc = table->hashFunc;
    size_t pos = (*hashFunc)(str) % table->table_size;
    ValueNode* pHead = table->buckets[pos];
    while (pHead){
        *search_count += 1;
        if(strcmp(pHead->keyValue, str) == 0)
        {
            return 1;
        }
        pHead = pHead->next;
    }
    return 0;
}

void hashtable_destroy(HashTable* table){
    int i;
    for(i = 0; i < table->table_size; i ++){
        ValueNode* pHead = table->buckets[i];
        while(pHead){
            ValueNode* pTemp = pHead;
            pHead = pHead->next;
            free(pTemp->keyValue);
            free(pTemp);
        }
    }
    free(table);
}

int main(int argc, char *argv[]){
    FILE *fp;
    FILE *fp1;
    FILE *fp2;
    char line[1024];
    char *p;
    size_t count;
    size_t words_count = 0;
    int c = 0;
    int *search_count = &c;
    size_t mem = 0;
    size_t* memory = & mem;

    //1:dictFile,2:wordsFile,3:loadFactor,4:size
    if(argc<5) {
        fprintf(stderr, "ERROR: No word file specified\n");
        return EXIT_FAILURE;
    }

    float loadFactor = atof(argv[3]);
    int _size = atoi(argv[4]);
    HashTable* hashTable = hashtable_init(_size,loadFactor,memory);

    if(!(fp=fopen(argv[1], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[3]);
        return EXIT_FAILURE;
    }
    while(fgets(line, 1024, fp)) {
        hashtable_insert(hashTable,line,memory);
    }
    fclose(fp);

    fp2 = fopen("hashtable_output.txt", "w");//打开输出文件
    if(!(fp1=fopen(argv[2], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[4]);
        return EXIT_FAILURE;
    }

    while(fgets(line,1024,fp1)){
        if(hashtable_search(hashTable,line,search_count) == 1){
            count ++;
            fputs(line,fp2);
        }
        words_count ++;
    }

    fclose(fp1);
    fclose(fp2);
    printf("哈希表负载因子：%f\n",loadFactor);
    printf("使用的哈希表内存：%ld 字节\n",*memory);
    printf("查找出的字符串个数：%ld\n",count);
    printf("总共要查找的字符串个数：%ld\n",words_count);
    printf("总查找次数：%d\n",*search_count);
    double temp = ((double)(*search_count)) / ((double) words_count);
    printf("平均查找次数：%f\n",temp);

    return EXIT_SUCCESS;

}

