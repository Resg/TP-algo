#include "Matrix.h"

CMatrixGraph::CMatrixGraph(size_t _num_of_vertices) : IGraph() {
    num_of_vertices = _num_of_vertices;
    adjacency_matrix = new bool*[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        adjacency_matrix[i] = new bool[num_of_vertices];
        for (int j = 0; j < num_of_vertices; ++j)
            adjacency_matrix[i][j] = false;
    }
}

CMatrixGraph::CMatrixGraph(const IGraph *other) : IGraph() {
    num_of_vertices = other->VerticesCount();
    adjacency_matrix = new bool*[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        adjacency_matrix[i] = new bool[num_of_vertices];
        for (int j = 0; j < num_of_vertices; ++j)
            adjacency_matrix[i][j] = false;
    }
    for (int i = 0; i < num_of_vertices; ++i) {
        std::vector<int> vertices;
        other->GetNextVertices(i, vertices);
        for (auto &itr : vertices) {
            adjacency_matrix[i][itr] = true;
            adjacency_matrix[itr][i] = true;
        }
    }
}

CMatrixGraph::CMatrixGraph(CMatrixGraph &&other) noexcept : IGraph(){
    num_of_vertices = other.num_of_vertices;
    adjacency_matrix = other.adjacency_matrix;
    other.adjacency_matrix = nullptr;
    other.num_of_vertices = 0;
}

CMatrixGraph& CMatrixGraph::operator=(const CMatrixGraph &other) {
    if (this == &other)
        return *this;
    Clear();
    num_of_vertices = other.num_of_vertices;
    adjacency_matrix = new bool*[num_of_vertices];
    for (int i = 0; i < num_of_vertices; ++i) {
        adjacency_matrix[i] = new bool[num_of_vertices];
        for (int j = 0; j < num_of_vertices; ++j)
            adjacency_matrix[i][j] = other.adjacency_matrix[i][j];
    }
    return *this;
}

CMatrixGraph& CMatrixGraph::operator=(CMatrixGraph &&other) noexcept {
    if (this == &other)
        return *this;
    *this = other;
    other.Clear();
}

void CMatrixGraph::AddEdge(int from, int to) {
    assert(from < num_of_vertices && to < num_of_vertices);
    adjacency_matrix[from][to] = true;
    adjacency_matrix[to][from] = true;
}

void CMatrixGraph::Clear() {
    for (int i = 0; i < num_of_vertices; ++i)
        delete[] adjacency_matrix[i];
    if (num_of_vertices)
        delete[] adjacency_matrix;
    num_of_vertices = 0;
}

int CMatrixGraph::VerticesCount() const {
    return num_of_vertices;
}

bool CMatrixGraph::IsOriented() const {
    return false;
}

void CMatrixGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert(vertex < num_of_vertices);
    for (int i = 0; i < num_of_vertices; ++i) {
        if (adjacency_matrix[vertex][i])
            vertices.push_back(i);
    }
}

void CMatrixGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    this->GetNextVertices(vertex, vertices);
}

CMatrixGraph::~CMatrixGraph() {
    Clear();
}
