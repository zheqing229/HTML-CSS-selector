#ifndef TREE_H
#define TREE_H
#include<iostream>
#include "token.h"
#include "sqlist.h"

template<class DataType>
class CSNode {
public:
    DataType data;
	CSNode<DataType>* firstchild;  //第一个孩子结点
	CSNode<DataType>* nextsibling; //下一个兄弟结点
    CSNode<DataType>* parent;   //物理结构的父亲结点，并非存储结构

    CSNode();
    CSNode(const CSNode& csn);
};

template<class DataType>
CSNode<DataType>::CSNode()
{
    firstchild = NULL;
    nextsibling = NULL;
    parent = NULL;
}

template<class DataType>
inline CSNode<DataType>::CSNode(const CSNode& csn): firstchild(csn.firstchild),nextsibling(csn.nextsibling)
{
    data = csn.data;
    firstchild = csn.firstchild;
    nextsibling = csn.nextsibling;
    parent = csn.parent;
}

template <class DataType>
class CSTree
{
public:
    CSNode<DataType>* root;
    CSTree();
    ~CSTree() { }
    void PreOrder() { PreOrder(root); }	//前序遍历
    void InOrder() { InOrder(root); }		//中序遍历
    void PostOrder() { PostOrder(root); }	//后序遍历

private:
    void PreOrder(CSNode<DataType>* cst);
    
    void InOrder(CSNode<DataType>* cst);
    void PostOrder(CSNode<DataType>* cst);
};

template<class DataType>
inline CSTree<DataType>::CSTree()
{
    root = new CSNode<DataType>;
}


template <class DataType>
void CSTree<DataType>::PreOrder(CSNode<DataType>* cst)
{
    if (cst == NULL) return;
    else {
        std::cout << cst->data << '\n';
        PreOrder(cst->firstchild);
        PreOrder(cst->nextsibling);
    }
}

template <class DataType>
void CSTree<DataType>::InOrder(CSNode<DataType>* cst)
{
    if (cst == NULL) return;
    else {
        InOrder(cst->firstchild);
        std::cout << cst->data;
        InOrder(cst->nextsibling);
    }
}

template <class DataType>
void CSTree<DataType>::PostOrder(CSNode<DataType>* cst)
{
    if (cst == NULL) return;
    else {
        PostOrder(cst->firstchild);
        PostOrder(cst->nextsibling);
        std::cout << cst->data;
    }
}


#endif // !TREE_H