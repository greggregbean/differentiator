#include "differentiator.h"

//TREE METHODS:
tree::tree()
{
    head_ = new treeEl{};
    head_ -> left = nullptr;
    head_ -> right = nullptr;
}

void tree::textDump()
{
    printf("Text dump: ");
    recursiveDump(head_);
    printf("\n");
}

void tree::graphDump(FILE* filep)
{
    printf("GraphDump: ");

    fprintf(filep, "digraph \n{\n");
    recursiveGraph(filep, head_);
    fprintf(filep, "}");

    printf("your graph in \"graph.dot\". \n");
}

void tree::reader(FILE* filep)
{
    printf("READER: \n");
    head_ = AddDivParse(filep);
    printf("\n");
}

void tree::differentiator()
{
    printf("+++DIFFERENTIATOR+++: \n");
    recursiveDif(head_);
    printf("\n");
}

//DUMP:
void recursiveGraph(FILE* filep, treeEl* node)
{
    fprintf(filep, "%d [shape=record, fillcolor = wheat1, style = filled, label = \" {", node); 
    fprintVal(filep, node);
    fprintf(filep, " | Addr: %p | {Left: %p | Right: %p }}\" ] \n", (void*) node, (void*) node -> left, (void*) node -> right);
    
    if (node -> left != nullptr)
    {
        fprintf(filep, "%d -> %d [label = \" Left \"]; \n", node, node -> left); 
        recursiveGraph(filep, node -> left);
    }

    if (node -> right != nullptr)
    {
        fprintf(filep, "%d -> %d [label = \" Right \"]; \n", node, node -> right); 
        recursiveGraph(filep, node -> right);
    }
}

void recursiveDump(treeEl* node)
{
    printf("(");
    
    if (node -> left != nullptr)
        recursiveDump(node -> left);
    
    printVal(node);

    if (node -> right != nullptr)
        recursiveDump(node -> right);

    printf(")");    
}

void nodeDump(treeEl* node)
{
    printf("Addr: %p, Left: %p, Right: %p, Type: %c, Value: ", (void*)node, (void*)node -> left, (void*)node -> right, node -> type);
    printVal(node);
    printf("\n");
}

void fprintVal(FILE* filep, treeEl* node)
{
    if(node -> type == NUM)
        fprintf(filep, "%lg", (node -> val).number);

    else
        fprintf(filep, "%c", (node -> val).varOrOper);
}

void printVal(treeEl* node)
{
    if(node -> type == NUM)
        printf("%lg", (node -> val).number);

    else
        printf("%c", (node -> val).varOrOper);
}

//READER:
void spaceSkip(FILE* filep)
{
    char symbol;
    while(((symbol = getc(filep)) == ' ') || (symbol == '\t')) ;
    ungetc(symbol, filep);
}

treeEl* AddDivParse(FILE* filep)
{ 
    printf("Зашли в AddDivParse.\n");

    treeEl* addOrDiv = new treeEl{};

    addOrDiv -> left = MulSubPowParse(filep);

    spaceSkip(filep);

    char symbol = getc(filep);
    printf("В AddDiv считали %c. \n", symbol);

    if(symbol == ADD)
    {
        addOrDiv -> type = ADD;
        (addOrDiv -> val).varOrOper = ADD;
        printf("Сделали узел +. \n");
    }

    else if(symbol == DIV)
    {
        addOrDiv -> type = DIV;
        (addOrDiv -> val).varOrOper = DIV;
    }

    else 
    {
        ungetc(symbol, filep);
        printf("Вышли из AddDivParse. \n");
        return addOrDiv -> left;
    }

    addOrDiv -> right = MulSubPowParse(filep);

    nodeDump(addOrDiv);

    printf("Вышли из AddDivParse. \n");

    return addOrDiv;
}

treeEl* MulSubPowParse(FILE* filep)
{
    printf("Зашли в MulSubParse.\n");

    treeEl* mulOrSubOrPow = new treeEl{};

    mulOrSubOrPow -> left = VarNumParse(filep);

    spaceSkip(filep);

    char symbol = getc(filep);
    printf("В MulSub считали %c. \n", symbol);

    if(symbol == MUL)
    {
        mulOrSubOrPow -> type = MUL;
        (mulOrSubOrPow -> val).varOrOper = MUL;
    }

    else if(symbol == SUB)
    {
        mulOrSubOrPow -> type = SUB;
        (mulOrSubOrPow -> val).varOrOper = SUB;
    }

    else if(symbol == POW)
    {
        mulOrSubOrPow -> type = POW;
        (mulOrSubOrPow -> val).varOrOper = POW;    
    }

    else
    {
        ungetc(symbol, filep);
        printf("Вышли из MulSubPowParse. \n");
        return mulOrSubOrPow -> left;
    }

    mulOrSubOrPow -> right = VarNumParse(filep);

    nodeDump(mulOrSubOrPow);

    printf("Вышли из MulSubPowParse. \n");

    return mulOrSubOrPow;
}

treeEl* VarNumParse(FILE* filep)
{
    printf("Зашли в VarNumParse.\n");

    spaceSkip(filep);

    char symbol = getc(filep);
    
    if((symbol <= '9') && (symbol >= '0'))
    {
        ungetc(symbol, filep);

        treeEl* num = new treeEl{};

        num -> type = NUM;
        fscanf(filep, "%lg", &((num -> val).number));
        num -> left  = nullptr;
        num -> right = nullptr;

        symbol = getc(filep);

        nodeDump(num);
        
        printf("Cчитали %lg, следующий символ \"%c\". \n", (num -> val).number, symbol);
        ungetc(symbol, filep); 

        printf("Вышли из VarNumParse. \n");
        
        return num;
    }

    else if ((symbol >= 'a') && (symbol <= 'z'))
    {
        treeEl* var = new treeEl{};

        var -> type = VAR;
        (var -> val).varOrOper = symbol;
        var -> left  = nullptr;
        var -> right = nullptr;

        nodeDump(var);

        printf("Cчитали %c, следующий символ \"%c\". \n", (var -> val).varOrOper, symbol);
        ungetc(symbol, filep);

        printf("Вышли из VarNumParse. \n");

        return var;
    }

    else if (symbol == '(')
    {
        treeEl* equation = AddDivParse(filep);
        if((symbol = getc(filep)) == ')')
        {
            printf("Считали выражение в скобках. \n");
            printf("Вышли из VarNumParse. \n");
            return equation;
        }
    }

    printf("!!! Wrong syntax !!! \n");

    return nullptr;
}

//DIFFERENTIATOR:


