#ifndef ALGO_HPP
#define ALGO_HPP
/* ------------------------------------------ */

#include <algorithm>
#include <vector>
#include <iostream>
#include "random_generator.hpp"

enum sorting_algo_t {
    bubble_sort_basic,
    bubble_sort_opt,

    insertion_sort_basic,
    insertion_sort_opt,

    merge_sort_recursive,
    merge_sort
};


namespace slb { // sorting library
    using vector_t = std::vector<int>;
    using index_t  = typename vector_t::size_type;
    using sorting_value_type = int;

    inline bool check_state (const vector_t& vector) {
        return std::is_sorted(vector.begin(), vector.end());
    }

    struct sorting_details {
        sorting_details(const vector_t& vector) :
            comp(0),
            assign(0),
            complex(0),
            sort_state(0),
            vector(vector)
        {}

    sorting_details& operator= (const sorting_details& rhs) {
        comp = rhs.comp;
        assign = rhs.assign;
        complex = rhs.complex;
        sort_state = rhs.sort_state;

        return *this;
    }
        uint64_t total() {
            return (uint64_t)(comp + assign + recall+ complex);
        }

        void show_details() {
            printf("%21s\n", "[Sorting details]");
            printf("%-15s %d\n","is sorted:", sort_state);
            printf("%-15s %u\n","compares:", comp);
            printf("%-15s %u\n","assigns:", assign);
            printf("%-15s %u\n","complex:", complex);
            printf("%-15s %u\n","recusion calls:", recall);
            printf("%-15s %lu\n","total:", total());
            // std::cout << "Sorting details" << '\n';
            // std::cout << "compares: " << comp << '\n'
            //           << "assigns: " << assign << '\n'
            //           << "recursion cals: " << recall << '\n'
            //           << "complex ops: " << complex << '\n'
            //           << "total: " << total() << '\n';
        }

        uint comp = 0;       // amount of comparisons
        uint assign = 0;     // amount of assignments
        uint complex = 0;    // amount of comples ops
        uint recall = 0;     // amount of recusion calls
        bool sort_state = 0; // is vector sorted
        const vector_t& vector; // unsorted vector
    };


    template <
        sorting_algo_t
    > inline sorting_details sort (vector_t&); // primaty template


    template<>
    inline sorting_details sort<bubble_sort_basic> (vector_t& vector) {
        sorting_details dt(vector);
        index_t sz = vector.size();                         ++dt.assign; // sz = ...
                                                            ++dt.assign; // i = 0;
        for (index_t i=0; i < sz-1; ++i, ++dt.assign, ++dt.comp) {
                                                            ++dt.assign; // j = i+1;
            for (index_t j = i+1; j < sz; ++j, ++dt.assign, ++dt.comp) {
                                                            ++dt.comp; // v[i] > v[j]
                if (vector[i] > vector[j]) {
                    std::swap(vector[i], vector[j]);        ++dt.complex; // swap
                }
            }
        }
        check_state(vector); return dt;
    }


    template<>
    inline sorting_details sort<bubble_sort_opt> (vector_t& vector) {
        sorting_details dt(vector);
        index_t sz = vector.size();                         ++dt.assign; // sz = ...
                                                            ++dt.assign; // i = 0;
        for (index_t i = 0; i < sz-1; ++i, ++dt.assign, ++dt.comp) {
                                                            ++dt.assign; // ... = false
            bool is_swapped = false;                        ++dt.assign; // j = 0
            for (index_t j = 0; j < sz-i-1; ++j, ++dt.assign, ++dt.comp) {
                                                            ++dt.comp; // v[j+1] < v[j]
                if(vector[j+1] < vector[j]) {
                    std::swap(vector[j], vector[j+1]);      ++dt.complex; // swap
                    is_swapped = true;                      ++dt.assign; // ... = true
                }
            }
                                                            ++dt.comp; // !is_swapped
            if (!is_swapped) { break; }
        }
        check_state(vector); return dt;
    }

    template<>
    inline sorting_details sort<insertion_sort_basic> (vector_t& vector) {
        sorting_details dt(vector);
        index_t sz = vector.size();                         ++dt.assign; // sz = ...
        for (index_t i = 1; i <= sz; ++i, ++dt.assign, ++dt.comp) {
                                                            ++dt.comp; // v[i-1] > v[i]
            if (vector[i-1] > vector[i]) {
                index_t j = i;                              ++dt.assign; // j = i

                while (j > 0 && vector[j-1] > vector[j]) { dt.comp+=2; // j>0, v[j-1] > v[j]
                    std::swap(vector[j], vector[j-1]);      ++dt.complex;
                    --j;                                    ++dt.assign;
                }
            }
        }
        check_state(vector); return dt;
    }


    template<>
    inline sorting_details sort<insertion_sort_opt> (vector_t& vector) {
        sorting_details dt(vector);
        index_t sz = vector.size();                         ++dt.assign; // sz = ...
        for (index_t i = 1; i <= sz; ++i,
                ++dt.assign, ++dt.comp) {
            int key = vector[i];                            ++dt.assign; // key = ...
            int j = (int) i-1;                              ++dt.assign; // j=i-1

            while (j >= 0 && vector[j] > key) {              dt.comp+=2;  // j>0, v[j-1] > v[j]
                vector[j+1] = vector[j];                    ++dt.assign; // v[j+1]=v[j]
                --j;                                        ++dt.assign;
            }
            vector[j+1] = key;                              ++dt.assign;
        }
        check_state(vector); return dt;
    }
}


















/* ------------------------------------------ */
#endif // ALGO_HPP

