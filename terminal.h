#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <fstream>
#include "sqlist.h"
#include <string>
#include "linklist.h"
#include "token.h"
#include "tree.h"
#include "linkstack.h"
#include "util.h"


class terminal
{
public:
	terminal() {}
	void run();		//运行
	void getInstruction(); //获取指令
	void readFile(std::string filepath); //读取文件
	void splitTokens(SqList<std::string>& sql, LinkList<token>& tokenList); //切分文件输入进来的token
	int tagOpen(std::string& s, int i, token& t, int len);	//处理输入'<'之后
	void buildDomTree(LinkList<token>& tokenList);		//构建DOMTree

	void query(std::string& input, CSNode<token>* ptr);	//处理用户输入query

private:
	CSTree<token> tokenTree;		//DOM Tree
	LinkList<tokenNode> result;		//query按照','切分后单个返回node
	SqList<tokenNode> sqresult;		//query返回node汇总
	std::vector<std::vector<std::string>> Elements;	//切分query形成单个匹配的element

	void initializeRes(const std::string& input, CSNode<token>* ptr);	//初始化结果，初始查找

	void startwithAsterisk();	//从'*'开始（输入单个'*'返回所有结点）
	void insertTN(const std::string& input, CSNode<token>* ptr);	//找到匹配的node插入到答案
	void PreOrderAst(CSNode<token>* ptr);	//先序遍历寻找满足'*' node
	void preOrderTag(CSNode<token>* ptr, const std::string& tag);	//先序遍历寻找与输入tag相匹配 node
	void preOrderClass(CSNode<token>* ptr, const std::string& clss);	//先序遍历寻找与输入class相匹配 node
	void preOrderID(CSNode<token>* ptr, const std::string& id);		//先序遍历寻找与输入id相匹配 node
	void preOrderAttVal(CSNode<token>* ptr, const std::string& input);	//先序遍历寻找匹配的属性键值 node

	void filterTN(const std::string& input, CSNode<token>* ptr,	
		 std::string (*funcget1)(const tokenNode&), std::string(*funcget2)(const tokenNode&));	//遍历找出的node，根据query过滤不满足的node，适用于'+','>'
	//两个函数指针，第一个获取其class/ id / attribute进行匹配，第二个获取其tag进行匹配

	void filterWave(const std::string& input, CSNode<token>* ptr);	//遍历找出的node，根据query过滤不满足的node，适用于'~'

	void filterSpace(const std::string& input, CSNode<token>* ptr); //遍历找出的node，根据query过滤不满足的node，适用于' '

	void filter(const std::string& input, CSNode<token>* ptr,  
		bool (*func)(const std::string&, const std::string&), std::string(*funcget)(const tokenNode&)); //遍历找出的node，过滤不满足的，第一个函数指针是匹配函数，第二个函数指针是获取node信息函数
	void ll2sql(); //把几个query片段找到结果组合起来

	void outQuery(std::string& input, int k); //处理查找第k个node

	std::string outHref(int k); //查找第k个node的href
	std::string outText(int k); //查找第k个node的innerText
	std::string outHTML(int k); //查找第k个node的outerHTML
	void outSelector(int k, std::string& input); //找出第k个node之后继续查找其属性

	void preOrderText(CSNode<token>* ptr, std::string& text); //按照先序遍历查找对应node，找出该node下innerText并延续到输入参数text之后
	void preOrderHTML(CSNode<token>* ptr, std::string& HTML); //按照先序遍历查找对应node，找出该node的outerHTML并延续到输入参数HTML之后
	
};



#endif // TERMINAL_H