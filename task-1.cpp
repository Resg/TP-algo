#include <iostream>
#include <climits>

int MaxElem(int* array, int beg_index, int end_index) {
    int max = INT_MIN, index=beg_index;
    for(int i=beg_index;i<end_index;++i){
        if(max<array[i]){
            max = array[i];
            index = i;
        }
    }
    return index;
}

int FirstSum(int *first_array, int *second_array, int size, int *i1, int *j1){
    *i1 = MaxElem(first_array,0,size);
    *j1 = MaxElem(second_array,*i1,size);
    return first_array[*i1] + second_array[*j1];

}

int SecondSum(int *first_array, int *second_array, int size, int *i2, int *j2){
    *j2 = MaxElem(second_array,0,size);
    *i2 = MaxElem(first_array,0,*j2+1);
    return first_array[*i2] + second_array[*j2];
}

int ThirdSum(int *first_array, int *second_array, int size, int *i3, int *j3){
    int sum = INT_MIN;
    int index = 0;
    for(int i =0; i < size; ++i){
        if ((first_array[i] + second_array[i]) > sum){
            index = i;
            sum = first_array[i] + second_array[i];
        }
    }
    *i3 = index;
    *j3 = index;
    return first_array[index] + second_array[index];
}

int CompareSum(int sum1, int sum2, int sum3, int i1, int j1, int i2, int j2, int i3, int j3){
    if (sum1 > sum2 && sum1 > sum3) return 1;
    else if (sum2 > sum1 && sum2 > sum3) return 2;
    else if (sum3 > sum1 && sum3 > sum2) return 3;
    if (sum1 == sum2 && sum1 > sum3){
        if (i1 < i2 || (i1 == i2 && j1 < j2)) return 1;
        else if (i2 < i1 || (i1 == i2 && j2 < j1)) return 2;
        else return 1;
    }
    else if (sum1 == sum3 && sum1 > sum2){
        if (i1 < i3 || (i1 == i3 && j1 < j3)) return 1;
        else if (i3 < i1 || (i1 == i3 && j3 < j1)) return 3;
        else return 1;
    }
    else if (sum2 == sum3 && sum2 > sum1){
        if (i2 < i3 || (i2 == i3 && j2 < j3)) return 2;
        else if (i3 < i2 || (i2 == i3 && j3 < j2)) return 3;
        else return 2;
    }
    if (sum1 == sum2 && sum1 == sum3){
        if (i1 < i2 && i1 < i3) return 1;
        else if (i2 < i3 && i2 < i1) return 2;
        else if (i3 < i2 && i3 < i1) return 3;
        if (i1 == i2 && i1 < i3 && j1 < j2) return 1;
        else if (i1 == i2 && i1 < i3 && j1 > j2) return 2;
        else if (i1 == i3 && i1 < i2 && j1 > j3) return 3;
    }
}

int main1()
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
    int i1,j1,i2,j2,i3,j3;
    int first_sum = FirstSum(first_array,second_array,size_of_array,&i1,&j1);
    int second_sum = SecondSum(first_array,second_array,size_of_array,&i2,&j2);
    int third_sum = ThirdSum(first_array, second_array,size_of_array,&i3,&j3);
    int result = CompareSum(first_sum,second_sum,third_sum,i1,j1,i2,j2,i3,j3);
    if (result == 1) std::cout << i1 << ' ' << j1;
    else if (result == 2) std::cout << i2 << ' ' << j2;
    else std::cout << i3 << ' ' << j3;
    delete []first_array;
    delete []second_array;
    return 0;
}

