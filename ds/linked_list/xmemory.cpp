struct _Iterator_base {
public:
    _Iterator_base() noexcept = default; // construct orphaned iterator

    _Iterator_base (const _Iterator_base& Rhs) noexcept {
        *this = Rhs;
    }

    _Iterator_base& operator= (const _Iterator_base& Rhs) noexcept {
        _Myproxy = Rhs._Myproxy;
        return *this;
    }

    mutable _Container_proxy* _Myproxy    = nullptr;
    mutable _Iterator_base*   _Mynextiter = nullptr;
};

struct _Container_base;

struct _Container_proxy {
    _Container_proxy() noexcept = default;
    _Container_proxy (_Container_base* Mycont) noexcept : _Mycont(Mycont) {}

    const   _Container_base* _Mycont      = nullptr;
    mutable _Iterator_base*  _Myfirtsiter = nullptr;
};
