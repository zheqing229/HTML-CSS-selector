#ifndef TOKEN_H
#define TOKEN_H

#include "datadef.h"
#include "tree.h"
#include <string>
#include <iostream>
#include "sqlist.h"

class token
{
private:
	std::string TagName;	//��ǩ��
	std::string attribute;  //����
	TokenType type;			//����	StartTag, EndTag, Comment, Character, Uninitialized, DOCTYPE, EndOfFile
	std::string text;		//��������

public:
	token() { type = Uninitialized; }
	token(const token& t);
	~token() {}

	std::string getTagName()const { return TagName; }
	void setTagName(std::string& s) { TagName = s; }

	std::string getAttribute()const { return attribute; }
	void setAttribute(std::string& s) { attribute = s; }
	
	TokenType getType() { return type; }
	void setType(TokenType t) { type = t; }
	
	std::string getText()const { return text; }
	void setText(std::string& s) { text = s; }

	token& operator = (const token & t);
	friend std::ostream& operator <<(std::ostream& out, const token& t);


	void display();		// ��ӡ������̨

	std::string write();	//	��һ���ǩд���ַ���
	std::string writeEnd();		// �ѱ�ǩ��β</>д���ַ���
};


struct tokenNode
{
	token t;
	CSNode<token>* ptr;
};

#endif // ! TOKEN_H