#include <iostream>
#include "list.cpp"


using msl::list_v2;
using msl::_List_node;


int main() {
   
    msl::_Container_base Cont_base1;
    Cont_base1._Orphan_all();
    Cont_base1._Alloc_proxy();
    Cont_base1._Free_proxy();
    Cont_base1._Alloc_proxy();

    msl::_Container_base Cont_base2;
    Cont_base2._Alloc_proxy();
    Cont_base1._Swap_proxy_and_iterators(Cont_base2);

    Cont_base1.~_Container_base();
    Cont_base2.~_Container_base();


    return 0;
}
