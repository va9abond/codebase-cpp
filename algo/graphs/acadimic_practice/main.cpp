#include <iostream>
#include <vector>
#include "vector_utils.hpp"
// #include "graph.hpp"


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

    // auto string = generate_vector_from_file<int>("graph.txt");
    // std::cout << string << "\n";

    std::string mystr { "hola muchaco" };
    // for (std::string::size_type sz{0}; sz < mystr.size(); ++sz) {
    //     std::cout << mystr[sz] << "\n";
    // }

    // std::cout << mystr.substr(0, 6);

    std::string sstr {"{34, 4, 332, gskg, 25,323 };"};
    std::vector<int> myvec = str_to_vec(sstr);
    print(myvec);

    // std::cout << mystr.substr(0,2);


    return 0;
}
