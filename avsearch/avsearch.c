//
// Created by Shiyu Zhang on 2021/10/12.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "avsearch.h"

int node_count;
size_t memory;

size_t BKDRHash(const char* str)
{
    size_t seed = 131;  // 31 131 1313 13131 131313
    size_t hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

NodePtr newNode(char* data){
    node_count += 1;
    size_t hash_val = BKDRHash(data);
    NodePtr nodePtr = malloc(sizeof(struct AVTreeNode));
    memory += sizeof(struct AVTreeNode);
    nodePtr->value_hash = hash_val;
    nodePtr->height = 1;
    nodePtr->value_list = malloc(sizeof(struct ValueList));
    memory += sizeof(struct ValueList);
    nodePtr->value_list->value = malloc(sizeof(char) * (strlen(data) + 1));
    memory += sizeof(char) * (strlen(data) + 1);
    strcpy(nodePtr->value_list->value,data);
    return nodePtr;
}

int max(int a, int b){
    return a > b? a : b;
}

int getHeight(NodePtr ptr){
    return ptr == NULL? 0 : ptr->height;
}

int maxHeight(NodePtr ptr){
    return max(getHeight(ptr->left), getHeight(ptr->right)) + 1;
}

bool search_valueList(NodePtr nodePtr,char* data){
    if (nodePtr == NULL) return false;
    ValueListPtr valueListPtr = nodePtr->value_list;
    while(valueListPtr != NULL){
        if(strcmp(valueListPtr->value,data) == 0) return true;
        else{
            valueListPtr = valueListPtr->next;
        }
    }
    return false;
}

NodePtr LL_rotate(NodePtr ptr){ //右旋基本操作
    if(ptr == NULL) return NULL;
    NodePtr temp = ptr->left;
    NodePtr LR = temp->right;
    ptr->left = LR;
    temp->right = ptr;
    ptr->height = maxHeight(ptr);
    temp->height = maxHeight(temp);
    return temp;
}

NodePtr LR_rotate(NodePtr ptr){
    ptr->left = RR_rotate(ptr->left);
    return LL_rotate(ptr);
}

NodePtr RL_rotate(NodePtr ptr){
    ptr->right = LL_rotate(ptr->right);
    return RR_rotate(ptr);
}

NodePtr RR_rotate(NodePtr ptr){ //左旋基本操作
    if(ptr == NULL) return NULL;
    NodePtr temp = ptr->right;
    NodePtr RL = temp->left;
    ptr->right = RL;
    temp->left = ptr;
    ptr->height = maxHeight(ptr);
    temp->height = maxHeight(temp);
    return temp;
}

NodePtr add(NodePtr root,char* data){
    size_t hash_val = BKDRHash(data);
    if(root == NULL){
        root = newNode(data);
    }
    else if (hash_val == root->value_hash){
        //同hash_value的Node已经在查找树中，则直接延长value_list
        if(!search_valueList(root,data)){
            ValueListPtr temp_new = malloc(sizeof(struct ValueList));
            memory += sizeof(struct ValueList);
            temp_new ->value = malloc(sizeof(char)* (strlen(data)+1));
            memory += sizeof(char)* (strlen(data)+1);
            strcpy(temp_new->value,data);
            temp_new->next = root->value_list;
            root->value_list = temp_new;
        }
        return root;
    }
    //加到左子树
    else if (hash_val < root->value_hash){
        root->left = add(root->left,data);
        if(getHeight(root->left) - getHeight(root->right) > 1){
            NodePtr ptr = root->left;
            if(getHeight(ptr->left) >= getHeight(ptr->right)){
                root  = LL_rotate(root);
            }
            else{
                root = LR_rotate(root);
            }
        }
        root->height = maxHeight(root);
    }
    //加到右子树
    else{
        root->right = add(root->right,data);
        if(getHeight(root->right) - getHeight(root->left) > 1){
            NodePtr ptr = root->right;
            if(getHeight(ptr->right) >= getHeight(ptr->left)){
                root = RR_rotate(root);
            }
            else{
                root = RL_rotate(root);
            }
        }
        root->height = maxHeight(root);
    }
    return root;
}

bool search(NodePtr root, char* data){
    size_t hash_val = BKDRHash(data);
    if(root == NULL) return false;
    if(root->value_hash == hash_val){
        return search_valueList(root,data);
    }
    else if (hash_val < root->value_hash){
        return search(root->left,data);
    }
    else return search(root->right,data);
}

void destroy_ValueList(ValueListPtr valueListPtr){
    if(valueListPtr != NULL) return;
    destroy(valueListPtr->next);
    free(valueListPtr->value);
    free(valueListPtr);
}

void destroy(NodePtr ptr){
    if(ptr == NULL) return;
    destroy(ptr->left);
    destroy(ptr->right);
    destroy_ValueList(ptr->value_list);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    FILE *fp1;
    FILE *fp2;
    char line[1024];
    char *p;
    NodePtr root;
    size_t count;

    //1:m阶，2:dictFile,3:wordsFile
    if(argc<4) {
        fprintf(stderr, "ERROR: No word file specified\n");
        return EXIT_FAILURE;
    }

    if(!(fp=fopen(argv[2], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    while(fgets(line, 1024, fp)) {
        root = add(root,line);
    }
    fclose(fp);

    fp2 = fopen("hashtable_output.txt", "w");//打开输出文件
    if(!(fp1=fopen(argv[3], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    while(fgets(line,1024,fp1)){
        if(search(root,line) == true){
            count ++;
            fputs(line,fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);
    printf("查找出的字符串个数：%ld\n",count);
    printf("AVL的深度：%ld\n",root->height);
    printf("节点总数：%d\n",node_count);
    printf("占用内存：%ld Byte\n",memory);

    destroy(root);

    return EXIT_SUCCESS;
}



