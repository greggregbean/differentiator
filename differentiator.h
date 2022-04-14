#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <cmath>

#define TREEBUFLEN 100

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
    char variable;
    double number;
    char operator;
};

struct treeEl
{
    union value;
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
        void textDump();
        void reader(FILE* textTree);
};

void recursiveDump(treeEl* node);
void liner(FILE* fp, char* treeBuf);
double charToDouble(char* treeBuf, int* index)
treeEl* recursiveReader(char* treeBuf, int* index);

#endif