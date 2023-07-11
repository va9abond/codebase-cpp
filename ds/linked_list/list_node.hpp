#include "xmemory.hpp"


template <
	class _Value_type
>
struct _List_node { // list node
    using value_type      = _Value_type;
    using _Nodeptr        = _List_node<_Value_type>*;           


    explicit _List_node (
    	const value_type& Value,
    	_Nodeptr Prev,
    	_Nodeptr Next
    ) noexcept :
    	_Myval(Value),
    	_Prev(Prev),
    	_Next(Next)
    {}

    explicit _List_node (
    	value_type&& Value,
    	_Nodeptr Prev,
    	_Nodeptr Next
    ) noexcept :
    	_Myval(move(Value)), // TODO: CHECK THIS
    	_Prev(Prev),
    	_Next(Next)
    {}

    _List_node (const _List_node&) = delete;
    _List_node& operator= (const _List_node&) = delete;
   
    static _Nodeptr _Buyheadnode() {}

    static void _Freenode0 (_Nodeptr ptr) noexcept {}

    static void _Freenode (_Nodeptr ptr) noexcept {}

    static void _Free_non_head (_Nodeptr head) noexcept {}

    template <class Ty_>
    friend std::ostream& operator<< (std::ostream& _output, const _List_node<Ty_>& Node);
    
    void get() {
    	std::cout << this << "\n" << *this;
    }

	_Value_type _Myval; // the stored value, unused if head
	_Nodeptr    _Prev;  // successor node, or first element if head
	_Nodeptr    _Next;  // the stored value, unused if head
};

template <
	class _Value_type
>
std::ostream& operator<< (std::ostream& _output, const _List_node<_Value_type>& Node) {
	_output << Node; 
    return _output;
};
