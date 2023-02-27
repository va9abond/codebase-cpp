#include "iterator.cpp"
// #include <exception>
// #include <limits.h>
#include <limits>


template <
    class Ty_,
    template <class ...> class List_node_ = Node,
    class Alloc_ = std::allocator<Ty_>
>
class linked_list { // biderectional linked list
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
        _begin(nullptr),
        _end(nullptr)
    {}
    // TODO: need check
    explicit linked_list (size_t size, const Ty_ &data = Ty_() ) :
        _size(size),
        _begin(nullptr),
        _end(nullptr)
    {
        try 
        {
            if (size <= 0 || size > _MAX_CAPACITY) {
                throw _ml exception("Invalid size");
            }

            _end = _begin = new Node_(data);
            for (size_t s = size; s > 0; s--)
            {
                auto new_node = new Node_(data); 
                new_node->setPrev(_end);
                _end->setNext(new_node); // TODO: PUSH_BACK_NOCHECK
                _end = new_node;
            }
        }
        catch (_ml exception invalid_size) {
            std::cerr << invalid_size.what();
        }
    }
    // TODO: need check
    linked_list (const linked_list& right) noexcept : // deep copy
        _size(right._size),
        _end(nullptr),
        _begin(nullptr)
    {
        if (_size != 0)
        {
            _end = _begin = new Node_( *(right._begin) );
            
            auto right_nodeptr = right._begin->_next;
            while (right_nodeptr != nullptr) 
            {
                auto new_node = new Node_(*right_nodeptr);
                new_node->setPrev(_end);
                _end->setNext(new_node); // TODO: PUSH_BACK_NOCHECK
                _end = new_node;

                right_nodeptr = right_nodeptr->_next;
            }

        }
    }
    // linked_list (const linked_list& right) noexcept :
    //     _size(right._size),
    //     _begin(nullptr),
    //     _end(nullptr)

    // {
    // 	if (_size != 0) {
    //         _begin = new Node_
    //     }
    // }




protected:
    size_t   _size;
    Nodeptr_ _begin;
    Nodeptr_ _end;
    static const unsigned int _MAX_CAPACITY = UINT_MAX;
};