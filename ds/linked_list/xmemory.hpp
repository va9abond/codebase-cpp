#include "msldef.h"


// [x] _Container_proxy
// [x] _Container_base
//         [?] _Orphan_all
//         [x] _Swap_proxy_and_iterators
//         [?] _Alloc_proxy
// [ ] _Iterator_base
//         [x] operator=
//         [x] ~_Iterator_base
//         [x] _Adopt_v1
//         [ ] _Adopt_v2
//         [x] _Getcont
//         [x] _Assign
//         [?] _Orphan_me
//    
//    NOTE: it's better to use STL trails? i think so
// [?] iterator_traits
// [?] iterator_traits<PtrTy_*>
// [?] _Simple_types 
// [?] _Default_allocator_traits // NOTE: it should be remove, i think

template <
    class ptrTy
>
constexpr auto _Unfancy (ptrTy Ptr) noexcept { // converts from a fancy pointer to a plain pointer
    return std::addressof(*Ptr);
    // A fancy pointer is just a term for something that acts like a pointer but is not. // copy from STL
}


template <
    class Ty 
>
constexpr Ty* _Unfancy (Ty* Ptr) noexcept { // do nothing with plain pointer // copy from STL
    return Ptr;
}


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
    void _Alloc_proxy (_Alloc&& _Al) { // TODO: where delete?
        _Container_proxy* const New_proxy = new _Container_proxy(this);
        _Myproxy = New_proxy;
        // New_proxy->_Mycont = this;
    }

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
    void _Assign (const _Iterator_base& Rhs) noexcept {
        if (_Myproxy == Rhs._Myproxy) { return; }
        
        if (Rhs._Myproxy) {
            _Adopt_v1(Rhs._Myproxy->_Mycont);
        } else { // becoming invalid, disown current parent
            _Orphan_me();
        }
    }

    void _Adopt_v2 (const _Container_base* Parent) noexcept {
        // TODO: impl 
    }

    void _Orphan_me() noexcept {
        if (!_Myproxy) { return; } // already orphaned

        // else iterator has been adopted, it should be removed from list
        _Iterator_base** Pnext = &_Myproxy->_Myfirstiter;
        while (*Pnext && *Pnext != this) {
            Pnext = &(*Pnext)->_Mynextiter;
        }

        _MSL_VERIFY_f(*Pnext, "ITERATOR LIST CORRUPTED");
        *Pnext = _Mynextiter; // <=> this = _Mynextiter, but we need there Pnext
                              // to make assign, expression (this = _Mynextiter)
                              // or (this = *_Mynextiter) cause error(idk why)
        _Myproxy = nullptr; // why _Myproxy should be nullptr now, this points to
                            // _Mynextiter, doesn't it? And _Mynextiter still
                            // child of current container
    }
};


void _Container_base::_Orphan_all() noexcept {
    if (!_Myproxy) { return; } // no proxy, already orphaned
    
    // else proxy allocated, should free it
    // TODO: does this for really work?
    for (_Iterator_base** Pnext = &_Myproxy->_Myfirstiter; Pnext; Pnext = &(*Pnext)->_Mynextiter) {
        (*Pnext)->_Myproxy = nullptr;
    }
}

// swap owners of proxy and iterators
void _Container_base::_Swap_proxy_and_iterators(_Container_base& Rhs) noexcept {
    // swap owners of iterators
    _Container_proxy* temp = _Myproxy;
    _Myproxy = Rhs._Myproxy;
    Rhs._Myproxy = temp;

    // swap owners of proxy
    if (_Myproxy) {
         _Myproxy->_Mycont = this;
    }

    if (Rhs._Myproxy) {
        Rhs._Myproxy->_Mycont = &Rhs;
    } 
}
