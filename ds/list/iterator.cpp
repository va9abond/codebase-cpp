#include <iterator>
#include "node.cpp"

template <
    class Mylist_,
    class Base_ = _MYL Iterator_base_
>
class _List_unchecked_const_iterator : public Base_ {
public: 
    using iterator_category = std::bidirectional_iterator_tag; 

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&;



    _List_unchecked_const_iterator() noexcept : Ptr_() {}

    _List_unchecked_const_iterator (Nodeptr_ Pnode, const Mylist_* Plist) noexcept : 
        Ptr_(Pnode)
    {
        this->Adopt_(Plist);
    }
    
    // ~_List_unchecked_const_iterator() = default;

    reference operator*() const noexcept {
        return Ptr_->_myval;
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


    Nodeptr_ Ptr_; // pointer to node

};



template <
    class Mylist_
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<Mylist_> {
public:
    using Mybase_           = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&;

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



template <
    class Mylist_
>
class _List_const_iterator : public _List_unchecked_const_iterator<Mylist_> {
public:
    using Mybase_ = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&; 

    using Mybase_::Mybase_;


    reference operator*() const {
        try {
            const auto Mycont = static_cast<const Mylist_*>(this->Getcont_());
            if (this->Ptr_ == Mycont->) 
        }
        catch (...) {

        }
    }
};

// int main() {
//     DListNode<int>* node = new DListNode<int>(1233);
//     _List_unchecked_const_iterator<int> itt(node); 

//     std::cout << *itt;
// }


