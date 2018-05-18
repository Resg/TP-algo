//Для сложения чисел используется старый компьютер.
// Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
// Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время,
// в зависимости от порядка вычислений.
// ((1+2)+3) -> 1+2 + 3+3 = 9 ((1+3)+2) -> 1+3 + 4+2 = 10 ((2+3)+1) -> 2+3 + 5+1 = 11
// Требуется написать программу, которая определяет минимальное время,
// достаточное для вычисления суммы заданного набора чисел.

#include <iostream>
#include <utility>
#include <cmath>
#include <cassert>
#include <cstring>
#include <functional>

template<typename Type>

class Heap{ //min-куча
private:
    Type *buffer;
    const size_t beg_capacity = 8;
    size_t temp_capacity;
    size_t num_of_elements;
    void resize();
    void siftup(const size_t&);
    void siftdown(const size_t&);
    void init();
public:
    Heap();
    Heap(const Heap&);
    Heap(Heap&&);
    Heap& operator = (const Heap&);
    Heap& operator = (Heap&&);
    void push(const Type&);
    Type pop();
    void Clear();
    bool empty();
    template<class U>
    friend std::ostream& operator << (std::ostream &, const Heap<U> &);
    ~Heap();
};

template <typename Type>

Heap<Type>::Heap() {
    init();
}

template <typename Type>

Heap<Type>::Heap(const Heap &obj) {
    temp_capacity = obj.temp_capacity;
    num_of_elements = obj.num_of_elements;
    buffer = new Type[temp_capacity];
    for (int i = 0; i < num_of_elements; ++i)
        buffer[i] = obj.buffer[i];
}

template<typename Type>

Heap<Type>::Heap(Heap &&obj) {
    temp_capacity = obj.temp_capacity;
    num_of_elements = obj.num_of_elements;
    buffer = obj.buffer;
    obj.buffer = nullptr;
    obj.num_of_elements = 0;
    obj.temp_capacity = beg_capacity;
}

template <typename Type>

Heap<Type>::init() {
    buffer = new Type[beg_capacity];
    temp_capacity = beg_capacity;
    num_of_elements = 0;
}

template<typename Type>

Heap<Type>& Heap<Type>::operator = (const Heap &obj) {
    if (this == &obj)
        return *this;
    Clear();
    temp_capacity = obj.temp_capacity;
    num_of_elements = obj.num_of_elements;
    buffer = new Type[temp_capacity];
    for (int i = 0; i < num_of_elements; ++i)
        buffer[i] = obj.buffer[i];
    return *this;
}

template<typename Type>

Heap<Type> &Heap<Type>::operator = (Heap &&obj) {
    Clear();
    if (this == &obj)
        return *this;
    temp_capacity = obj.temp_capacity;
    num_of_elements = obj.num_of_elements;
    buffer = obj.buffer;
    obj.buffer = nullptr;
    obj.num_of_elements = 0;
    obj.temp_capacity = beg_capacity;
    return *this;
}

template<typename Type>

void Heap<Type>::resize(){ //при переполнении буфера
    Type * new_buffer = new Type[temp_capacity*2];
    for (int i = 0; i < temp_capacity; ++i)
        new_buffer[i] = buffer[i];
    temp_capacity *= 2;
    delete[] buffer;
    buffer = new_buffer;
}

template<typename Type>

void Heap<Type>::siftup(const size_t &index) { //просеивание элемента вверх
    if (num_of_elements == 0 || index <= 0)
        return;
    int i = index;
    while (buffer[i] < buffer[(i - 1) / 2]) {
        std::swap(buffer[i],buffer[(i - 1) / 2]);
        i = (i - 1) / 2;
        if (i == 0) break;
    }
}

template<typename Type>

void Heap<Type>::push(const Type &elem){
    if (!temp_capacity)
        init();
    if (num_of_elements == temp_capacity)
        resize();
    buffer[num_of_elements] = elem;
    if (num_of_elements > 0)
        siftup(num_of_elements);
    ++num_of_elements;
};

template<typename Type>

void Heap<Type>::siftdown(const size_t &index) { //просеивание элемента вниз
    if (num_of_elements <= 1)
        return;
    size_t i = index;
    size_t left, right, smallest;
    while (i < num_of_elements) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < num_of_elements && buffer[left] < buffer[smallest])
            smallest = left;
        if (right < num_of_elements && buffer[right] < buffer[smallest])
            smallest = right;
        if (smallest == i)
            break;
        std::swap(buffer[smallest], buffer[i]);
        i = smallest;
    }
}

template<typename Type>

Type Heap<Type>::pop() {
    if (num_of_elements == 0) return Type();
    Type result = buffer[0];
    buffer[0] = buffer[num_of_elements-1];
    --num_of_elements;
    siftdown(0);
    return result;
}

template<typename Type>

bool Heap<Type>::empty() {
    if (num_of_elements == 0)
        return true;
    return false;
}

template<typename Type>

Heap<Type>::~Heap() {
    Clear();
}

template<typename Type>

void Heap<Type>::Clear() {
    num_of_elements = 0;
    delete[] buffer;
}

template<typename Type>

std::ostream& operator << (std::ostream &os, const Heap<Type> &obj) {
    size_t i = 0;
    while (i < obj.num_of_elements) {
        os << obj.buffer[i] << ' ';
        ++i;
    }
    return os;
}

int main4()
{
    Heap<int> h;
    size_t num_of_operands;
    std::cin >> num_of_operands;
    assert(num_of_operands > 0);
    int operand;
    for(int i = 0; i < num_of_operands; ++i) //добавление всех чисел в кучу
    {
        std::cin >> operand;
        assert(operand <= 1000000000);
        h.push(operand);
    }
    if (num_of_operands == 0) {
        std::cout << 0;
        return 0;
    }
    size_t min_time = 0;
    size_t temp_sum = 0;
    while (true) {
        temp_sum = h.pop() + h.pop(); //вытаскиваем 2 минимальных элемента и складываем их
        assert(temp_sum <= 2000000000);
        min_time += temp_sum; //прибавляем время, нужное для их сложения
        if (h.empty()) //когда все числа просуммированны
        {
            break;
        }
        h.push(temp_sum); //добавляем результат сложения в кучу
    }
    std::cout << min_time;
    return 0;
}
