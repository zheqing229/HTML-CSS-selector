#include "terminal.h"

void terminal::run()
{
	while (true)
	{
		getInstruction();
	}
}

void terminal::getInstruction()
{
	std::string instruction;
	getline(std::cin, instruction);
	int len = instruction.length();
	if (instruction[0] == 'r' && instruction[1] == 'e' && instruction[2] == 'a' && instruction[3] == 'd' && instruction[4] == '(')
	{
		std::string input;
		for(int i = 5; i < len && instruction[i] != ')'; i++)
		{
			if (instruction[i] == '"')
				continue;
			input.push_back(instruction[i]);
		}
		readFile(input);
		std::cout << "Done." << '\n' << '\n';
	}
	else if (instruction[0] == 'q' && instruction[1] == 'u' && instruction[2] == 'e' && instruction[3] == 'r' && instruction[4] == 'y')
	{
		sqresult.Clear();
		result.Clear();
		Elements.clear();
		std::string input;
		for (int i = 6; instruction[i] != ')' && i < len; i++)
		{
			if (instruction[i] == '"')
				continue;
			input += instruction[i];
		}
		query(input, tokenTree.root->firstchild);
		std::cout << '\n';
	}
	else if (instruction[0] == 'O' && instruction[1] == 'u' && instruction[2] == 't')
	{
		std::string input;
		std::string num;
		int i = 0;
		for (i = 4; instruction[i] != ']'; i++)
		{
			num += instruction[i];
		}
		int k = std::stoi(num);
		i += 2;
		input = instruction.substr(i, instruction.length() - i);
		if (k < 0 || k >= sqresult.Length())
		{
			std::cout << "Index out of range." << '\n' << '\n';
			return;
		}
		outQuery(input, k);
		std::cout << '\n';
	}
}

void terminal::readFile(std::string filepath)
{
	std::ifstream infile;
	infile.open(filepath, std::ios::in | std::ios::binary);
	//std::cout << filepath << '\n';
	if (!infile.is_open())
	{
		std::cout << "读取文件失败" << std::endl;
		return;
	}
	std::string s;
	SqList<std::string> sl;
	// 把文件一次读入，去除不必要的空格，按行存储在顺序表sl
	while (getline(infile, s))
	{
		while (s[0] == ' ' && s[1] == ' ') s.erase(0, 1);
		s += " ";
		sl.push_back(s);
	}

	infile.close();
	LinkList<token> tokenList;
	splitTokens(sl, tokenList);
	buildDomTree(tokenList);
	// tokenTree.InOrder();
}


/// 一行一行处理，读入到'<'，跳转tagOpen函数进行切分，否则按照文字存储 
void terminal::splitTokens(SqList<std::string>& sql, LinkList<token>& tokenList)
{
	int size = sql.Length();
	if (!size)
		return;
	
	std::string html;
	for (int i = 0; i < size; i++)
	{
		html.append(sql[i]);
	}

	int len = html.length();
	for (int i = 0; i < len; i++)
	{
		token t;
		if (html[i] == '<')
		{
			++i;
			int j = tagOpen(html, i, t, len);
			i = j;
			tokenList.append(t);
			continue;
		}
		else if (html[i] == EOF)
		{
			t.setType(EndOfFile);
			tokenList.append(t);
			break;
		}
		else
		{
			t.setType(Character);
			std::string text;
			int j = 0;
			for ( j = i; j < len; j++)
			{
				if (html[j] == '<')
					break;
				text += html[j];
			}
			i = j - 1;
			t.setText(text);
			tokenList.append(t);
			continue;
		}
	}
	//std::cout << tokenList;
}


/// tagOpen之后，判断接下来的字符
/// 1. '/'按结束tag处理
/// 2. '!'
///		2.1 '!-- '按照注释处理
///		2.2 '!DOCTYPE 按照DOCTYPE处理
/// 3. 处理成开始tag
int terminal::tagOpen(std::string& s, int i, token& t, int len)
{
	if (s[i] == '/')
	{
		t.setType(EndTag);
		std::string tagName;
		int j = 0;
		for (j = i + 1; s[j] != '>' && j < len; j++)
		{
			tagName += s[j];
		}
		//j++;
		t.setTagName(tagName);
		return j;
	}
	else if (s[i] == '!')
	{
		if (s[i + 1] == '-' && s[i + 2] == '-')
		{
			i += 4;
			int j = 0;
			t.setType(Comment);
			std::string comment;
			for (j = i; j < len; j++)
			{
				if (s[j] == '-' && s[j + 1] == '-' && s[j + 2] == '>')
					break;
				comment += s[j];
			}
			t.setText(comment);
			j += 2;
			return j;
		}
		else
		{
			i += 9;
			int j = 0;
			std::string doc;
			t.setType(DOCTYPE);
			for (j = i; s[j] != '>' && j < len; j++)
			{
				doc += s[j];
			}
			t.setAttribute(doc);
			return j;
		}
	}
	else if ((s[i] <= 'z' && s[i] >= 'a') || (s[i] >= 'A' && s[i] <= 'Z'))
	{
		std::string tagName;
		t.setType(StartTag);
		int j = 0;
		for (j = i; j < len; j++)
		{
			if (s[j] == ' ')
			{
				std::string attribute;
				int m = 0;
				for (m = j + 1; s[m] != '>' && m < len; m++)
				{
					attribute += s[m];
				}
				//m++;
				t.setAttribute(attribute);
				t.setTagName(tagName);
				return m;
			}
			else if (s[j] == '>')
			{
				//j++;
				t.setTagName(tagName);
				return j;
			}
			tagName += s[j];
		}
	}
}

/// 建立的DOMTree，有个根节点"document"，不是文件的内容，其兄弟结点是DOCTYPE
/// 遍历建立的链表token
/// 用栈辅助
/// 1. 遇到StartTag，把这个token存储到当前栈顶结点的孩子结点，接着入栈。而自闭合标签存储在当前栈顶结点的孩子结点
/// 2. 遇到EndTag，不存储到树里，弹栈直到弹出的与EndTag相匹配
/// 3. 文本存储在当前栈顶结点的孩子结点
void terminal::buildDomTree(LinkList<token>& tokenList)
{
	if (!tokenList.len())
		return;
	//std::cout << tokenList;
	token tokenRoot;
	tokenRoot.setType(StartTag);
	std::string document = "document";
	tokenRoot.setTagName(document);

	tokenTree.root->data = tokenRoot;
	LinkedStack<tokenNode> tokenStack;
	tokenNode tNode = { tokenRoot, tokenTree.root };
	tokenStack.Push(tNode);

	CSNode<token>* pCSNode = tokenTree.root;
	LNode<token>*pLNode = tokenList.getHead()->getNext();
	while (pLNode != NULL)
	{
		token temp = pLNode->getData();
		//std::cout << temp << '\n';
		if (temp.getType() == StartTag)
		{
			std::string startTagName = temp.getTagName();
			if (startTagName != "br" && startTagName != "hr" 
				&& startTagName != "meta" && startTagName != "img"
				&& startTagName != "input" && startTagName != "area"
				&& startTagName != "link" && startTagName != "source"
				&& startTagName != "base")	// 去除自闭合标签
			{
				tokenNode top = tokenStack.getTopElement();
				CSNode<token>* pTemp = top.ptr->firstchild;
				CSNode<token>* current = NULL;
				if (pTemp == NULL)
				{
					top.ptr->firstchild = new CSNode<token>;
					top.ptr->firstchild->data = temp;
					top.ptr->firstchild->parent = top.ptr;
					current = top.ptr->firstchild;
				}
				else
				{
					while (pTemp->nextsibling != NULL)
					{
						pTemp = pTemp->nextsibling;
					}
					pTemp->nextsibling = new CSNode<token>;
					pTemp = pTemp->nextsibling;
					pTemp->data = temp;
					pTemp->parent = top.ptr;
					current = pTemp;
				}
				tokenNode tnode = { temp, current };
				tokenStack.Push(tnode);

				pLNode = pLNode->getNext();
				pCSNode = current;
				continue;
			}
			// 处理自闭合标签
			else
			{
				tokenNode top = tokenStack.getTopElement();
				CSNode<token>* pTemp = top.ptr->firstchild;
				if (pTemp == NULL)
				{
					top.ptr->firstchild = new CSNode<token>;
					top.ptr->firstchild->data = temp;
					top.ptr->firstchild->parent = top.ptr;
				}
				else 
				{
					while (pTemp->nextsibling != NULL)
					{
						pTemp = pTemp->nextsibling;
					}
					pTemp->nextsibling = new CSNode<token>;
					pTemp = pTemp->nextsibling;
					pTemp->data = temp;
					pTemp->parent = top.ptr;
				}
				
				pLNode = pLNode->getNext();
			}
		}
		else if (temp.getType() == EndTag)
		{
			tokenNode tempNode = tokenStack.getTopElement();
			while (tempNode.t.getTagName() != temp.getTagName())
			{
				tokenStack.Pop();
				tempNode = tokenStack.getTopElement();
			}
			tokenStack.Pop();
			tempNode = tokenStack.getTopElement();
			
			pCSNode = tempNode.ptr;
			pLNode = pLNode->getNext();
		}
		else if (temp.getType() == DOCTYPE)
		{
			tokenTree.root->nextsibling = new CSNode<token>;
			tokenTree.root->nextsibling->data = temp;
			pLNode = pLNode->getNext();
			continue;
		}
		else
		{
			tokenNode top = tokenStack.getTopElement();
			CSNode<token>* pTemp = top.ptr->firstchild;
			if (pTemp == NULL)
			{
				top.ptr->firstchild = new CSNode<token>;
				top.ptr->firstchild->data = temp;
				top.ptr->firstchild->parent = top.ptr;
			}
			else
			{
				while (pTemp->nextsibling != NULL)
				{
					pTemp = pTemp->nextsibling;
				}
				pTemp->nextsibling = new CSNode<token>;
				pTemp = pTemp->nextsibling;
				pTemp->data = temp;
				pTemp->parent = top.ptr;
			}

			pLNode = pLNode->getNext();
		}
	}
}

void terminal::query(std::string& input, CSNode<token>* ptr)
{
	if (input[0] == '*' && input.length() == 1 && ptr == tokenTree.root)
	{
		startwithAsterisk();
		return;
	}
	splitQuery(input, Elements);
	size_t outerSize = Elements.size();
	for (size_t i = 0; i < outerSize; i++)
	{
		int innerSize = Elements[i].size();
		std::string last = Elements[i][innerSize - 1];
		// 从后往前找，逐步缩小范围，提升性能
		initializeRes(last, ptr);
		for (int j = innerSize - 2; j >= 0; j--)
		{
			
			if (Elements[i][j] == ">")
			{
				filterTN(Elements[i][j - 1], ptr, getParentAtt, getParentTag);
				j--;
			}
			else if (Elements[i][j] == "+")
			{
				filterTN(Elements[i][j - 1], ptr, getSiblingAtt, getSiblingTag);
				j--;
			}
			else if (Elements[i][j] == "~")
			{
				filterWave(Elements[i][j - 1], ptr);
				j--;
			}
			else
			{
				filterSpace(Elements[i][j], ptr);
			}
		}
		ll2sql();
		result.Clear();
		if (result.getHead() == NULL)
		{
			LNode<tokenNode>* p = new LNode<tokenNode>;
			result.setHead(p);
		}
			
	}
	int size = sqresult.Length();
	for (int i = 0; i < size; i++)
	{
		sqresult[i].t.display();
	}
	std::cout << '\n' << size << '\n';
}

void terminal::PreOrderAst(CSNode<token>* ptr)
{
	if (ptr == NULL) return;
	token temp = ptr->data;
	if (temp.getType() == StartTag)
	{
		tokenNode tempNode;
		tempNode.t = temp;
		tempNode.ptr = ptr;
		//sqresult.push_back(tempNode);
		result.append(tempNode);
		//temp.display();
	}
	PreOrderAst(ptr->firstchild);
	PreOrderAst(ptr->nextsibling);
}

void terminal::startwithAsterisk()
{
	PreOrderAst(tokenTree.root->firstchild);
	ll2sql();
	int size = sqresult.Length();
	for (int i = 0; i < size; i++)
	{
		sqresult[i].t.display();
	}
	std::cout << '\n' << size << '\n';
}

void terminal::initializeRes(const std::string& input, CSNode<token>* ptr)
{
	std::vector<std::string> splitedInput = splitElements(input);
	insertTN(splitedInput[0], ptr);
	size_t size = splitedInput.size();
	for (size_t i = 1; i < size; i++)
	{
		filterTN(splitedInput[i], ptr, getMineAtt, getMineTag);
	}
}

void terminal::insertTN(const std::string& input, CSNode<token>* ptr)
{
	if (input[0] == '*')
	{
		PreOrderAst(ptr); //tokenTree.root->firstchild);
	}
	else if (input[0] == '#')
	{
		std::string id = input.substr(1, input.length() - 1);
		preOrderID(ptr, id);// tokenTree.root->firstchild, id);
	}
	else if (input[0] == '.')
	{
		std::string clss = input.substr(1, input.length() - 1);
		preOrderClass(ptr, clss);// tokenTree.root->firstchild, clss);
	}
	else if (input[0] == '[')
	{
		preOrderAttVal(ptr, input);// tokenTree.root->firstchild, input);
	}
	else
	{
		preOrderTag(ptr, input);// tokenTree.root->firstchild, input);
	}
}

void terminal::preOrderTag(CSNode<token>* ptr, const std::string& tag)
{
	if (ptr == NULL)
		return;
	token temp = ptr->data;
	if (temp.getType() == StartTag && matchTag(temp.getTagName(), tag))// && temp.getTagName() == tag)
	{
		tokenNode tempNode;
		tempNode.t = temp;
		tempNode.ptr = ptr;
		result.append(tempNode);
	}
	preOrderTag(ptr->firstchild, tag);
	preOrderTag(ptr->nextsibling, tag);
}

void terminal::preOrderClass(CSNode<token>* ptr, const std::string& clss)
{
	if (ptr == NULL)
		return;
	token temp = ptr->data;
	if (temp.getType() == StartTag && !temp.getAttribute().empty())
	{
		std::string att = temp.getAttribute();
		if (matchClass(att, clss))
		{
			tokenNode tempNode;
			tempNode.t = temp;
			tempNode.ptr = ptr;
			result.append(tempNode);
		}
	}
	preOrderClass(ptr->firstchild, clss);
	preOrderClass(ptr->nextsibling, clss);
}

void terminal::preOrderID(CSNode<token>* ptr, const std::string& id)
{
	if (ptr == NULL)
		return;
	token temp = ptr->data;
	if (temp.getType() == StartTag && !temp.getAttribute().empty())
	{
		std::string att = temp.getAttribute();
		if (matchID(att, id))
		{
			tokenNode tempNode;
			tempNode.t = temp;
			tempNode.ptr = ptr;
			result.append(tempNode);
		}

	}
	preOrderID(ptr->firstchild, id);
	preOrderID(ptr->nextsibling, id);
}

void terminal::preOrderAttVal(CSNode<token>* ptr, const std::string& input)
{
	if (ptr == NULL)
		return;
	token temp = ptr->data;
	if (temp.getType() == StartTag && !temp.getAttribute().empty())
	{
		std::string att = temp.getAttribute();
		if (matchAttVal(att, input))
		{
			tokenNode tempNode;
			tempNode.t = temp;
			tempNode.ptr = ptr;
			result.append(tempNode);
		}
		
	}
	preOrderAttVal(ptr->firstchild, input);
	preOrderAttVal(ptr->nextsibling, input);
}

void terminal::filterTN(const std::string& input, CSNode<token>* ptr, std::string(*funcget1)(const tokenNode&), std::string(*funcget2)(const tokenNode&))
{
	if (input[0] == '.')
	{
		std::string fit = input.substr(1, input.length() - 1);	// 去除'.'
		filter(fit, ptr, matchClass, funcget1);
		/*LNode<tokenNode>* prev = result.getHead();
		LNode<tokenNode>* cur = result.getHead()->getNext();
		while (cur != NULL)
		{
			std::string att = cur->getData().t.getAttribute();
			if (!matchClass(att, fit))
			{
				result.Delete(prev, cur);
				continue;
			}
			prev = cur;
			cur = cur->getNext();
		}*/
	}
	else if (input[0] == '#')
	{
		std::string fit = input.substr(1, input.length() - 1);	// 去除'#'
		filter(fit, ptr, matchID, funcget1);
		/*LNode<tokenNode>* prev = result.getHead();
		LNode<tokenNode>* cur = result.getHead()->getNext();
		while (cur != NULL)
		{
			std::string att = cur->getData().t.getAttribute();
			if (!matchID(att, fit))
			{
				result.Delete(prev, cur);
				continue;
			}
			prev = cur;
			cur = cur->getNext();
		}*/
	}
	else if (input[0] == '[')
	{
		filter(input, ptr, matchAttVal, funcget1);
		/*LNode<tokenNode>* prev = result.getHead();
		LNode<tokenNode>* cur = result.getHead()->getNext();
		while (cur != NULL)
		{
			std::string att = cur->getData().t.getAttribute();
			if (!matchAttVal(att, input))
			{
				result.Delete(prev, cur);
				continue;
			}
			prev = cur;
			cur = cur->getNext();
		}*/
	}
	else
	{
		filter(input, ptr, matchTag, funcget2);
	}
}


void terminal::filterWave(const std::string& input, CSNode<token>* ptr)
{
	std::vector<std::string> splitInput = splitElements(input);
	int size = splitInput.size();
	for (int i = 0; i < size; i++)
	{
		if (splitInput[i][0] == '.')
		{
			std::string fit = splitInput[i].substr(1, splitInput[i].length() - 1);	// 去除'.'
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchWave(fit, cur->getData().ptr, matchClass, getAtt))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
		else if (splitInput[i][0] == '#')
		{
			std::string fit = splitInput[i].substr(1, splitInput[i].length() - 1);	// 去除'#'
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchWave(fit, cur->getData().ptr, matchID, getAtt))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
		else if (splitInput[i][0] == '[')
		{
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchWave(splitInput[i], cur->getData().ptr, matchAttVal, getAtt))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
		else
		{
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchWave(splitInput[i], cur->getData().ptr, matchTag, getTag))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
	}
}

void terminal::filterSpace(const std::string& input, CSNode<token>* ptr)
{
	std::vector<std::string> splitInput = splitElements(input);
	int size = splitInput.size();
	for (int i = 0; i < size; i++)
	{
		if (splitInput[i][0] == '.')
		{
			std::string fit = splitInput[i].substr(1, splitInput[i].length() - 1);	// 去除'.'
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchSpace(fit, cur->getData().ptr, tokenTree.root, matchClass, getAtt))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
		else if (splitInput[i][0] == '#')
		{
			std::string fit = splitInput[i].substr(1, splitInput[i].length() - 1);	// 去除'#'
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchSpace(fit, cur->getData().ptr, tokenTree.root, matchID, getAtt))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
		else if (splitInput[i][0] == '[')
		{
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchSpace(splitInput[i], cur->getData().ptr, tokenTree.root, matchAttVal, getAtt))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
		else
		{
			LNode<tokenNode>* prev = result.getHead();
			LNode<tokenNode>* cur = result.getHead()->getNext();
			while (cur != NULL)
			{
				if (!matchSpace(splitInput[i], cur->getData().ptr, tokenTree.root, matchTag, getTag))
				{
					result.Delete(prev, cur);
					continue;
				}
				prev = cur;
				cur = cur->getNext();
			}
		}
	}
}

void terminal::filter(const std::string& input, CSNode<token>* ptr, bool(*func)(const std::string&, const std::string&), std::string (*funcGet)(const tokenNode&))
{
	LNode<tokenNode>* prev = result.getHead();
	LNode<tokenNode>* cur = result.getHead()->getNext();
	while (cur != NULL)
	{
		std::string att = funcGet(cur->getData());
		//std::string att = cur->getData().t.getAttribute();
		//std::string att = cur->getData().ptr->parent->data.getAttribute();
		//std::string att = cur->getData().ptr->nextsibling->data.getAttribute();
		if (!func(att, input))
		{
			result.Delete(prev, cur);
			continue;
		}
		prev = cur;
		cur = cur->getNext();
	}
}

void terminal::ll2sql()
{
	LNode<tokenNode>* p = result.getHead()->getNext();
	int size = sqresult.Length();
	while (p != NULL)
	{
		tokenNode temp = p->getData();
		bool repetition = false;
		for (int i = 0; i < size; i++)
		{
			if (temp.ptr == sqresult[i].ptr)
			{
				repetition = true;
				break;
			}
		}
		if (!repetition)
			sqresult.push_back(p->getData());
		p = p->getNext();
	}
}

void terminal::outQuery(std::string& input, int k)
{
	std::string output = R"()";
	if (input == "innerText")
	{
		output = outText(k);
	}
	else if (input == "outerHTML")
	{
		output = outHTML(k);
	}
	else if (input == "href")
	{
		output = outHref(k);
	}
	else
	{
		std::string query = input.substr(7, input.length() - 9);
		outSelector(k, query);
	}
	std::istringstream iss(output);

	// 使用 std::getline 循环逐行输出
	std::string line;
	while (std::getline(iss, line, '\r')) {
		std::cout << line << std::endl;
	}
	// std::cout << output << '\n';
}

std::string terminal::outHref(int k)
{
	token tokenk = sqresult[k].t;
	if (tokenk.getTagName() != "a")
	{
		std::cout << "This is not 'a' tag." << '\n';
		return std::string();
	}
	std::vector<std::vector<std::string>> att_val = splitTagAttribute(tokenk.getAttribute());
	for (auto& i : att_val)
	{
		if (i[0] == "href")
		{
			return i[1];
		}
	}
	return std::string();
}

std::string terminal::outText(int k)
{
	tokenNode nodek = sqresult[k];
	std::string text;
	preOrderText(nodek.ptr->firstchild, text);
	//std::cout << text << '\n';
	return text;
}

std::string terminal::outHTML(int k)
{
	tokenNode nodek = sqresult[k];
	std::string HTML;
	HTML += nodek.t.write();
	preOrderHTML(nodek.ptr->firstchild, HTML);
	HTML += nodek.t.writeEnd();
	return HTML;
}

void terminal::outSelector(int k, std::string& input)
{
	tokenNode nodek = sqresult[k];
	sqresult.Clear();
	Elements.clear();
	query(input, nodek.ptr->firstchild);
}

void terminal::preOrderText(CSNode<token>* ptr, std::string& text)
{
	if (ptr == NULL)return;
	token t = ptr->data;
	if (t.getType() == Character)
	{
		text += t.getText();
		//std::cout << text << '\n';
	}
	preOrderText(ptr->firstchild, text);
	preOrderText(ptr->nextsibling, text);
}

void terminal::preOrderHTML(CSNode<token>* ptr, std::string& HTML)
{
	if (ptr == NULL)
		return;
	token t = ptr->data;
	HTML += t.write();
	preOrderHTML(ptr->firstchild, HTML);
	preOrderHTML(ptr->nextsibling, HTML);
	HTML += t.writeEnd();
}

