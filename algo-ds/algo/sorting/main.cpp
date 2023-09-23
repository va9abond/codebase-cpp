#include <iostream>
#include <algorithm>
#include "mysorts.hpp"
#include "random_generator.hpp"


template<
template <class... > class container_t,
class val_t
>
void print (const container_t<val_t>& cont)
{
    std::cout << "\n" << "{ ";
    for (const val_t& val : cont) {
        std::cout << val << " ";
    }
    std::cout << "};\n";
}


template <
    sorting_algo_t sort_t
>
void sort_presentation (std::vector<int> Array)
{
    std::cout << "============================================\n";
    std::cout << "[before] Is sorted: " <<
        std::is_sorted(Array.begin(), Array.end()) << "\n";

    sort<sort_t>(Array);

    std::cout << "[after] Is sorted: " <<
        std::is_sorted(Array.begin(), Array.end()) << "\n";
    std::cout << "============================================\n";

}

int main()
{
    auto unsorted_array = generate_unsorted_array(100, 1000);

    sort_presentation<bubble_sort_classic>(unsorted_array);
    sort_presentation<bubble_sort_opt>(unsorted_array);

    sort_presentation<insertion_sort>(unsorted_array);


    return 0;
}
