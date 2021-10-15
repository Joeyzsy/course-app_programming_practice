//
// Created by Shiyu Zhang on 2021/10/12.
//

#ifndef AVSEARCH_AVSEARCH_H
#define AVSEARCH_AVSEARCH_H

#include <stddef.h>
#include <stdbool.h>

enum BALANCE_STATUS{LH = -1,EH = 0, RH = 1};
typedef struct ValueList{
    char* value;
    struct ValueList *next;
}*ValueListPtr;
typedef struct AVTreeNode{
    ValueListPtr value_list;
    size_t value_hash;
    int height;
    struct AVTreeNode *left;
    struct AVTreeNode *right;
    //struct AVTreeNode *parent;
}*NodePtr;

NodePtr add(NodePtr root,char* data);
bool search(NodePtr root, char* data);
NodePtr LL_rotate(NodePtr ptr);
NodePtr LR_rotate(NodePtr ptr);
NodePtr RL_rotate(NodePtr ptr);
NodePtr RR_rotate(NodePtr ptr);
void destroy(NodePtr ptr);


#endif //AVSEARCH_AVSEARCH_H
