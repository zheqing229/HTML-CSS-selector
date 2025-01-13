#ifndef LINKLIST_H
#define LINKLIST_H

#include <iostream>
#include <cstdlib>
#include "LNode.h"
#include "datadef.h"
#include "sqlist.h"
template<class Element>
class LinkList
{
public:
    LinkList();
    LinkList(int n);
    LinkList(const LinkList& L);
    ~LinkList();
    int Insert(Element& e, int i);
    void append(Element e);
    Element Delete(int i);

    Element Delete(LNode<Element>*& prev, LNode<Element>*& cur)
    {
        LNode<Element>* p = cur;
        cur = cur->getNext();
        prev->setNext(cur);
        Element e = p->getData();
        delete p;
        p = NULL;
        length--;
        return e;
    }

    int len() { return length; }
    LNode<Element>* getHead() { return head; }
    void setHead(LNode<Element>* p) { head = p; }
    void setLength(int n) { length = n; }
    void setTail(LNode<Element>* p) { tail = p; }
    LNode<Element>* getTail() { return tail; }
    void Reverse();

    bool find(Element& e, SqList<int>& array);

    void Clear();

    Element& operator[] (int i);
    const Element& operator[] (int i)const;

    friend std::ostream& operator << (std::ostream& out, const LinkList<Element>& list)
    {
        LNode<Element>* p = list.head;
        p = p->getNext();
        while (p != NULL)
        {
            out << p->getData() << std::endl;
            p = p->getNext();
        }
        return out;
    }

    friend std::wostream& operator << (std::wostream& out, const LinkList<Element>& list)
    {
        std::locale china("zh_CH");
        LNode<Element>* p = list.head;
        p = p->getNext();
        while (p != NULL)
        {
            out << p->getData() << std::endl;
            p = p->getNext();
        }
        return out;
    }

private:
    LNode<Element>* head;
    LNode<Element>* tail;
    int length;

};

template<class Element>
LinkList<Element>::LinkList()
{
    head = new LNode<Element>;
    tail = head;
    length = 0;
}

template<class Element>
LinkList<Element>::LinkList(int n) : head(NULL), length(n)
{
    head = new LNode<Element>;
    tail = head;
    head->setNext(NULL);
    for (int i = n; i > 0; i--)
    {
        LNode<Element>* p = new LNode<Element>;
        Element e;
        std::cin >> e;
        p->setData(e);
        p->setNext(head->getNext());
        head->setNext(p);
        tail = p;
    }
}

template<class Element>
LinkList<Element>::LinkList(const LinkList& L)
{
    head = new LNode<Element>;
    length = 0;
    LNode<Element>* temp = L.head;
    tail = new LNode<Element>;
    head->setNext(tail);
    while (temp->getNext() != NULL)
    {
        temp = temp->getNext();
        tail->data = temp->data;
        LNode<Element>* p = new LNode<Element>;
        tail->next = p;
        tail = p;
        length++;
    }
    tail->next = NULL;
}

template <typename Element>
LinkList<Element>::~LinkList()
{
    if (length == 0)
    {
        if (head == NULL)
            return;
        delete head;
        head = NULL;
        return;
    }
    while (head->getNext() != NULL)
    {
        LNode<Element>* temp = head;
        head = head->getNext();
        delete temp;
        temp = NULL;
    }
    delete head;
    head = NULL;
    length = 0;
}

template<class Element>
int LinkList<Element>::Insert(Element& e, int i)
{
    LNode<Element>* p = head;
    int j = 0;
    while (p != NULL && j < i - 1)
    {
        p = p->getNext();
        ++j;
    }
    if (p == NULL || j > i - 1)
    {
        exit(1);
        return 0;
    }
    LNode<Element>* s = new LNode<Element>;
    s->setData(e);
    s->setNext(p->getNext());
    p->setNext(s);
    length++;
    return 1;
}

template<class Element>
void LinkList<Element>::append(Element e)
{
    LNode<Element>* p = new LNode<Element>;
    p->setData(e);
    if (tail == NULL)
        exit(1);
    tail->setNext(p);
    tail = p;
    length++;
}

template<class Element>
void LinkList<Element>::Reverse()
{
    LNode<Element>* previous = NULL;
    LNode<Element>* current = head->getNext();
    LNode<Element>* next = NULL;
    tail = current;
    while (current != NULL)
    {
        next = current->getNext();
        current->setNext(previous);
        previous = current;
        current = next;
    }
    head->setNext(previous);
}


template<class Element>
bool LinkList<Element>::find(Element& e1, SqList<int>& array)
{
    LNode<Element>* p = head->getNext();
    bool result = false;
    for (int i = 0; i < length && p != NULL; i++)
    {
        Element e2 = p->getData();
        if (e1 == e2)
        {
            int j = i + 1;
            array.push_back(j);
            result = true;
        }
        p = p->getNext();
    }
    return result;
}

template<class Element>
void LinkList<Element>::Clear()
{
    if (length == 0)
    {
        //if (head == NULL)
          //  return;
        //delete head;
        //head = NULL;
        return;
    }
    LNode<Element>* p = head->getNext();
    while (p != NULL)
    {
        LNode<Element>* temp = p;
        p = p->getNext();
        delete temp;
        temp = NULL;
        length--;
    }
    // delete head;
    // head = NULL;
    head->setNext(NULL);
    tail = head;
}

template<class Element>
Element& LinkList<Element>::operator[](int i)
{
    int j = 1;
    LNode<Element>* p = head->getNext();
    while (p != NULL && j < i)
    {
        p = p->getNext();
        j++;
    }
    if (p == NULL || j > i) std::exit(-1);
    return p->getData();
}

template<class Element>
const Element& LinkList<Element>::operator[](int i) const
{
    int j = 1;
    LNode<Element>* p = head->getNext();
    while (p != NULL && j < i)
    {
        p = p->getNext();
        j++;
    }
    if (p == NULL || j > i) std::exit(-1);
    return p->getData();
}

template<class Element>
Element LinkList<Element>::Delete(int i)
{
    LNode<Element>* p = head;
    int j = 0;
    while (p->getNext() != NULL && j < i - 1)
    {
        p = p->getNext();
        j++;
    }
    if (p->getNext() == NULL || j > i - 1)
        exit(-1);
    LNode<Element>* q = p->getNext();
    p->setNext(q->getNext());
    Element e = q->getData();
    delete q;
    length--;
    return e;
}
#endif //LINKLIST_H