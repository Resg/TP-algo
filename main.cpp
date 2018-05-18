#include "module 3/task-1/Arc.h"

int main()
{
    CArcGraph g(4, false);
    g.AddEdge(0, 1);
    g.AddEdge(0, 2);
    g.AddEdge(0, 3);
    g.AddEdge(1, 3);
    g.AddEdge(1, 2);
    g.AddEdge(2, 3);
    CArcGraph G = &g;
    G = g;
    std::vector<int> v;
    G.GetNextVertices(2, v);
    for (auto &i : v)
        std::cout << i;
    return 0;
}