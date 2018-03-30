//Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
// Необходимо найти первую пару индексов i0 и j0, , такую что A[i0] + B[j0] = max {A[i] + B[j],
// где 0 <= i < n, 0 <= j < n, i <= j}. Время работы - O(n). n ≤ 100000.

#include <iostream>

std::pair<size_t, size_t> GetIndicies(int *array1, int *array2, size_t size) {
    size_t max1 = 0;
    size_t max2 = 0;
    size_t temp = 0;
    for(int i = 0; i < size; ++i) {
        if (array1[i] > array1[temp])
            temp = i;
        int first_sum = array1[temp] + array2[i];
        int second_sum = array1[max1] + array2[max2];
        if (first_sum > second_sum) {
            max1 = temp;
            max2 = i;
        }
    }
    return std::pair<size_t, size_t>{max1, max2};
};

int main()
{
    int size_of_array;
    std::cin >> size_of_array;
    int *first_array = new int[size_of_array];
    for (int i=0;i<size_of_array;++i){
        std::cin >> first_array[i];
    }
    int *second_array = new int[size_of_array];
    for (int i=0;i<size_of_array;++i){
        std::cin >> second_array[i];
    }
    auto result = GetIndicies(first_array,second_array,size_of_array);
    std::cout << result.first << ' ' << result.second;
    delete []first_array;
    delete []second_array;
    return 0;
}

