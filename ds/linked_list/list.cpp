#include "msldef.h"
#include "xmemory.hpp"
#include "list_node.hpp"
#include "list_iterator.hpp"

_MSL_BEGIN


template <class _Value_type>
struct _List_simple_type_traits : _Simple_type_traits<_Value_type> {
    using _Node    = _List_node<_Value_type>;
    using _Nodeptr = _List_node<_Value_type>*;
};


template <class _Val_types> 
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


// [] _List_node_emplace_v2
//     [] Ctor
//         [] _Value_types&&... Vals
//         [x] _Vauley_type&& Val
//     [x] Dtor
//     [x] _Tranfer_before

template <class _Node_t>
struct _List_node_emplace_v2 { // Does not specialize in allocator types
    using pointer = _Node_t*;
    
    template <class... _Value_types>
    explicit _List_node_emplace_v2 (_Value_types&&... Vals) {
         
    }

    template <class _Value_type>
    explicit _List_node_emplace_v2 (_Value_type&& Val) {
        _Mynode = new _Node_t(std::forward(Val));
    }

    ~_List_node_emplace_v2() { 
        if (_Mynode != nullptr) { // emplace didn't happen; TODO: does it works?
           ~(*_Mynode);
            _Mynode = nullptr;
        } 
    }

    _List_node_emplace_v2 (const _List_node_emplace_v2&) = delete;
    _List_node_emplace_v2 operator= (const _List_node_emplace_v2&) = delete;

    pointer _Transfer_before (const pointer Insert_before) noexcept {
        const pointer Insert_after = Insert_before->_Prev; 
        // inserting
        Insert_after->_Next = _Mynode;
        Insert_before->_Prev = _Mynode;
        _Mynode->_Next = Insert_before;
        _Mynode->_Prev = Insert_after;
        
        const auto Result = _Mynode; _Mynode = nullptr;
        return Result;
    }

    pointer _Mynode;
};


template <class _Node_t>
struct _List_node_insert_v2 { // Does not specialize in allocator types
    // _List_node_insert_op2 in STL
    // list insert operation (which maintains exception safety)
    using value_type = _Node_t;
    using pointer    = _Node_t*;
    using size_type  = std::size_t;

    explicit _List_node_insert_v2() : _Added(0), _Head(nullptr), _Tail(nullptr) {}

    _List_node_insert_v2 (const _List_node_insert_v2&) = delete;
    _List_node_insert_v2& operator= (const _List_node_insert_v2&) = delete;

    void _Append_n (size_type Count) { 
        // create an append list with Count pointers 
        // or add them to current one
        if (Count <= 0) { return; }

        if (_Added == 0) { // no _Head, make it
            pointer Newnode = static_cast<pointer>(::operator new(sizeof(value_type)));
            _Head = Newnode; _Tail = Newnode;
            ++_Added;
            --Count;
        }

        for (; Count > 0; --Count) {
            auto Newnode = static_cast<pointer>(::operator new(sizeof(value_type)));
            _Tail->_Next = Newnode;
            Newnode->_Prev = _Tail;
            _Tail = Newnode;
            ++_Added;
        }
    }
    
    // template <class... _CArgT>
    // void _Append_n (size_type Count, const _CArgT&... _Carg);
    
    template <class _Val_types>
    pointer _Attach_before (_List_val<_Val_types>& List, const pointer Insert_before) noexcept { // NOTO: check this!!
        // Attach the elements from *this in List before Where
        // If *this is empty, returns Where; otherwise returns a pointer to first inserted list node
        // Resets *this to the default-initialized state
    
        if (_Added == 0) { return Insert_before; }

        const auto Local_head = _Head;
        const auto Local_tail = _Tail;
        const auto Insert_after = Insert_before->_Prev;

        Insert_after->_Next = Local_head;
        Local_head->_Prev = Insert_after;
        Insert_before->_Prev = Local_tail;
        Local_tail->_Next = Insert_before;
        List._Mysize += _Added;

        _Head = nullptr; _Tail = nullptr; _Added = 0;

        return Local_head;
    }

    template <class _Val_types>
    void _Attach_at_end (_List_val<_Val_types>& List) noexcept {
        _Attach_before(List, List._Myhead);
    }

    template <class _Val_types>
    void _Attach_head (_List_val<_Val_types>& List) { // 
        // Create within *this list head pointer and transfer new whole append list in empty List
        pointer Newhead = static_cast<pointer>(::operator new(sizeof(value_type)));
        // create new head
        if (_Added == 0) { 
            Newhead->_Next = Newhead;
            Newhead->_Prev = Newhead;
        } else {
            const auto Local_head = _Head;
            const auto Local_tail = _Tail;
            Newhead->_Next = Local_head;
            Newhead->_Prev = Local_tail;
            Local_head->_Prev = Newhead;
            Local_tail->_Next = Newhead;
        }
       
        List._Mysize = _Added;
        List._Myhead = Newhead; 
        _Head = nullptr; _Tail = nullptr; _Added = 0;
    }

    ~_List_node_insert_v2() {
        if (_Added == 0) { return; }

        // _Head->_Prev = nullptr; _Tail->_Next = nullptr;
        pointer Pnode = _Head;
        for (pointer Pnext; Pnode; Pnode = Pnext) {
            Pnext = Pnode->_Next;
            value_type::_Freenode(Pnode);
        }
    }

private:
    size_type _Added; // if 0, the values of _Head and _Tail are indeterminate
    pointer   _Head; // points to the first appended element; it doesn't have _Prev constructed
    pointer   _Tail; // points to most recently appended element; id doesn't have _Next constructed
};

_MSL_END
