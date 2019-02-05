#pragma once

#include <vector>
#include <string> 

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