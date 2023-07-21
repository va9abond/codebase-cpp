#include <iostream>
#include <list>
#include "list.cpp"


using msl::list_v2;
using msl::_List_node;


template< 
    template <class... > class _Container_t,
    class _Value_t
>
void print(const _Container_t<_Value_t>& Cont) {
    auto It = Cont.begin(); std::cout << "\n" << "{ ";
    while (It != Cont.end()) {
        std::cout << *(++It) << " ";
    }
    std::cout << "};" << "\n";
}


int main() {
   
    std::list<int> exmpl;
    list_v2<int> mylist;
    print(mylist);




    std::cout << "END";
    return 0;
}
