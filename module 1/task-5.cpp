//На числовой прямой окрасили N отрезков.
// Известны координаты левого и правого концов каждого отрезка (Li и Ri).
// Найти длину окрашенной части числовой прямой.
//В первой строке записано количество отрезков.
//В каждой последующей строке через пробел записаны координаты левого и правого концов отрезка.

#include <iostream>
#include <cassert>

struct Point {
    int coordinate;
    bool IsOpening;

    friend bool operator < (const Point &obj1, const Point &obj2) {
        return obj1.coordinate < obj2.coordinate;
    }
    friend bool operator > (const Point &obj1, const Point &obj2) {
        return obj1.coordinate > obj2.coordinate;
    }
    friend bool operator <= (const Point &obj1, const Point &obj2) {
        return obj1.coordinate <= obj2.coordinate;
    }
    friend bool operator >= (const Point &obj1, const Point &obj2) {
        return obj1.coordinate >= obj2.coordinate;
    }
    friend bool operator == (const Point &obj1, const Point &obj2) {
        return obj1.coordinate == obj2.coordinate;
    }
    friend bool operator != (const Point &obj1, const Point &obj2) {
        return obj1.coordinate != obj2.coordinate;
    }
};

template<class T>
class IsLess
{
public:
    bool operator()(const T &op1, const T &op2) {
        return op1 < op2;
    }
};

template<class T, class functor>
void merge(T *array,size_t beg_i1, size_t mid, size_t end_i2, functor &func) { //[beg_i1; mid) и [mid, end_i2)
    size_t left_index = beg_i1;
    size_t right_index = mid;
    size_t size = end_i2 - beg_i1;
    T *result = new T[size];
    size_t index = 0;
    while (left_index < mid && right_index < end_i2) {
        if (func(array[left_index],array[right_index]) || array[right_index] == array[left_index]) {
            result[index] = array[left_index];
            ++left_index;
        }
        else if (func(array[right_index],array[left_index])) {
            result[index] = array[right_index];
            ++right_index;
        }
        ++index;
    }
    if (left_index != mid) {
        while (left_index < mid) {
            result[index] = array[left_index];
            ++index;
            ++left_index;
        }
    }
    else if (right_index != end_i2) {
        while (right_index < end_i2) {
            result[index] = array[right_index];
            ++index;
            ++right_index;
        }
    }
    index = 0;
    for (int i = beg_i1; i < end_i2; ++i) {
        array[i] = result[index];
        ++index;
    }
    delete[] result;
}

template<class T, class functor>
void MergeSort(T *array, size_t n, functor func) {
    size_t mid;
    size_t shift = 1;
    for (size_t i = 1; i < n; i *= 2) {
        for (size_t j = 0; j <= n - 1; j += 2 * shift) {
            size_t beg = j;
            size_t mid; //начало второго массива
            size_t end; //конец второго массива
            if (j + shift > n - 1)
                mid = n;
            else
                mid = j + shift;
            if (j + 2 * shift > n)
                end = n;
            else
                end = j + 2 * shift;
            merge(array, beg, mid, end, func);
        }
        shift *= 2;
    }
}

int main()
{
    size_t amount_of_lines;
    std::cin >> amount_of_lines;
    assert(amount_of_lines <= 10000);
    Point *array = new Point[2*amount_of_lines];
    for (int i = 0; i < 2*amount_of_lines; ++i) {
        std::cin >> array[i].coordinate;
        assert(array[i].coordinate <= 1000000000);
        if (i % 2 == 0)
            array[i].IsOpening = true;
        else
            array[i].IsOpening = false;
    }
    MergeSort(array,2*amount_of_lines,IsLess<Point>());
    size_t counter = 0;
    size_t summ = 0;
    for (int i = 0; i < 2*amount_of_lines; ++i) {
        if (counter > 0)
            summ += (array[i].coordinate - array[i - 1].coordinate);
        if (array[i].IsOpening)
            ++counter;
        else
            --counter;
    }
    std::cout << summ;
    delete[] array;
}