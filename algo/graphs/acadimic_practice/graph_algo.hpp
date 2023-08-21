// [TODO]: is_tree (связность + ацикличность)
// [TODO]: Back removing
// [TODO]: Kruskal
// [TODO]: Prima
// [TODO]: Boruvka
//
// [TODO]: make ostov
// [TODO]: make min ostov


#ifndef GRAPHALGO_HPP
#define GRAPHALGO_HPP


#include <queue>
#include <stack>
#include <set>
#include <utility>
#include "graph.hpp"


template <
    class Graph_t
>
inline void BFSvoid (const Graph_t& Graph, int Vstart = 1) {
    std::queue<int> Queue;
    std::set<int> Visited;

    Queue.push(Vstart);
    Visited.insert(Vstart);

    while (!Queue.empty())
    {
        int Vnow = Queue.front();
        const std::vector<int>& Neighbours = Graph[Vnow];

        for (int Vnext {1}; Vnext <= Neighbours.size(); ++Vnext)
        {
            if (Neighbours[Vnext - 1]) { // Vnow connected with Vnext
                if (Visited.find(Vnext) != Visited.end()) {
                    Visited.insert(Vnext);
                    Queue.push(Vnext);
                }
            }
        }
        Queue.pop();
    }
}


template <
    class Graph_t
>
inline void DFSvoid (
    const Graph_t& Graph,
    int Vnow,
    std::set<int>& Visited
) {
    // std::set<int> Visited;
    const std::vector<int>& Neighbours = Graph[Vnow];

    for (int Vnext {1}; Vnext <= Neighbours.size(); ++Vnext) {
       if (Neighbours[Vnext - 1]) { // Vnow connected with Vnex
            if (Visited.find(Vnext) != Visited.end()) {
                Visited.insert(Vnext);
                DFS(Graph, Vnext, Visited);
            }
        }
    }
}


#endif // GRAPHALGO_HPP
