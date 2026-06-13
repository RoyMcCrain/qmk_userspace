#pragma once
#include QMK_KEYBOARD_H
#include "nglist.h"

#define LIST_SIZE 5 // 集合の最大サイズ

typedef struct {
    NGList elements[LIST_SIZE];
    int size;
} NGListArray;

void initializeListArray(NGListArray *);
bool addToListArray(NGListArray *, NGList *);
int includeListArray(NGListArray *, NGList *);
bool removeFromListArrayAt(NGListArray *, int);
