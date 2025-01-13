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
	std::string TagName;	//标签名
	std::string attribute;  //属性
	TokenType type;			//类型	StartTag, EndTag, Comment, Character, Uninitialized, DOCTYPE, EndOfFile
	std::string text;		//包含文字

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


	void display();		// 打印到控制台

	std::string write();	//	把一般标签写成字符串
	std::string writeEnd();		// 把标签结尾</>写成字符串
};


struct tokenNode
{
	token t;
	CSNode<token>* ptr;
};

#endif // ! TOKEN_H