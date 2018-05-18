#include "Set.h"

CSetGraph::CSetGraph(size_t _num_of_vertices, bool oriented) : IGraph() {
    num_of_vertices = _num_of_vertices;
    is_oriented = oriented;
    hash_tables = new std::unordered_set<int>[num_of_vertices];
}

CSetGraph::CSetGraph(const IGraph *other) : IGraph() {
    is_oriented = other->IsOriented();
    num_of_vertices = other->VerticesCount();
    hash_tables = new std::unordered_set<int>[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        std::vector<int> vertices;
        other->GetNextVertices(i, vertices);
        for (auto &itr : vertices)
            hash_tables[i].insert(itr);
    }
}

CSetGraph::CSetGraph(CSetGraph &&other) noexcept : IGraph() {
    num_of_vertices = other.num_of_vertices;
    is_oriented = other.is_oriented;
    hash_tables = other.hash_tables;
    other.hash_tables = nullptr;
    other.num_of_vertices = 0;
}

CSetGraph& CSetGraph::operator=(const CSetGraph &other) {
    Clear();
    if (this == &other)
        return *this;
    is_oriented = other.IsOriented();
    num_of_vertices = other.VerticesCount();
    hash_tables = new std::unordered_set<int>[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        std::vector<int> vertices;
        other.GetNextVertices(i, vertices);
        for (auto &itr : vertices)
            hash_tables[i].insert(itr);
    }
    return *this;
}

CSetGraph& CSetGraph::operator=(CSetGraph &&other) noexcept  {
    Clear();
    if (this == &other) {
        return *this;
    }
    num_of_vertices = other.num_of_vertices;
    is_oriented = other.is_oriented;
    hash_tables = other.hash_tables;
    other.hash_tables = nullptr;
    other.num_of_vertices = 0;
    return *this;
}

void CSetGraph::AddEdge(int from, int to) {
    assert(from < num_of_vertices && to < num_of_vertices);
    hash_tables[from].insert(to);
    if (!is_oriented) {
        hash_tables[to].insert(from);
    }
}

int CSetGraph::VerticesCount() const {
    return num_of_vertices;
}

bool CSetGraph::IsOriented() const {
    return is_oriented;
}

void CSetGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    for (int i = 0; i < num_of_vertices; ++i) {
        if (hash_tables[vertex].find(i) != hash_tables[vertex].end())
            vertices.push_back(i);
    }
}

void CSetGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    for (int i = 0; i < num_of_vertices; ++i) {
        if (hash_tables[i].find(vertex) != hash_tables[vertex].end())
            vertices.push_back(i);
    }
}

void CSetGraph::Clear() {
    if (num_of_vertices)
        delete[] hash_tables;
}

CSetGraph::~CSetGraph() {
    Clear();
}