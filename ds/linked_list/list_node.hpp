#ifndef LISTNODE_HPP
#define LISTNODE_HPP


#include "xmemory.hpp"

_MSL_BEGIN
// [ ] _List_node
//       [x] _List_node()
//       [x] _List_node (const _List_node&) = delete
//       [x] _List_node& operator= (const _List_node&) = delete
//       [-] _Buy_head_node // construct head node
//       [ ] _Free_node // destroy pointer members and deallocate
//       [ ] _Free_list_nodes // free a list node [_First, nullptr) 
//       [x] operator<<


template <
	class _Value_type
>
struct _List_node { // list node
    using value_type      = _Value_type;
    using _Nodeptr        = _List_node<_Value_type>*;           


    // explicit _List_node (
    // 	const value_type& Value,
    // 	_Nodeptr Prev,
    // 	_Nodeptr Next
    // ) noexcept :
    // 	_Myval(Value),
    // 	_Prev(Prev),
    // 	_Next(Next)
    // {}
    //
    // explicit _List_node (
    // 	value_type&& Value,
    // 	_Nodeptr Prev,
    // 	_Nodeptr Next
    // ) noexcept :
    // 	_Myval(std::move(Value)), // TODO: check this
    // 	_Prev(Prev),
    // 	_Next(Next)
    // {}

    explicit _List_node (
    	value_type&& Value,
    	_Nodeptr Prev,
    	_Nodeptr Next
    ) noexcept :
    	_Myval(std::forward<value_type>(Value)), // TODO: check this
    	_Prev(Prev),
    	_Next(Next)
    {}

    _List_node() = default;
    _List_node (const _List_node&) = delete;
    _List_node& operator= (const _List_node&) = delete;
    
    static _Nodeptr _Buy_head_node() {
        const auto Result = static_cast<_Nodeptr>(::operator new(sizeof(_List_node<_Value_type>)));
        Result->_Next = Result;
        Result->_Prev = Result;
        return Result;
    }

    static void _Freenode0 (_Nodeptr Ptr) noexcept {
        // destroy pointer members
        Ptr->_Next = nullptr; Ptr->_Prev = nullptr;
        delete Ptr; 
    }
    
    static void _Freenode (_Nodeptr Ptr) noexcept {
        // destroy all members in Ptr
        Ptr->~_Myval();
        _Freenode0(Ptr);
    }

    static void _Free_non_head (_Nodeptr Head) noexcept { 
        // free a list starting at Head and terminate at nullptr
        Head->_Prev->_Next = nullptr;
        auto Pnode = Head;
        for (_Nodeptr Pnext; Pnode; Pnode = Pnext) {
            Pnext = Pnode->_Next;
            _Freenode(Pnode);
        }     
    }


    template <class Ty_>
    friend std::ostream& operator<< (std::ostream& _Output, const _List_node<Ty_>& Node);
   

	_Value_type _Myval; // the stored value, unused if head
	_Nodeptr    _Prev;  // successor node, or first element if head
	_Nodeptr    _Next;  // the stored value, unused if head
};

template <
	class _Value_type
>
std::ostream& operator<< (std::ostream& _Output, const _List_node<_Value_type>& Node) {
	_Output << Node; 
    return _Output;
};

_MSL_END
#endif // LISTNODE_HPP
