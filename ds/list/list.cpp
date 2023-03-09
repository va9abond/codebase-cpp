#include "iterator.cpp"
// #include <exception>
// #include <limits.h>
#include <limits>
#include <memory>


template <
    class Ty_,
    template <class ...> class List_node_ = DListNode,
    class Alloc_ = std::allocator<Ty_>
>
class linked_list { // biderectional double linked list
private:
    using Node_    = List_node_<Ty_>;
    using Nodeptr_ = List_node_<Ty_>*;

public:
    using value_type      = Ty_;
    using allocator_type  = Alloc_;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator       = _List_unchecked_iterator<Ty_>;
    using const_iterator = _List_unchecked_const_iterator<Ty_>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;



    linked_list() noexcept :
        _size(0),
        _head(nullptr),
        _tail(nullptr)
    {}

    // TODO: need check
    explicit linked_list (size_t size, const Ty_ &data = Ty_() ) :
        _size(size),
        _head(nullptr),
        _tail(nullptr)
    {
        try 
        {
            if (size <= 0 || size > _MAX_CAPACITY) {
                _size = 0;
                throw _ml exception("Invalid size");
            }

            _head = _tail = new Node_(data);
            for (size_t s = size; s > 0; s--)
            {
                auto new_node = new Node_(data); 
                PUSH_BACK_NOCHECK(new_node);
                // new_node->setPrev(_end);
                // _end->setNext(new_node); // TODO: PUSH_BACK_NOCHECK
                // _end = new_node;
            }
        }
        catch (_ml exception invalid_size) {
            std::cerr << invalid_size.what();
        }
    }

    // TODO: need check
    linked_list (const linked_list& right) noexcept : // deep copy
        _size(right._size),
        _tail(nullptr),
        _head(nullptr)
    {
        if (_size != 0)
        {
            _head = _tail = new Node_( *(right._head) );
            
            auto right_nodeptr = right._head->_next;
            while (right_nodeptr != nullptr) 
            {
                auto new_node = new Node_(*right_nodeptr);
                PUSH_BACK_NOCHECK(new_node);
                // new_node->setPrev(_end);
                // _end->setNext(new_node); // TODO: PUSH_BACK_NOCHECK
                // _end = new_node;

                right_nodeptr = right_nodeptr->_next;
            }

        }
    }

    ~linked_list() { this->clear(); }

    // TODO: check
    linked_list& operator= (const linked_list& right) noexcept {
        if (std::addressof(*this) == std::addressof(right)) { return *this; }

        this->clear();

        if (right._size != 0)
        {
            _head = _tail = new Node_( *(right._head) );
            
            auto right_nodeptr = right._head->_next;
            while (right_nodeptr != nullptr) 
            {
                auto new_node = new Node_(*right_nodeptr);
                PUSH_BACK_NOCHECK(new_node);
                // new_node->setPrev(_end);
                // _end->setNext(new_node); // TODO: PUSH_BACK_NOCHECK
                // _end = new_node;

                right_nodeptr = right_nodeptr->_next;
            }
        }
        return *this;
    }

    // TODO: need check
    linked_list (linked_list&& right) noexcept : // shallow copy
        _size(right._size),
        _head(right._head),
        _tail(right._tail)
    {
        right._size  = 0;
        right._head = nullptr;
        right._tail   = nullptr;
    }

    // TODO: need check
    linked_list& operator= (linked_list&& right) noexcept {
        if (std::addressof(*this) == std::addressof(right)) { return *this; }

        this->clear();

        _size  = right._size;  
        _head = right._head; 
        _tail   = right._tail;

        right._size  = 0;
        right._head = nullptr;
        right._tail   = nullptr;   

        return *this;
    }

    template <
        class T_,
        size_t size
    >
    linked_list (T_ (&&init_list)[size]) noexcept :
        _size(size),
        _head(nullptr),
        _tail(nullptr)
    {
        if (size > 0) {
            _head = _tail = new Node_(std::move(init_list[0]));
            
            size_t i = 1; Node_* new_node;
            while (i < size) {
                new_node = new Node_(std::move(init_list[i++]));
                PUSH_BACK_NOCHECK(new_node);
                // new_node->setPrev(_end);
                // _end->setNext(new_node); // TODO: PUSH_BACK_NOCHECK
                // _end = new_node;
            }
        }
    }

    Ty_& operator[] (size_t index) const { // index is a copy of argument
        try
        {
            if (index < 0 || index >= _size) {
                throw _ml exception("Invalid index");
            }

            auto node = _head;
            while (index--) { // thats why original index doesnt change
                node = node->_next;
            }

            return node->_data;
        }
        catch (_ml exception invalid_index) {
            std::cerr << invalid_index.what();
        }
    }

    



protected:
    Nodeptr_ PUSH_BACK_NOCHECK (Nodeptr_ new_node) noexcept {
        // **be careful**                                            
        // new_node may have a not nullptr as the _next                                                      
        
        new_node->_prev = _tail;    // set _prev for new_node
        _tail->next     = new_node; // set _next for old _end 
        _tail           = new_node; // set new _end of container

        return new_node; // this is new _end element of the container
    }


    






    void clear() noexcept {}

private:






protected:
    size_t   _size;
    Nodeptr_ _head;
    Nodeptr_ _tail;
    static const unsigned int _MAX_CAPACITY = UINT_MAX;
};


int main() {
    linked_list<int, DListNode> list;
}