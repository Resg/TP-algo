//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы
// с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
//Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
//g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

#include <iostream>
#include <cassert>
#include <algorithm>

template <class T>

struct default_hash {
    size_t  operator() (const T &value) {
        std::hash<T> hash_fn;
        return hash_fn(value);
    }
};

template <class T, class Hash = default_hash<T>>

class HashTable {
private:
    const size_t beg_size = 8;
    T *array;
    bool *is_deleted;
    size_t size;
    size_t allocated;
    Hash hash_fn;
    void rehash();
    float load_factor();
    bool* init_bool_array(const size_t &size);
    void init();
public:
    HashTable();
    HashTable(const HashTable &);
    HashTable(HashTable &&) noexcept ;
    HashTable& operator = (const HashTable &);
    HashTable& operator = (HashTable &&) noexcept ;
    bool Add(const std::string &);
    bool Remove(const std::string &);
    bool Has(const std::string &);
    void Clear();
    ~HashTable();
};

template <class T, class Hash>

HashTable<T, Hash>::HashTable() {
    init();
}

template <class T, class Hash>

HashTable<T, Hash>::HashTable(const HashTable &other) {
    size = other.size;
    allocated = other.allocated;
    array = new std::string[allocated];
    for (int i = 0; i < allocated; ++i)
        array[i] = other.array[i];
    is_deleted = init_bool_array(allocated);
    for (int i = 0; i < allocated; ++i)
        is_deleted[i] = other.is_deleted[i];
}

template <class T, class Hash>

HashTable<T, Hash>::HashTable(HashTable &&other) noexcept {
    size = other.size;
    allocated = other.allocated;
    array = other.array;
    is_deleted = other.is_deleted;
    other.array = nullptr;
    other.is_deleted = nullptr;
    other.size = 0;
    other.allocated = 0;
}

template <class T, class Hash>

void HashTable<T, Hash>::init() {
    array = new std::string[beg_size];
    is_deleted = init_bool_array(beg_size);
    allocated = beg_size;
    size = 0;
}

template <class T, class Hash>

HashTable<T, Hash>& HashTable<T, Hash>::operator=(const HashTable &other) {
    auto table = HashTable<T, Hash>(other);
    *this = table;
    return *this;
}

template <class T, class Hash>

HashTable<T, Hash>& HashTable<T, Hash>::operator=(HashTable<T, Hash> &&other) noexcept{
    auto table = HashTable<T, Hash>(std::move(other));
    *this = table;
    return *this;
}

template <class T, class Hash>

bool HashTable<T, Hash>::Add(const std::string &value) {
    if (!allocated)
        init();
    if (Has(value))
        return false;
    size_t hash = hash_fn(value) % allocated;
    size_t shift = 1;
    while (!array[hash].empty()) {
        hash += shift + 1;
        hash %= allocated;
        ++shift;
    }
    array[hash] = value;
    is_deleted[hash] = false;
    ++size;
    if (load_factor() >= 0.75)
        rehash();
    return true;
}

template <class T, class Hash>

bool HashTable<T, Hash>::Has(const std::string &value) {
    if (!allocated)
        init();
    size_t hash = hash_fn(value) % allocated;
    size_t shift = 1;
    while (array[hash] != value) {
        if (array[hash].empty() && !is_deleted[hash])
            return false;
        hash += shift + 1;
        hash %= allocated;
        ++shift;
    }
    return true;
}

template <class T, class Hash>

bool HashTable<T, Hash>::Remove(const std::string &value) {
    if (!allocated)
        init();
    size_t hash = hash_fn(value) % allocated;
    size_t shift = 1;
    while (array[hash] != value) {
        if (array[hash].empty() && !is_deleted[hash])
            return false;
        hash += shift + 1;
        hash %= allocated;
        ++shift;
    }
    array[hash].clear();
    is_deleted[hash] = true;
    return true;
}

template <class T, class Hash>

void HashTable<T, Hash>::rehash() {
    std::string *old_array = array;
    array = new std::string[2 * allocated];
    delete[] is_deleted;
    is_deleted = init_bool_array(2 * allocated);
    allocated *= 2;
    size = 0;
    for (int i =0; i < allocated / 2; ++i) {
        if (!old_array[i].empty()) {
            Add(old_array[i]);
            old_array[i].clear();
        }
    }
    delete[] old_array;
}

template <class T, class Hash>

float HashTable<T, Hash>::load_factor() {
    return (float) size / allocated;
}
struct string_hash {
public:
    string_hash() {
    }
    size_t operator()(const std::string &value) {
        const size_t hash_cff = 5;
        const size_t n = value.length();
        if (!n)
            return 0;
        size_t result = 0;
        size_t i = n - 1;
        while (i > 0) {
            result += (size_t) value[i];
            result *= hash_cff;
            --i;
        }
        result += (size_t) value[0];
        return result;
    }
};

template <class T, class Hash>

void HashTable<T, Hash>::Clear() {
    size = 0;
    allocated = 0;
    delete[] array;
    delete[] is_deleted;
}

template <class T, class Hash>

HashTable<T, Hash>::~HashTable() {
    Clear();
}

template <class T, class Hash>

bool* HashTable<T, Hash>::init_bool_array(const size_t &size) {
    assert(size > 0);
    bool *array = new bool[size];
    for (int i = 0 ; i < size ; ++i)
        array[i] = false;
    return array;
}

int main()
{
    HashTable<std::string, string_hash> table;
    char cmd;
    std::string arg;
    while (std::cin >> cmd >> arg) {
        if (cmd == '+') {
            std::cout << ( table.Add(arg) ? "OK" : "FAIL" ) << std::endl;
        }
        else if (cmd == '-') {
            std::cout << ( table.Remove(arg) ? "OK" : "FAIL" ) << std::endl;
        }
        else if (cmd == '?') {
            std::cout << ( table.Has(arg) ? "OK" : "FAIL" ) << std::endl;
        }
        else assert(false);
    }
    return 0;
}