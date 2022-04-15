#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <cmath>

enum types
{
    NUM = 1,
    VAR = 2,
    MUL = 42,
    ADD = 43,
    DED = 45,
    SUB = 47
};

union value
{
    char varOrOper;
    double number;
};

struct treeEl
{
    value val;
    int type;
    treeEl* left;
    treeEl* right;
};

class tree
{
    private:
        treeEl* head_;

    public:
        tree();
        void reader(FILE* filep);
};

void spaceSkip(FILE* filep);
treeEl* treeInsert(int type, value val);
void recursiveReader(FILE* filep);

#endif