#ifndef LISTITERATOR_HPP
#define LISTITERATOR_HPP


#include "list_node.hpp"


_MSL_BEGIN
// [x] _List_unchecked_const_terator
// [ ] _List_unchecked_iterator
// [ ] _List_const_iterator
// [ ] _List_iterator


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

    _List_unchecked_const_iterator() noexcept : _Myptr() {}

    _List_unchecked_const_iterator (_Nodeptr Parent_node, const _Mylist* Parent_list) noexcept : _Myptr(Parent_node) {
        _Adopt_me(Parent_list); 
    }

    reference operator*() const noexcept {
        return _Myptr->_Myval;
    } 

    pointer operator->() const noexcept {
        return this;
    }

    explicit operator bool() const noexcept { // type conversation iterator to bool
                                              // _Myptr == nullptr ? true : false;
        return _Myptr;
    }

    _List_unchecked_const_iterator& operator++() noexcept {
        _Myptr = _Myptr->_Next;
        return *this;
    }

    _List_unchecked_const_iterator& operator++(int) noexcept {
        _List_unchecked_const_iterator Temp = *this;
        _Myptr = _Myptr->_Next;
        return Temp;
    }

    _List_unchecked_const_iterator& operator--() noexcept {
        _Myptr = _Myptr->_Prev;
        return *this;
    }

    _List_unchecked_const_iterator& operator--(int) noexcept {
        _List_unchecked_const_iterator Temp = *this;
        _Myptr = _Myptr->_Prev;
        return Temp;
    }

    bool operator== (const _List_unchecked_const_iterator& Rhs) const noexcept {
        return _Myptr == Rhs._Myptr;
    }
    
    bool operator!= (const _List_unchecked_const_iterator& Rhs) const noexcept {
        return _Myptr != Rhs._Myptr;
    }


    _Nodeptr _Myptr; // pointer to node
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

_MSL_END
#endif // LISTITERATOR_HPP
