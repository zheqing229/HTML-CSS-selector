#ifndef LNODE_H
#define LNODE_H

#include <cwchar>

template<class Element>
class LNode
{
public:
    LNode() { next = NULL; }
    LNode(Element& e) :data(e), next(NULL) { next = new LNode<Element>; }
    LNode(const LNode& e) { next = e.next; data = e.data; }
    ~LNode() { }
    Element& getData() { return data; }
    void setData(Element& e) { data = e; }

    LNode<Element>* getNext() { return next; }
    void setNext(LNode<Element>* pNext) { next = pNext; }


private:
    Element data;
    LNode<Element>* next;
};
#endif // !LNODE_H

