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
	void run();		//����
	void getInstruction(); //��ȡָ��
	void readFile(std::string filepath); //��ȡ�ļ�
	void splitTokens(SqList<std::string>& sql, LinkList<token>& tokenList); //�з��ļ����������token
	int tagOpen(std::string& s, int i, token& t, int len);	//��������'<'֮��
	void buildDomTree(LinkList<token>& tokenList);		//����DOMTree

	void query(std::string& input, CSNode<token>* ptr);	//�����û�����query

private:
	CSTree<token> tokenTree;		//DOM Tree
	LinkList<tokenNode> result;		//query����','�зֺ󵥸�����node
	SqList<tokenNode> sqresult;		//query����node����
	std::vector<std::vector<std::string>> Elements;	//�з�query�γɵ���ƥ���element

	void initializeRes(const std::string& input, CSNode<token>* ptr);	//��ʼ���������ʼ����

	void startwithAsterisk();	//��'*'��ʼ�����뵥��'*'�������н�㣩
	void insertTN(const std::string& input, CSNode<token>* ptr);	//�ҵ�ƥ���node���뵽��
	void PreOrderAst(CSNode<token>* ptr);	//�������Ѱ������'*' node
	void preOrderTag(CSNode<token>* ptr, const std::string& tag);	//�������Ѱ��������tag��ƥ�� node
	void preOrderClass(CSNode<token>* ptr, const std::string& clss);	//�������Ѱ��������class��ƥ�� node
	void preOrderID(CSNode<token>* ptr, const std::string& id);		//�������Ѱ��������id��ƥ�� node
	void preOrderAttVal(CSNode<token>* ptr, const std::string& input);	//�������Ѱ��ƥ������Լ�ֵ node

	void filterTN(const std::string& input, CSNode<token>* ptr,	
		 std::string (*funcget1)(const tokenNode&), std::string(*funcget2)(const tokenNode&));	//�����ҳ���node������query���˲������node��������'+','>'
	//��������ָ�룬��һ����ȡ��class/ id / attribute����ƥ�䣬�ڶ�����ȡ��tag����ƥ��

	void filterWave(const std::string& input, CSNode<token>* ptr);	//�����ҳ���node������query���˲������node��������'~'

	void filterSpace(const std::string& input, CSNode<token>* ptr); //�����ҳ���node������query���˲������node��������' '

	void filter(const std::string& input, CSNode<token>* ptr,  
		bool (*func)(const std::string&, const std::string&), std::string(*funcget)(const tokenNode&)); //�����ҳ���node�����˲�����ģ���һ������ָ����ƥ�亯�����ڶ�������ָ���ǻ�ȡnode��Ϣ����
	void ll2sql(); //�Ѽ���queryƬ���ҵ�����������

	void outQuery(std::string& input, int k); //������ҵ�k��node

	std::string outHref(int k); //���ҵ�k��node��href
	std::string outText(int k); //���ҵ�k��node��innerText
	std::string outHTML(int k); //���ҵ�k��node��outerHTML
	void outSelector(int k, std::string& input); //�ҳ���k��node֮���������������

	void preOrderText(CSNode<token>* ptr, std::string& text); //��������������Ҷ�Ӧnode���ҳ���node��innerText���������������text֮��
	void preOrderHTML(CSNode<token>* ptr, std::string& HTML); //��������������Ҷ�Ӧnode���ҳ���node��outerHTML���������������HTML֮��
	
};



#endif // TERMINAL_H