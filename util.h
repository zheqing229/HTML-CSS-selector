#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include "token.h"
#include "tree.h"

/// 按照指定分隔符切分字符串
std::vector<std::string> split(const std::string& s, char delimiter);

///','连接是或的关系，先切分；接着按空格切分每一个
void splitQuery(const std::string& query, std::vector<std::vector<std::string>>& Elements);

///  切分空格出来的每一个小项
std::vector<std::string> splitElements(const std::string& element);

/// 切分小项中的attribute 
std::vector<std::string>  splitAttribute(const std::string& input);

/// 切分html文件中token的tag的attribute值
std::vector<std::vector<std::string>> splitTagAttribute(const std::string& input);

/// tag匹配函数
bool matchTag(const std::string& tag, const std::string& tartag);

/// class匹配函数
bool matchClass(const std::string& att, const std::string& clss);

/// id匹配函数
bool matchID(const std::string& att, const std::string& id);

/// attribute匹配函数
bool matchAttVal(const std::string& att, const std::string& input);

/// ~关系下的匹配函数
bool matchWave(const std::string& str, const CSNode<token>* cur,
	bool (*match)(const std::string&, const std::string&), std::string(*get)(const token&));

/// 空格关系下的匹配函数
bool matchSpace(const std::string& str, const CSNode<token>* cur, const CSNode<token>* root,
	bool (*match)(const std::string&, const std::string&), std::string(*get)(const token&));

std::string getParentAtt(const tokenNode& tn); /// 获取父结点attribute
std::string getParentTag(const tokenNode& tn); ///  获取父结点的tag

std::string getMineAtt(const tokenNode& tn); /// 获取自己的tag
std::string getMineTag(const tokenNode& tn);///获取自己的attribute

std::string getSiblingAtt(const tokenNode& tn);/// 获取兄弟结点的attribute
std::string getSiblingTag(const tokenNode& tn);/// 获取兄弟结点的tag

std::string getTag(const token& t);/// 获取tag
std::string getAtt(const token& t);///获取attribute