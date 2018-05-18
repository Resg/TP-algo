#include "Arc.h"

CArcGraph::CArcGraph(size_t _num_of_vertices, bool oriented) {
    num_of_vertices = _num_of_vertices;
    is_oriented = oriented;
}

CArcGraph::CArcGraph(const IGraph *other) {
    num_of_vertices = other->VerticesCount();
    is_oriented = other->IsOriented();
    for (int i = 0; i < num_of_vertices; ++i) {
        std::vector<int> vertices;
        other->GetNextVertices(i, vertices);
        for (auto &itr : vertices) {
            pairs.push_back({i, itr});
        }
    }
}

CArcGraph::CArcGraph(CArcGraph &&other) noexcept {
    for (auto &itr : other.pairs)
        pairs.push_back(itr);
    num_of_vertices = other.num_of_vertices;
    is_oriented = other.is_oriented;
    other.Clear();
}

CArcGraph& CArcGraph::operator=(const CArcGraph &other) {
    if (this == &other)
        return *this;
    Clear();
    for (auto &itr : other.pairs)
        pairs.push_back(itr);
    num_of_vertices = other.num_of_vertices;
    is_oriented = other.is_oriented;
    return *this;
}

CArcGraph& CArcGraph::operator=(CArcGraph &&other) noexcept {
    Clear();
    if (this == &other)
        return *this;
    for (auto &itr : other.pairs)
        pairs.push_back(itr);
    num_of_vertices = other.num_of_vertices;
    is_oriented = other.is_oriented;
    other.Clear();
    return *this;
}

void CArcGraph::AddEdge(int from, int to) {
    assert(from < num_of_vertices && to < num_of_vertices);
    if (std::find(pairs.begin(), pairs.end(), Pair{from, to}) == pairs.end()) {
        pairs.push_back(Pair{from, to});
    }
    if (!is_oriented) {
        if (std::find(pairs.begin(), pairs.end(), Pair{to, from}) == pairs.end()) {
            pairs.push_back(Pair{to, from});
        }
    }
}

int CArcGraph::VerticesCount() const {
    return num_of_vertices;
}

bool CArcGraph::IsOriented() const {
    return is_oriented;
}

void CArcGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    for (auto &itr : pairs) {
        if (itr.from == vertex)
            vertices.push_back(itr.to);
    }
}

void CArcGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    for (auto &itr : pairs) {
        if (itr.to == vertex)
            vertices.push_back(itr.from);
    }
}

void CArcGraph::Clear() {
    num_of_vertices = 0;
    pairs.clear();
}
