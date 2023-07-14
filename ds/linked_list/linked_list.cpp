// #include <xmemory>
#include "msldef.h"
#include "xmemory.hpp"
#include "list_node.hpp"
#include "list_iterator.hpp"
#include <cinttypes>


_MSL_BEGIN
// [x] _List_simple_types
// [x] _List_iter_types
// [ ] _List_val
// [ ] linked_list
template <class _Value_type>
struct _List_simple_type_traits : _Simple_type_traits<_Value_type> { // _List_simple_types in STL
    using _Node    = _List_node<_Value_type>;
    using _Nodeptr = _List_node<_Value_type>*;
};


template <
    class _Value_type,
    class _Size_type,
    class _Difference_type,
    class _Pointer,
    class _Const_pointer,
    class _Reference,
    class _Const_reference,
    class _Nodeprt_type
>
struct _List_iter_traits { // _List_iter_types in STL
    using value_type      = _Value_type;
    using size_type       = _Size_type;
    using difference_type = _Difference_type;
    using pointer         = _Pointer;
    using const_pointer   = _Const_pointer;
    using _Nodeptr        = _Nodeprt_type;
};


template <class _Val_types> // NOTE: does it list for iterators that makes
                            // a base for main list?
class _List_val : public _Container_base {
public:
    using _Nodeptr = typename _Val_types::_Nodeptr;
    
    using value_type      = typename _Val_types::value_type;
    using size_type       = typename _Val_types::size_type;
    using difference_type = typename _Val_types::difference_type;
    using pointer         = typename _Val_types::pointer;
    using const_pointer   = typename _Val_types::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;    
    
    _List_val() noexcept : _Myhead(), _Mysize(0) {} // NOTE: doesn't construct _Container_base implicitly

    _List_val (const _List_val& Rhs) noexcept : _Myhead(), _Mysize(Rhs._Mysize) {} // NOTE: mb it't unneccessary

    void _Oprhan_iterators (_Nodeptr Ptr) noexcept;

    void _Orphan_non_end() noexcept;

    _Nodeptr _Unlink_node (_Nodeptr Node) noexcept; // unlink node at Where from the list

    void _Adopt_unique (_List_val& Other, _Nodeptr Node) noexcept {
        // adopt iterators pointing to spliced node
    }

    void _Adopt_all (_List_val& Other) noexcept {
        // adopt all iterators except Other.end()
    }

    void _Adopt_range (_List_val& Other, const _Nodeptr First, const _Nodeptr Last) noexcept {
        // adopt all iterators pointing to nodes in the range [First, Last) by marking nodes
    }


    _Nodeptr  _Myhead; // pointer to head node
    size_type _Mysize; // number of elements
};


template <
    class _Ty,
    class _Alloc = std::allocator<_Ty>
>
class list_v1 {
private:
    using _Alty          = _Rebind_alloc_t<_Alloc, _Ty>;
    using _Alty_traits   = std::allocator_traits<_Alty>;
    using _Node          = _List_node<_Ty>;
    using _Alnode        = _Rebind_alloc_t<_Alloc, _Node>;
    using _Alnode_traits = std::allocator_traits<_Alnode>;
    using _Nodeptr       = typename _Alnode_traits::pointer;

    using _Val_types = 
        std::conditional_t<
            _Is_simple_alloc_v<_Alnode>,
                _List_simple_type_traits<_Ty>, // simple alloc
                _List_iter_traits< // complex alloc
                    _Ty,
                    typename _Alty_traits::size_type,
                    typename _Alty_traits::difference_type,
                    typename _Alty_traits::pointer,
                    typename _Alty_traits::const_pointer,
                    _Ty&,
                    const _Ty&,
                    _Nodeptr
                >
        >;
    using _List_scary_val = _List_val<_Val_types>; // _Scary_val in STL

public:
    static_assert(std::is_same_v<_Ty, typename _Alloc::value_type>,
                  "list<T, Allocator> and T MISMATCHED ALLOCATOR");
    static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids containers of non-object types "
                                         "because of [container.requirements].");

    using value_type      = _Ty;
    using allocator_type  = _Alloc;
    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_List_scary_val>;
    using const_iterator            = _List_const_iterator<_List_scary_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_List_scary_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_List_scary_val>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // _Compressed_pair<_Alnode, _List_scary_val> _Mypair;
    _List_scary_val _Mycont;
    _Alnode         _Myalnode;
};

template < 
    class _Ty,
    class _Alloc = std::allocator<_Ty>
>
class list_v2 { // same class as list_v1, but alloc trais is simpler
private:
    using _Alty        = _Rebind_alloc_t<_Alloc, _Ty>;
    using _Alty_traits = std::allocator_traits<_Alty>;
    using _Node        = _List_node<_Ty>;
    using _Nodeptr     = _List_node<_Ty>*;

    using _Val_types = _List_iter_traits < // complex alloc
                            _Ty,
                            typename _Alty_traits::size_type,
                            typename _Alty_traits::difference_type,
                            typename _Alty_traits::pointer,
                            typename _Alty_traits::const_pointer,
                            _Ty&,
                            const _Ty&,
                            _Nodeptr
                      >;
    using _List_scary_val = _List_val<_Val_types>; // _Scary_val in STL
    // using list = list_v2;

public:
    static_assert(std::is_same_v<_Ty, typename _Alloc::value_type>,
                  "list<T, Allocator> and T MISMATCHED ALLOCATOR");
    static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids containers of non-object types "
                                         "because of [container.requirements].");

    using value_type      = _Ty;
    using allocator_type  = _Alloc;
    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_List_scary_val>;
    using const_iterator            = _List_const_iterator<_List_scary_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_List_scary_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_List_scary_val>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // _Compressed_pair<_Alnode, _List_scary_val> _Mypair;
    _List_scary_val _Mycont;
   
    list_v2() noexcept : _Mycont() {
        _Alloc_head_and_proxy();
    } 

private:
    void _Construct_n (size_type Count);

public:
    explicit list_v2 (size_type Count) : _Mycont() {
        _Construct_n(Count);
    }

private:
    void _Construct_n (size_type Count, const _Ty& Val);

public:
    list_v2 (size_type Count, const _Ty& Val) : _Mycont() {
        _Construct_n(Count, Val);
    }

private:
    template <class _Iter_t>
    void _Construct_range_unchecked_my (_Iter_t First, _Iter_t Last);

public:
    list_v2 (const list_v2& Rhs) : _Mycont(Rhs._Mycont) {
        // _Ald_verify_range();
        _Construct_range_unchecked_my(Rhs._Unchecked_begin(), Rhs._Unchecked_end());
    }

    list_v2 (list_v2&& Rhs); // TODO

    list_v2& operator= (list_v2&& Rhs) noexcept;

private:
    void _Swap_val (list_v2& Rhs) noexcept; // swap with Rhs, same allocator

public:
    void push_front (_Ty&& Val); // insert element at beginning

    void push_back (_Ty&& Val); // insert element at end

    iterator insert (const_iterator Where, _Ty&& Val); // Val at Where

    decltype(auto) emplace_front (_Ty&& Val); // insert element at beggining

    decltype(auto) emplace_back (_Ty&& Val); // insert element at end

    iterator emplace (const const_iterator Where, _Ty&& Val); // insert element at Where

    _Nodeptr _Emplace (const _Nodeptr Where, _Ty&& Val); // insert element at Where

    // list_v2 (initializer_list<_Ty> Ilist) : _Mycont() {
    //     _Construct_range_unchecked_my(Ilist.begin(), Ilist.end());
    // }

    ~list_v2() noexcept;
    
    list_v2& operator= (const list_v2& Rhs);

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    _Unchecked_iterator _Unchecked_begin() noexcept;

    _Unchecked_const_iterator _Unchecked_begin() const noexcept;

    _Unchecked_iterator _Unchecked_end() noexcept;

    _Unchecked_const_iterator _Unchecked_end() const noexcept;

    iterator _Make_iter(_Nodeptr Where) const noexcept;

    const_iterator _Make_const_iterator (_Nodeptr Where) const noexcept;

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept {
        return rend();
    }


private:
    void _Alloc_head_and_proxy() { // NOTE: _Mycont doesn't exist yet in ctor list_v2()
        // _Container_proxy* Newproxy = new _Container_proxy(&_Mycont); // TODO: delete
        _Mycont._Container_base::_Alloc_proxy();
        _Nodeptr Newhead = static_cast<_Nodeptr>(::operator new(sizeof(_Node))); // TODO: operator delete
        _Mycont._Myhead = Newhead;
        Newhead->_Next = Newhead; Newhead->_Prev = Newhead;
    }
    
    void _Orphan_all() noexcept {
        _Mycont._Orphan_all();
    } 
};
    
_MSL_END
