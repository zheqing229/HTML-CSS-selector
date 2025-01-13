#include "token.h"

token::token(const token& t)
{
	TagName = t.TagName;
	attribute = t.attribute;
	type = t.type;
	text = t.text;
}

token& token::operator=(const token& t)
{
	TagName = t.TagName;
	type = t.type;
	text = t.text;
	attribute = t.attribute;
	return *this;
}

/// <summary>
/// 判断是何种类型，结束标签特殊调用，其余正常调用
/// </summary>
void token::display() 
{
	std::string show;
	if (!attribute.empty())
		show = '<' + TagName + ' ' + attribute + '>';
	else if (!TagName.empty())
		show = '<' + TagName + '>';
	std::cout << show << '\t';
}

/// <summary>
/// 按照类型组成html形式
/// </summary>
/// <returns>html形式字符串<   > </returns>
std::string token::write()
{
	std::string res;
	switch (type)
	{
	case(StartTag):
		res += '<' + TagName;
		if (!attribute.empty())	res += ' ' + attribute;
		res += '>';
		break;
	case(Comment):
		res = "<!-- " + text + " -->";
		break;
	case(Character):
		res = text;
		break;
	case(DOCTYPE):
		res = "<!DOCTYPE " + attribute + ">";
		break;
	default:
		break;
	}
	return res;
}

/// <summary>
/// 结尾标签特殊实现
/// </summary>
/// <returns>结尾标签html形式字符串</  ></returns>
std::string token::writeEnd()
{
	if (type != StartTag)
		return std::string();
	if (TagName == "br" || TagName == "hr"
		|| TagName == "meta" || TagName == "img"
		|| TagName == "input" || TagName == "area"
		|| TagName == "link" || TagName == "source"
		|| TagName == "base")
		return std::string();
	std::string ret = "</" + TagName + ">";
	return ret;
}

std::ostream& operator<<(std::ostream& out, const token& t)
{
	out << "tagName: " << t.TagName << ' ';
	out << "type: " << t.type << ' ';
	out << "attribute: " << t.attribute << ' ';
	out << "text: " << t.text << '\n';
	return out;
}