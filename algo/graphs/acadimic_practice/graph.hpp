#ifndef GRAPH_HPP
#define GRAPH_HPP


#include "vector_utils.hpp"
#include <cassert>
#include <limits.h>

// [TODO]: split weight function and graph
//         (it makes possible to passing one graph with different weight functions)
// [TODO]: different ways to store graph
// [TODO]: tree is derived class from Grap_base_ with static_assert(is_tree);


struct Graph_base_ {
    //                  Traits
    // +------------------------------------------+
    // |          pair.first ~ source;            |
    // |          pair.second ~ target;           |
    /* | */   using edge = std::pair<int,int>; // |
    /* | */   using size_type = unsigned;      // |
    /* | */   using vert      = int;           // |
    // +------------------------------------------+

    Graph_base_(size_type vertsCount = 0) : m_Verts() {
        Construct_verts_range(vertsCount);
    }

    void resize (size_type newCount) noexcept {
        assert(newCount > 0 && newCount <= UINT_MAX && "Invalid Value: newCount");
        if (newCount > m_Verts.size()) {
            Construct_verts_range(newCount - m_Verts.size());
        } else { // [NOTE]: it's expensive and never should be used!
            while (m_Verts.size() > newCount) {
                m_Verts.erase(m_Verts.end());
            }
        }
    }

    // Graph_base_& operator= (const Graph_base_&) = delete;
    // Graph_base_ (const Graph_base_&)            = delete;

    size_type size() const noexcept { return m_Verts.size(); }

private:
    void Construct_verts_range (size_type vertsCount) noexcept {
        for (vert vi {1}; vi <= vertsCount; ++vi) {
            m_Verts.insert(vi);
        }
    }

public:
    mutable std::set<vert> m_Verts;
};


// there is a function of weights on edges
template <
    class Weight_t
>
struct weighted_graph : Graph_base_ {
    using Mybase      = Graph_base_;
    using weight_type = Weight_t;
    using size_type   = Mybase::size_type;
    using vert        = Mybase::vert;

    // generate graph with vertsCount*vertsCount adjacency matrix filled 0
    weighted_graph(size_type vertsCount) : Graph_base_(vertsCount) {
        m_Adjmatrix = std::vector<std::vector<int>>(vertsCount, std::vector<int>(vertsCount, 0));
    }

    weighted_graph (std::string Filename) : Graph_base_() {
        m_Adjmatrix = generate_vector_from_file(Filename);
        Mybase::resize(m_Adjmatrix.size());
    }

    weighted_graph& operator= (const weighted_graph& Rhs) = default;

    virtual std::map<weight_type, edge> get_edges() const noexcept {
        std::map<weight_type, edge> Result;
        size_type sz = Mybase::size();
        for (vert vi {0}; vi < sz; ++vi) {
            for (vert vj {vi + 1}; vj < sz; ++vj) {
                weight_type weight = m_Adjmatrix[vi][vj];
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


// all edges have source and target
template <
    class Weight_t
>
struct oriented_graph : weighted_graph<Weight_t> {
    using Mybase      = weighted_graph<Weight_t>;
    using weight_type = bool;
    using size_type   = typename Mybase::size_type;
    using vert        = typename Mybase::vert;
    using Mybase::Mybase;


    std::map<Weight_t, typename Mybase::edge> get_edges() const noexcept override {
        /* Override here */
    };

};


#endif // GRAPH_HPP
