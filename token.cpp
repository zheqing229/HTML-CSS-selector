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
/// �ж��Ǻ������ͣ�������ǩ������ã�������������
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
/// �����������html��ʽ
/// </summary>
/// <returns>html��ʽ�ַ���<   > </returns>
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
/// ��β��ǩ����ʵ��
/// </summary>
/// <returns>��β��ǩhtml��ʽ�ַ���</  ></returns>
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