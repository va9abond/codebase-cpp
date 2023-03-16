#include <iostream>
#include <list> 
#include <vector>


template <
    class Ty
>
class node {
public:

    explicit node(Ty value) : _value(value), _next(nullptr) {}

    ~node() {
        delete _next;
        _next = nullptr;
    }

    template <
        class Vty
    >
    friend class container;


protected:
    Ty        _value;
    node<Ty>* _next;
};


template <
    class Mylist
>
class list_iterator {
public:

    // using Node = node<Vty>;
    using Nodeptr = typename Mylist::Nodeptr;
    using value_type = typename Mylist::value_type;
    using reference = value_type&;

    list_iterator() noexcept : Ptr() {}

    list_iterator(Nodeptr ptr) noexcept : Ptr(ptr) {}

    reference operator*() const noexcept {
        return Ptr->_value;
    }


    Nodeptr Ptr;

};


template <
    class Vty
>
class container {
public:
    
    using Node = node<Vty>;
    using Nodeptr = node<Vty>*;
    using value_type = Vty;

    using iterator = list_iterator<Vty>;


    explicit container(Nodeptr head) : _head(head) {}

    explicit container() : _head(nullptr) {}

    iterator begin() noexcept {
        return iterator(_head);
    }

    // void push_back(Nodeptr_  

protected:
    Nodeptr _head;
};


template <
    template<class...> class TContainer,
    class Valty_
>
void print (const TContainer<Valty_> &container) {
    std::cout << "\n" << "{ ";
    
    for ( auto value : container ) {
        std::cout << value << " ";
    }
    
    std::cout << "}" << "\n";
}
#include <iterator>


int main() {


    std::list<int> list1{2134,324};
    std::list<int> list2{12,65,546,8765,32};
    auto itt = list1.begin();
    itt = list2.end();
    itt--;
    std::cout << *itt;



    // node<int> node1(324);
    // list_iterator<int>(node1);






    // node<int>* head = new node<int>(1);
    // container<int> cont(head);

    // // container<int>::iterator itt;
    // std::list<int> list{123, 324, 235, 56, 45, 465, 4574, 4574};
    // std::list<int>::iterator itt = list.begin();
    // *itt = 1;
    // std::cout << *(list.begin());


    // std::list<int> ilist{324,1,546,685,34,885,76543};
    // print<std::list,int>(ilist);

    // auto it = ilist.begin();
    // it++; it++;
    // *it = 888;
    // print<std::list,int>(ilist);



    // itt._Ptr;
    // std::list<int>::_Unchecked_iterator utt;
    // std::list<int>::_Nodeptr nptr;

}


// using _Val_types = conditional_t<
//     _Is_simple_alloc_v
//     <_Alnode>,
//     _List_simple_types<_Ty>,
//     _List_iter_types<
//         _Ty,
//         typename _Alty_traits::size_type,
//         typename _Alty_traits::difference_type,
//         typename _Alty_traits::pointer,
//         typename _Alty_traits::const_pointer,
//         _Ty&,
//         const _Ty&,
//         _Nodeptr
//     >
// >;