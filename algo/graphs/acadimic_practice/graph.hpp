/* [[WARNING]]: */


#ifndef GRAPH_HPP
#define GRAPH_HPP


#include "vector_utils.hpp"
#include <cassert>
#include <limits>

// [TODO]: split weight function and graph
//         (it makes possible to passing one graph with different weight functions)
// [TODO]: different ways to store graph
// [TODO]: tree is derived class from Grap_base_ with static_assert(is_tree);


struct Graph_base_ {
    //                      Traits
    // +---------------------------------------------------------+
    // |             pair.first ~ source;                        |
    // |             pair.second ~ target;                       |
    /* | */   using vert      = int;                          // |
    /* | */   using edge      = std::pair<vert*,vert*>;       // |
    /* | */   using size_type = std::set<vert*>::size_type;   // |
    // +---------------------------------------------------------+
    // [WARNING]: Despite the fact that size_type is usigned long
    //            for std::set<vert*>, we allow to create just
    //            INT_MAX vertice!

    Graph_base_ (int Count = 0) : m_Verts() {
        assert(Count > 0 &&
               Count <= std::numeric_limits<int>::max() &&
               "Invalid Value: Count");
        Construct_verts(Count);
    }

    void add_n_verts (int Count) noexcept {
        assert(Count > 0 &&
               Count <= std::numeric_limits<int>::max() - this->size() &&
               "Invalid Value: Count");
        int startNo = 1 + m_Verts.size();
        Construct_verts(Count, startNo);
    }

    Graph_base_& operator= (const Graph_base_&) = delete;
    Graph_base_ (const Graph_base_&)            = delete;

    ~Graph_base_() { 
        // [NOTE]: does it really work? (= yes, valgrind approves)
        // [TODO]: better way?
        for (auto Vit = m_Verts.begin(); Vit != m_Verts.end(); ++Vit) {
            delete *Vit;
        }
    }

    int size() const noexcept { return (int)m_Verts.size(); }

private:
    void Construct_verts (int Count, int startNo = 1) noexcept { // [TODO]: is there a better way?
        for (int i {0}; i < Count; ++i) {
            vert* Vnew = new vert(startNo + i);
            m_Verts.insert(Vnew);
        }
    }

public:
    mutable std::set<vert*> m_Verts;
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

    // generate graph with Count verts and zero weight function
    weighted_graph (size_type Count) : Graph_base_(Count) {
        m_Weightfunc = std::vector<std::vector<int>>(Count, std::vector<int>(Count, 0));
    }

    // generate graph with weight function from file
    weighted_graph (std::string Filename) : Graph_base_() {
        m_Weightfunc = generate_2dvector_from_file(Filename);
        Mybase::add_n_verts(m_Weightfunc.size());
        Construct_edges();
    }

    weighted_graph& operator= (const weighted_graph& Rhs) = default;

    // std::map<weight_type, edge> get_edges() const noexcept {
    //     return m_Edges;
    // }

private:
    // construct edges by weight function
    void Construct_edges() noexcept {
        for (vert vi {0}; vi < size(); ++vi) {
            for (vert vj {vi + 1}; vj < size(); ++vj) {
                weight_type weight = m_Weightfunc[vi][vj];
                if (weight) {
                    m_Edges.emplace(weight, {vi + 1, vj + 1});
                }
            }

        }
    }

public:
    // weighted_graph must have a weight function on it's
    // edges; weight of edge e is equals
    // m_Weightfunc[e.first][e.secont]
    std::vector<std::vector<Weight_t>> m_Weightfunc;
    mutable std::multimap<Weight_t, edge> m_Edges;
    // [TODO]: key = 0 is unacceptable
    // [TODO]: try make edge as std::pair<*vert, *vert>;
    //         1. to connect m_Verts and m_Edges
    //         2. replace one vert to another (edge contraction)
    //            is nice with pointers
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
