#ifndef TREE_H
#define TREE_H
#include<iostream>
#include "token.h"
#include "sqlist.h"

template<class DataType>
class CSNode {
public:
    DataType data;
	CSNode<DataType>* firstchild;  //��һ�����ӽ��
	CSNode<DataType>* nextsibling; //��һ���ֵܽ��
    CSNode<DataType>* parent;   //����ṹ�ĸ��׽�㣬���Ǵ洢�ṹ

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
    void PreOrder() { PreOrder(root); }	//ǰ�����
    void InOrder() { InOrder(root); }		//�������
    void PostOrder() { PostOrder(root); }	//�������

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