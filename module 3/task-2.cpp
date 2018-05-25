#include <queue>
#include <algorithm>

#include <iostream>
#include <vector>
#include <set>
#include <cassert>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;
    virtual bool IsOriented() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

class CListGraph : public IGraph {
private:
    struct Node {
        Node* next;
        int vertex;
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
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    bool IsOriented() const override;
    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
    ~CListGraph() override;
};

CListGraph::CListGraph(size_t _num_of_vertices, bool oriented) : IGraph() {
    num_of_vertices = _num_of_vertices;
    list = new Node*[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        list[i] = new Node;
        list[i]->next = nullptr;
        list[i]->vertex = i;
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
        std::vector<int> vertices;
        other->GetNextVertices(i, vertices);
        Node* tmp = list[i];
        for (auto &itr : vertices) {
            tmp->next = new Node;
            tmp = tmp->next;
            tmp->next = nullptr;
            tmp->vertex = itr;
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
        std::vector<int> vertices;
        other.GetNextVertices(i, vertices);
        Node* tmp = list[i];
        for (auto &itr : vertices) {
            tmp->next = new Node;
            tmp = tmp->next;
            tmp->next = nullptr;
            tmp->vertex = itr;
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
        std::vector<int> vertices;
        other.GetNextVertices(i, vertices);
        Node* tmp = list[i];
        for (auto &itr : vertices) {
            tmp->next = new Node;
            tmp = tmp->next;
            tmp->next = nullptr;
            tmp->vertex = itr;
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

void CListGraph::AddEdge(int from, int to) {
    assert(from < num_of_vertices && to < num_of_vertices);
    Node* tmp = list[from];
    while (tmp->next) {
        if (tmp->next->vertex == to)
            return;
        tmp = tmp->next;
    }
    tmp->next = new Node;
    tmp = tmp->next;
    tmp->next = nullptr;
    tmp->vertex = to;
    if (!is_oriented) {
        tmp = list[to];
        while (tmp->next) {
            if (tmp->next->vertex == from)
                return;
            tmp = tmp->next;
        }
        tmp->next = new Node;
        tmp = tmp->next;
        tmp->next = nullptr;
        tmp->vertex = from;
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

void CListGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert(vertex < num_of_vertices);
    Node *tmp = list[vertex]->next;
    while (tmp) {
        vertices.push_back(tmp->vertex);
        tmp = tmp->next;
    }
}

void CListGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    assert(vertex < num_of_vertices);
    for (int i =0; i < num_of_vertices; ++i) {
        Node* tmp =  list[i]->next;
        while (tmp) {
            if (tmp->vertex == vertex)
                vertices.push_back(i);
            tmp = tmp->next;
        }
    }
}

CListGraph::~CListGraph() {
    Clear();
}

int main() {
    int n = 0, v = 0;
    std::cin >> v >> n;
    assert(v <= 50000 && n <= 200000);
    CListGraph Graph(v, false);
    for (int i = 0 ; i < n; ++i) {
        int from, to;
        std::cin >> from >> to;
        Graph.AddEdge(from, to);
    }
    int start, stop;
    std::cin >> start >> stop;
    std::vector<std::pair<int, int>> num_of_ways;
    for (int i = 0; i < n; ++i)
        num_of_ways.push_back({-1, 0});
    std::vector<bool> processed(v, false);
    std::queue<int> to_process;
    to_process.push(start);
    num_of_ways[start].second = 1;
    num_of_ways[start].first = 0;
    while (!to_process.empty()) {
        int temp_vertex = to_process.front();
        to_process.pop();
        if (!processed[temp_vertex]) {
            std::vector<int> next_vertices;
            Graph.GetNextVertices(temp_vertex, next_vertices);
            for (auto &itr : next_vertices) {
                if (num_of_ways[itr].first - 1 > num_of_ways[temp_vertex].first) {
                    num_of_ways[itr].first = num_of_ways[temp_vertex].first + 1;
                    num_of_ways[itr].second = num_of_ways[temp_vertex].second;
                }
                else if (num_of_ways[itr].first == -1) {
                    num_of_ways[itr].first = num_of_ways[temp_vertex].first + 1;
                    num_of_ways[itr].second = num_of_ways[temp_vertex].second;
                }
                else if (num_of_ways[itr].first - 1 == num_of_ways[temp_vertex].first)
                    num_of_ways[itr].second += num_of_ways[temp_vertex].second;
                to_process.push(itr);
            }
            processed[temp_vertex] = true;
        }
    }
    std::cout << num_of_ways[stop].second;
    return 0;
}