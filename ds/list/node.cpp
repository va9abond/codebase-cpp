#include "myldef.cpp"



template <
	class Vty_
>
struct List_node_ {

    using value_type      = Vty_;
    using Nodeptr_        = List_node_<Vty_>*;           



    List_node_ (const List_node_&) = delete;
    List_node_& operator= (const List_node_&) = delete;

    void _DEALLOCATE (Nodeptr_ ptr) noexcept {
    	delete ptr->_next; ptr->_next = nullptr;
    	delete ptr->_prev; ptr->_prev = nullptr;

    	::operator delete(ptr);
    } 

    void _Freenode (Nodeptr_ ptr) noexcept { // destroy all members in ptr and deallocate memory
    	_MYL _destroy(_MYL addressof(ptr->_myval));
    	_DEALLOCATE(ptr);
    }

    void _Free_non_head (Nodeptr_ head) noexcept { // free a list starting at head 
    	head->_prev->_next = nullptr;

    	auto Pnode = head->_next;
    	for (Nodeptr_ Pnext; Pnode != nullptr; Pnode = Pnext) {
    		Pnext = Pnode->_next;
    		_Freenode(Pnode);
    	}
    }

    template <class Ty_>
    friend std::ostream& operator<< (std::ostream& output, const List_node_<Ty_>& node);



	value_type _myval; // the stored value, unused if head
	Nodeptr_   _prev;  // successor node, or first element if head
	Nodeptr_   _next;  // the stored value, unused if head
};



template <
	class Vty_
>
std::ostream& operator<< (std::ostream& output, const List_node_<Vty_>& node) {
	output << *node;

	return output;
}








































// int main() {


// 	int x = 32;
// 	int* px = &x;
// 	std::cout << px << "\n";

// 	int* ptr = new int(32);
// 	std::cout << *ptr << '\n';

// 	auto node1 = new Node<int>(1);
// 	auto node2 = new Node<int>(2);

// 	node1->setNext(node2);
// 	node2->setPrev(node1);

// 	std::cout << *node1 << " " << *node2;

// 	swap(node1, node2);

// 	std::cout << '\n' << *node1 << " " << *node2;
// }