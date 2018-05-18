#include <iostream>
#include <vector>
#include <unordered_set>
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

class CSetGraph : public IGraph {
private:
    std::unordered_set<int>* hash_tables;
    size_t num_of_vertices;
    bool is_oriented;
    void Clear();
public:
    CSetGraph() = delete;
    CSetGraph(size_t, bool);
    explicit CSetGraph(const IGraph*);
    CSetGraph(CSetGraph&&) noexcept;
    CSetGraph& operator = (const CSetGraph&);
    CSetGraph& operator = (CSetGraph&&) noexcept;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    bool IsOriented() const override;
    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
    ~CSetGraph() override;
};