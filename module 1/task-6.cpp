//Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..10^9] размера n.
//Требуется найти k-ю порядковую статистику. т.е. напечатать число,
//которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
//Напишите нерекурсивный алгоритм.
//Требования к дополнительной памяти: O(n).
//Требуемое среднее время работы: O(n).
//Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.

#include <iostream>
#include <utility>
#include <cassert>

int PivotElem(int size) { //генерация случайного элемента, взятого по модулю size
    srand(time(0));
    if (size == 0) size = 1;
    return rand() % size;
}

int partition(int *array, int beg, int end, int pivot) { //функция прохода по части массива [beg, end]
// двух указателей от конца к началу
// основная задача - вставить array[pivot] на нужное место в массиве
    std::swap(array[pivot], array[beg]);
    int i = end, j = end;
    while (j > beg && i > beg) {
        if (array[j] > array[beg]) {
            std::swap(array[j], array[i]);
            --i;
            --j;
        }
        else
            --j;
    }
    std::swap(array[i], array[beg]);
    return i;
}

int QuickSearch(int * array, int size, int index) {
    int beg = 0;
    int end = size - 1;
    while (true) {
        int pivot = beg + PivotElem(end - beg); //выбор опорного элемента
        int limit =  partition(array,beg,end,pivot);
        if (limit == index) {
            return limit; //если индекс, на который в конце partition встал выбранный опорный элемент
            //совпал с искомым индексом, возвращаем ответ
        }
        else if(index < limit) {
            end = limit - 1; //сужаем интервал поиска
        }
        else if(index > limit){
            beg = limit + 1; //----------//-----------
        }
    }
}

int main()
{
    int size;
    int index;
    std::cin >> size >> index;
    int* array = new int[size];
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
        assert(array[i] <= 1000000000);
    }
    std::cout << array[QuickSearch(array,size,index)];
    delete[] array;
    return 0;
}