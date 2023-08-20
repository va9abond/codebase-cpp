#ifndef GRAPH_HPP
#define GRAPH_HPP


#include "vector_utils.hpp"


template <
    class Weight_t
>
struct weighted_graph {
public:
    weighted_graph() = delete;

    weighted_graph (std::string file_name) {
        data = generate_vector_from_file(file_name);
    }

    ~weighted_graph() = default;

    std::vector<std::vector<Weight_t>> data; // vector to store graph
};






struct graph : public weighted_graph<bool> {};





#endif // GRAPH_HPP
