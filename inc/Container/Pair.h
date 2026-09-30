#ifndef PAIR_H
#define PAIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pair {
    size_t ELEMENT_SIZE1;
    size_t ELEMENT_SIZE2;
    void* Data;
} Pair;

Pair Pair_New(size_t ELEMENT_SIZE1,void* First,size_t ELEMENT_SIZE2,void* Second) {
    Pair v;
    v.ELEMENT_SIZE1 = ELEMENT_SIZE1;
    v.ELEMENT_SIZE2 = ELEMENT_SIZE2;
    v.Data = malloc(ELEMENT_SIZE1 + ELEMENT_SIZE2);
    memcpy((char*)v.Data,First,ELEMENT_SIZE1);
    memcpy((char*)v.Data + ELEMENT_SIZE1,Second,ELEMENT_SIZE2);
    return v;
}

void* Pair_First(Pair* p) {
    return p->Data;
}

void* Pair_Second(Pair* p) {
    return (char*)p->Data + p->ELEMENT_SIZE1;
}

Pair Pair_NewInt32(int First,size_t ELEMENT_SIZE2,void* Second) {
    Pair v = Pair_New(sizeof(int),&First,ELEMENT_SIZE2,Second);
    return v;
}

Pair Pair_NewCStr(char* First,size_t ELEMENT_SIZE2,void* Second) {
    Pair v = Pair_New(sizeof(char*),&First,ELEMENT_SIZE2,Second);
    return v;
}

void Pair_Free(Pair* p) {
    free(p->Data);
}

#endif