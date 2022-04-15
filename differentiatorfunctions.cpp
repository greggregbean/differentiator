#include "differentiator.h"

tree::tree()
{
    head_ = new treeEl{};
    head_ -> left = nullptr;
    head_ -> right = nullptr;
}

void printVal(treeEl* node)
{
    if(node -> type == NUM)
        printf("%lg", node -> val.number);

    else
        printf("%c", node -> val.varOrOper);
}

/*void recursiveDump(treeEl* node)
{
    printf("(");

    printVal(node);

    if (node -> left != nullptr)
        recursiveDump(node -> left);
    if (node -> right != nullptr)
        recursiveDump(node -> right);

    printf(")");
}*/

/*void tree::textDump()
{
    printf("Text dump: ");
    recursiveDump(head_);
    printf("\n");
}*/


void spaceSkip(FILE* filep)
{
    char symbol;
    while(((symbol = getc(filep)) == ' ') || (symbol == '\t')) ;
    ungetc(symbol, filep);
}

void nodeDump(treeEl* node)
{
    printf("Addr: %p, Left: %p, Right: %p, Type: %c, Value: ", (void*)node, (void*)node -> left, (void*)node -> right, node -> type);
    printVal(node);
}

treeEl* treeInsert(int type, value val_)
{
    treeEl* node = new treeEl{};

    node -> left  = nullptr;
    node -> right = nullptr;

    if (type == NUM)
    {
        node -> type = NUM;
        (node -> val).number = val_.number;
    }

    if (type == VAR)
    {
        node -> type = VAR;
        (node -> val).varOrOper = val_.varOrOper;
    }

    if (type == MUL)
    {
        node -> type = MUL;
        (node -> val).varOrOper = MUL;
    }

    if (type == ADD)
    {
        node -> type = ADD;
        (node -> val).varOrOper = ADD;
    }

    if (type == DED)
    {
        node -> type = DED;
        (node -> val).varOrOper = DED;
    }

    if (type == SUB)
    {
        node -> type = SUB;
        (node -> val).varOrOper = SUB;
    }

    return node;
}

void recursiveReader (FILE* filep)
{   
    //Пропускаем '('
    assert(getc(filep) == '(');

    spaceSkip(filep);
    
    char symbol = getc(filep);
    printf("Сейчас мы на: %c. \n", symbol);

    int type = 0;
    value val = {};

    if((symbol <= '9') && (symbol >= '0'))
    {
        ungetc(symbol, filep);

        printf("Это число. \n");

        double num;
        fscanf(filep,"%lg", &num);

        printf("Оно равно %lg", num);

        type = NUM;
        val.number = num;
    }

    if(symbol == MUL)
    {
        printf("Это оператор умножения. \n");
        type = MUL;
    }

    if(symbol == ADD)
    {
        printf("Это оператор сложения. \n");
        type = ADD;
    }

    if(symbol == DED)
    {
        printf("Это оператор вычитания. \n");
        type = DED;
    }

    if(symbol == SUB)
    {
        printf("Это оператор деления. \n");
        type = SUB;
    }

    else
    {
        printf("Это переменная. \n");
        type = VAR;
        val.varOrOper = symbol;
    }

    treeEl* node = treeInsert(type, val);
    nodeDump(node);

}



void tree::reader(FILE* filep)
{
    printf("READER: \n");

    //Запустили рекурсивную читалку буффера
    recursiveReader(filep);
    printf("\n");
}
