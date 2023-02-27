#include <iostream>
#include <memory>

#define _ml ML::


namespace ML {

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

	class semantic_error : exception {
	public:
		semantic_error (const char* other) noexcept :
			exception("SEMANTIC_ERROR"),
			_message(other) 
		{}

		semantic_error (const semantic_error&) noexcept = default;
		
		semantic_error& operator= (const semantic_error&) noexcept = default;

		semantic_error (semantic_error&& other) noexcept
			: exception("SEMANTIC_ERROR"),
			_message(other._message)
		{
			other._message = nullptr;
		}

		semantic_error& operator= (semantic_error&& other) noexcept {
			if (this != &other)
			{
				delete[] _message;

				_message = other._message;

				other._message = nullptr; 
			}

			return *this;
		}

		virtual ~semantic_error() {
			delete[] _message;
			_message = nullptr;
		};

		const char* what() const noexcept {
			return _message;
		}

	private:
		const char *_message;
	};


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
}




template <
	class Dty_
>
class Node {
public:
    using value_type      = Dty_;
    using Nodeptr_        = Node<Dty_>*;
    using Noderef_        = Node<Dty_>&;           
    using pointer         = Dty_*;
    using const_pointer   = const Dty_*;  
    using reference       = Dty_&; 
    using const_reference = const Dty_&;



    explicit Node (const_reference data,
    	  Nodeptr_ prev = nullptr,
    	  Nodeptr_ next = nullptr
    ) noexcept :
    	_data(data), 
    	_prev(prev),
    	_next(next)
    {} 

    explicit Node (Nodeptr_ other) noexcept :
    	_data(other->_data), 
    	_prev(other->_prev), 
    	_next(other->_next)
    {}
    	
    virtual ~Node() {
    	_next = nullptr;
    	_prev = nullptr;
    }

    Noderef_ operator= (const Nodeptr_ other) noexcept {
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


    // const?
    Nodeptr_ getPrev() const noexcept { return _prev; }
    
    Nodeptr_ getNext() const noexcept { return _next; }
    
    const_reference operator*() const noexcept { return _data; }


    void setPrev (Nodeptr_ prev) noexcept { _prev = prev; }

    void setNext (Nodeptr_ next) noexcept { _next = next; }

    void setData (const_reference data) noexcept { _data = data; }



    Node& operator= (const_reference data) noexcept {
    	_data(data);
		return this;
    }



    template <
    	class Ty_
    >
    friend std::ostream& operator<< (std::ostream& output, const Node<Ty_> node);

    template <
    	class Ty_
    >
    friend void swap (Node<Ty_>* a, Node<Ty_>* b);


protected:
	Dty_     _data;
	Nodeptr_ _prev; 
	Nodeptr_ _next;
};

template <
	class Dty_
>
std::ostream& operator<< (std::ostream& output, const Node<Dty_> node) {
	output << *node;

	return output;
}

template <
	class Dty_
>
void swap (Node<Dty_>* a, Node<Dty_>* b) {
	_ml swap(a->_prev, b->_prev);
	_ml swap(a->_next, b->_next);
 	_ml swap(a->_data, b->_data);
}






































int main() {


	int x = 32;
	int* px = &x;
	std::cout << px << "\n";

	int* ptr = new int(32);
	std::cout << *ptr << '\n';

	auto node1 = new Node<int>(1);
	auto node2 = new Node<int>(2);

	node1->setNext(node2);
	node2->setPrev(node1);

	std::cout << *node1 << " " << *node2;

	swap(node1, node2);

	std::cout << '\n' << *node1 << " " << *node2;
}