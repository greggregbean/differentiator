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
    recursiveDump(head_, head_);
    printf("\n\n");
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
    head_ = MainParse(filep);
    printf("\n");
}

void tree::differentiator()
{
    printf("+++DIFFERENTIATOR+++  \n");
    head_ = recursiveDif(head_);
    printf("---DIFFERENTIATOR---  \n");
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

void recursiveDump(treeEl* node, treeEl* head)
{
    if((node -> type != NUM) && (node -> type != VAR) && (node != head))
        printf("(");
    
    if (node -> left != nullptr)
        recursiveDump(node -> left, head);
    
    printVal(node);

    if (node -> right != nullptr)
        recursiveDump(node -> right, head);

    if((node -> type != NUM) && (node -> type != VAR) && (node != head))
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

//НАДО ДОРАБОТАТЬ
/*void AddDivDump(treeEl* node)
{
    if(node -> left != nullptr)
        MulSubDump(node -> left);

    printVal(node);

    if(node -> right != nullptr)
        MulSubDump(node -> right);
}

void MulSubDump(treeEl* node)
{
    if(node -> left != nullptr)
        PowDump(node -> left);

    printVal(node);

    if(node -> right != nullptr)
        PowDump(node -> right);
}

void PowDump(treeEl* node)
{
    if(node -> left != nullptr)
        VarNumDump(node -> left);

    printVal(node);

    if(node -> right != nullptr)
        VarNumDump(node -> right);
}

void VarNumDump(treeEl* node)
{
    if ((node -> type == NUM) || (node -> type == VAR))
        printVal(node);

    else
    {
        printf("(");
        AddDivDump(node);
        printf(")");
    }
}*/


//READER:
void spaceSkip(FILE* filep)
{
    char symbol;
    while(((symbol = getc(filep)) == ' ') || (symbol == '\t') || (symbol == '\r') || (symbol == '\n')) ;
    ungetc(symbol, filep);
}

treeEl* MainParse(FILE* filep)
{
    printf("Зашли в MainParse. \n");

    treeEl* mainNode = new treeEl{};

    mainNode = AddDivParse(filep);

    spaceSkip(filep);

    char symbol = getc(filep);
    printf("В MainParse считали %c. \n", symbol);

    if(symbol == '$')
        printf("Уравнение считано верно. \nВышли из MainParse.\n");

    else
        printf("Не обнаружен закрывающий символ '$'. \n");

    return mainNode;
}

treeEl* AddDivParse(FILE* filep)
{ 
    printf("Зашли в AddDivParse.\n");

    treeEl* addOrDiv = new treeEl{};

    addOrDiv -> left = MulSubParse(filep);

    spaceSkip(filep);

    char symbol = getc(filep);
    printf("В AddDivParse считали %c. \n", symbol);

    while(true)
    {
        if(symbol == ADD)
        {
            addOrDiv -> type = ADD;
            (addOrDiv -> val).varOrOper = ADD;
            addOrDiv -> right = AddDivParse(filep);

            nodeDump(addOrDiv);

            printf("Вышли из AddDivParse. \n");
            return addOrDiv;
        }

        else if(symbol == DIV)
        {
            addOrDiv -> type = DIV;
            (addOrDiv -> val).varOrOper = DIV;
            addOrDiv -> right = AddDivParse(filep);

            nodeDump(addOrDiv);

            printf("Вышли из AddDivParse. \n");
            return addOrDiv;
        }

        else
        {
            ungetc(symbol, filep);
            printf("Вышли из AddDivParse. \n");
            return addOrDiv -> left;
        }

        symbol = getc(filep);
        printf("В MulSub считали %c. \n", symbol);
    }

    addOrDiv -> right = MulSubParse(filep);
        
    nodeDump(addOrDiv);

    printf("Вышли из AddDivParse. \n");
    return addOrDiv;
}

treeEl* MulSubParse(FILE* filep)
{
    printf("Зашли в MulSubParse.\n");

    treeEl* mulOrSub = new treeEl{};

    mulOrSub -> left = PowParse(filep);

    spaceSkip(filep);

    char symbol = getc(filep);
    printf("В MulSub считали %c. \n", symbol);

    while (true)
    {
        if(symbol == MUL)
        {
            mulOrSub -> type = MUL;
            (mulOrSub -> val).varOrOper = MUL;
            mulOrSub -> right = MulSubParse(filep);

            nodeDump(mulOrSub);

            printf("Вышли из MulSubParse. \n");
            return mulOrSub;
        }

        else if(symbol == SUB)
        {
            mulOrSub -> type = SUB;
            (mulOrSub -> val).varOrOper = SUB;
            mulOrSub -> right = MulSubParse(filep);

            nodeDump(mulOrSub);

            printf("Вышли из MulSubParse. \n");
            return mulOrSub;
        }

        else
        {
            ungetc(symbol, filep);
            printf("Вышли из MulSubParse. \n");
            return mulOrSub -> left;
        }

        symbol = getc(filep);
        printf("В MulSub считали %c. \n", symbol);
    }

    mulOrSub -> right = PowParse(filep);

    nodeDump(mulOrSub);

    printf("Вышли из MulSubParse. \n");
    return mulOrSub;
}

treeEl* PowParse(FILE* filep)
{
    printf("Зашли в PowParse.\n");

    treeEl* pow = new treeEl{};

    pow -> left = VarNumParse(filep);
    
    spaceSkip(filep);

    char symbol = getc(filep);
    printf("В PowParse считали %c. \n", symbol);

    while(true)
    {
        if(symbol == POW)
        {
            pow -> type = POW;
            (pow -> val).varOrOper = POW;
            pow -> right = PowParse(filep);

            nodeDump(pow);

            printf("Вышли из PowParse. \n");
            return pow;   
        }

        else
        {
            ungetc(symbol, filep);
            printf("Вышли из PowParse. \n");
            return pow -> left;
        }

        symbol = getc(filep);
        printf("В MulSub считали %c. \n", symbol);
    }

    pow -> right = VarNumParse(filep);

    nodeDump(pow);

    printf("Вышли из PowParse. \n");
    return pow;
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

        nodeDump(num);

        symbol = getc(filep);
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

        symbol = getc(filep);
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

    //else:
    ungetc(symbol, filep);
    printf("Вышли из VarNumParse. \n");
    return nullptr;    
}

//DIFFERENTIATOR:
treeEl* newTreeEl(int type, value val, treeEl* left, treeEl* right)
{
    treeEl* node = new treeEl{};

    node -> left = left;
    node -> right = right;

    switch (type)
    {
        case NUM:
        {      
            node -> type = NUM;
            (node -> val).number = val.number;
            break;
        }

        case VAR:
        {
            node -> type = VAR;
            (node -> val).varOrOper = val.varOrOper;
            break;
        }

        case ADD:
        {
            node -> type = ADD;
            (node -> val).varOrOper = ADD;
            break;
        } 

        case DIV:
        {
            node -> type = DIV;
            (node -> val).varOrOper = DIV;
            break;
        }

        case POW:
        {
            node -> type = POW;
            (node -> val).varOrOper= POW;
            break;
        }

        case MUL:
        {
            node -> type = MUL;
            (node -> val).varOrOper = MUL;
            break;
        }

        case SUB:
        {
            node -> type = SUB;
            (node -> val).varOrOper = SUB;
            break;
        }
    }

    return node;
}

treeEl* recursiveDif(treeEl* node)
{
    switch (node -> type)
    {
        case NUM:
        {
            (node -> val).number = 0;
            printf("Продифференцировали число. \n");
            return node;
        }

        case VAR:
        {
            (node -> val).number = 1;
            node -> type = NUM;
            printf("Продифференцировали переменную. \n");
            return node;
        }

        case POW:
        {
            //Проверяем равна ли левая ветка числом:
            if(((node -> right -> val).number == 0) || (node -> left -> type == NUM))
            {
                value zero;
                zero.number = 0;
                treeEl* zeroEl = newTreeEl(NUM, zero, nullptr, nullptr); 
                printf("Возвращаем 0. \n");   
                return zeroEl;
            }

            //Проверяем равна ли степень 1:
            else if((node -> right -> val).number == 1)
                return recursiveDif(node -> left);

            //Если же степень больше 1:
            value val;
            val.varOrOper = MUL;
            treeEl* oldDegree = newTreeEl(NUM, node -> right -> val, nullptr, nullptr);
            treeEl* mulOldDegree = newTreeEl(MUL, val, node, oldDegree);
            ((node -> right -> val).number)--;
            nodeDump(mulOldDegree);
            nodeDump(oldDegree);
            nodeDump(node);
            nodeDump(node -> right);
            nodeDump(node -> left);

            //Проверяем, является ли выражение в степени простой переменной:
            if(node -> left -> type == VAR)
            {
                printf("Возвращаем mulOldDegree. \n");
                return mulOldDegree;
            }

            //Иначе производную скобки домножаем на производную внутреннего выражения: 
            treeEl* mulDif = newTreeEl(MUL, val, mulOldDegree, recursiveDif(node ->left));
            nodeDump(mulDif);
            printf("Возвращаем mulDif. \n");
            return mulDif;
        }

        default:
        {
            printf("!!!Differentiator doesn't know this function!!! \n");
            return nullptr;
        }
    }
}


