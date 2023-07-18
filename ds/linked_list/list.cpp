#include <limits>
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
    
    _List_val() noexcept : _Myhead(nullptr), _Mysize(0) {} 

    void _Oprhan_iter_on_ptr (_Nodeptr Ptr) noexcept { // orphan iterators with specified node pointers
        _Iterator_base** Pnext = &(this->_Myproxy->_Myfirstiter);
        const auto Head = _Myhead;
        while (*Pnext) {
            _Iterator_base** Pnextnext = &(*Pnext)->_Mynextiter;
            const auto Pnextptr = static_cast<_List_const_iterator<_List_val>&>(**Pnext)._Myptr;
            if (Pnextptr == Head || Pnextptr != Ptr) {
                // iterator is end() or doesn't point at the one we are orphaning, move on
                Pnext = Pnextnext;
            } else { // orphan the iterator
                (*Pnext)->_Myproxy = nullptr;
                *Pnext = *Pnextnext; // now ...->_Mynextiter = *Pnextnext
            }
        }
    }

    void _Orphan_non_end() noexcept { // orphan iterators except end()
        _Iterator_base** Pnext = &(this->_Myproxy->_Myfirstiter); 
        const auto Head = _Myhead;
        while (*Pnext) { // *Pnext ~ _Mynextiter
            _Iterator_base** Pnextnext = &(*Pnext)->_Mynextiter;
            // TODO: how it works? 
            // casting from _Iterator_base to _List_const_iterator
            if (static_cast<_List_const_iterator<_List_val>&>(**Pnext)._Myptr == Head) {
                // iterator is end(), move on
                Pnext = Pnextnext;
            } else {
                (*Pnext)->_Myproxy = nullptr;
                (*Pnext)->_Mynextiter = nullptr;
                *Pnext = *Pnextnext;
            }
        }
    }

    _Nodeptr _Unlink_node (_Nodeptr Pnode) noexcept { // unlink node at Where from the list
        _Orphan_ptr(Pnode);
        Pnode->_Prev->_Next = Pnode->_Next;
        Pnode->_Next->_Prev = Pnode->_Prev;
        --_Mysize;
        return Pnode;
    }
    
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
        } // NOTE: after that we have:
          // ... <--> Local_tail <--> Newhead <--> Loca_head <--> ...
          // Local_head doesn't have _Myval
       
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


template < 
    class _Ty
>
class list_v2 { // does not prepare for custom allocators!
private:
    using _Alloc       = std::allocator<_Ty>;
    using _Node        = _List_node<_Ty>;
    using _Nodeptr     = _List_node<_Ty>*;

    using _Val_types = _List_simple_type_traits<_Ty>;
    using _List_SCARY_val = _List_val<_Val_types>; // not SCARY to tell the truth
                                                   // because _Val_type doesn't provide SCARY
    using _List_node_insert  = _List_node_insert_v2<_List_node<_Ty>>;
    using _List_node_emplace = _List_node_emplace_v2<_List_node<_Ty>>;

public:
    static_assert(std::is_same_v<_Ty, typename _Alloc::value_type>,
                  "list<T, Allocator> and T MISMATCHED ALLOCATOR");
    static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids containers of non-object types "
                                         "because of [container.requirements].");

    using value_type      = _Ty;
    using allocator_type  = _Alloc;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = _Ty*;
    using const_pointer   = const _Ty*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    using iterator                  = _List_iterator<_List_SCARY_val>;
    using const_iterator            = _List_const_iterator<_List_SCARY_val>;
    using _Unchecked_iterator       = _List_unchecked_iterator<_List_SCARY_val>;
    using _Unchecked_const_iterator = _List_unchecked_const_iterator<_List_SCARY_val>;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    _List_SCARY_val _Mycont;

    
    list_v2() : _Mycont() {
        _Alloc_head_and_proxy();
    }

private:
    void _Construct_n (size_type Count) {
        // construct and apply Proxy for _Mycont
        _Mycont._Container_base::_Alloc_proxy();
        // append n elements
        _List_node_insert Appended;
        Appended._Append_n(Count);
        Appended._Attach_head(_Mycont);
    }

public:
    explicit list_v2 (size_type Count) : _Mycont() {
        _Construct_n(Count);
    }

private:
    void _Construct_n (size_type Count, const _Ty& Val); // TODO: impl _Append(Count, Val)

public:
    list_v2 (size_type Count, const _Ty& Val) : _Mycont() {
        _Construct_n(Count, Val);
    }

    list_v2 (const list_v2& Rhs); // TODO: _Construct_range_unchecked

    list_v2 (list_v2&& Rhs) : _Mycont(std::move(Rhs)) {
        _Alloc_head_and_proxy();
        _Swap_val(Rhs);
    }

    list_v2& operator= (list_v2&& Rhs) noexcept; // TODO: Dtor need

private:
    void _Swap_val (list_v2& Rhs) noexcept { // swap with Rhs
        auto& Mycont = _Mycont;
        auto& Rhscont = Rhs._Mycont;
        Mycont._Swap_proxy_and_iterators(Rhscont);
        
        auto* Temp = _Mycont._Myhead;
        _Mycont._Myhead = Rhs._Mycont._Myhead;
        Rhs._Mycont._Myhead = Temp;
        
        std::swap(Mycont._Mysize, Rhscont._Mysize);
    }

public:
    void push_front (_Ty&& Val) { // insert element at beginning
        _Emplace(_Mycont._Myhead->_Next, std::move(Val)); 
    }

    void push_back (_Ty&& Val) { // insert element at end
        _Emplace(_Mycont._Myhead, std::move(Val));
    }

    iterator insert (const_iterator Where, _Ty&& Val) { // Val at Where
        return emplace(Where, std::move(Val)); 
    }

    decltype(auto) emplace_front (_Ty&& Val) { // insert element at beggining
        reference Result = _Emplace(_Mycont._Myhead->_Next, std::move(Val)); 
        return Result;
    }

    decltype(auto) emplace_back (_Ty&& Val) { // insert element at end
        reference Result = _Emplace(_Mycont._Myhead, std::move(Val));
        return Result;
    }

    iterator emplace (const const_iterator Where, _Ty&& Val) { // insert element at Where
        msl::_MSL_VERIFY_f(Where._Getcont() == std::addressof(_Mycont), "list emplace iterator outside range"); 
        return _Make_iter(_Emplace(Where._Myptr, std::forward(Val)));
    }

    _Nodeptr _Emplace (const _Nodeptr Where, _Ty&& Val) { // insert element at Where
        size_type& _Mysize = _Mycont._Mysize;
        if (_Mysize == max_size()) {
            _MSL_REPORT_ERROR_f("list too long");
        } 

        _List_node_emplace Emplaced{ std::forward(Val) };
        ++_Mysize;
        return Emplaced._Transfer_before(Where);
    }

    list_v2 (std::initializer_list<_Ty> Ilist) : _Mycont() {
        _Construct_range_unchecked_my(Ilist.begin(), Ilist.end()); // TODO: 
    }

    list_v2& operator= (std::initializer_list<_Ty> Ilist) {
        assign(Ilist.begin(), Ilist.end());
        return *this;
    }

    void assing (std::initializer_list<_Ty> Ilist) {
        assign(Ilist.begin, Ilist.end());
    }

    iterator insert (const_iterator Where, std::initializer_list<_Ty> Ilist) { // insert initializer_list
        return insert(Where, Ilist.begin(), Ilist.end());
    }
    
    ~list_v2() noexcept {
        // delete all elements
        _Tidy();
        // delete proxy
        _Mycont._Free_proxy();
    }

    list_v2& operator= (const list_v2& Rhs); // TODO:

    iterator begin() noexcept {
        return iterator(_Mycont._Myhead->_Next, std::addressof(_Mycont));
        // TODO: &_Mycont == std::addressof(_Mycont) ?
        // TODO: check iterator Ctor, does it properly connect with _Mycont and proxy?
    }

    const_iterator begin() const noexcept {
        return const_iterator(_Mycont._Myhead->_Next, std::addressof(_Mycont));
    }

    iterator end() noexcept {
        return iterator(_Mycont._Myhead, std::addressof(_Mycont));
    }

    const_iterator end() const noexcept {
        return const_iterator(_Mycont._Myhead, std::addressof(_Mycont));
    }

    _Unchecked_iterator _Unchecked_begin() noexcept {
        return _Unchecked_iterator(_Mycont._Myhead->_Next, nullptr);
    }

    _Unchecked_const_iterator _Unchecked_begin() const noexcept {
        return _Unchecked_const_iterator(_Mycont._Myhead->_Next, nullptr);
    }

    _Unchecked_iterator _Unchecked_end() noexcept {
        return _Unchecked_iterator(_Mycont._Myhead, nullptr);
    }

    _Unchecked_const_iterator _Unchecked_end() const noexcept {
        return _Unchecked_const_iterator(_Mycont._Myhead, nullptr);
    }

    iterator _Make_iter(_Nodeptr Where) const noexcept {
        return iterator(Where, std::addressof(_Mycont));
    }

    const_iterator _Make_const_iterator (_Nodeptr Where) const noexcept {
        return const_iterator(Where, std::addressof(_Mycont));
    }

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

    void resize (size_type Newsize) { // determine new length, padding with _Ty() elements as needed
        _MSL_VERIFY_f(Newsize < max_size(), "list too long");
        
        if (Newsize > _Mycont._Mysize) { // pad to make larger
            _List_node_insert Appended;
            Appended._Append_n(Newsize - _Mycont._Mysize);
            Appended._Attach_at_end();
        } else {
            while (Newsize < _Mycont._Mysize) {
                pop_back();
            }
        }
    }

    void resize (size_type Newsize, const _Ty& Val) { 
        // determine new length, padding with Val elements as needed
        _MSL_VERIFY_f(Newsize < max_size(), "list too long");
        
        if (Newsize > _Mycont._Mysize) { // pad to make larger
            _List_node_insert Appended;
            Appended._Append_n(Newsize - _Mycont._Mysize, Val);
            Appended._Attach_at_end();
        } else {
            while (Newsize < _Mycont._Mysize) {
                pop_back();
            }
        }
    }

    size_type size() const noexcept {
        return _Mycont._Mysize;
    }

    size_type max_size() const noexcept {
        return static_cast<size_type>(std::numeric_limits<difference_type>::max); // redefine diff_type
    }

    bool empty() const noexcept {
        return _Mycont._Mysize == 0;
    }

    reference front() noexcept {
        _MSL_VERIFY_f(_Mycont._Mysize != 0, "front() called on empty list");
        return _Mycont._Myhead->_Next->_Myval;
    }

    const_reference front() const noexcept {
        _MSL_VERIFY_f(_Mycont._Mysize != 0, "front() called on empty list");
        return _Mycont._Myhead->_Next->_Myval;
    }

    reference back() noexcept {
        _MSL_VERIFY_f(_Mycont._Mysize != 0, "back() called on empty list");
        return _Mycont._Myhead->_Prev->_Myval;
    }

    const_reference back() const noexcept {
        _MSL_VERIFY_f(_Mycont._Mysize != 0, "back() called on empty list");
        return _Mycont._Myhead->_Prev->_Myval;
    }

    void push_front (const _Ty& Val) {
        _Emplace(_Mycont._Myhead->_Next, Val);
    }

    void pop_front() noexcept { 
        _MSL_VERIFY_f(_Mycont._Mysize != 0, "pop_front() called on empty list");
        _Unchecked_erase(_Mycont._Myhead->_Next);
    }

    void push_back (const _Ty& Val) {
        _Emplace(_Mycont._Myhead, Val);
    }

    void pop_back() noexcept {
        _MSL_VERIFY_f(_Mycont._Mysize != 0, "pop_back() called on empty list");
        _Unchecked_erase(_Mycont._Myhead->_Prev);
    }

    iterator insert (const_iterator Where, const _Ty& Val) { // insert Val at Where
        
    }

    iterator insert (const_iterator Where, size_type Count, const _Ty& Val) {

    }
    
    template <class _Iter_t>
    iterator insert (const_iterator Where, _Iter_t First, _Iter_t Last) { // insert [First, Last) before Where
        
    }

    iterator erase (const const_iterator Where) noexcept {

    }

private:
    _Nodeptr _Unchecked_erase (const _Nodeptr Pnode) noexcept { // erase element at Pnode

    }

public:
    iterator erase (const const_iterator First, const const_iterator Last) noexcept {

    }

private:
    _Nodeptr _Unchecked_erase (_Nodeptr First, _Nodeptr Last) noexcept { // erase [First, Last)

    }

public:
    void clear() noexcept { // erase all 
        _Mycont._Orphan_non_end(); // erase all iterators exclude end

        _Nodeptr Myhead = _Mycont._Myhead;
        _Node::_Free_non_head(Myhead);
        Myhead->_Next = Myhead;
        Myhead->_Prev = Myhead;
        _Mycont._Mysize = 0;
    }

private:
    void _Tidy() noexcept {
        _Mycont._Container_base::_Orphan_all(); // erase all itearators
        _Node::_Free_non_head(_Mycont._Myhead);
        _Node::_Freenode0(_Mycont._Myhead); // _Myhead doesn't have constructed _Myval
    }

public:
    void swap (list_v2& Rhs) noexcept {

    }

private:
    // template <class 
    // void _Assign_cast ()


// TODO: check type conversation const_iterator <--> iterator












private:
    void _Alloc_head_and_proxy() { // NOTE: _Mycont doesn't exist yet in ctor list_v2()
        // _Container_proxy* Newproxy = new _Container_proxy(&_Mycont); // TODO: delete
        _Mycont._Container_base::_Alloc_proxy();
        _Mycont._Myhead = _Node::_Buy_head_node();
    }

    void _Orphan_all() noexcept {
        _Mycont._Orphan_all();
    }
};
_MSL_END
