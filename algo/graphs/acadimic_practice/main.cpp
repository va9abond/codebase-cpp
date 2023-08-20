#include <iostream>
#include <vector>
#include "graph.hpp"


template<
    template <class... > class _Container_t,
    class _Value_t
>
void print (const _Container_t<_Value_t>& Cont) {
    auto It = Cont.begin(); std::cout << "\n" << "{ ";
    while (It != Cont.end()) {
        std::cout << *(It++) << " ";
    }
    std::cout << "};\n";
}


int main() {
    weighted_graph<int> graph { "graph.txt" };
    for (const std::vector<int> &vec : graph.data) {
        print(vec);
    }

    return 0;
}
