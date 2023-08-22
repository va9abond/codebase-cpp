#ifndef GRAPH_HPP
#define GRAPH_HPP


#include "vector_utils.hpp"


// [TODO]: split weight function and graph
//         (it makes possible to passing one graph with different weight functions)
// [TODO]: different ways to store graph
// [TODO]: tree is derived class from Grap_base_ with static_assert(is_tree);


struct Graph_base_ {
    // pair.first ~ source; pair.second ~ targer
    using edge = std::pair<int,int>;


    Graph_base_(int vertCount = 0) : m_vertsCount(vertCount) {}

    // Graph_base_& operator= (const Graph_base_&) = delete;
    // Graph_base_ (const Graph_base_&)            = delete;

    size_t size() const noexcept { return m_vertsCount; }


    mutable int m_vertsCount;
};


// there is a function of weights on edges
template <
    class Weight_t
>
struct weighted_graph : Graph_base_ {
    using Mybase = Graph_base_;

    // generate graph with vertCount*vertCount adjacency matrix filled 0
    weighted_graph(int vertCount) : Graph_base_(vertCount) {
        m_Adjmatrix = std::vector<std::vector<int>>(vertCount, std::vector<int>(vertCount, 0));
    }

    weighted_graph (std::string Filename) : Graph_base_() {
        m_Adjmatrix = generate_vector_from_file(Filename);
        Mybase::m_vertsCount = m_Adjmatrix.size();
    }

    weighted_graph& operator= (const weighted_graph& Rhs) = default;

    virtual std::map<Weight_t, edge> get_edges() const noexcept {
        std::map<Weight_t, edge> Result;
        size_t sz = Mybase::size();
        for (int vi {0}; vi < sz; ++vi) {
            for (int vj {vi + 1}; vj < sz; ++vj) {
                Weight_t weight = m_Adjmatrix[vi][vj];
                if (weight) {
                    Result.insert({ weight, {vi,vj} });
                    Result.insert({ weight, {vj,vi} });
                }
            }
        }
    }


    std::vector<std::vector<Weight_t>> m_Adjmatrix; // graph presented by Adjacency Matrix
};


// no weights on edges, just 1 if i, j verts connected, else 0
struct general_graph : weighted_graph<bool> {};


// all edges have source and targer
template <
    class Weight_t
>
struct oriented_graph : weighted_graph<Weight_t> {
    using Mybase = weighted_graph<Weight_t>;
    using Mybase::Mybase;


    std::map<Weight_t, typename Mybase::edge> get_edges() const noexcept override {
        /* Override here */
    };

};


#endif // GRAPH_HPP
