# msldef.h

| defines    | token string    |
| ---------- | --------------- |
| _MSL_BEGIN | namespace msl { |
| _MSL_END   | }               |
| _MSL       | ::msl::         |
| _STD       | ::std::         |

| keyword / feature | return      | name                   | args               | notes  | status |
| ----------------- | ----------- | ---------------------- | ------------------ | ------ | -----  |
| inline / noexcept | void        | _MSL_REPORT_ERROR_f    | const char\*       |        |        |
| inline / noexcept | void        | _MSL_VERIFY_f          | bool, const char\* |        |        |


# xmemory.hpp
### struct _Simple_type_traits
| _Simple_type_traits<_Value_type> |                    |
| -------------------------------- | ------------------ |
| value_type                       | _Value_type        |
| size_type                        | std::size_t        |
| difference_type                  | std::ptrdiff_t     |
| pointer                          | value_type\*       |
| const_pointer                    | const value_type\* |

### struct _Container_proxy
| _Container_proxy |                   |
| ---------------- | ----------------- |
| const            | _Container_base\* |
| mutable          | _Iterator_base\*  |

| keyword / feature | return           | name                       | args              | notes | status |
| ----------------- | ---------------- | -------------------------- | ----------------- | ----- | ------ |
| noexcept          | _Container_proxy | default Ctor [ = default ] |                   |       |        |
| noexcept          | _Container_proxy | Ctor [ = delete ]          | _Container_base\* |       |        |

### struct _Container_base
| _Container_base  |
| ---------------- |
| _Container_proxy |

| keyword / feature | return           | name                       | args                    | notes     | status    |
| ----------------- | ---------------- | -------------------------- | ----------------------- | --------- | --------- |
| noexcept          | _Container_base  | default Ctor [ = default ] |                         |           |           |
|                   | _Container_base  | Ctor [ = delete ]          | const \_Container_base& |           |           |
|                   | _Container_base& | Ctor [ = delete ]          | const \_Container_base& |           |           |
| \--------         | \--------        | \--------                  | \--------               | \-------- | \-------- |
| noexcept          | void             | _Orphan_all                |                         |           |           |
|                   | void             | _Swap_proxy_and_iterators  | _Container_base&        |           |           |
|                   | void             | _Alloc_proxy               |                         |           |           |
| noexcept          | void             | _Free_proxy                |                         |           |           |

### struct _Iterator_base
| _Iterator_base |                    |             |
| -------------- | ------------------ | ----------- |
| mutable        | _Container_proxy\* | _Myproxy    |
| mutable        | _Iterator_base\*   | _Mynextiter |

| keyword / feature | return            | name                       | args                     | notes     | status    |
| ----------------- | ----------------- | -------------------------- | ------------------------ | --------- | --------- |
| noexcept          | _Iterator_base    | default Ctor [ = default ] |                          |           |           |
| noexcept          | _Iterator_base    | Ctor                       | const \_Iterator_base&   |           |           |
| noexcept          | _Iterator_base&   | operator=                  | const \_Iterator_base&   |           |           |
| noexcept          |                   | Dtor                       |                          |           |           |
| \--------         | \--------         | \--------                  | \--------                | \-------- | \-------- |
| noexcept          | _Container_base\* | _Get_cont                  |                          |           |           |
| noexcept          | void              | _Assign                    | const \_Iterator_base&   |           |           |
|                   | void              | _Alloc_proxy               |                          |           |           |
| noexcept          | void              | _Free_proxy                |                          |           |           |
| noexcept          | void              | _Adopt_me                  | const \_Container_base\* |           |           |
| noexcept          | void              | _Orphan_me_v1              |                          |           |           |
| noexcept          | void              | _Orphan_me_v2              |                          |           |           |


# list_node.hpp
### struct _List_node
| _List_node\<Value_type\> |                         |
| ---------------------- | ------------------------- |
| value_type             | _Value_type               |
| _Nodeptr               | _List_node<_Value_type>\* |

| keyword / feature  | return        | name                       | args                                   | notes     | status                |
| ------------------ | ------------- | -------------------------- | -------------------------------------- | --------- | --------------------- |
| explicit / template| _List_node    | Ctor                       | _Arg_t&&, \_Nodeptr, \_Nodeptr         | _Arg_t --> value_type |           |
|                    | _List_node    | default Ctor [ = default ] |                                        |           |                       |
|                    | _List_node    | Ctor [ = delete ]          | const \_List_node&                     |           |                       |
|                    | _List_node&   | operator= [ = delete ]     | const \_List_node&                     |           |                       |
| \--------          | \--------     | \--------                  | \--------                              | \-------- | \--------             |
| static             | _Nodeptr      | _Buy_head_node             |                                        |           |                       |
| static             | void          | _Freenode0                 | _Nodeptr                               |           |                       |
| static             | void          | _Freenode                  | _Nodeptr                               |           |                       |
| static             | void          | _Free_non_head             | _Nodeptr                               |           |                       |
| \--------          | \--------     | \--------                  | \--------                              | \-------- | \--------             |
| friend             | std::ostream& | operator<<                 | std::ostream&, const \_List_node<_Ty>& |           |                       |


# list_iterator.hpp

| Iterators                      |
| ------------------------------ |
| _List_unchecked_const_iterator |
| _List_const_iterator           |
| _List_const_iterator           |
| _List_iterator                 |

### struct _List_unchecked_const_iterator
| _List_unchecked_const_iterator<\_Mylist, \_Base>|                                |
| ----------------------------------------------- | ------------------------------- |
| iterator_category                               | std::bidirectional_iterator_tag |
| _Nodeptr                                        | _Mylist::_Nodeptr               |
| value_type                                      | _Mylist::value_type             |
| difference_type                                 | _Mylist::difference_type        |
| pointer                                         | _Mylist::const_pointer          |
| reference                                       | const value_type&               |

| _List_unchecked_const_iterator |        |
| ------------------------------ | ------ |
| _Nodeptr                       | _Myptr |

| keyword / feature           | return                         | name            | args                                  | notes     | status    |
| --------------------------- | ------------------------------ | --------------- | ------------------------------------- | --------- | --------- |
| noexcept                    | _List_unchecked_const_iterator | default Ctor    |                                       |           |           |
| noexcept                    | _List_unchecked_const_iterator | Ctor            | _Nodeptr, const \_Mylist\*            |           |           |
| \--------                   | \--------                      | \--------       | \--------                             | \-------- | \-------- |
| const / noexcept            | reference                      | operator\*      |                                       |           |           |
| const / noexcept            | pointer                        | operator->      |                                       |           |           |
| explicit / const / noexcept | bool                           | operator bool   |                                       |           |           |
| noexcept                    | _List_unchecked_const_iterator | operator++      |                                       |           |           |
| noexcept                    | _List_unchecked_const_iterator | operator++(int) |                                       |           |           |
| noexcept                    | _List_unchecked_const_iterator | operator--      |                                       |           |           |
| noexcept                    | _List_unchecked_const_iterator | operator--(int) |                                       |           |           |
| const / noexcept            | bool                           | operator==      | const \_List_unchecked_const_iterator |           |           |
| const / noexcept            | bool                           | operator!=      | const \_List_unchecked_const_iterator |           |           |

### struct _List_const_iterator
| _List_const_iterator | : public \_List_unchecked_const_iterator |
| -------------------- | ---------------------------------------- |
| _Mybase              | _List_unchecked_const_iterator           |
| iterator_category    | std::bidirectional_iterator_tag          |
| _Nodeptr             | _Mylist::_Nodeptr                        |
| value_type           | _Mylist::value_type                      |
| difference_type      | _Mylist::difference_type                 |
| pointer              | _Mylist::const_pointer                   |
| reference            | const value_type&                        |

| keyword / feature           | return                         | name            | args                                                     | notes     | status    |
| --------------------------- | ------------------------------ | --------------- | -------------------------------------------------------- | --------- | --------- |
| const / noexcept            | reference                      | operator\*      |                                                          |           |           |
| const / noexcept            | pointer                        | operator->      |                                                          |           |           |
| explicit / const / noexcept | bool                           | operator bool   |                                                          |           |           |
| noexcept                    | _List_const_iterator           | operator++      |                                                          |           |           |
| noexcept                    | _List_const_iterator           | operator++(int) |                                                          |           |           |
| noexcept                    | _List_const_iterator           | operator--      |                                                          |           |           |
| noexcept                    | _List_const_iterator           | operator--(int) |                                                          |           |           |
| const / noexcept            | bool                           | operator==      | const \_List_const_iterator                              |           |           |
| const / noexcept            | bool                           | operator!=      | const \_List_const_iterator                              |           |           |
| \--------                   | \--------                      | \--------       | \--------                                                | \-------- | \-------- |
| friend                      | void                           | _Verify_range   | const \_List_const_iterator, const \_List_const_iterator |           |           |
|                             | _List_unchecked_const_iterator | _Unwrapped      |                                                          |           |           |


# list.cpp
