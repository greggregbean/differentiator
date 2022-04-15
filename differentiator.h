#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <cmath>

enum types
{
    NUM = 'n',
    VAR = 'v',
    ADD = '+',
    DIV = '-',
    POW = '^',
    MUL = '*',
    SUB = '/'
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
        void textDump();
        void graphDump(FILE* filep);
        void reader(FILE* filep);
        void differentiator();
};

//DUMP:
void recursiveGraph(FILE* filep, treeEl* node);
void recursiveDump(treeEl* node);
void nodeDump(treeEl* node);
void fprintVal(FILE* filep, treeEl* node);
void printVal(treeEl* node);

//READER:
void spaceSkip(FILE* filep);
treeEl* AddDivParse(FILE* filep);
treeEl* MulSubPowParse(FILE* filep);
treeEl* VarNumParse(FILE* filep);

//DIFFERENTIATOR:
treeEl* newTreeEl(int type, value val, treeEl* left, treeEl* right);
treeEl* recursiveDif(treeEl* node);

#endif