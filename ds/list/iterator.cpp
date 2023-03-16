#include "node.cpp"

template <
    class Mylist_,
    class Base_ = _MYL Iterator_base_
>
class _List_unchecked_const_iterator : public Base_ {
public: 
    using iterator_category = std::bidirectional_iterator_tag; 

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&;



    _List_unchecked_const_iterator() noexcept : Ptr_() {}

    _List_unchecked_const_iterator (Nodeptr_ Pnode, const Mylist_* Plist) noexcept : 
        Ptr_(Pnode)
    {
        this->Adopt(Plist);
    }
    
    // ~_List_unchecked_const_iterator() = default;

    reference operator*() const noexcept {
        return Ptr_->_myval;
    }

    pointer operator->() const noexcept { 
        return this;
    }

    explicit operator bool() const {
        return Ptr_; // is nullptr
    }

    _List_unchecked_const_iterator& operator++ () noexcept {
        Ptr_ = Ptr_->_next; 
        return *this;
    }

    _List_unchecked_const_iterator operator++(int) noexcept {
        auto tmp = *this;
        Ptr_     = Ptr_->_next;
        return tmp;
    }

    _List_unchecked_const_iterator& operator--() noexcept {
        Ptr_ = Ptr_->_prev;
        return *this;
    }

    _List_unchecked_const_iterator operator--(int) noexcept {
        auto tmp = *this;
        Ptr_     = Ptr_->_prev;
        return tmp;
    }

    bool operator== (const _List_unchecked_const_iterator& right) const noexcept {
        return Ptr_ == right.Ptr_;
    }
   
    bool operator!= (const _List_unchecked_const_iterator& right) const noexcept {
        return !(*this == right);
    }


    Nodeptr_ Ptr_; // pointer to node

};



template <
    class Mylist_
>
class _List_unchecked_iterator : public _List_unchecked_const_iterator<Mylist_> {
public:
    using Mybase           = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&;

    using Mybase::Mybase;



    reference operator*() const noexcept {
        return const_cast<reference>(Mybase::operator*());
    }    

    pointer operator->() const noexcept {
        return this;
    }

    _List_unchecked_iterator& operator++() {
        Mybase::operator++();
        return *this;
    }

    _List_unchecked_iterator operator++(int) {
        auto tmp = *this;
        Mybase::operator++();
        return tmp;
    }

    _List_unchecked_iterator& operator--() {
        Mybase::operator--();
        return *this;
    }

    _List_unchecked_iterator operator--(int) {
        auto tmp = *this;
        Mybase::operator--();
        return tmp;
    }
};



template <
    class Mylist_
>
class _List_const_iterator : public _List_unchecked_const_iterator<Mylist_> {
public:
    using Mybase = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&; 

    using Mybase::Mybase;


    reference operator*() const {
        try {
            const auto Mycont = static_cast<const Mylist_*>(this->Getcont());
            if (Mycont == nullptr) {
                throw _MYL exception("Cannot dereference value-initialized list iterator");
            }

            if (this->Ptr_ == Mycont->Myfirstiter->Ptr_) {
                throw _MYL exception("Cannot dereference end list iterator");
            }

            return this->Ptr_->_myval; 
        }
        catch (_MYL exception general_exception) {
            std::cerr << general_exception.what();
        }
    }

    pointer operator->() const noexcept {
        return this;
    }

    _List_const_iterator& operator++() {
        try {
            const auto Mycont = static_cast<const Mylist_*>(this->Getcont());
            if (Mycont == nullptr) {
                throw _MYL exception("Cannot increment value-initialized list iterator");
            }

            if (this->Ptr_ == Mycont->Myfirstiter->Ptr_) {
                throw _MYL exception("Cannot increment end list iterator");
            } 
            this->Ptr_ = this->Ptr_->_next;
            return *this;
        }   
        catch (_MYL exception general_exception) {
            std::cerr << general_exception.what();
        }
    }

    _List_const_iterator operator++(int) {
        _List_const_iterator tmp = *this;
        ++*this;
        return tmp;
    }

    _List_const_iterator& operator--() {
        try {
            const auto Mycont = static_cast<const Mylist_*>(this->Getcont());
            const auto New_ptr = this->Ptr_->_prev;
            if (Mycont == nullptr) {
                throw _MYL exception("Cannot decrement value-initialized list iterator");
            }

            if (New_ptr == Mycont->Myfirstiter->Ptr_) {
                throw _MYL exception("Cannot decrement end list iterator");
            } 
            this->Ptr_ = New_ptr;
            return *this;
        }   
        catch (_MYL exception general_exception) {
            std::cerr << general_exception.what();
        }
    }

    _List_const_iterator operator--(int) {
        _List_const_iterator tmp = *this;
        --*this;
        return tmp;
    }

    bool operator== (const _List_const_iterator& Right) const {
        try {
            if ( (this->Getcont() =! Right.Getcont()) ) {
                throw _MYL exception("List iterators incompatible");
            }

            return this->Ptr_ == Right.Ptr_; 
        }
        catch (_MYL exception incorrect_Right) {
            std::cerr << incorrect_Right.what();
        }
    }

    bool operator!= (const _List_const_iterator& Right) const {
        return !(*this == Right);
    }

    friend bool Verify_range_ (const _List_const_iterator& First, const _List_const_iterator& Last) noexcept {
        return First.Getcont() == Last.Getcont();
    }

    _List_unchecked_const_iterator<Mylist_> Unwrapped_() const noexcept {
        return _List_unchecked_const_iterator<Mylist_>(this->Ptr_, static_cast<const Mylist_*>(this->Getcont())); 
    }
};



template <
    class Mylist_
>
class _List_iterator : public _List_const_iterator<Mylist_> {
public:
    using Mybase = _List_unchecked_const_iterator<Mylist_>;
    using iterator_category = std::bidirectional_iterator_tag;

    using Nodeptr_        = typename Mylist_::Nodeptr_;
    using value_type      = typename Mylist_::value_type;
    using difference_type = typename Mylist_::difference_type;
    using pointer         = typename Mylist_::pointer;
    using reference       = const value_type&; 

    using Mybase::Mybase;


    reference operator*() const {
        return const_cast<reference>(Mybase::operator*());
    }

    pointer operator->() const noexcept {
        return const_cast<pointer>(this);
    }

    _List_iterator& operator++() {
        Mybase::operator++();
        return *this;
    }

    _List_iterator operator++(int) {
        _List_iterator tmp = *this;
        Mybase::operator++();
        return tmp;
    }

    _List_iterator& operator--() {
        Mybase::operator--();
        return *this;
    }

    _List_iterator operator--(int) {
        _List_iterator tmp = *this;
        Mybase::operator--();
        return tmp;
    }

    _List_unchecked_iterator<Mylist_> Unwrapped_() const noexcept {
        return _List_unchecked_iterator<Mylist_>(this->Ptr_, static_cast<const Mylist_*>(this->Getcont()));
    }

};
