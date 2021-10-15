//
// Created by Shiyu Zhang on 2021/9/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bloom.h"

int main (int argc, char *argv[])//0:errorRate,1:dictSize,2:dictFile,3:wordsFile
{
    FILE *fp;
    FILE *fp1;
    FILE *fp2;
    char line[1024];
    BLOOM *bloom;
    char *p;
    size_t count;

    if(argc<4) {
        fprintf(stderr, "ERROR: No word file specified\n");
        return EXIT_FAILURE;
    }

    float errorRate = atof(argv[1]);
    long dictSize = 1270000;
    size_t bitArraySize =  dictSize * 1.44 * log(1 / errorRate) / log(2);
    size_t numOfHash = 0.693 * dictSize * 1.44 * log(1 / errorRate) / log(2) / dictSize;
    bloom = bloom_create(bitArraySize,numOfHash);


    if(!(fp=fopen(argv[2], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    while(fgets(line, 1024, fp)) {
        bloom_add(bloom, line);
    }
    fclose(fp);

    fp2 = fopen("output.txt", "w");//打开输出文件
    if(!(fp1=fopen(argv[3], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[4]);
        return EXIT_FAILURE;
    }

    while(fgets(line,1024,fp1)){
        if(bloom_check(bloom, line)){
            count ++;
            fputs(line,fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);
    bloom_destroy(bloom);
    printf("找到字符串数量：%ld\n",count);
    double er = ((double)abs(count - 6306)) / ((double)6306);
    printf("错误率：%f\n",er);

    return EXIT_SUCCESS;

}
