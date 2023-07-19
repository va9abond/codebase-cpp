| _List_node<Value_type> |                           |
| ---------------------- | ------------------------- |
| value_type             | _Value_type               |
| _Nodeptr               | _List_node<_Value_type>\* |

| keyword  | name                                      | status  | notes   |
| -------- | ----------------------------------------- | ------- | ------- |
| explicit | _List_node (const &)                      |         |         |
| explicit | _List_node (&&)                           |         |         |
|          | _List_node (std::forward<value_type>)     |         |         |
|          | _List_node ()                             |         |         |
|          | _List_node (const \_List_node&)           |         |         |
|          | _List_node operator= (const \_List_node&) |         |         |
| \------  | \------------                             | \------ | \------ |
| static   | _Nodeptr \_Buy_head_node ()               |         |         |
| static   | void \_Freenode0 (_Nodeptr)               |         |         |
| static   | void \_Freenode (_Nodeptr)                |         |         |
| static   | void \_Free_non_head (_Nodeptr)           |         |         |
| friend   | std::ostream& operator<< (...)            |         |         |

| keyword  | return        | name                           | args                                   | status  | notes   |
| -------- | ------------- | ------------------------------ | -------------------------------------- | ------- | ------- |
| explicit | _List_node    | Dtor                           | const value_type&                      |         |         |
| explicit | _List_node    | Dtor                           | value_type&&                           |         |         |
| explicit | _List_node    | Dtor                           |                                        |         |         |
|          | _List_node    | Dtor: default                  |                                        |         |         |
|          | _List_node    | Dtor: delete                   | const \_List_node&                     |         |         |
|          | _List_node&   | operator=: delete              | const \_List_node&                     |         |         |
| \------  |               | \------------                  |                                        | \------ | \------ |
| static   | _Nodeptr      | _Buy_head_node                 |                                        |         |         |
| static   | void          | _Freenode0                     | _Nodeptr                               |         |         |
| static   | void          | _Freenode                      | _Nodeptr                               |         |         |
| static   | void          | _Free_non_head                 | _Nodeptr                               |         |         |
| \------  |               | \------------                  |                                        | \------ | \------ |
| friend   | std::ostream& | std::ostream& operator<< (...) | std::ostream&, const \_List_node<_Ty>& |         |