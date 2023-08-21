#ifndef GRAPH_HPP
#define GRAPH_HPP


#include "vector_utils.hpp"


// pair.first ~ source; pair.second ~ targer
using vertice_index = std::pair<int,int>;


// there is a function of weights on edges
template <
    class Weight_t
>
struct weighted_graph {
    weighted_graph() = delete;

    weighted_graph (std::string Filename) {
        m_Matrix = generate_vector_from_file(Filename);
    }

    weighted_graph& operator= (const weighted_graph& Rhs) = default;

    std::vector<std::vector<Weight_t>> m_Matrix; // graph presented by Adjacency Matrix
};



// no weights on edges, just 1 if i, j verts connected, else 0
struct general_graph : weighted_graph<bool> {};


// all edges have source and targer
template <
    class Weight_t
>
struct oriented_graph : weighted_graph<Weight_t> {};

#endif // GRAPH_HPP
