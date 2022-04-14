#include "differentiator.h"

tree::tree()
{
    head_ = new treeEl;
    head_ -> value -> number = 0;
    head_ -> left = nullptr;
    head_ -> right = nullptr;
}

void printVal(treeEl* node)
{
    if(node -> type == NUM)
        printf("%lg", node -> value -> number);

    else if(node -> type == VAR)
        printf("%c", node -> value -> variable);

    else
        printf("%c", node -> value -> operator);
}

void recursiveDump(treeEl* node)
{
    printf("(");

    printVal(node);

    if (node -> left != nullptr)
        recursiveDump(node -> yes);
    if (node -> right != nullptr)
        recursiveDump(node -> no);

    printf(")");
}

void tree::textDump()
{
    printf("Text dump: ");
    recursiveDump(head_);
    printf("\n");
}

/*treeEl* treeInsert (value val)
{   
    treeEl* newNode = new treeEl{};

    if(type == DOUBLE)

    newNode -> left = nullptr;
    newNode -> right = nullptr;

    return newNode;
}*/

double charToDouble(char* treeBuf, int* index)
{
    while(treeBuf[*index] <= 9)
    {
        (*index)++;
    }

    int afterdot = 0;

    if(treeBuf[*index] == '.')
    {
        (*index)++;

        while(treeBuf[*index] <= 9)
        {
            afterdot--;
            (*index)++;
        }   
    }

    (*index)--;

    double num = 0;

    for(afterdot; (treeBuf[*index] <= 9) || (treeBuf[*index] == '.'); afterdot++)
    {
        if(treeBuf[*index] == '.')
        {
            (*index--);
            afterdot = -1;
            continue;
        }

        num += (treeBuf[*index] - '0') * pow(10, afterdot);
        (*index)--;
    }

    while((treeBuf[(*index) + 1] <= 9) || (treeBuf[(*index) + 1] == '.'))
    {
        (*index)++;
    }

    return num;
}

treeEl* recursiveReader (char* treeBuf, int* index)
{   
    //Пропускаем '('
    assert(treeBuf[*index] == '(');
    (*index)++;

    value newEl = new value{};
    int type;

    //Проверяем встрети ли мы переменную или число
    if(treeBuf[*index] >= 10)
    {
        if(treeBuf[*index] == MUl)
        {
            newEl -> operator = MUL;
            type = MUL;
        }

        else if(treeBuf[*index] == ADD)
        {
            newEl -> operator = ADD;
            type = ADD;
        }

        else if(treeBuf[*index] == DED)
        {
            newEl -> operator = DED;
            type = DED;
        }

        else if(treeBuf[*index] == SUB)
        {
            newEl -> operator = SUB;
            type = SUB;
        }

        else
        {
            newEl -> variable = treeBuf[*index];
            type = VAR;
        }
    }

    else
    {
        newEl -> number = charToDouble(treeBuf, index);
        type = NUM;
    }
    
    //Cоздаем новый узел
    treeEl* node = treeInsert();

    //Если есть '{' рекурсивно вызываем функцию для поддерева yes
    if(treeBuf[(*index)] == '{')
    {
        node -> yes = recursiveReader(treeBuf, index); 
        node -> no = recursiveReader(treeBuf, index);
    }

    nodeDump(node);
    assert(treeBuf[(*index)] == '}');

    (*index)++;

    return node;
}

void liner(FILE* fp, char* treeBuf)
{
    char symbol = getc(fp);

    int i = 0;

    while(symbol != EOF)
    {
        if(symbol)
        if((symbol != '\n') && (symbol != ' ') && (symbol != '\r') && (symbol != '\t'))
        {
            treeBuf[i] = symbol; 
            i++;
        }

        symbol = getc(fp);
    }
}

void tree::reader(FILE* textTree)
{
    printf("READER: \n");

    //Закинули всё в буффер
    char* treeBuf = (char*) calloc(TREEBUFLEN, sizeof(char)); 
    liner(textTree, treeBuf);

    //Распечатали буффер
    printf("Data in buffer: ");
    for(int i = 0; treeBuf[i] != '\0'; i++)
    {
        printf("%c", treeBuf[i]);
    }
    printf("\n");

    //Запустили рекурсивную читалку буффера
    int index = 0;
    head_ = recursiveReader(treeBuf, &index);
    printf("\n");
}
