#include <iostream> 
#include <iterator>






#define _MYL_BEGIN namespace myl {
#define _MYL_END   }
#define _MYL       ::myl::
#define _STD       ::std::


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



// ************ Container_base_ ************ //

struct _Iterator_base;


struct _Container_base {
	
	_Container_base() noexcept = default;

	_Container_base (const _Container_base&) = delete;
	_Container_base& operator= (const _Container_base&) = delete;

	~_Container_base() {
		Myfirstiter = nullptr;
	}


	_Iterator_base* Myfirstiter = nullptr;
};



// ************ Iterator_base_ ************ //

struct _Iterator_base { // store links to container and next iterator

	_Iterator_base() noexcept = default; // construct orphaned iterator
	
	_Iterator_base (const _Iterator_base& Right) noexcept {
		*this = Right;
	}

	_Iterator_base& operator= (const _Iterator_base& Right) noexcept {
		Mycont = Right.Mycont;
		return *this;
	}

	~_Iterator_base() {
		Mycont     = nullptr;
		Mynextiter = nullptr;
	}

	const _Container_base* Getcont() const noexcept {
		return Mycont != nullptr ? Mycont : nullptr;
	}

	void Adopt (_Container_base* Parent) noexcept {
		if (Parent != nullptr) { // have a parent, do adoption
			Mycont = Parent;
		} 
		else { // no future parent, just disown current parent
			Mycont = nullptr;
		}
	}


	mutable _Container_base* Mycont     = nullptr;
	mutable _Iterator_base*  Mynextiter = nullptr;
};


template <class IterTy_>
struct iterator_traits { // traits for iterators
	using value_type        = typename IterTy_::value_type;
	using difference_type   = typename IterTy_::difference_type;
	using iterator_category = typename IterTy_::iterator_category;
	using pointer           = typename IterTy_::pointer;
	using reference         = typename IterTy_::reference;
};


template <class PtrTy_>
struct iterator_traits<PtrTy_*> { // traits for pointers
	using value_type        = PtrTy_;
	using difference_type   = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;
	using pointer           = PtrTy_*;
	using reference         = PtrTy_&;
};


template <class ValTy_>
struct _Simple_types { // wraps types from allocators with simple addressing for use in iterators
                       // and other SCARY machinery
	using value_type      = ValTy_;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using pointer         = value_type*;
	using const_pointer   = const value_type*;
}; // full copy from STD













_MYL_END

