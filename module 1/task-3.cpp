//Сделать очередь с динмаическим циклическим буфером

#include <iostream>
#include <cstring>
#include <cassert>

class Queue
{
private:
    const size_t beg_capacity = 8; //начальная емкость очереди
    int *buffer;
    size_t head_index;
    size_t tail_index;
    size_t temp_capacity;
    size_t num_of_elements;
    void resize();
public:
    Queue();
    Queue(const Queue&);
    Queue(Queue&&) noexcept;
    Queue& operator = (const Queue&);
    Queue& operator = (Queue&&) noexcept;
    friend std::ostream& operator << (std::ostream&, const Queue&);
    void push_back(const int&);
    int pop_front();
    size_t size();
    ~Queue();
};

Queue::Queue() {
    num_of_elements = 0;
    temp_capacity = beg_capacity;
    buffer = new int[temp_capacity];
    head_index = 0;
    tail_index =0;
}

Queue::Queue(const Queue &other) {
    num_of_elements = other.num_of_elements;
    temp_capacity = other.temp_capacity;
    head_index = other.head_index;
    tail_index = other.tail_index;
    buffer = new int[temp_capacity];
    for (int i = head_index % temp_capacity; i <= tail_index % temp_capacity; ++i)
        buffer[i] = other.buffer[i];
}

Queue::Queue(Queue &&other) noexcept{
    num_of_elements = other.num_of_elements;
    temp_capacity = other.temp_capacity;
    head_index = other.head_index;
    tail_index = other.tail_index;
    buffer = other.buffer;
    other.buffer = nullptr;
    other.tail_index = 0;
    other.head_index = 0;
    other.temp_capacity = 0;
    other.num_of_elements = 0;
}

Queue& Queue::operator=(const Queue &other) {
    num_of_elements = other.num_of_elements;
    temp_capacity = other.temp_capacity;
    head_index = other.head_index;
    tail_index = other.tail_index;
    buffer = new int[temp_capacity];
    for (int i = head_index % temp_capacity; i <= tail_index % temp_capacity; ++i)
        buffer[i] = other.buffer[i];
    return *this;
}

Queue& Queue::operator=(Queue &&other) noexcept{
    num_of_elements = other.num_of_elements;
    temp_capacity = other.temp_capacity;
    head_index = other.head_index;
    tail_index = other.tail_index;
    buffer = other.buffer;
    other.buffer = nullptr;
    other.tail_index = 0;
    other.head_index = 0;
    other.temp_capacity = 0;
    other.num_of_elements = 0;
    return *this;
}

void Queue::push_back(const int &elem) {
    if (num_of_elements == 0) { //при добавлении первого элемента head_index равен tail_index
        buffer[head_index % temp_capacity] = elem;
        ++num_of_elements;
    }
    else {
        if (num_of_elements < temp_capacity) {  // если не ожидается переполнение
            ++tail_index;
            buffer[tail_index % temp_capacity] = elem;
            ++num_of_elements;
        }
        else {
            resize();
            buffer[tail_index % temp_capacity] = elem; //вставка нового элемента во вторую половину нового буфера
            ++num_of_elements;
        }
    }
}

int Queue::pop_front() {
    if (num_of_elements == 0)
        return -1;
    else {
        int result = buffer[head_index % temp_capacity];
        ++head_index;
        --num_of_elements;
        if (num_of_elements == 0) { // при полной очистке очереди вернуть индексы в первоначальное состояние
            tail_index = 0;
            head_index = 0;
        }
        return result;
    }
}

std::ostream& operator << (std::ostream& os, const Queue& obj) {
    if (obj.num_of_elements == 0) return os;
    else if (obj.num_of_elements == 1) os << obj.buffer[obj.head_index % obj.temp_capacity];
    else {
        if (obj.tail_index % obj.temp_capacity > obj.head_index % obj.temp_capacity) {
            for (int i = obj.head_index % obj.temp_capacity; i <= obj.tail_index % obj.temp_capacity; ++i)
                os << obj.buffer[i] << ' ';
        }
        else {
            for (int i = obj.head_index % obj.temp_capacity; i < obj.temp_capacity; ++i)
                os << obj.buffer[i] << ' ';
            for (int i = 0; i <= obj.tail_index % obj.temp_capacity; ++i)
                os << obj.buffer[i] << ' ';
        }
    }
    return os;
}

void Queue::resize() {
    int * new_buffer = new int[temp_capacity * 2]; // выделяется память, в 2 раза большая, чем до переполнения буфера
    int j =0;
    for (int i = head_index; i <= tail_index; ++i) { // перенос элементов в правильном порядке
        new_buffer[j] = buffer[i % temp_capacity];
        ++j;
    }
    delete []buffer;
    buffer = new_buffer;
    head_index = 0;
    tail_index = temp_capacity;
    temp_capacity *= 2;
}

size_t Queue::size() {
    return num_of_elements;
}

Queue::~Queue() {
    if (num_of_elements > 0)
        delete []buffer;
}


int main()
{
    Queue q;
    size_t num_of_commands;
    std::cin >> num_of_commands;
    assert(num_of_commands <= 1000000);
    size_t type, arg;
    bool answer = true;
    for (int i = 0; i < num_of_commands; ++i) {
        std::cin >> type;
        std::cin >> arg;
        if (type == 3)
            q.push_back(arg);
        else if (type == 2) {
            if (q.pop_front() != arg)
                answer = false;
        }
        else
            answer = false;
    }
    if (answer)
        std::cout << "YES";
    else
        std::cout << "NO";
    return 0;
}
