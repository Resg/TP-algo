//Требуется отыскать самый выгодный маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
//Требуемое время работы O((N + M)log N), где N – количество городов,
// M – известных дорог между ними.
//N ≤ 10000, M ≤ 250000.
//Длина каждой дороги ≤ 10000.

#include <iostream>
#include <utility>
#include <cmath>
#include <cassert>
#include <cstring>
#include <functional>
#include <vector>
#include <time.h>

template<typename Type>

class Heap{ //min-куча
private:
    Type *buffer;
    size_t deleted;
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
    Heap(Heap&&) noexcept ;
    Heap& operator = (const Heap&);
    Heap& operator = (Heap&&) noexcept ;
    void push(const Type&);
    Type pop();
    template <class Pred>
    void delete_by_key(Type key, Pred &func);
    size_t size();
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

Heap<Type>::Heap(Heap &&obj) noexcept {
    temp_capacity = obj.temp_capacity;
    num_of_elements = obj.num_of_elements;
    buffer = obj.buffer;
    obj.buffer = nullptr;
    obj.num_of_elements = 0;
    obj.temp_capacity = beg_capacity;
}

template <typename Type>

void Heap<Type>::init() {
    buffer = new Type[beg_capacity];
    temp_capacity = beg_capacity;
    num_of_elements = 0;
}

template <typename Type>

size_t Heap<Type>::size() {
    return num_of_elements;
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

Heap<Type> &Heap<Type>::operator = (Heap &&obj) noexcept {
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

template <typename Type>
template <class Pred>

void Heap<Type>::delete_by_key(Type key, Pred &pred) {
    for (int i = 0; i < num_of_elements; ++i) {
        if (pred(buffer[i], key)) {
            buffer[i].first = INT32_MAX;
            buffer[i].second = INT32_MAX;
            std::swap(buffer[num_of_elements - 1], buffer[i]);
            num_of_elements--;
            siftdown(i);
            buffer[num_of_elements].first = 0;
            buffer[num_of_elements].second = 0;
            return;
        }
    }
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
    int i = index;
    int left, right;
    while (2 * i + 1 < num_of_elements) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        int j = left;
        if (right < num_of_elements && buffer[right] < buffer[left])
            j = right;
        if (buffer[i] <= buffer[j])
            break;
        std::swap(buffer[i], buffer[j]);
        i = j;
    }
}

template<typename Type>

Type Heap<Type>::pop() {
    if (num_of_elements == 0) return Type();
    Type result = buffer[0];
    buffer[0] = buffer[num_of_elements - 1];
    buffer[num_of_elements - 1] = Type();
    num_of_elements--;
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


struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to, int w) = 0;

    virtual int VerticesCount() const  = 0;
    virtual bool IsOriented() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
};

class CListGraph : public IGraph {
private:
    struct Node {
        Node* next;
        int vertex;
        int weight;
    };
    Node** list;
    size_t num_of_vertices;
    bool is_oriented;
    void Clear();
public:
    CListGraph() = delete;
    explicit CListGraph(size_t, bool);
    explicit CListGraph(const IGraph*);
    CListGraph(CListGraph&&) noexcept;
    CListGraph& operator = (const CListGraph&);
    CListGraph& operator = (CListGraph&&) noexcept;
    void AddEdge(int from, int to, int w) override;
    int VerticesCount() const override;
    bool IsOriented() const override;
    void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const override;
    ~CListGraph() override;
};

CListGraph::CListGraph(size_t _num_of_vertices, bool oriented) : IGraph() {
    num_of_vertices = _num_of_vertices;
    list = new Node*[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        list[i] = new Node;
        list[i]->next = nullptr;
        list[i]->vertex = i;
        list[i]->weight = 0;
    }
    is_oriented = oriented;
}

CListGraph::CListGraph(const IGraph *other) : IGraph() {
    num_of_vertices = other->VerticesCount();
    is_oriented = other->IsOriented();
    list = new Node*[num_of_vertices];
    for (int i = 0; i < num_of_vertices;  ++i) {
        list[i] = new Node;
        list[i]->next = nullptr;
        list[i]->vertex = i;
        std::vector<std::pair<int, int>> vertices;
        other->GetNextVertices(i, vertices);
        Node* tmp = list[i];
        for (auto &itr : vertices) {
            tmp->next = new Node;
            tmp = tmp->next;
            tmp->next = nullptr;
            tmp->vertex = itr.first;
            tmp->weight = itr.second;
        }
    }
}

CListGraph::CListGraph(CListGraph &&other) noexcept : IGraph() {
    num_of_vertices = other.VerticesCount();
    is_oriented = other.is_oriented;
    list = new Node*[num_of_vertices];
    for (int i = 0; i < num_of_vertices;  ++i) {
        list[i] = new Node;
        list[i]->next = nullptr;
        list[i]->vertex = i;
        std::vector<std::pair<int, int>> vertices;
        other.GetNextVertices(i, vertices);
        Node* tmp = list[i];
        for (auto &itr : vertices) {
            tmp->next = new Node;
            tmp = tmp->next;
            tmp->next = nullptr;
            tmp->vertex = itr.first;
            tmp->weight = itr.second;
        }
    }
    other.Clear();
}

CListGraph& CListGraph::operator=(const CListGraph &other) {
    if (this == &other)
        return *this;
    Clear();
    num_of_vertices = other.VerticesCount();
    is_oriented = other.is_oriented;
    list = new Node*[num_of_vertices];
    for (int i = 0; i < num_of_vertices;  ++i) {
        list[i] = new Node;
        list[i]->next = nullptr;
        list[i]->vertex = i;
        std::vector<std::pair<int, int>> vertices;
        other.GetNextVertices(i, vertices);
        Node* tmp = list[i];
        for (auto &itr : vertices) {
            tmp->next = new Node;
            tmp = tmp->next;
            tmp->next = nullptr;
            tmp->vertex = itr.first;
            tmp->weight = itr.second;
        }
    }
    return *this;
}

CListGraph& CListGraph::operator=(CListGraph &&other) noexcept {
    if (this == &other)
        return *this;
    *this = other;
    other.Clear();
}

void CListGraph::AddEdge(int from, int to, int w) {
    assert(from < num_of_vertices && to < num_of_vertices);
    Node* tmp = list[from];
    bool found = false;
    while (tmp->next) {
        if (tmp->next->vertex == to) {
            if (tmp->next->weight > w)
                tmp->next->weight = w;
            found = true;
        }
        tmp = tmp->next;
    }
    if (tmp) {
        if (tmp->vertex == to) {
            found = true;
            if (tmp->weight > w) {
                tmp->weight = w;
            }
        }
    }
    if (!found) {
        tmp->next = new Node;
        tmp = tmp->next;
        tmp->next = nullptr;
        tmp->vertex = to;
        tmp->weight = w;
    }
    if (to == from)
        return;
    if (!is_oriented) {
        tmp = list[to];
        while (tmp->next) {
            if (tmp->next->vertex == from) {
                if (tmp->next->weight > w)
                    tmp->next->weight = w;
                return;
            }
            tmp = tmp->next;
        }
        if (tmp) {
            if (tmp->vertex == from && tmp->weight > w) {
                tmp->weight = w;
                return;
            }
        }
        tmp->next = new Node;
        tmp = tmp->next;
        tmp->next = nullptr;
        tmp->vertex = from;
        tmp->weight = w;
    }
}

void CListGraph::Clear() {
    for (int i = 0; i < num_of_vertices; ++i) {
        Node* to_del = list[i];
        Node* tmp;
        while (to_del) {
            tmp = to_del->next;
            delete to_del;
            to_del = tmp;
        }
    }
    if (num_of_vertices) delete[] list;
    num_of_vertices = 0;
}

int CListGraph::VerticesCount() const {
    return num_of_vertices;
}

bool CListGraph::IsOriented() const {
    return is_oriented;
}

void CListGraph::GetNextVertices(int vertex, std::vector<std::pair<int, int>> &vertices) const {
    assert(vertex < num_of_vertices);
    Node *tmp = list[vertex]->next;
    while (tmp) {
        vertices.push_back({tmp->vertex, tmp->weight});
        tmp = tmp->next;
    }
}

void CListGraph::GetPrevVertices(int vertex, std::vector<std::pair<int, int>> &vertices) const {
    assert(vertex < num_of_vertices);
    for (int i =0; i < num_of_vertices; ++i) {
        Node* tmp =  list[i]->next;
        while (tmp) {
            if (tmp->vertex == vertex)
                vertices.push_back({i, tmp->weight});
            tmp = tmp->next;
        }
    }
}

CListGraph::~CListGraph() {
    Clear();
}

struct PairKeyEquiv {
    PairKeyEquiv() = default;
    bool operator () (const std::pair<int, int> &p1, const std::pair<int, int> &p2) {
        return p1.second == p2.second;
    }
};

int main()
{
    //srand(time(0));
    int n = 0, v = 0, start = 0, stop = 0;
    std::cin >> n >> v;
    //n = 10000;
    //v = 250000;
    //assert(n <= 10000 && v <= 250000);
    CListGraph G(n, false);
    for (int i = 0; i < v; ++i) {
        int from, to, w;
        //from = rand() % 10000;
        //to = rand() % 10000;
        //w = rand() % 10000;
        std::cin >> from >> to >> w;
        //assert(w <= 10000);
        G.AddEdge(from, to, w);
    }
    //start = rand() % 10000;
    //stop = rand() % 10000;
    std::cin >> start >> stop;
    Heap<std::pair<int, int>> heap;
    std::vector<int> d(n, INT32_MAX);
    d[start] = 0;
    std::vector<int> used(n, false);
    heap.push({0, start});
    PairKeyEquiv pred;
    while(!heap.empty()) {
        auto temp_v = heap.pop();
        std::vector<std::pair<int, int>> next;
        G.GetNextVertices(temp_v.second, next);
        used[temp_v.second] = true;
        for (auto &i : next) {
            if (d[i.first] > d[temp_v.second] + i.second) {
                d[i.first] = d[temp_v.second] + i.second;
                if (!used[i.first]) {
                    heap.delete_by_key({0, i.first}, pred);
                    heap.push({d[i.first], i.first});
                }
            }
        }
    }
    std::cout << d[stop];
}