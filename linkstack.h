#ifndef STACK_H
#define STACK_H

#include "lnode.h"
#include <cstddef>
#include <cstdlib>

template<class Element>
class LinkedStack
{
public:
    LinkedStack();
    LinkedStack(const LinkedStack& LS);
    ~LinkedStack();

    bool isEmpty();
    void Push(Element& e);
    Element Pop();
    Element getTopElement();
    void Clear();

    LNode<Element>* getTop() { return top; }
    void setTop(LNode<Element>* p) { top = p; }
    int len() { return length; }

private:
    int length;
    LNode<Element>* top;
};

template<class Element>
LinkedStack<Element>::LinkedStack()
{
    length = 0;
    top = new LNode<Element>;
}

template<class Element>
LinkedStack<Element>::LinkedStack(const LinkedStack& ls)
{
    length = ls.length;
    top = new LNode<Element>;
    LNode<Element>* pls = ls.top->getNext();
    LNode<Element>* temp = top;
    while (pls != NULL)
    {
        auto* newNode = new LNode<Element>;
        newNode->setData(pls->getData());
        temp->getNext(newNode);
        temp = temp->getNext();
        pls = pls->getNext();
    }
}

template<class Element>
LinkedStack<Element>::~LinkedStack() {}

template<class Element>
Element LinkedStack<Element>::getTopElement()
{
    if (top == NULL || !length)
    {
        exit(1);
    }
    Element e = top->getNext()->getData();
    return e;
}

template<class Element>
Element LinkedStack<Element>::Pop()
{
    if (top == NULL)
        exit(1);
    Element e = top->getNext()->getData();
    LNode<Element>* temp = top->getNext();
    top->setNext(temp->getNext());
    delete temp;
    length--;
    return e;
}

template<class Element>
void LinkedStack<Element>::Clear()
{
    LNode<Element>* p = top->getNext();
    while (p != NULL)
    {
        LNode<Element>* temp = p;
        p = p->getNext();
        delete temp;
    }
    top->setNext(NULL);
}

template<class Element>
bool LinkedStack<Element>::isEmpty()
{
    return length == 0;
}

template<class Element>
void LinkedStack<Element>::Push(Element& e)
{
    LNode<Element>* newNode = new LNode<Element>;
    newNode->setData(e);
    newNode->setNext(top->getNext());
    top->setNext(newNode);
    length++;
}
#endif //STACK_H