#ifndef MYSORTS_HPP
#define MYSORTS_HPP
// -----------------------------------------------------------------

#include <vector>
#include "random_generator.hpp"


using vi = std::vector<int>;
using index_t = typename vi::size_type;


enum sorting_algo_t {
    bubble_sort_classic,
    bubble_sort_opt,
    insertion_sort,
};


template <
    sorting_algo_t
>
inline void sort (vi& Array); // primary sort template

template<>
inline void sort<bubble_sort_classic> (vi& Array)
{
    index_t sz = Array.size();

    for (index_t i = {0}; i < sz - 1; ++i) {
        for (index_t j = {i+1}; j < sz; ++j) {
            if (Array[i] > Array[j]) {
                std::swap(Array[i], Array[j]);
            }
        }
    }
}

template <>
inline void sort<bubble_sort_opt> (vi& Array)
{
    index_t sz = Array.size();

    for (index_t i = {0}; i < sz - 1; ++i) {
        bool is_swapped = false;
        for (index_t j = {0}; j < sz - i - 1; ++j) {
            if (Array[j+1] < Array[j]) {
                std::swap(Array[j], Array[j+1]);
                is_swapped = true;
            }
        }
        if (!is_swapped) { break; }
    }
}

template<>
inline void sort<insertion_sort> (vi& Array)
{
    for (index_t i = {1}; i < Array.size(); ++i) {
        if (Array[i-1] > Array[i]) {
            index_t j = {i};
            while (j > 0 && Array[j-1] > Array[j]) {
                std::swap(Array[j], Array[j-1]);
                --j;
            }
        }
    }
}










// -----------------------------------------------------------------
#endif // MYSORTS_HPP
