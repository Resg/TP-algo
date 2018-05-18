#include <iostream>
#include <vector>
#include <algorithm>
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

class CArcGraph : public IGraph {
private:
    struct Pair {
        int from;
        int to;
        friend bool operator == (const Pair &p1, const Pair &p2) {
            return (p1.from == p2.from && p1.to == p2.to);
        }
    };
    std::vector<Pair> pairs;
    size_t num_of_vertices;
    bool is_oriented;
    void Clear();
public:
    CArcGraph() = delete;
    CArcGraph(size_t, bool);
    CArcGraph(const IGraph*);
    CArcGraph(CArcGraph&&) noexcept;
    CArcGraph& operator = (const CArcGraph&);
    CArcGraph& operator = (CArcGraph&&) noexcept;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    bool IsOriented() const override;
    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
    ~CArcGraph() override = default;
};