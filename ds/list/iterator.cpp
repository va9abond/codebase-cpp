#include <iterator>
#include "list.cpp"

template <
    class Nty_
>
class iterator_base {
public: 
    
    // tags:
    using iterator_category  = std::bidirectional_iterator_tag; 

    using Nodeptr_           = Node<Nty_>*;
    using value_type         = Nty_;
    using difference_type    = std::ptrdiff_t;
    using pointer            = Nty_*;
    using const_pointer      = const Nty_*;
    using reference          = Nty_&;
    using const_reference    = const Nty_&;



    explicit iterator_base (pointer item = nullptr) : _item(item) {}
    
    iterator_base (const iterator_base&) = default;
    
    iterator_base (iterator_base&&) noexcept = default;
    
    iterator_base& operator= (const iterator_base&) = default;
    
    iterator_base& operator= (iterator_base&&) noexcept = default;
    
    iterator_base& operator= (pointer item) {
        _item = item;
        return *this;
    }

    ~iterator_base() = default;



    explicit operator bool() const {
        return _item; // is nullptr
    }



    bool operator== (const iterator_base& itt_ref) const noexcept { return _item == itt_ref._item; }
   
    bool operator!= (const iterator_base& itt_ref) const noexcept {return !(*this == itt_ref); }

    iterator_base& operator++ () noexcept {
        ++_item; // why not item_++;
        return *this;
    }

    iterator_base operator++(int) noexcept {
        auto tmp = *this;
        ++_item;

        return tmp;
    }

    iterator_base& operator-- () noexcept {
        --_item; // why not item_++;
        return *this;
    }

    iterator_base operator--(int) noexcept {
        auto tmp = *this;
        --_item;

        return tmp;
    }



    reference operator* () { return *_item; }

    const_reference operator* () const { return *_item; }
    
    pointer operator-> () const { return _item; }



private:
    pointer _item;

};



template <
    class Nty_
>
class _List_unchecked_const_iterator {
public: 
    using iterator_category = std::bidirectional_iterator_tag; 

    using Nodeptr_        = Node<Nty_>*;
    using value_type      = Node<Nty_>;
    using difference_type = std::ptrdiff_t;
    using pointer         = const Node<Nty_>*;
    using reference       = const Node<Nty_>&;



    _List_unchecked_const_iterator() noexcept : ptr_() {}

    _List_unchecked_const_iterator (Nodeptr_ node = nullptr) noexcept : 
        ptr_(node)
    {}
    
    ~_List_unchecked_const_iterator() = default;

    // _List_unchecked_const_iterator (const _List_unchecked_const_iterator&) = default;
    
    // _List_unchecked_const_iterator (_List_unchecked_const_iterator&&) noexcept = default;
    
    // _List_unchecked_const_iterator& operator= (const _List_unchecked_const_iterator&) = default;
    
    // _List_unchecked_const_iterator& operator= (_List_unchecked_const_iterator&&) noexcept = default;
    
    // _List_unchecked_const_iterator& operator= (pointer item) {
    //     ptr_ = item;
    //     return *this;
    // }

    reference operator*() const noexcept {
        return *ptr_;
    }

    pointer operator->() const noexcept { 
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_; // is nullptr
    }

    _List_unchecked_const_iterator& operator++ () noexcept {
        ptr_ = ptr_->_next; 
        return *this;
    }

    _List_unchecked_const_iterator operator++(int) noexcept {
        auto tmp = *this;
        ptr_     = ptr_->_next;
        return tmp;
    }

    _List_unchecked_const_iterator& operator--() noexcept {
        ptr_ = ptr_->_prev;
        return *this;
    }

    _List_unchecked_const_iterator operator--(int) noexcept {
        auto tmp = *this;
        ptr_     = ptr_->_prev;
        return tmp;
    }

    bool operator== (const _List_unchecked_const_iterator& right) const noexcept {
        return ptr_ == right.ptr_;
    }
   
    bool operator!= (const _List_unchecked_const_iterator& right) const noexcept {
        return !(*this == right);
    }


private:
    Nodeptr_ ptr_;

};


template <
    class Nty_
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<Nty_> {
public:
    using Mybase_           = _List_unchecked_const_iterator<Nty_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = Node<Nty_>*;
    using value_type      = Node<Nty_>;
    using difference_type = std::ptrdiff_t;
    using pointer         = Node<Nty_>*;
    using reference       = Node<Nty_>&;

    using Mybase_::Mybase_;

    reference operator*() const noexcept {
        return const_cast<reference>(Mybase_::operator*());
    }    

    pointer operator->() const noexcept {
        return Mybase_::ptr_;
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

// template <
//     class Nty_
// >
// class _List_const_iterator : public _List_unchecked_const_iterator<Nty_> {
// public:
//     using Mybase_           = _List_unchecked_const_iterator<Nty_>;
//     using iterator_category = std::bidirectional_iterator_tag;
    
//     using Nodeptr_        = Node<Nty_>*;
//     using value_type      = Node<Nty_>;
//     using difference_type = std::ptrdiff_t;
//     using pointer         = Node<Nty_>*;
//     using reference       = Node<Nty_>&;

//     using Mybase_::Mybase_;

//     reference operator*() const noexcept {

//     }

// };


template <
    class Ty_,
    class Alloc_ = std::allocator<Ty_>
>
class list { // biderectional linked list
private:
    using Node_    = Node<Ty_>;
    using Nodeptr_ = Node<Ty_>*;

public:
    using value_type      = Ty_;
    using allocator_type  = Alloc_;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator = _List_unchecked_iterator<Ty_>;

};