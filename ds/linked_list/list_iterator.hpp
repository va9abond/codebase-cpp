#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP


#include "list_node.hpp"


template <
    class _Mylist,
    class _Base = _Iterator_base
>
class _List_unchecked_const_iterator : public _Base {
public:
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::const_pointer;
    using reference       = const value_type&;

    _Nodeptr _Myptr;
};


template <
    class _Mylist
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<_Mylist> {
public:
    using _Mybase = _List_unchecked_const_iterator<_Mylist>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::pointer;
    using reference       = value_type&;

    using _Mybase::_Mybase; // TODO: how it works?
};


template <
    class _Mylist
>
class _List_const_iterator : public _List_unchecked_const_iterator<_Mylist> {
public:
    using _Mybase = _List_unchecked_const_iterator<_Mylist>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::const_pointer;
    using reference       = const value_type&;

    using _Mybase::_Mybase;
};


template <
    class _Mylist
>
class _List_iterator : public _List_const_iterator<_Mylist> {
public:
    using _Mybase = _List_const_iterator<_Mylist>;
    using iterator_category = std::bidirectional_iterator_tag;

    using _Nodeptr        = typename _Mylist::_Nodeptr;
    using value_type      = typename _Mylist::value_type;
    using difference_type = typename _Mylist::difference_type;
    using pointer         = typename _Mylist::pointer;
    using reference       = value_type&;

    using _Mybase::_Mybase;
};

#endif // LISTITERATOR_HPP
