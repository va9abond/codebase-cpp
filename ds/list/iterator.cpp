#include <iterator>
#include "node.cpp"

template <
    class Nty_,
    template <class ...> class List_node_ = DListNode
>
class _List_unchecked_const_iterator {
public: 
    using iterator_category = std::bidirectional_iterator_tag; 

    using Nodeptr_        = List_node_<Nty_>*;
    using value_type      = List_node_<Nty_>;
    using difference_type = std::ptrdiff_t;
    using pointer         = const List_node_<Nty_>*;
    using reference       = const List_node_<Nty_>&;



    _List_unchecked_const_iterator() noexcept : Ptr_() {}

    _List_unchecked_const_iterator (Nodeptr_ node = nullptr) noexcept : 
        Ptr_(node)
    {}
    
    ~_List_unchecked_const_iterator() = default;



    reference operator*() const noexcept {
        return *Ptr_;
    }

    pointer operator->() const noexcept { 
        return Ptr_;
    }

    explicit operator bool() const {
        return Ptr_; // is nullptr
    }

    _List_unchecked_const_iterator& operator++ () noexcept {
        Ptr_ = Ptr_->_next; 
        return *this;
    }

    _List_unchecked_const_iterator operator++(int) noexcept {
        auto tmp = *this;
        Ptr_     = Ptr_->_next;
        return tmp;
    }

    _List_unchecked_const_iterator& operator--() noexcept {
        Ptr_ = Ptr_->_prev;
        return *this;
    }

    _List_unchecked_const_iterator operator--(int) noexcept {
        auto tmp = *this;
        Ptr_     = Ptr_->_prev;
        return tmp;
    }

    bool operator== (const _List_unchecked_const_iterator& right) const noexcept {
        return Ptr_ == right.Ptr_;
    }
   
    bool operator!= (const _List_unchecked_const_iterator& right) const noexcept {
        return !(*this == right);
    }
    
    
    Nodeptr_ Ptr_;

};



template <
    class Nty_,
    template <class ...> class List_node_ = DListNode
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<Nty_, List_node_> {
public:
    using Mybase_           = _List_unchecked_const_iterator<Nty_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = List_node_<Nty_>*;
    using value_type      = List_node_<Nty_>;
    using difference_type = std::ptrdiff_t;
    using pointer         = List_node_<Nty_>*;
    using reference       = List_node_<Nty_>&;

    using Mybase_::Mybase_;



    reference operator*() const noexcept {
        return const_cast<reference>(Mybase_::operator*());
    }    

    pointer operator->() const noexcept {
        return Mybase_::Ptr_;
        // return const_cast<pointer>(Mybase_::operator->());
    }

    _List_unchecked_iterator& operator++() {
        Mybase_::operator++();
        return *this;
    }

    _List_unchecked_iterator operator++(int) {
        auto tmp = *this;
        Mybase_::operator++();
        return tmp;
    }

    _List_unchecked_iterator& operator--() {
        Mybase_::operator--();
        return *this;
    }

    _List_unchecked_iterator operator--(int) {
        auto tmp = *this;
        Mybase_::operator--();
        return tmp;
    }
};


// int main() {
//     DListNode<int>* node = new DListNode<int>(1233);
//     _List_unchecked_const_iterator<int> itt(node); 

//     std::cout << *itt;
// }


