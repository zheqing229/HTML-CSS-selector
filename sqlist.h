#ifndef SQLIST_H
#define SQLIST_H
#include "datadef.h"
#include <iostream>
#include <cstdlib>

template<class Element>
class SqList
{
private:
    Element* elem;
    int length;
    int listsize;

    void copyfrom(const SqList<Element>& sql);

public:
    SqList();
    SqList(Element* elem, int length, int listsize);
    SqList(const SqList<Element>& sql);
    ~SqList();

    void Clear();
    bool isEmpty();
    int Length();
    void resize(int new_length, bool to_copy = true);
    int Insert(int i, Element& e);
    Element Delete(int i);
    int Locate(Element& e);
    void push_back(Element& e);
    Element& Prior(Element& cur_e);
    Element& Next(Element& cur_e);
    bool Traverse();
    void Union(SqList<Element>& sql)
    {
        for (int i = 0; i < sql.length; i++)
        {
            if (!Locate(sql.elem[i]))
                Insert(++length, sql.elem[i]);
        }
    }


    Element& operator[] (int i);
    const Element& operator[] (int i)const;
    friend std::istream& operator >> (std::istream& in,
        SqList<Element>& sql)
    {
        in >> sql.length;
        for (int i = 0; i < sql.length; i++)
        {
            in >> sql.elem[i];
        }
        return in;
    }

    friend std::ostream& operator << (std::ostream& out,
        const SqList<Element>& sql)
    {
        for (int i = 0; i < sql.length; i++)
        {
            if (i != sql.length)
                out << sql.elem[i] << ' ';
            else
                out << sql.elem[i] << '\n';
        }
        return out;
    }
};

template<class Element>
void SqList<Element>::push_back(Element& e)
{
    if (length >= listsize)
    {
        resize(listsize + LISTINCREMENT);
    }
    elem[length] = e;
    length++;
}

template <class Element>
SqList<Element>::SqList()
{
    elem = new Element[LIST_INIT_SIZE];
    if (elem == NULL) std::exit(OVERFLOW);
    length = 0;
    listsize = LIST_INIT_SIZE;
}

template<class Element>
SqList<Element>::SqList(Element* elem, int length, int listsize) :
    elem(elem), length(length), listsize(listsize)
{}

template<class Element>
SqList<Element>::SqList(const SqList<Element>& sql)
{
    length = sql.length;
    listsize = sql.listsize;
    elem = new Element[length];
    copyfrom(sql);
}

template<class Element>
void SqList<Element>::copyfrom(const SqList<Element>& sql)
{
    if (sql.length != length) return;
    for (int i = 0; i < length; i++)
        elem[i] = sql.elem[i];
}


template<class Element>
SqList<Element>::~SqList()
{}

template<class Element>
void SqList<Element>::Clear()
{
    if (elem != NULL)
    {
        delete[] elem;
    }
    elem = new Element[LIST_INIT_SIZE];
    length = 0;
    listsize = LIST_INIT_SIZE;
}

template<class Element>
inline bool SqList<Element>::isEmpty()
{
    if (elem != NULL) return false;
    if (length == 0) return false;
    return true;
}

template<class Element>
inline int SqList<Element>::Length()
{
    return length;
}

template<class Element>
void SqList<Element>::resize(int new_length, bool to_copy)
{
    int m = length > new_length ? length : new_length;
    Element* newbase = new Element[m];
    if (to_copy && m == new_length)
    {
        for (int i = 0; i < length; i++)
        {
            newbase[i] = elem[i];
        }
    }
    else if (to_copy && m == length)
    {
        for (int i = 0; i < new_length; i++)
        {
            newbase[i] = elem[i];
        }
    }
    if (newbase == NULL) std::exit(OVERFLOW);
    delete[] elem;
    elem = NULL;
    elem = newbase;
    listsize += LISTINCREMENT;
}

template<class Element>
int SqList<Element>::Insert(int i, Element& e)
{
    if (i < 1 || i > length + 1) return ERROR;
    if (length >= listsize)
    {
        resize(listsize + LISTINCREMENT);
    }
    for (int j = length; j >= i; j--)
    {
        elem[j] = elem[j - 1];
    }
    elem[i - 1] = e;
    length++;
    return 1;
}

template<class Element>
Element SqList<Element>::Delete(int i)
{
    if (i < 1 || i>length) std::exit(ERROR);
    Element e;
    e = elem[i - 1];
    for (int j = i - 1; j < length - 1; j++)
    {
        elem[j] = elem[j + 1];
    }
    --length;
    return e;
}

template<class Element>
int SqList<Element>::Locate(Element& e)
{
    int i = 1;
    while (i <= length && elem[i - 1] != e)
    {
        i++;
    }
    if (i <= length) return i;
    else return 0;
}

template<class Element>
Element& SqList<Element>::Prior(Element& cur_e)
{
    if (elem == NULL) std::exit(ERROR);
    int loc = Locate(cur_e);
    if (loc == 0 || loc == 1) std::exit(ERROR);
    return elem[loc - 2];
}

template<class Element>
Element& SqList<Element>::Next(Element& cur_e)
{
    if (elem == NULL) std::exit(ERROR);
    int loc = Locate(cur_e);
    if (loc == 0 || loc == length) std::exit(ERROR);
    return elem[loc];
}

template<class Element>
Element& SqList<Element>::operator[](int i)
{
    if (elem == NULL)std::exit(ERROR);
    if (i < 0 || i >= length) std::exit(ERROR);
    return elem[i];
}

template<class Element>
const Element& SqList<Element>::operator[] (int i)const
{
    if (elem == NULL) std::exit(ERROR);
    if (i < 0 || i >= length) std::exit(ERROR);
    return elem[i];
}

template<class Element>
bool SqList<Element>::Traverse()
{
    for (int i = 0; i < length; i++)
        if (!elem[i])
            return false;
    return true;
}


#endif // !SQLIST_H
