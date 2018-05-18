#include <iostream>
#include <vector>
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

class CMatrixGraph : public IGraph {
private:
    bool** adjacency_matrix;
    size_t num_of_vertices;
    void Clear();
public:
    CMatrixGraph() = delete;
    explicit CMatrixGraph(size_t);
    explicit CMatrixGraph(const IGraph*);
    CMatrixGraph(CMatrixGraph&&) noexcept;
    CMatrixGraph& operator = (const CMatrixGraph &);
    CMatrixGraph& operator = (CMatrixGraph &&) noexcept;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    bool IsOriented() const override;
    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
    ~CMatrixGraph() override;
};