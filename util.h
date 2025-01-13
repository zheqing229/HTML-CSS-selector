#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include "token.h"
#include "tree.h"

/// ����ָ���ָ����з��ַ���
std::vector<std::string> split(const std::string& s, char delimiter);

///','�����ǻ�Ĺ�ϵ�����з֣����Ű��ո��з�ÿһ��
void splitQuery(const std::string& query, std::vector<std::vector<std::string>>& Elements);

///  �зֿո������ÿһ��С��
std::vector<std::string> splitElements(const std::string& element);

/// �з�С���е�attribute 
std::vector<std::string>  splitAttribute(const std::string& input);

/// �з�html�ļ���token��tag��attributeֵ
std::vector<std::vector<std::string>> splitTagAttribute(const std::string& input);

/// tagƥ�亯��
bool matchTag(const std::string& tag, const std::string& tartag);

/// classƥ�亯��
bool matchClass(const std::string& att, const std::string& clss);

/// idƥ�亯��
bool matchID(const std::string& att, const std::string& id);

/// attributeƥ�亯��
bool matchAttVal(const std::string& att, const std::string& input);

/// ~��ϵ�µ�ƥ�亯��
bool matchWave(const std::string& str, const CSNode<token>* cur,
	bool (*match)(const std::string&, const std::string&), std::string(*get)(const token&));

/// �ո��ϵ�µ�ƥ�亯��
bool matchSpace(const std::string& str, const CSNode<token>* cur, const CSNode<token>* root,
	bool (*match)(const std::string&, const std::string&), std::string(*get)(const token&));

std::string getParentAtt(const tokenNode& tn); /// ��ȡ�����attribute
std::string getParentTag(const tokenNode& tn); ///  ��ȡ������tag

std::string getMineAtt(const tokenNode& tn); /// ��ȡ�Լ���tag
std::string getMineTag(const tokenNode& tn);///��ȡ�Լ���attribute

std::string getSiblingAtt(const tokenNode& tn);/// ��ȡ�ֵܽ���attribute
std::string getSiblingTag(const tokenNode& tn);/// ��ȡ�ֵܽ���tag

std::string getTag(const token& t);/// ��ȡtag
std::string getAtt(const token& t);///��ȡattribute