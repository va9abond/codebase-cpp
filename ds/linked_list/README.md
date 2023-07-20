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
### struct _List_simple_type_traits
| _List_simple_type_traits<_Value_type> | : public \_Simple_type_traits |
| ------------------------------------- | ----------------------------- |
| _Node                                 | _List_node<_Value_type>       |
| _Nodeptr                              | _List_node<_Value_type>\*     |

### class _List_val
| _List_val<_Val_types> | : public \_Container_base  |
| --------------------- | -------------------------- |
| _Nodeptr              | _Val_type::_Nodeptr        |
| value_type            | _Val_type::value_type      |
| size_type             | _Val_type::size_type       |
| defference_type       | _Val_type::difference_type |
| pointer               | _Val_type::pointer         |
| const_pointer         | _Val_type::const_pointer   |
| reference             | value_type&                |
| const_reference       | const value_type&          |

| _List_val | : public \_Container_base |
| --------- | ------------------------- |
| _Nodeptr  | _Myhead                   |
| size_type | _Mysize                   |

| keyword / feature | return    | name                | args      | notes     |
| ----------------- | --------- | ------------------- | --------- | --------- |
|                   | _List_val | default Ctor        |           |           |
| \--------         | \-------- | \--------           | \-------- | \-------- |
| noexcept          | void      | _Orphan_iter_on_ptr | _Nodeptr  |           |
| noexcept          | void      | _Orphan_non_end     |           |           |
| noexcept          | _Nodeptr  | _Unlink_node        | _Nodeptr  |           |

### struct _List_node_emplace_v2
| _List_node_emplace_v2<_Node_t> |           |
| ------------------------------ | --------- |
| pointer                        | _Node_t\* |

| _List_node_emplace_v2 |         |
| --------------------- | ------- |
| pointer               | _Mynode |

| keyword / feature   | return                | name                   | args           | notes     |
| ------------------- | --------------------- | ---------------------- | -------------- | --------- |
| template / explicit | _List_node_emplace_v2 | Ctor                   | _Value_types&& |           |
| template / explicit | _List_node_emplace_v2 | Ctor                   | _Value_types&& |           |
|                     |                       | Dtor                   |                | TODO: fix |
|                     | _List_node_emplace_v2 | Ctor [ = delete ]      |                |           |
|                     | _List_node_emplace_v2 | operator= [ = delete ] |                |           |
| \--------           | \--------             | \--------              | \--------      | \-------- |
| noexcept            | pointer               | _Transfer_before       | const pointer  |           |

### struct _List_node_insert_v2
| _List_node_insert_v2<_Node_t> |             |
| ----------------------------- | ----------- |
| value_type                    | _Node_t     |
| pointer                       | _Node_t\*   |
| size_type                     | std::size_t |

| _List_node_insert_v2 |        |
| -------------------- | ------ |
| size_type            | _Added |
| pointer              | _Head  |
| pointer              | _Tail  |

| keyword / feature | return               | name                    | args                                 | notes     |
| ----------------- | -------------------- | ----------------------- | ------------------------------------ | --------- |
| explicit          | _List_node_insert_v2 | default Ctor            |                                      |           |
|                   | _List_node_insert_v2 | Ctor [ = delete ]       |                                      |           |
|                   | _List_node_insert_v2 | Ctor [ = delete ]       |                                      |           |
|                   |                      | Dtor                    |                                      |           |
| \--------         | \--------            | \--------               | \--------                            | \-------- |
|                   | void                 | _Append_n               | size_type                            |           |
| template          | void                 | _Append_range_unchecked | _Iter_t1, \_Iter_t2                  |           |
| template          | pointer              | _Attach_before          | _List_val<_Val_type>&, const pointer |           |
| template          | void                 | _Attach_at_end          | _List_val<_Val_type>&                |           |
| template          | void                 | _Attach_head            | _List_val<_Val_type>&                |           |

### class list_v2

| list_v2<_Ty>              |                                                 |
| ------------------------- | ----------------------------------------------- |
| _Alloc                    | std::allocator<_Ty>                             |
| _Node                     | _List_node<_Ty>                                 |
| _Nodeptr                  | _List_node<_Ty>\*                               |
| \--------                 | \--------                                       |
| _Val_types                | _List_simple_type_traits<_Ty>                   |
| _List_SCARY_val           | _List_val<_Val_types>                           |
| _List_node_insert         | _List_node_insert_v2<_Node>                     |
| _List_node_emplace        | _List_node_emplace_v2<_Node>                    |
| \--------                 | \--------                                       |
| value_type                | _Ty                                             |
| allocator_type            | _Alloc                                          |
| size_type                 | std::size_t                                     |
| difference_type           | std::ptrdiff_t                                  |
| pointer                   | _Ty\*                                           |
| const_pointer             | const \_Ty\*                                    |
| reference                 | value_type&                                     |
| const_reference           | const value_type&                               |
| \--------                 | \--------                                       |
| iterator                  | _List_iterator<_List_SCARY_val>                 |
| const_iterator            | _List_const_iterator<_List_SCARY_val>           |
| _Unchecked_iterator       | _List_unchecked_iterator<_List_SCARY_val>       |
| _Unchecked_const_iterator | _List_const_unchecked_iterator<_List_SCARY_val> |
| \--------                 | \--------                                       |
| reverse_iterator          | std::reverse_iterator<iterator>                 |
| const_reverse_iterator    | std::reverse_iterator<const_iterator>           |

| list_v2         |         |
| --------------- | ------- |
| _List_SCARY_val | _Mycont |

| keyword / feature | return          | name                       | args                                       | notes                                        |
| ----------------- | --------------- | -------------------------- | ------------------------------------------ | -------------------------------------------- |
|                   | list_v2         | default Ctor               |                                            |                                              |
| explicit          | list_v2         | Ctor                       | size_type                                  |                                              |
|                   | list_v2         | Ctor                       | size_type, const \_Ty&                     |                                              |
|                   | list_v2         | Ctor                       | const list_v2&                             |                                              |
|                   | list_v2         | Ctor                       | list_v2&&                                  |                                              |
|                   | list_v2         | Ctor                       | std::init_list                             |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
|                   | void            | _Construct_n               | size_type                                  |                                              |
|                   | void            | _Construct_n               | size_type, const \_Ty&                     | TODO: impl \_Append(size_type, const \_Ty&)  |
| template          | void            | _Construct_range_unchecked | _Iter_t1, const \_Iter_t2                  |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
|                   | list_v2&        | operator=                  | list_v2&&                                  | TODO: Dtor                                   |
|                   | list_v2&        | operator=                  | std::init_list                             | TODO: assign (_Iter_t1, \_Iter_t2)           |
|                   | list_v2&        | operator=                  | const list_v2&                             | TODO:                                        |
|                   | void            | assign                     | std::init_list                             | TODO: assign (_Iter_t1, \_Iter_t2)           |
|                   |                 | Dtor                       |                                            |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
| noexcept          | void            | _Swap_val                  | list_v2&                                   |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
|                   | void            | push_front                 | _Ty&&                                      |                                              |
|                   | void            | push_back                  | _Ty&&                                      |                                              |
|                   | void            | push_front                 | _Ty&                                       |                                              |
|                   | void            | push_back                  | _Ty&                                       |                                              |
|                   | decltype(auto)  | emplace_front              | _Ty&&                                      | TODO: template                               |
|                   | decltype(auto)  | emplace_back               | _Ty&&                                      | TODO: template                               |
|                   | iterator        | emplace                    | const_iterator, \_Ty&&                     | TODO: template                               |
|                   | _Nodeptr        | emplace                    | const \_Nodeptr, \_Ty&&                    | TODO: template                               |
|                   | iterator        | insert                     | const_iterator, \_Ty&&                     |                                              |
|                   | iterator        | insert                     | const_iterator, std::init_list             |                                              |
|                   | iterator        | insert                     | const_iterator, const \_Ty&                |                                              |
|                   | iterator        | insert                     | const_iterator, size_type, const \_Ty&     | TODO: impl \_Append(size_type, const \_Ty&)  |
| template          | iterator        | insert                     | _Iter_t, \_Iter_t                          | TODO: verify range; 2 iter types in template |
|                   | void            | pop_front                  |                                            |                                              |
|                   | void            | pop_back                   |                                            |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
|                   | all iterators   |                            |                                            |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
|                   | void            | resize                     | size_type                                  |                                              |
|                   | void            | resize                     | size_type, const \_Ty&                     |                                              |
| const / noexcept  | size_type       | size                       |                                            |                                              |
| const / noexcept  | size_type       | max_size                   |                                            |                                              |
| const / noexcept  | bool            | empty                      |                                            |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
| noexcept          | reference       | front                      |                                            |                                              |
| const / noexcept  | const_reference | front                      |                                            |                                              |
| noexcept          | reference       | back                       |                                            |                                              |
| const / noexcept  | const_reference | back                       |                                            |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
| noexcept          | iterator        | erase                      | const const_iterator                       |                                              |
| noexcept          | iterator        | erase                      | const const_iterator, const const_iterator | TODO: verify range                           |
| noexcept          | _Nodeptr        | _Unchecked_erase           | const \_Nodeptr                            |                                              |
| noexcept          | _Nodeptr        | _Unchecked_erase           | _Nodeptr, const \_Nodeptr                  |                                              |
| noexcept          | void            | clear                      |                                            |                                              |
| noexcept          | void            | _Tidy                      |                                            |                                              |
| noexcept          | swap            | list_v2&                   |                                            |                                              |
| \--------         | \--------       | \--------                  | \--------                                  | \--------                                    |
|                   | void            | _Alloc_head_and_proxy      |                                            |                                              |
| noexcept          | void            | _Orphan_all                |                                            |                                              |