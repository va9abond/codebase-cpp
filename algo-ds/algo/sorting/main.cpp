#include <iostream>
#include <string>
#include <algorithm>
#include "algo.hpp"

template <
    template <class... > class container_t,
    class val_t
>
void print (const container_t<val_t>& cont) {
    std::cout << "\n" << "{ ";
    for (const val_t& val : cont) {
        std::cout << val << " ";
    }
    std::cout << "};\n";
}

template <
    sorting_algo_t sort_t
>
void sort_presentation (std::vector<int> vector, const char algo_t[]) {
    using namespace slb;
    std::cout << "============================================\n";
    std::cout << algo_t << '\n';
    sorting_details details(vector);
    std::cout << "[before] Is sorted: "
        << check_state(vector) << '\n';


    details = sort<sort_t>(vector);


    details.show_details();

    std::cout << "[after] Is sorted: "
        << check_state(vector) << '\n';
    std::cout << "============================================\n";

}
int main() {
    auto unsorted_vector = generate_random_vector(100, 1000);

    sort_presentation<bubble_sort_basic>(unsorted_vector, "bubble_sort_basic");
    sort_presentation<bubble_sort_opt>(unsorted_vector, "bubble_sort_opt");

    sort_presentation<insertion_sort_basic>(unsorted_vector, "insertion_sort_basic");
    sort_presentation<insertion_sort_opt>(unsorted_vector, "insertion_sort_opt");

    // sort_presentation<merge_sort_recursive>(unsorted_array, "merge_sort_recursive");

    // vi myvec = {7,8,2,4,9};
    // merge_sort_merge(myvec, 0,2,5);
    // print(myvec);

    return 0;
}
