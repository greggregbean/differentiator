#include "differentiator.h"

int main()
{
    tree tr;
    FILE* equation = fopen("equation.txt", "r");
    tr.reader(equation);
    tr.textDump();
    FILE* graph = fopen("graph.dot", "w");
    tr.graphDump(graph);
}