

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "Query.h"


#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_word=1,
		_number=2,
		_tablename=3,
		_attrname=4
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

Query q;

double WstrToDouble(std::wstring const &str){
    double val;
    std::wstringstream input(str);

    input >> val;

    return val;
}

/*----------------- Scanner Specifications ----------------------*/



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void whole_query();
	void query(Query &tmpq);
	void basic_query(Query &tmpq);
	void selparams(std::vector <std::wstring> &attrs);
	void frparams(std::vector<std::wstring> &tables);
	void whparams(std::vector<Predicate> &preds );
	void selparam(std::wstring &str);
	void frparam(std::wstring &str);
	void whparam(Predicate &pr);
	void relation(std::wstring &rel);
	void expr(std::wstring &ex);
	void term(std::wstring &ex);

	void Parse();

}; // end Parser



#endif

