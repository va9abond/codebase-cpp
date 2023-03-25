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
struct _Container_base;

struct _Container_proxy { // struct to connect _Container_base and _Iterator_base
	_Container_proxy() noexcept = default;
	_Container_proxy (_Container_base* Mycont) noexcept : _Mycont(Mycont) {} 


	const   _Container_base* _Mycont      = nullptr;
	mutable _Iterator_base*  _Myfirstiter = nullptr;
};

struct _Container_base {
	
	_Container_base() noexcept = default;

	_Container_base (const _Container_base&) = delete;
	_Container_base& operator= (const _Container_base&) = delete;

	~_Container_base() {
		delete _Myproxy; _Myproxy = nullptr;
	}

	template <class Alloc>
	void Alloc_proxy (Alloc&& Al) {
		_Container_proxy* const New_proxy = new _Container_proxy(this);  // TODO: *WARNING* where i need to delete _Myproxy
		_Myproxy = New_proxy;
		// New_proxy->_Mycont = this;
	}


	_Container_proxy* _Myproxy = nullptr;
};



// ************ Iterator_base_ ************ //

struct _Iterator_base { // store links to container and next iterator

	_Iterator_base() noexcept = default; // construct orphaned iterator
	
	_Iterator_base (const _Iterator_base& Right) noexcept {
		*this = Right;
	}

	_Iterator_base& operator= (const _Iterator_base& Right) noexcept {
		_Assign(Right);
		_Myproxy = Right._Myproxy;
		
		return *this;
	}

	~_Iterator_base() {
		this->_Orphan_me();
		_Myproxy    = nullptr;
		_Mynextiter = nullptr;
	}

	const _Container_base* Getcont() const noexcept {
		return _Myproxy->_Mycont != nullptr ? _Myproxy->_Mycont : nullptr;
	}


	mutable _Container_proxy* _Myproxy    = nullptr;
	mutable _Iterator_base*   _Mynextiter = nullptr;


private:
	void _Adopt (const _Container_base* Parent) noexcept {
		if (Parent == nullptr) {
			_Orphan_me(); return;
		}

		_Container_proxy* Parent_proxy = Parent->_Myproxy;
		if (_Myproxy != Parent_proxy) { // change parentage
			if (_Myproxy != nullptr) { // already have a parent
				_Orphan_me();
			}
			_Mynextiter = Parent_proxy->_Myfirstiter;
			Parent_proxy->_Myfirstiter = this;
			_Myproxy = Parent_proxy;
		}
	}

	void _Orphan_me() { // TODO: how it works?
		if (_Myproxy == nullptr) { // already orphaned
			return;
		}

		// adopted, remove self from list
		_Iterator_base** Pnext = &_Myproxy->_Myfirstiter;
		while (*Pnext != nullptr && *Pnext != this) {
			const auto Tmp = *Pnext;
			Pnext = &Tmp->_Mynextiter;
		}

		try {
			if (*Pnext == nullptr) {
				throw exception("ITERATOR LIST CORRUPTED!");
			}

			*Pnext   = _Mynextiter;
			_Myproxy = nullptr;
		}
		catch (exception corrupted_list) { // TODO: why there is a warning?
			std::cerr << corrupted_list.what();
		}
	}

	void _Assign (const _Iterator_base& Right) noexcept {
		if (_Myproxy == Right._Myproxy) {
			return;
		}

		if (Right._Myproxy != nullptr) {
			this->_Adopt(Right._Myproxy->_Mycont);
		}
		else {
			this->_Orphan_me();
		}
	}

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


// template <class _Alloc>
// struct _Normal_allocator_traits { // defines traits for allocators
//     using allocator_type = _Alloc;
//     using value_type     = typename _Alloc::value_type;

//     using pointer            = typename _Get_pointer_type<_Alloc>::type;
//     using const_pointer      = typename _Get_const_pointer_type<_Alloc>::type;
//     using void_pointer       = typename _Get_void_pointer_type<_Alloc>::type;
//     using const_void_pointer = typename _Get_const_void_pointer_type<_Alloc>::type;

//     using size_type       = typename _Get_size_type<_Alloc>::type;
//     using difference_type = typename _Get_difference_type<_Alloc>::type;
// };


template <class _Alloc>
struct _Default_allocator_traits { // traits for std::allocator
    using allocator_type = _Alloc;
    using value_type     = typename _Alloc::value_type;

    using pointer            = value_type*;
    using const_pointer      = const value_type*;
    using void_pointer       = void*;
    using const_void_pointer = const void*;

    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    template <class _Other>
    using rebind_alloc = std::allocator<_Other>;

}; // full copy from STD








_MYL_END

