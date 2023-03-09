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
class DListNode {
public:
    using value_type      = Dty_;
    using Nodeptr_        = DListNode<Dty_>*;           
    using pointer         = Dty_*;
    using const_pointer   = const Dty_*;  
    using reference       = Dty_&; 
    using const_reference = const Dty_&;



    explicit DListNode (
    	const_reference data,
    	Nodeptr_ prev = nullptr,
		Nodeptr_ next = nullptr
    ) noexcept :
    	_data(data), 
    	_prev(prev),
    	_next(next)
    {} 

    explicit DListNode (Nodeptr_ other) noexcept :
    	_data(other->_data), 
    	_prev(other->_prev), 
    	_next(other->_next)
    {}
    	
    virtual ~DListNode() {
    	_next = nullptr;
    	_prev = nullptr;
    }

    DListNode& operator= (const DListNode& other) noexcept {
    	_data = other._data;
    	_prev = other._prev;
    	_next = other._next;

    	return *this;
    }


    const_reference operator*() const noexcept { return _data; }


    template <
    	class Ty_
    >
    friend std::ostream& operator<< (std::ostream& output, const DListNode<Ty_> node);


    template <
    	class Ty_
    >
    friend void swap (DListNode<Ty_>* a, DListNode<Ty_>* b);


    template <
    	class,
    	template <class ...> class,
    	class
    >
    friend class linked_list;


protected:
	Dty_     _data;
	Nodeptr_ _prev; 
	Nodeptr_ _next;
};



template <
	class Dty_
>
std::ostream& operator<< (std::ostream& output, const DListNode<Dty_> node) {
	output << *node;

	return output;
}

template <
	class Dty_
>
void swap (DListNode<Dty_>& a, DListNode<Dty_>& b) {
	if (std::addressof(a) != std::addressof(b))
	{
		_ml swap(a._prev, b._prev);
		_ml swap(a._next, b._next);
 		_ml swap(a._data, b._data);
	}
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