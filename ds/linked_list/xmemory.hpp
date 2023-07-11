#ifndef XMEMORY_HPP
#define XMEMORY_HPP


#include "msldef.h"


// [x] _Container_proxy
// [ ] _Container_base
//         m[ ] _Orphan_all
//         m[ ] _Swap_proxy_and_iterators
//         m[ ] _Alloc_proxy
//         m[ ] _Reload_proxy
// [ ] _Iterator_base
//         [ ] operator=
//         [ ] ~_Iterator_base
//        m[ ] _Adopt_v1
//         [ ] _Adopt_v2
//        m[ ] _Getcont
//         [ ] _Assign
//         [ ] _Orphan_me
//    
//    NOTE: it's better to use STL trails? i think so
// [?] iterator_traits
// [?] iterator_traits<PtrTy_*>
// [?] _Simple_types 
// [?] _Default_allocator_traits // NOTE: it should be remove, i think


template <class _Alloc, class _Value_type>
using _Rebind_alloc_t = typename std::allocator_traits<_Alloc>::template rebind_alloc<_Value_type>;


template <class _Value_type> 
struct _Simple_types {
    using value_type      = _Value_type;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
};


struct _Iterator_base;
struct _Container_base;


struct _Container_proxy {
    _Container_proxy() noexcept = default;
    _Container_proxy (_Container_base* Mycont) noexcept : _Mycont(Mycont) {}

    const   _Container_base* _Mycont      = nullptr;
    mutable _Iterator_base*  _Myfirstiter = nullptr;
};


struct _Container_base {
public:
    _Container_base (const _Container_base&) = delete;
    _Container_base& operator= (const _Container_base&) = delete;

    void _Orphan_all() noexcept;
    void _Swap_proxy_and_iterators (_Container_base&) noexcept;
    
    void _Alloc_proxy();

    
    _Container_proxy* _Myproxy = nullptr;
};


struct _Iterator_base {
public:
    _Iterator_base() noexcept = default; // construct orphaned iterator

    _Iterator_base (const _Iterator_base& Rhs) noexcept {
        *this = Rhs;
    }

    _Iterator_base& operator= (const _Iterator_base& Rhs) noexcept {
        _Assign(Rhs);
        return *this;
    }

    ~_Iterator_base() noexcept {
        _Orphan_me();
    }

    void _Adopt_v1 (const _Container_base* Parent) noexcept {
        if (Parent != nullptr) {
            _Myproxy = Parent->_Myproxy;
        } else {
            _Myproxy = nullptr;
        }
    } 

    const _Container_base* _Getcont() noexcept {
        return _Myproxy ? _Myproxy->_Mycont : nullptr;
    }


    mutable _Container_proxy* _Myproxy    = nullptr;
    mutable _Iterator_base*   _Mynextiter = nullptr;

private:
    // Assign this iterator to other container by given
    // iterator from other container
    void _Assign (const _Iterator_base& Rhs) noexcept {
        if (_Myproxy == Rhs._Myproxy) { return; }
        if (Rhs._Myproxy) { // != nullptr => do adoption
            _Adopt(Rhs._Myproxy->_Mycont);
        } else { // == nullptr => no parent container now
            _Orphan_me();
        }
    }

    void _Adopt (const _Container_base* Parent) noexcept {}
    void _Orphan_me() noexcept {}

};


inline void _Container_base::_Orphan_all() noexcept {}

// swap owners of proxy and iterators
inline void _Container_base::_Swap_proxy_and_iterators(_Container_base& Rhs) noexcept {}

#endif // XMEMORY_HPP
