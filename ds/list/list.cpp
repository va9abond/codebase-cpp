#include "iterator.cpp"
// #include <exception>
// #include <limits.h>
#include <limits.h>
#include <limits>
#include <memory>
#include <stdlib.h>
#include <utility>


template <
    class Ty_,
    template <class ...> class List_node_ = DListNode // ERROR HERE: linke_list<int, DListNode<std::string>> list;
>
class linked_list { // biderectional double linked list
private:
    using Node_    = List_node_<Ty_>;
    using Nodeptr_ = List_node_<Ty_>*;

public:
    using value_type      = Ty_;
    using allocator_type  = typename List_node_<Ty_>::allocator_type;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = _List_unchecked_iterator<Ty_>;
    using const_iterator = _List_unchecked_const_iterator<Ty_>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    /* Ctors, Dtor & Assignment Operators */

    linked_list() noexcept :
        _size(0),
        _head(nullptr)
    {}

    // TODO: need check
    explicit linked_list (size_t size, const_reference value = Ty_() ) :
    // **be careful**
    // type Ty_ must have default ctor
        // _size(size),
        _head(nullptr)
    {
        try {
            if (size <= 0 || size > UINT_MAX) {
                _size = 0;
                throw _ml exception("Invalid size");
            }

            _ALLOCATE_HEAD();
            _head->_prev = _head->_next = new Node_(value, _head, _head);
            
            while (size --> 0) { 
                _EMPLACE_BACK( std::move( Ty_(value) ) ); 
                // there I use std::move instead of std::forward, because
                // Ty_(data) return rvalue of type Ty_
            }
        }
        catch (_ml exception invalid_size) {
            std::cerr << invalid_size.what();
        }
    }

protected:
    //TODO: need to check
    void _ALLOCATE_HEAD() noexcept {
        _head = static_cast<Nodeptr_>(::operator new(sizeof(Node_)));
    }

    // TODO: need check
    Nodeptr_ _EMPLACE (Nodeptr_ where, Ty_&& data) noexcept { // create element at where
        try {
            
            if (_size == max_size()) {
                throw _ml exception("list too long");
            }

            Nodeptr_ new_node = new Node_(std::forward<Ty_>(data), where->_prev, where);
            where->_prev->_next = new_node;
            where->_prev = new_node;

            _size++;
            return new_node;
        }
        catch (_ml exception length_error) {
            std::cerr << length_error.what();
        }
    }

    Nodeptr_ _EMPLACE_BACK (Ty_&& data) noexcept {
        return _EMPLACE(_head, std::forward<Ty_>(data));
    }

    Nodeptr_ _EMPLACE_FRONT (Ty_&& data) noexcept {
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
            _head->_prev = _head->_next = new Node_(std::move(init_list[0]), _head, _head);
            
            for (size_t i = 1; i < size; i++) {
                _EMPLACE_BACK(std::move(init_list[i]));
            }
        }
    }


    /* Iterators */


    iterator begin() noexcept {

    }

    const_iterator begin() const noexcept {

    }

    iterator end() noexcept {

    }

    const_iterator end() const noexcept {

    }

protected:

    iterator _MAKE_ITER (Nodeptr_ Where_) const noexcept {

    }

    const_iterator _MAKE_CONST_ITER (Nodeptr_ Where_) const noexcept {

    }

public:

    // TODO: reverse iterators

    const_iterator cbegin() const noexcept {

    }

    const_iterator cend() const noexcept {

    }

    // TODO: const reverse iterators


    /* Accessors & Mutators */


    Ty_& operator[] (size_t index) const {
        try {
            if (index < 0 || index >= _size) {
                throw _ml exception("Invalid index");
            }

            auto node_it = _head->_next;
            for (size_t i = 0; i < index; i++) {
                node_it = node_it->_next;
            }

            return node_it->_data;
        }
        catch (_ml exception invalid_index) {
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
                throw _ml exception("front() called on empty list");
            }

            return _head->_next->_data;     
        }
        catch (_ml exception length_error) {
            std::cerr << length_error.what();
        }
    }  
        
    const_reference front() const {
        try {
            if ( _size == 0 ) {
                throw _ml exception("front() called on empty list");
            }

            return _head->_next->_data;     
        }
        catch (_ml exception length_error) {
            std::cerr << length_error.what();
        }
    }


    reference back() { 
        try { 
            if ( _size == 0 ) {
                throw _ml exception("back() called on empty list");
            }

            return _head->_prev->_data;     
        }
        catch (_ml exception length_error) {
            std::cerr << length_error.what();
        }
    }
        
    const_reference back() const { 
        try {
            if ( _size == 0 ) {
                throw _ml exception("back() called on empty list");
            }

            return _head->_prev->_data;     
        }
        catch (_ml exception length_error) {
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
                throw _ml exception("pop_front() called on empty list");
            }

            _UNCHECKED_ERASE(_head->_next);
        }
        catch (_ml exception length_error) {
            std::cerr << length_error.what();
        }
    }

    void pop_back() {
        try {
            if (_size == 0) {
                throw _ml exception("pop_back() called on empty list");
            }

            _UNCHECKED_ERASE(_head->_prev);
        }
        catch (_ml exception length_error) {
            std::cerr << length_error.what();
        }
    }

    iterator emplace (const const_iterator where, value_type&& data) { // insert element at where

    }

    reference emplace_front (value_type&& data) { // insert element at beginning

    }

    reference emplace_back (value_type&& data) { // insert element at end

    }

    iterator insert (const_iterator where, const_reference data) { // insert data at where

    }

    iterator insert (const_iterator where, int count, const_reference data) { // insert count * data before where

    }

    template <
        class Iter_
    >
    iterator insert (const_iterator where, Iter_ First_, Iter_ Last_) { // insert [First_, Last_) before where

    }

    iterator erase (const_iterator where) noexcept {

    }

protected:
    
    Nodeptr_ _UNCHECKED_ERASE (const Nodeptr_ Pnode_) {

    }

public:

    iterator erase (const const_iterator First_, const const_iterator Last_) noexcept {

    }

protected:

    Nodeptr_ _UNCHECKED_ERASE (Nodeptr_ First_, const Nodeptr_ Last_) noexcept {

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
    Nodeptr_ _head;
};


int main() {
    linked_list<int, DListNode> list;
}

