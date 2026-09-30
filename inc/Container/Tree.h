#ifndef TREE_H
#define TREE_H

#include "Vector.h"

#define LAYER_ROOT          0
#define LAYER(n)            (n)

#define BRANCH_DONTCARE     NULL

typedef struct Branch {
    unsigned int ELEMENT_SIZE;
    void* Memory;
    struct Branch* Parent;
    struct Vector Childs;
} Branch;

Branch* Branch_New(void* Item,unsigned int ELEMENT_SIZE) {
    Branch* n = (Branch*)malloc(sizeof(Branch));
    n->ELEMENT_SIZE = ELEMENT_SIZE;
    n->Memory = malloc(ELEMENT_SIZE);
    memcpy(n->Memory,Item,ELEMENT_SIZE);
    n->Childs = Vector_New(sizeof(Branch*));
    n->Parent = NULL;
    return n;
}
Branch* Branch_Zero() {
    Branch* n = (Branch*)malloc(sizeof(Branch));
    n->ELEMENT_SIZE = 0U;
    n->Memory = NULL;
    n->Childs = Vector_New(sizeof(Branch*));
    n->Parent = NULL;
    return n;
}
Branch* Branch_Null() {
    return NULL;
}
void Branch_Set(Branch* b,void* Item,unsigned int ELEMENT_SIZE) {
    if(b->Memory) free(b->Memory);
    b->ELEMENT_SIZE = ELEMENT_SIZE;
    b->Memory = malloc(ELEMENT_SIZE);
    memcpy(b->Memory, Item, ELEMENT_SIZE);
}
void Branch_Add(Branch* n,Branch* c) {
    c->Parent = n;
    Vector_Push(&n->Childs,&c);
}
void Branch_Free(Branch* n) {
    Vector_Free(&n->Childs);
    n->ELEMENT_SIZE = 0U;
    n->Parent = NULL;
    
    if(n && n->Memory)  free(n->Memory);
    n->Memory = NULL;
    if(n)               free(n);
}
void Branch_Delete(Branch* n) {
    if(!n) return;
	
    for(int i = 0;i<n->Childs.size;i++){
        Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
        Branch_Delete(c);
    }
    Branch_Free(n);
}
void Branch_Clear(Branch* n) {
    for(int i = 0;i<n->Childs.size;i++){
        Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
        Branch_Delete(c);
    }
    Vector_Clear(&n->Childs);
}
void Branch_ForEach(Branch* n,void (*Func)(void* Element)) {
    if(!n) return;
    
    for(int i = 0;i<n->Childs.size;i++){
        Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
        Func(c->Memory);
        Branch_ForEach(c,Func);
    }
}
void Branch_Print(Branch* n,int Indent){
    printf("|");
    for(int i = 0;i<Indent;i++) printf("--");
    
    if(n->Parent==NULL)         printf("Root  \n");
    else if(n->Childs.size==0)  printf("Leaf  \n");
    else                        printf("Branch\n");

    for(int i = 0;i<n->Childs.size;i++){
        Branch* c = *(Branch**)Vector_Get(&n->Childs,i);
        Branch_Print(c,Indent+1);
    }
}


typedef struct Tree {
    Branch* Root;
} Tree;

Tree Tree_New() {
    Tree v;
    v.Root = Branch_Zero();
    return v;
}
Tree Tree_Null() {
    Tree v;
    v.Root = Branch_Null();
    return v;
}
Branch* Tree_Get_Branch(Tree* v,int Layers,int* Indices) {
    Branch* Before = v->Root;
    for(int i = 0;i<Layers;i++){
        if(Indices[i]>=Before->Childs.size) return NULL;

        Branch* b = *(Branch**)Vector_Get(&Before->Childs,Indices[i]);
        Before = b;
    }

    return Before;
}
void Tree_SetC_Branch(Tree* v,int Layers,int* Indices,void* Item,unsigned int ELEMENT_SIZE,Branch** b) {
    Branch* Before = v->Root;
    
    for(int i = 0;i<Layers;i++){
        Branch* const bh = *(Branch**)Vector_Get(&Before->Childs,Indices[i]);
        Before = bh;
    }

    Branch_Set(Before,Item,ELEMENT_SIZE);
    Branch_Clear(Before);
    
    for(int i = 0;b[i] != NULL;i++){
        Vector_Push(&Before->Childs,b + i);
    }
}

void* Tree_Get(Tree* v,int Layers,int* Indices) {
    Branch* b = Tree_Get_Branch(v,Layers,Indices);
    if(b) return b->Memory;
    return NULL;
}
void Tree_Set(Tree* v,int Layers,int* Indices,void* Item,unsigned int ELEMENT_SIZE) {
    Branch* Before = v->Root;
    for(int i = 0;i<Layers;i++){
        Branch* b = *(Branch**)Vector_Get(&Before->Childs,Indices[i]);
        Before = b;
    }

    Branch_Set(Before,Item,ELEMENT_SIZE);
}
void Tree_Add(Tree* v,int Layers,int* Indices,void* Item,unsigned int ELEMENT_SIZE) {
    Branch* Before = v->Root;
    for(int i = 0;i<Layers;i++){
        Branch* b = *(Branch**)Vector_Get(&Before->Childs,Indices[i]);
        Before = b;
    }

    Branch* NewBranch = Branch_New(Item,ELEMENT_SIZE);
    Branch_Add(Before,NewBranch);
}
void Tree_Delete(Tree* v,int Layers,int* Indices) {// 2 - {1,1}
    Branch* Before = v->Root;
    for(int i = 0;i<Layers;i++){
        Branch* b = *(Branch**)Vector_Get(&Before->Childs,Indices[i]);
        Before = b;
    }

    Branch* Parent = Before->Parent;
    Vector_Remove(&Parent->Childs,Indices[Layers-1]);
    Branch_Delete(Before);
}
void Tree_ForEach(Tree* v,void (*Func)(void* Element)) {
    Branch_ForEach(v->Root,Func);
}
void Tree_Clear(Tree* v) {
    Branch_Delete(v->Root);
    v->Root = Branch_Zero();
}
void Tree_Print(Tree* v) {
    printf("Tree: --------\n");
    Branch_Print(v->Root,0);
    printf("--------------\n");
}
void Tree_Free(Tree* v) {
    Branch_Delete(v->Root);
    v->Root = NULL;
    //printf("Freed Hole Tree.\n");
}

#endif // !TREE_H!T`