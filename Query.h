#pragma once

#include <vector>
#include <string> 
#include <fstream>
#include <iostream>

/*struct Node{
	enum Type {ATTR, NUMBER, OP};
	Node (Type t, Node* l = nullptr, Node* r = nullptr) : type(t), left(l), right(r) {}
	Node::Type getType () { return type; }
	virtual ~Node() {} 
	Node* left;
	Node* right;
	Type type;
};

struct NumberNode : Node {
	NumberNode(Node* l, Node* r, double val) : Node(Type::NUMBER, l, r), value(val) {}
	double value;
};

struct AttrNode : Node {
	AttrNode(Node* l, Node* r, std::wstring name_) : Node(Type::ATTR, l , r), attrname(name_) {}
	std::wstring attrname;
};

struct OperNode : Node {
	OperNode(Node* l, Node* r, std::wstring oper) : Node(Type::OP, l , r), op(oper) {}
	std::wstring op;
};
*/
struct Predicate{
    std::wstring left;
    std::wstring right;
    std::wstring relation;
};

struct Query{
	std::vector <std::wstring> attrs;
	std::vector <std::wstring> from;
	std::vector <Predicate> where;
	void print();
};