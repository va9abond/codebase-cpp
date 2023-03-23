#include "List_iterator.cpp"
// #include <exception>
// #include <limits.h>
#include <initializer_list>
#include <limits.h>
#include <limits>
#include <memory>
#include <sal.h>
#include <stdlib.h>
#include <utility>
#include <xmemory>


_MYL_BEGIN

template <class Ty_>
struct _List_simple_types : _Simple_types<Ty_> {
    using _Node    = _List_node<Ty_>;
    using _Nodeptr = _List_node<Ty_>*;
};


template < 
    class Value_type_,
    class Size_type_,
    class Difference_type_,
    class Pointer_,
    class Const_pointer_,
    class Reference_,
    class Const_reference,
    class Nodeptr_type
>
struct _List_iter_types {
    using value_type      = Value_type_;
    using size_type       = Size_type_;
    using difference_type = Difference_type_;
    using pointer         = Pointer_;
    using const_pointer   = Const_pointer_;
    using _Nodeptr        = Nodeptr_type;
};


template <class Val_types_>
class _List_val : public _Container_base {
public:
    using _Nodeptr = typename Val_types_::_Nodeptr;

    using value_type      = typename Val_types_::value_type;
    using size_type       = typename Val_types_::size_type; // ?????????
    using difference_type = typename Val_types_::difference_type;
    using pointer         = typename Val_types_::pointer;
    using const_pointer   = typename Val_types_::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;


    _List_val() noexcept : Myhead(), Mysize(0) {} // initialize data

    void _Orphan_iterator (_Nodeptr Ptr) noexcept { // orphan iterator from node it contains to
        _Iterator_base* ItNext = this->Myfirstiter;
        const auto head = Myhead;

        while(ItNext != nullptr) {
            _Iterator_base* ItNextNext = ItNext->Mynextiter;
            const auto PtrNext = static_cast<_List_const_iterator<_List_val>&>(ItNext)._Ptr;
            
            if (PtrNext == head || PtrNext != Ptr) {
                // ItNext == end() or ItNext doesn't point at the one we are orphaning, move on
                ItNext = ItNextNext;
            } else { // orphan the iterator
                ItNext->Mycont = nullptr;
                ItNext         = ItNextNext;
                // break;
            }
        }
    }

    _Nodeptr _Unlinknode(_Nodeptr Pnode) noexcept { // unlink Pnode from the list

        Pnode->_prev->_next = Pnode->_next;
        Pnode->_next->_prev = Pnode->_prev;
        --Mysize;
        return Pnode;
    }


    _Nodeptr  Myhead; // pointer to head node
    size_type Mysize; // number of elements
};


























































template <
    class Ty_,
    class _Alloc = _STD allocator<Ty_>
>
class linked_list { // biderectional double linked list
private:
    using _Alty          = std::_Rebind_alloc_t<_Alloc, Ty_>;
    using _Alty_traits   = std::allocator_traits<_Alty>;
    using _Node          = _List_node<Ty_>;
    using _Alnode        = std::_Rebind_alloc_t<_Alloc, _Node>;
    using _Alnode_traits = std::allocator<_Alnode>;
    using _Nodeptr       = typename _Alnode_traits::pointer;

    using _Val_types = _List_iter_types <
        Ty_,
        typename _Alty_traits::size_type,
        typename _Alty_traits::difference_type,        
        typename _Alty_traits::pointer,
        typename _Alty_traits::const_pointer,
        Ty_&,
        const Ty_&,
        _Nodeptr
    >;
    using _Scary_val = _List_val<_Val_types>;

public:
    using value_type      = Ty_;
    using allocator_type  = _Alloc;
    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_Scary_val>;
    using const_iterator            = _List_const_iterator<_Scary_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_Scary_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_Scary_val>;

    using reverse_iterator       = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;


    /* Ctors, Dtor & Assignment Operators */

    linked_list() noexcept :
        _size(0),
        _head(nullptr)
    {}

    // TODO: need check
    // **be careful** type Ty_ must have default ctor
    explicit linked_list (size_t size, const_reference value = Ty_() ) :
        // _size(size),
        _head(nullptr)
    {
        try {
            if (size <= 0 || size > UINT_MAX) {
                _size = 0;
                throw _MYL exception("Invalid size");
            }

            _ALLOCATE_HEAD();
            _head->_prev = _head->_next = new _Node(value, _head, _head);
            
            while (size --> 0) { 
                _EMPLACE_BACK( std::move( Ty_(value) ) ); 
                // there I use std::move instead of std::forward, because
                // Ty_(data) return rvalue of type Ty_
            }
        }
        catch (_MYL exception invalid_size) {
            std::cerr << invalid_size.what();
        }
    }

protected:
    //TODO: need to check
    void _ALLOCATE_HEAD() noexcept {
        _head = static_cast<_Nodeptr>(::operator new(sizeof(_Node)));
        // ::operator new return void*, i need to cast void* to _Nodeptr by static_cast
    }

    // TODO: need check
    _Nodeptr _EMPLACE (_Nodeptr where, Ty_&& data) noexcept { // create element at where
        try {
            
            if (_size == max_size()) {
                throw _MYL exception("list too long");
            }

            _Nodeptr new_node = new _Node(std::forward<Ty_>(data), where->_prev, where);
            where->_prev->_next = new_node;
            where->_prev = new_node;

            _size++;
            return new_node;
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }

    _Nodeptr _EMPLACE_BACK (Ty_&& data) noexcept {
        return _EMPLACE(_head, std::forward<Ty_>(data));
    }

    _Nodeptr _EMPLACE_FRONT (Ty_&& data) noexcept {
        return _EMPLACE(_head->_next, std::forward<Ty_>(data));
    }


public:
    // TODO: need check
    linked_list (const linked_list& right) noexcept : // deep copy
        // _size(right._size),
        _head(nullptr)
    {
        if (_size != 0)
        {
            _ALLOCATE_HEAD();
            
            auto right_nodeptr = right._head->_next;
            while (right_nodeptr != nullptr) 
            {
                _EMPLACE_BACK( Ty_(right_nodeptr->_data) );
                right_nodeptr = right_nodeptr->_next;
            }

        }
    }

    ~linked_list()
    { 
        this->clear();
        ::operator delete(_head);
    }

    // TODO: check
    linked_list& operator= (const linked_list& right) noexcept {
        if (std::addressof(*this) != std::addressof(right)) 
        {
            this->clear();

            if (right._size != 0)
            {   
                auto right_nodeptr = right._head->_next;
                while (right_nodeptr != nullptr) 
                {
                    _EMPLACE_BACK( Ty_(right_nodeptr->_data) );
                    right_nodeptr = right_nodeptr->_next;
                }
            }
        }

        return *this;
    }

    // TODO: need check
    linked_list (linked_list&& right) noexcept : // shallow copy
        _size(right._size),
        _head(nullptr) // _head(std::move(right._head))
    {
        _ALLOCATE_HEAD();
        _head->_prev = right._head->_prev;
        _head->_next = right._head->_next; // TODO: SWAP HEADS

        right._size  = 0;
        right._head = nullptr; //TODO: operator delete(right._head); MEMORY LEAK
    }

    // TODO: need check
    linked_list& operator= (linked_list&& right) noexcept {
        if (std::addressof(*this) != std::addressof(right)) 
        {
            this->clear();

            _size  = right._size;  
            _head = right._head; 

            right._size = 0;
            right._head = nullptr; // ::operator delete(right._head); MEMORY LEAK
        }

        return *this;
    }

    // TODO: need check
    template <
        class T_,
        size_t size
    >
    linked_list (T_ (&&init_list)[size]) noexcept :
        // _size(size),
        _head(nullptr)
    {
        if (size > 0)
        {
            _ALLOCATE_HEAD();
            _head->_prev = _head->_next = new _Node(std::move(init_list[0]), _head, _head);
            
            for (size_t i = 1; i < size; i++) {
                _EMPLACE_BACK(std::move(init_list[i]));
            }
        }
    }


    /* Iterators */


    iterator begin() noexcept {
        return iterator(_head->_next);
    }

    const_iterator begin() const noexcept {
        return const_iterator(_head->_next);
    }

    iterator end() noexcept {
        return iterator(_head);
    }

    const_iterator end() const noexcept {
        return const_iterator(_head);
    }

protected:

    iterator _MAKE_ITER (_Nodeptr Where_) const noexcept {
        return iterator(Where_);
    }

    const_iterator _MAKE_CONST_ITER (_Nodeptr Where_) const noexcept {
        return const_iterator(Where_);
    }

public:

    // TODO: reverse iterators

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    // TODO: const reverse iterators


    /* Accessors & Mutators */


    Ty_& operator[] (size_t index) const {
        try {
            if (index < 0 || index >= _size) {
                throw _MYL exception("Invalid index");
            }

            auto node_it = _head->_next;
            for (size_t i = 0; i < index; i++) {
                node_it = node_it->_next;
            }

            return node_it->_data;
        }
        catch (_MYL exception invalid_index) {
            std::cerr << invalid_index.what();
        }
    }

    size_t size() const noexcept {
        return _size;
    }

    size_t max_size() const noexcept {
        return UINT_MAX;
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    reference front() { 
        try {
            if ( _size == 0 ) {
                throw _MYL exception("front() called on empty list");
            }

            return _head->_next->_data;     
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }  
        
    const_reference front() const {
        try {
            if ( _size == 0 ) {
                throw _MYL exception("front() called on empty list");
            }

            return _head->_next->_data;     
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }


    reference back() { 
        try { 
            if ( _size == 0 ) {
                throw _MYL exception("back() called on empty list");
            }

            return _head->_prev->_data;     
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }
        
    const_reference back() const { 
        try {
            if ( _size == 0 ) {
                throw _MYL exception("back() called on empty list");
            }

            return _head->_prev->_data;     
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }


    /* Modifiers */


    void push_front (value_type&& data) {
        _EMPLACE(_head->_next, std::move(data));
        // _EMPLACE_FRONT(std::move(data));
    }

    void push_back (value_type&& data) {
        _EMPLACE(_head, std::move(data));
        // _EMPLACE_BACK(std::move(data));
    }

    void push_front (const_reference data) {
        _EMPLACE(_head->_next, data);
    }

    void push_back (const_reference data) { // TODO: vs. _EMPLACE(_head, data);
        _EMPLACE_BACK(data);
    }

    void pop_front() {
        try {
            if (_size == 0) {
                throw _MYL exception("pop_front() called on empty list");
            }

            _UNCHECKED_ERASE(_head->_next);
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }

    void pop_back() {
        try {
            if (_size == 0) {
                throw _MYL exception("pop_back() called on empty list");
            }

            _UNCHECKED_ERASE(_head->_prev);
        }
        catch (_MYL exception length_error) {
            std::cerr << length_error.what();
        }
    }

    iterator emplace (const const_iterator Where_, value_type&& data) { // insert element at Where_
        return _MAKE_ITER(_EMPLACE(Where_.Ptr_, std::forward<value_type>(data)));
    }

    reference emplace_front (value_type&& data) { // insert element at beginning
        reference Result_ = _EMPLACE(_head->_next, std::forward<value_type>(data))->_data;
        return Result_;
    }

    reference emplace_back (value_type&& data) { // insert element at end
        reference Result_ = _EMPLACE(_head, std::forward<value_type>(data))->_data;
    }

    iterator insert (const_iterator Where_, std::initializer_list<Ty_> Ilist_) { // insert initializer_list
        return insert(Where_, Ilist_.begin(), Ilist_.end());
    }

    iterator insert (const_iterator Where_, const_reference data) { // insert data at Where_
        return _MAKE_ITER(_EMPLACE(Where_.Ptr_, data));
    }

    iterator insert (const_iterator where, int count, const_reference data) { // insert count * data before where

    }

    template <
        class Iter_
    >
    iterator insert (const_iterator where, Iter_ First_, Iter_ Last_) { // insert [First_, Last_) before where

    }

    iterator erase (const_iterator Where_) { // erase element at Where_
        // TODO: check if iterator outside list 
        const auto Result_ = Where_.Ptr_->_next;
        _FREENODE(_UNLINK_NODE(Where_.Ptr_));
    }

protected:

    _Nodeptr _UNLINK_NODE (_Nodeptr Dnode_) { // detach Dnode_ from list   
        Dnode_->_prev->_next = Dnode_->_next;
        Dnode_->_next->_prev = Dnode_->_prev;

        Dnode_->_prev = Dnode_->_next = nullptr;
        return Dnode_;
    }

    void _FREENODE (_Nodeptr Pnode_) { // DANGEROUS //
        delete Pnode_;
    }
    
    _Nodeptr _UNCHECKED_ERASE (const _Nodeptr Pnode_) noexcept { // erase element at Pnode_
        const auto Result_ = Pnode_->_next;
        // TODO: orphan iterators with specified node pointers      
        Pnode_->_prev->_next = Result_;
        Result_->_prev       = Pnode_->_prev;
        
        _FREENODE(Pnode_);
        _size--;
        return Result_;
    }

public:

    iterator erase (const const_iterator First_, const const_iterator Last_) noexcept {

    }

protected:

    _Nodeptr _UNCHECKED_ERASE (_Nodeptr First_, const _Nodeptr Last_) noexcept {

    }

public:

    auto remove (const_reference data) { // erase each element matching data

    }

    template <
        class PredTy_
    >
    auto remove_if (PredTy_ Pred_) { // erase each element satisfying Pred_

    }

    void clear() noexcept {

    }

    void resize (size_t newSize) {

    }

    void resize (size_t newSize, const_reference data) {

    }



protected:
    size_t   _size;
    _Nodeptr _head;
};


_MYL_END