//Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A,
// равного или превосходящего B[i]: A[k] >= B[i].
// Если такого элемента нет, выведите n.
// Время работы поиска k для каждого элемента B[i]: O(log(k)). n, m ≤ 10000.
#include <iostream>
#include <cassert>
#include <math.h>

struct IntPair // структура для хранения границ интервала
{
    int num1, num2;
};

IntPair FindInterval(int *array,const int &elem,int beg_index, int end_index){ // найти эффективный интервал для бинарного поиска
// он буфет в виде [beg_index ; 2^i] или [2^(i-1) ; 2^i] или [2^i ; end_index]
    int i = beg_index;
    while (pow(2,i) <= end_index){ //не выйти за пределы массива
        if (array[(int)pow(2,i)] < elem) ++i; //искать пока array[2^i] <= elem
        else break;
    }
    IntPair result;
    if (i == beg_index) result.num1 = beg_index; // если первый элемент массива уже >= передаваемого элемента
    else  result.num1 = (int)pow(2,i-1); // если левая граница правее первого элемента массива
    if (pow(2,i)<=end_index) result.num2 = (int)pow(2,i); //если правая граница левее
    else result.num2 = end_index; // если все элементы массива <= передаваемого элемента
    return result;
}

int FindIndex(int *array, const int &elem, int beg_index, int end_index){ // бинарный поиск на заданном интервале
    // [beg_index ; end_index]
    int middle;
    int index1 = beg_index;
    int index2 = end_index;
    while (index1 < index2){
        middle = (index1 + index2)/2;
        if(array[middle] >= elem) index2 = middle;
        else index1 = middle + 1;
    }
    if (elem > array[index2]) return end_index+1; // в этом случае end_index = n-1. Чтобы вернуть размер массива - n
        //надо прибавить 1
    else return index2;
}

int main2()
{
    int n, m;
    assert(m <= 10000);
    std :: cin >> n >> m;
    int *A = new int[n];
    int *B = new int[m];
    for (int i =0; i< n; ++i){
        std::cin >> A[i];
    }
    for (int i =0; i< m; ++i){
        std::cin >> B[i];
    }
    for (int i = 0; i < m; ++i){
        IntPair interval = FindInterval(A,B[i],0,n-1);
        std::cout << FindIndex(A,B[i],interval.num1,interval.num2) << ' ';
    }
    delete[] A;
    delete[] B;
}

