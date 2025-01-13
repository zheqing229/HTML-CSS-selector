#include "util.h"

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        if (token != " " && !token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

void splitQuery(const std::string& query, std::vector<std::vector<std::string>>& Elements) {
    // Split the query based on comma to separate different selectors
    std::vector<std::string> selectors = split(query, ',');

    // Process each selector
    for (const std::string& selector : selectors) {
        // Split the selector based on space to identify descendant relationships
        std::vector<std::string> elements = split(selector, ' ');

        Elements.push_back(elements);
    }
}

std::vector<std::string> splitElements(const std::string& element) {
    std::vector<std::string> tags;
    int len = element.length();

    for (int i = 0; i < len; ++i) {
        std::string tag;

        if (element[i] == '.' || element[i] == '#') {
            tag += element[i];
            for (int j = i + 1; (element[j] >= 'a' && element[j] <= 'z') || (element[j] >= 'A' && element[j] <= 'Z')
                || element[j] == '_' || element[j] == '-' || (element[j] >= '0' && element[j] <= '9' && j < len); ++j) {
                tag += element[j];
            }
            tags.push_back(tag);
            i += tag.length() - 1;
        }
        else if (element[i] == '[') {
            tag += '[';
            for (int j = i + 1; element[j] != ']' && j < len; ++j) {
                tag += element[j];
            }
            tag += ']';
            tags.push_back(tag);
            i += tag.length() - 1;
        }
        else if (element[i] == '*')
        {
            tags.push_back("*");
        }
        else{
            for (int j = i; (element[j] >= 'a' && element[j] <= 'z') || (element[j] >= 'A' && element[j] <= 'Z')
                || element[j] == '_' || element[j] == '-' || (element[j] >= '0' && element[j] <= '9' && j < len); ++j) {
                tag += element[j];
            }
            tags.push_back(tag);
            i += tag.length() - 1;
        }
    }

    return tags;
}

std::vector<std::string>  splitAttribute(const std::string& input)
{
	std::string temp = input.substr(1, input.length() - 2);
    std::vector<std::string> res = split(temp, '=');
    if (res.size() == 1)
        res.push_back("%");
    else {
        int len1 = res[0].length();
        char a = res[0][len1 - 1];
        if (a == '$' || a == '^' || a == '*')
        {
            res[0].pop_back();
            std::string s;
            s = a;
            res.push_back(s);
        }
        else
            res.push_back("=");
    }
    return res;
}

std::vector<std::vector<std::string>> splitTagAttribute(const std::string& input)
{
    std::vector<std::string> att_val;
    int len = input.length();
    for (int i = 0; i < len; i++)
    {
        std::string single;
        for (int j = i;  j < len; j++)
        {  
            single += input[j];
            if (input[j] == '"' && input[j + 1] == ' ')
                break;
            
        }
        i += single.length();
        att_val.push_back(single);
    }
    std::vector<std::vector<std::string >> res;
    for (auto& i : att_val)
    {
        std::vector<std::string> temp = split(i, '=');
        res.push_back(temp);
    }
    return res;
}

bool matchTag(const std::string& tag, const std::string& tartag)
{
    if (tag == tartag) return true;
    return false;
}

bool matchClass(const std::string& att, const std::string& clss)
{
    if (att.empty()) return false;
    std::vector<std::vector<std::string>> att_val = splitTagAttribute(att);
    size_t attsize = att_val.size();
    for (size_t i = 0; i < attsize; i++)
    {
        if (att_val[i][0] == "class")
        {
            std::string val = att_val[i][1];
            val = val.substr(1, val.length() - 2);
            std::vector<std::string> valvec = split(val, ' ');
            for (auto& i : valvec)
            {
                if (i == clss)
                    return true;
            }
        }
    }
    return false;
}

bool matchID(const std::string& att, const std::string& id)
{
    if (att.empty()) return false;
    std::vector<std::vector<std::string>> att_val = splitTagAttribute(att);
    size_t attsize = att_val.size();
    for (size_t i = 0; i < attsize; i++)
    {
        if (att_val[i][0] == "id")
        {
            std::string val = att_val[i][1];
            val = val.substr(1, val.length() - 2);
            
            if (val == id)
                return true;
        }
    }
    return false;
}

bool matchAttVal(const std::string& att, const std::string& input)
{
    if (att.empty()) return false;
    std::vector<std::string> target = splitAttribute(input);
	std::vector<std::vector<std::string>> att_val = splitTagAttribute(att);
	size_t attsize = att_val.size();
	size_t tarsize = target.size();
    if (target[tarsize - 1] == "%")
    {
        for (size_t i = 0; i < attsize; i++)
            if (att_val[i][0] == target[0])
                return true;
    }
	else if (target[tarsize - 1] == "=")
	{
		for (size_t i = 0; i < attsize; i++)
		{
			if (att_val[i][0] == target[0])
			{
				std::string val = att_val[i][1];
				val = val.substr(1, val.length() - 2);
                std::string tarval = target[1];// .substr(1, target[1].length() - 2);
				if (val == tarval)
					return true;
				
			}
		}
	}
	else if (target[tarsize - 1] == "^")
	{
		for (size_t i = 0; i < attsize; i++)
		{
			if (att_val[i][0] == target[0])
			{
				std::string val = att_val[i][1];
				val = val.substr(1, val.length() - 2);

                std::string tarval = target[1];// .substr(1, target[1].length() - 2);

				if (val.find(tarval) == 0)
				{
                    return true;
				}
			}
		}
	}
	else if (target[tarsize - 1] == "$")
	{
		for (size_t i = 0; i < attsize; i++)
		{
			if (att_val[i][0] == target[0])
			{
                std::string val = att_val[i][1];
                val = val.substr(1, val.length() - 2);

                std::string tarval = target[1];// .substr(1, target[1].length() - 2);
				
				if (val.find_last_of(tarval) == val.length() - 1)
				    return true;
			}
		}
	}
	else if (target[tarsize - 1] == "*")
	{
		for (size_t i = 0; i < attsize; i++)
		{
            if (att_val[i][0] == target[0])
            {
                std::string val = att_val[i][1];
                val = val.substr(1, val.length() - 2);

                std::string tarval = target[1];// .substr(1, target[1].length() - 2);

                if (val.find(tarval) != -1)
                    return true;
            }
				
		}
	}

    return false;
}

bool matchWave(const std::string& str, const CSNode<token>* cur, 
    bool(*match)(const std::string&, const std::string&), std::string (*get)(const token&))
{
    CSNode<token>* bro = cur->parent->firstchild;
    while (bro->data.getType() != StartTag)
        bro = bro->nextsibling;
    CSNode<token>* sis = bro->nextsibling;
    if (bro == cur) return false;
    while (sis != cur)
    {
        std::string tomatch = get(bro->data);
        if (match(tomatch, str))
            return true;
        if (sis->data.getType() == StartTag)
            bro = sis;
        sis = sis->nextsibling;
    }

    return false;
}

bool matchSpace(const std::string& str, const CSNode<token>* cur, const CSNode<token>* root,
    bool(*match)(const std::string&, const std::string&), std::string(*get)(const token&))
{
    CSNode<token>* parent = cur->parent;
    while (parent != root)
    {
        std::string tomatch = get(parent->data);
        if (match(tomatch, str))
            return true;
        parent = parent->parent;
    }
    return false;
}



std::string getParentAtt(const tokenNode& tn) 
{ return tn.ptr->parent->data.getAttribute(); }

std::string getParentTag(const tokenNode& tn)
{
    return tn.ptr->parent->data.getTagName();
}

std::string getMineAtt(const tokenNode& tn) { return tn.t.getAttribute(); }

std::string getMineTag(const tokenNode& tn)
{
    return tn.t.getTagName();
}

std::string getSiblingAtt(const tokenNode& tn)
{
    CSNode<token>* bro = tn.ptr->parent->firstchild;
    while (bro->data.getType() != StartTag)
        bro = bro->nextsibling;
        
    if (bro == tn.ptr) return std::string(); // 自己就是长兄结点，返回空字符串
   
    CSNode<token>* sis = bro->nextsibling;
    
    while (sis != tn.ptr)
    {
        if (sis->data.getType() == StartTag)
            bro = sis;
        sis = sis->nextsibling;
    }
    return bro->data.getAttribute();
    
}

std::string getSiblingTag(const tokenNode& tn) {
    CSNode<token>* bro = tn.ptr->parent->firstchild;
    while (bro->data.getType() != StartTag)
        bro = bro->nextsibling;
    if (bro == tn.ptr) return std::string(); // 不存在长兄结点，返回空字符串
    CSNode<token>* sis = bro->nextsibling;
    
    while (sis != tn.ptr)
    {
        if (sis->data.getType() == StartTag)
            bro = sis;
        sis = sis->nextsibling;
    }
    return bro->data.getTagName();
}

std::string getTag(const token& t)
{
    return t.getTagName();
}

std::string getAtt(const token& t)
{
    return t.getAttribute();
}

