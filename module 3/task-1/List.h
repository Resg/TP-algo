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
