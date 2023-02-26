#include <iostream>
#include <memory>

#define _ml ML::

class exception {
public:
	exception (const char* other) noexcept : _message(other) {}
	
	exception (const exception&) noexcept = default;
	
	exception& operator= (const exception&) noexcept = default;
	
	exception (exception&& other) noexcept : _message(other._message) {
		other._message = nullptr;
	}

	exception& operator= (exception&& other) {
		if (this != &other)
		{
			delete[] _message;

			_message = other._message;

			other._message = nullptr;
		}

		return *this;
	}

	virtual ~exception() {
		delete[] _message;
		_message = nullptr;
	}
	
	virtual const char* what() const noexcept {
		return _message;
	}


protected:
	const char *_message;
};

namespace ML {

	template <
		class Ty_
	>
	void swap (Ty_& a, Ty_& b) {
		if (std::addressof(a) != std::addressof(b)) 
		{
			auto tmp = std::move(a);
			a = std::move(b);
			b = std::move(tmp);
		}
	}

	// template <
	// 	class Ty_
	// >
	// void swap (Ty_* a, Ty_* b) {
	// 	std::swap(a,b);
	// 	// Ty_ tmp(a);
	// 	// a = b;
	// 	// b = tmp;
	// }

	// template <
	// 	class Ty_
	// >
	// void swap (Ty_&& a, Ty_&& b) {
	// 	auto tmp(std::move(a));
	// 	a = std::move(b);
	// 	b = std::move(tmp);

	// 	// auto tmp(a);
	// 	// a = b;
	// 	// b = tmp;
	// }
}




template <
	class Dty_
>
class Node {
public:

	// tags:
    using value_type      = Dty_;           
    using pointer         = Node<Dty_>*;
    using const_pointer   = const Node<Dty_>*;  
    using reference       = Dty_&; 
    using const_reference = const Dty_&;



    Node (const_reference data,
    	  pointer prev = nullptr,
    	  pointer next = nullptr
    ) noexcept :
    	_data(data), 
    	_prev(prev),
    	_next(next)
    {} 

    explicit Node (const_pointer other) noexcept :
    	_data(other->_data), 
    	_prev(other->_prev), 
    	_next(other->_next)
    {}
    	
    virtual ~Node() {
    	// _next = nullptr;
    	// _prev = nullptr;
    }

    pointer operator= (const_pointer other) noexcept {
    	_data = other->_data;
    	_prev = other->_prev;
    	_next = other->_next;

    	return this;
    }

    // Node (Node&& other) noexcept :
    // 	_data(other.data),
    // 	_prev(other._prev),
    // 	_next(other._next)
    // {
    // 	// _other._data ??
    // 	other._prev = nullptr;
    // 	other._next = nullptr;
    // }

    // pointer operator= (Node&& other) {
    // 	if (this != &other)
    // 	{
    // 		_data = other._data;
    // 		_prev = other._prev;
    // 		_next = other._next;

    // 		other._prev = nullptr;
    // 		other._next = nullptr;
    // 	}
    // }



    pointer getPrev() const noexcept { return _prev; }
    
    pointer getNext() const noexcept { return _next; }
    
    const_reference getData() const noexcept { return _data; }



    void setPrev (pointer prev) noexcept { _prev = prev; }

    void setNext (pointer next) noexcept { _next = next; }

    void setData (const_reference data) noexcept { _data = data; }



    pointer operator= (const_reference data) noexcept {
    	_data(data);
		return this;
    }

    bool operator== (const_pointer other) {
    	try {
    		if (other == nullptr) {
    			throw exception("Comparison with nullptr is imppssible");
    		}

    		if (_prev == other->_prev &&
    			_next == other->_next &&
    			_data == other->_data) { return true; }
    		
    		return false;
    	}
    	catch (exception A) {
    		std::cerr << A.what();
    	}
    }

    template <
    	class Ty_
    >
    friend std::ostream& operator<< (std::ostream& output, const_pointer node);

    template <
    	class Ty_
    >
    friend void swap (Node<Ty_>* a, Node<Ty_>* b);


protected:
	Dty_    _data;
	pointer _prev; 
	pointer _next;
};

template <
	class Dty_
>
std::ostream& operator<< (std::ostream& output, const Node<Dty_>* node) {
	output << node->getData();

	return output;
}

template <
	class Ty_
>
void swap (Node<Ty_>* a, Node<Ty_>* b) {
	_ml swap(a->_prev, b->_prev);
	_ml swap(a->_next, b->_next);
 	_ml swap(a->_data, b->_data);
}






































int main() {


	auto node1 = new Node<int>(1);
	auto node2 = new Node<int>(2);

	node1->setNext(node2);
	node2->setPrev(node1);

	std::cout << node1 << " " << node2;

	swap(node1, node2);

	std::cout << '\n' << node1 << " " << node2;
}