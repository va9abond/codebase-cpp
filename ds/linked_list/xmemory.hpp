#include "msldef.h"


// [ ] _Container_proxy
// [ ] _Container_base
//         [ ] _Orphan_all
//         [ ] _Swap_proxy_and_iterators
//         [ ] _Alloc_proxy
// [ ] _Iterator_base
//         [ ] operator=
//         [ ] ~_Iterator_base
//         [ ] _Adopt_v1
//         [ ] _Adopt_v2
//         [ ] _Getcont
//         [ ] _Assign
//         [ ] _Orphan_me
//    
//    NOTE: it's better to use STL trails? i think so
// [?] iterator_traits
// [?] iterator_traits<PtrTy_*>
// [?] _Simple_types 
// [?] _Default_allocator_traits // NOTE: it should be remove, i think



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

    template <
        class _Alloc
    >
    void _Alloc_proxy (_Alloc&& _Al) {}


    _Container_proxy* _Myproxy = nullptr;
};


struct _Iterator_base {
public:
    _Iterator_base() noexcept = default; // construct orphaned iterator

    _Iterator_base (const _Iterator_base& Rhs) noexcept {
        *this = Rhs;
    }

    _Iterator_base& operator= (const _Iterator_base& Rhs) noexcept {
        _Assign(Rhs); // TODO: impl _Assign
        _Myproxy = Rhs._Myproxy;
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
    void _Assign (const _Iterator_base& Rhs) noexcept {}
    void _Adopt_v2 (const _Container_base* Parent) noexcept {}
    void _Orphan_me() noexcept {}

};


inline void _Container_base::_Orphan_all() noexcept {}

// swap owners of proxy and iterators
inline void _Container_base::_Swap_proxy_and_iterators(_Container_base& Rhs) noexcept {}
