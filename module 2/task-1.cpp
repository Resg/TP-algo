//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы
// с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
//Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
//g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

#include <iostream>
#include <cassert>

bool* init_bool_array(const size_t &size);

class StringHashTable {
private:
    const size_t beg_size = 8;
    const size_t hash_cff = 5;
    std::string *str_array;
    bool *is_deleted;
    size_t size;
    size_t allocated;
    size_t hash(const std::string &value);
    void rehash();
    float load_factor();
public:
    StringHashTable();
    StringHashTable(const StringHashTable &);
    StringHashTable(StringHashTable &&) noexcept ;
    StringHashTable& operator = (const StringHashTable &);
    StringHashTable& operator = (StringHashTable &&) noexcept ;
    bool Add(const std::string &);
    bool Remove(const std::string &);
    bool Has(const std::string &);
    ~StringHashTable();
};

StringHashTable::StringHashTable() {
    str_array = new std::string[beg_size];
    is_deleted = init_bool_array(beg_size);
    allocated = beg_size;
    size = 0;
}

StringHashTable::StringHashTable(const StringHashTable &other) {
    size = other.size;
    allocated = other.allocated;
    str_array = new std::string[allocated];
    for (int i = 0; i < allocated; ++i)
        str_array[i] = other.str_array[i];
    is_deleted = init_bool_array(allocated);
    for (int i = 0; i < allocated; ++i)
        is_deleted[i] = other.is_deleted[i];
}

StringHashTable::StringHashTable(StringHashTable &&other) noexcept {
    size = other.size;
    allocated = other.allocated;
    str_array = other.str_array;
    is_deleted = other.is_deleted;
    other.str_array = nullptr;
    other.is_deleted = nullptr;
    other.size = 0;
    other.allocated = 0;
}

StringHashTable& StringHashTable::operator=(const StringHashTable &other) {
    size = other.size;
    allocated = other.allocated;
    str_array = new std::string[allocated];
    for (int i = 0; i < allocated; ++i)
        str_array[i] = other.str_array[i];
    is_deleted = init_bool_array(allocated);
    for (int i = 0; i < allocated; ++i)
        is_deleted[i] = other.is_deleted[i];
}

StringHashTable& StringHashTable::operator=(StringHashTable &&other) noexcept{
    *this = other;
    other.str_array = nullptr;
    other.is_deleted = nullptr;
    other.size = 0;
    other.allocated = 0;
}

bool StringHashTable::Add(const std::string &value) {
    if (Has(value))
        return false;
    size_t str_hash = hash(value);
    size_t shift = 1;
    while (!str_array[str_hash].empty()) {
        str_hash += shift + 1;
        str_hash %= allocated;
        ++shift;
    }
    str_array[str_hash] = value;
    is_deleted[str_hash] = false;
    ++size;
    if (load_factor() >= 0.75)
        rehash();
    return true;
}

bool StringHashTable::Has(const std::string &value) {
    size_t str_hash = hash(value);
    size_t shift = 1;
    while (str_array[str_hash] != value) {
        if (str_array[str_hash].empty() && !is_deleted[str_hash])
            return false;
        str_hash += shift + 1;
        str_hash %= allocated;
        ++shift;
    }
    return true;
}

bool StringHashTable::Remove(const std::string &value) {
    size_t str_hash = hash(value);
    size_t shift = 1;
    while (str_array[str_hash] != value) {
        if (str_array[str_hash].empty() && !is_deleted[str_hash])
            return false;
        str_hash += shift + 1;
        str_hash %= allocated;
        ++shift;
    }
    str_array[str_hash].clear();
    is_deleted[str_hash] = true;
    return true;
}

void StringHashTable::rehash() {
    std::string *old_array = str_array;
    str_array = new std::string[2 * allocated];
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

float StringHashTable::load_factor() {
    return (float) size / allocated;
}

size_t StringHashTable::hash(const std::string &value) {
    const size_t n = value.length();
    if (!n)
        return 0;
    size_t result = 0;
    size_t i = n - 1;
    while (i > 0) {
        result += (size_t)value[i];
        result *= hash_cff;
        result = result % allocated;
        --i;
    }
    result += (size_t)value[0];
    result = result % allocated;
    return result;
}

StringHashTable::~StringHashTable() {
    delete[] str_array;
    delete[] is_deleted;
}

bool* init_bool_array(const size_t &size) {
    assert(size > 0);
    bool *array = new bool[size];
    for (int i = 0 ; i < size ; ++i)
        array[i] = false;
    return array;
}

int main()
{
    StringHashTable table;
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