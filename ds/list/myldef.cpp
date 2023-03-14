#include <iostream> 



#define _MYL_BEGIN namespace myl {
#define _MYL_END   }
#define _MYL       ::myl::



_MYL_BEGIN // my library to replace functions from STD 

	


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
void _destroy(_Ty* _Ptr) {
	_Ptr->~_Ty();
}


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

protected:
	const char *_message;
};






























_MYL_END
