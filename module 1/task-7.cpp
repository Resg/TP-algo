//Дан массив неотрицательных целых 64-битных чисел.
// Количество чисел не больше 10^6.
// Отсортировать массив методом поразрядной сортировки LSD по байтам.
//
#include <iostream>
#include <sstream>
#include <cassert>

size_t max_byte( long long *array, size_t size) {
    size_t max = 0;
    for (int i = 0; i < size; ++i) {
         long long tmp = array[i];
        size_t j = 0;
        while (tmp > 0) {
            tmp = tmp / 256;
            j++;
        }
        if (j > max)
            max = j;
    }
    return max;
}

size_t digit( long long num, size_t index) {
    for (int i = 0 ; i < index - 1; ++i)
        num = num / 256;
    return (int)(num % 256);
}

 long long* LSD( long long *array, size_t amount_of_numbers, size_t num_of_bytes) {
     long long *new_array = new  long long[amount_of_numbers];
    size_t* additional_array = new size_t[256]; //массив-счетчик для сотрировки подсчетом
    for (int i = 0; i < num_of_bytes; ++i) {
        for (int j = 0; j < 256 ; ++j)
            additional_array[j] = 0;
        for (int j = 0; j < amount_of_numbers; ++j) {
            ++additional_array[digit(array[j],i + 1)];
        }
        size_t index = 0;
        size_t temp = 0;
        for (int j = 0; j < 256; ++j) {
            temp = additional_array[j];
            additional_array[j] = index;
            index += temp;
        }
        for (int j = 0; j < amount_of_numbers; ++j) {
            size_t d = digit(array[j],i + 1);
            new_array[additional_array[d]] = array[j];
            ++additional_array[d];
        }
        for (int j = 0; j < amount_of_numbers; j++)
            array[j] = new_array[j];
    }
    delete[] additional_array;
    return new_array;
}

int main()
{
    size_t size;
    std::cin >> size;
    assert(size <= 1000000);
     long long* array = new  long long[size];
    for (int i = 0; i < size; ++i)
        std::cin >> array[i];
     long long* result = LSD(array,size,max_byte(array,size));
    for (int i = 0; i < size; ++i) {
        std::cout << result[i] << ' ';
    }
    delete[] array;
    delete[] result;
    return 0;
}
