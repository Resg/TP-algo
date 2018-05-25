#include "task-1/Arc.h"
#include <ctime>
#include <iostream>

int main()
{
    unsigned int start_time =  clock();
    CArcGraph G(10000, false);
    for (int i = 0; i < 10000; ++i) {
        int from = rand() % 10000;
        int to = rand() % 10000;
        G.AddEdge(from, to);
        std::cout << from << ' ' << to << ' ' << i << std::endl;
    }
    unsigned int stop_time =  clock();
    std::cout << (stop_time - start_time) / 1000;
    return 0;
}