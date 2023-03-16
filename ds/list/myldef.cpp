#include <iostream> 
#include <iterator>





#define _MYL_BEGIN namespace myl {
#define _MYL_END   }
#define _MYL       ::myl::



_MYL_BEGIN // my library to replace functions from STD 



// ************ utility ************ //
	
template <class _Ty>
constexpr _Ty* addressof(_Ty& _Val) noexcept { // copy of std::addressof()
    // return __builtin_addressof(_Val);	
    return &_Val;
}

template <class _Ty>
const _Ty* addressof (const _Ty&&) = delete; // copy of std::addressof()




// STRUCT TEMPLATE remove_reference // full copy from STD
template <class _Ty>
struct remove_reference {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&> {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty&;
};

template <class _Ty>
struct remove_reference<_Ty&&> {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty&&;
};

template <class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;

template <class _Ty>
constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable // copy from STD
    return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}

template <class _Ty>
constexpr _Ty&& forward(
    remove_reference_t<_Ty>& _Arg) noexcept { // forward an lvalue as either an lvalue or an rvalue
    return static_cast<_Ty&&>(_Arg);
}

// template <class _Ty> // https://habr.com/ru/post/568306/ 
// constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept { // forward an rvalue as an rvalue
//     static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
//     return static_cast<_Ty&&>(_Arg);
// }

template <class _Ty>
void swap(_Ty& _Left, _Ty& _Right) { // *WARNING* no check if _Ty is is_nothrow_move_constructible_v && is_nothrow_move_assignable_v
	if ( addressof(_Left) != addressof(_Right) )
	{
	    _Ty _Tmp =  move(_Left);
	    _Left    =  move(_Right);
	    _Right   =  move(_Tmp);
	}
}

template <class _Ty>
void destroy_(_Ty* _Ptr) {
	_Ptr->~_Ty();
}




// ************ class exception ************ //  

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

// ************ class semantic_error ************ //

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

protected:
	const char *_message;
};





// ************ Iterators ************ //
struct Iterator_base_;
struct Container_base_;


struct Container_proxy_ { // store head of iterator chain and back pointer    
    
    Container_proxy_() noexcept = default;
    
    Container_proxy_(Container_base_* Mycont_) noexcept : Mycont(Mycont_) {}

    ~Container_proxy_() {
    	Mycont      = nullptr;
    	Myfirstiter = nullptr;
    } 

    const   Container_base_* Mycont      = nullptr; // pointer to parent container
    mutable Iterator_base_*  Myfirstiter = nullptr; // head iterator of parent container
};


struct Container_base_ {
	
	Container_base_() noexcept = default;

	Container_base_ (const Container_base_&) = delete;
	Container_base_& operator= (const Container_base_&) = delete;

	~Container_base_() {
		delete Myproxy; Myproxy = nullptr;
	}

	void Alloc_proxy_() {
		Myproxy = new Container_proxy_(this);
		// new_proxy->Mycont = this;
	}

	void Orphan_all_() noexcept {}


	Container_proxy_* Myproxy = nullptr;
};



// ************ Iterators ************ //

struct Iterator_base_ { // store links to container and next iterator

	Iterator_base_() noexcept = default; // construct orphaned iterator
	
	Iterator_base_ (const Iterator_base_& Right) noexcept {
		*this = Right;
	}

	Iterator_base_& operator= (const Iterator_base_& Right) noexcept {
		Myproxy = Right.Myproxy;
		return *this;
	}

	~Iterator_base_() {
		Myproxy    = nullptr;
		Mynextiter = nullptr;
	}

	const Container_base_* Getcont_() const noexcept {
		return Myproxy != nullptr ? Myproxy->Mycont : nullptr;
	}

	void Adopt_ (const Container_base_* Parent) noexcept {
		if (Parent != nullptr) { // have a parent, do adoption
			Myproxy = Parent->Myproxy;
		} 
		else { // no future parent, just disown current parent
			Myproxy = nullptr;
		}
	}

	mutable Container_proxy_* Myproxy    = nullptr;
	mutable Iterator_base_*   Mynextiter = nullptr;
};

template <class IterTy_>
struct iterator_traits {
	using value_type        = typename IterTy_::value_type;
	using difference_type   = typename IterTy_::difference_type;
	using iterator_category = typename IterTy_::iterator_category;
	using pointer           = typename IterTy_::pointer;
	using reference         = typename IterTy_::reference;
};


template <class PtrTy_>
struct iterator_traits<PtrTy_*> {
	using value_type        = PtrTy_;
	using difference_type   = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;
	using pointer           = PtrTy_*;
	using reference         = PtrTy_&;
};






















_MYL_END

