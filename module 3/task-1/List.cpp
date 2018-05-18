#include "List.h"

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