//
// Created by Shiyu Zhang on 2021/9/24.
//

#ifndef BLOOM_FILTER_C_BLOOM_H
#define BLOOM_FILTER_C_BLOOM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef const char* KeyValue;
typedef struct {
    size_t _range;//要存储的long的个数
    size_t _size;
    unsigned char *a;
    size_t nfuncs;//hash函数数量
} BLOOM;

BLOOM *bloom_create(size_t size, size_t nfuncs);
void bloom_destroy(BLOOM *bloom);
void bloom_add(BLOOM *bloom, KeyValue s);
void bloom_putBits(BLOOM *bloom, uint32_t hash_value);
bool bloom_check(BLOOM *bloom, KeyValue s);
bool bloom_checkBits(BLOOM *bloom, uint32_t hash2);

#endif //BLOOM_FILTER_C_BLOOM_H
