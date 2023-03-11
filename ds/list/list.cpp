#include "iterator.cpp"
// #include <exception>
// #include <limits.h>
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



    linked_list() noexcept :
        _size(0),
        _head(nullptr)
    {}

    // TODO: need check
    explicit linked_list (size_t size, const_reference data = Ty_() ) :
    // **be careful**
    // type Ty_ must have default ctor
        _size(size),
        _head(nullptr)
    {
        try 
        {
            if (size <= 0 || size > _MAX_CAPACITY) {
                _size = 0;
                throw _ml exception("Invalid size");
            }

            _ALLOCATE_HEAD();
            _head->_prev = _head->_next = new Node_(data, _head, _head);
            
            while (size --> 0) { 
                _EMPLACE_BACK_NOCHECK( std::move( Ty_(data) ) ); 
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
    Nodeptr_ _EMPLACE_NOCHECK (Nodeptr_ where, Ty_&& data) noexcept { // new element will be emplaced at where
        
        Nodeptr_ new_node = new Node_(std::forward<Ty_>(data), where->_prev, where);
        
        where->_prev->_next = new_node;
        where->_prev = new_node;

        return new_node;
    }

    Nodeptr_ _EMPLACE_BACK_NOCHECK (Ty_&& data) noexcept {
        return _EMPLACE_NOCHECK(_head, std::forward<Ty_>(data));
    }

    Nodeptr_ _EMPLACE_FRONT_NOCHECK (Ty_&& data) noexcept {
        return _EMPLACE_NOCHECK(_head->_next, std::forward<Ty_>(data));
    }


public:
    // TODO: need check
    linked_list (const linked_list& right) noexcept : // deep copy
        _size(right._size),
        _head(nullptr)
    {
        if (_size != 0)
        {
            _ALLOCATE_HEAD();
            
            auto right_nodeptr = right._head->_next;
            while (right_nodeptr != nullptr) 
            {
                _EMPLACE_BACK_NOCHECK( Ty_(right_nodeptr->_data) );
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
                    _EMPLACE_BACK_NOCHECK( Ty_(right_nodeptr->_data) );
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
        _head->_next = right._head->_next;

        right._size  = 0;
        right._head = nullptr; // ::operator delete(right._head); MEMORY LEAK
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
        _size(size),
        _head(nullptr)
    {
        if (size > 0)
        {
            _ALLOCATE_HEAD();
            _head->_prev = _head->_next = new Node_(std::move(init_list[0]), _head, _head);
            
            for (size_t i = 1; i < size; i++) {
                _EMPLACE_BACK_NOCHECK(std::move(init_list[i]));
            }
        }
    }

    Ty_& operator[] (size_t index) const {
        try
        {
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







    void clear() noexcept {}

private:






protected:
    size_t   _size;
    Nodeptr_ _head;
    static const unsigned int _MAX_CAPACITY = UINT_MAX;
};


int main() {
    linked_list<int, DListNode> list;
}

// Чего я хочу? Я хочу, чтобы 
//     list.front()->prev = head;
//     list.back()->next = head;
//     head->prev = list.back(); 
//     list.end() = head;
//     list.begin() = head->next
