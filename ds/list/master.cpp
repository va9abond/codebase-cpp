#include "List_node.cpp"


using namespace myl;
template<class Ty_> using node = _List_node<Ty_>;
using std::cout;
using std::endl;

int main() {
    node<int>* head = new node<int>(42, nullptr, nullptr);
    head->_Next = head->_Prev = head;
    head->_Next = new node<int>(24, head, head->_Prev);

    cout << head->_Myval;

}