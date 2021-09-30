#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
struct Node{
    string var;
	string type;
	string data;
	int pos;
	bool seen = 0;
	int level = 0;
	bool open = 0;
	bool closed = 0;
	bool status = 1;
	bool print = 0;
    Node* link;
    Node* link_prev;
	Node* link_tail;
	Node* next;
	Node* prev;
	Node(){
    	this->link = nullptr;
    	this->link_prev = nullptr;
		this->link_tail = nullptr;
		this-> next = nullptr;
		this->prev = nullptr;
	}
	Node(int level = 0 , Node* n = nullptr , Node* pr = nullptr)
		:level(level), next(n), prev(pr)
	{}
};
class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
    string INSERT_(Node* pos_node , string &data , const int &level , const string &error);
    string ASSIGN_(Node* pos_node , string &data , const int &level ,  const string &error);
	void LOOKUP_(Node* pos_node ,const int &lev, const string &var , const string &error);
	void PRINT_(Node* pos_node , const int &lev);
	void RPRINT_(Node* pos_node, const int &lev);
};
Node* creativeNode(Node* __NODE , const int &level);
string find_and_cut(string&data ,const string& element);
void deleteAllNode(Node *__NODE);
bool const_number(const string &number);
bool const_string(const string &string);
#endif