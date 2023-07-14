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


template <class _Val_types> // NOTE: does it list for iterators?
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

    _List_val() noexcept : _Myhead(), _Mysize(0) {}

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
class linked_list_v1 {
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
class linked_list_v2 {};
    
_MSL_END
