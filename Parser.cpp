

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::whole_query() {
		Query tq; 
		query(tq);
		Parser::q = tq; 
}

void Parser::query(Query &tmpq) {
		Query tq; 
		basic_query(tq);
		Expect(5 /* ";" */);
		tmpq = tq; 
}

void Parser::basic_query(Query &tmpq) {
		std::vector <std::wstring> attrs;
		std::vector <std::wstring> tables;
		
		Expect(6 /* "SELECT" */);
		selparams(attrs);
		Expect(7 /* "FROM" */);
		frparams(tables);
		tmpq.attrs = std::move(attrs);
		tmpq.from  = std::move(tables);
		
		if (la->kind == 8 /* "WHERE" */) {
			std::vector<Predicate> preds;
			Get();
			whparams(preds);
			tmpq.where = preds; 
		}
}

void Parser::selparams(std::vector <std::wstring> &attrs) {
		if (la->kind == 9 /* "*" */) {
			Get();
			attrs.push_back(L"ALL"); 
		} else if (la->kind == _attrname) {
			std::wstring attr; 
			selparam(attr);
			attrs.push_back(attr); 
			std::wstring attr1; 
			while (la->kind == 10 /* "," */) {
				Get();
				selparam(attr1);
				attrs.push_back(attr1); 
			}
		} else SynErr(21);
}

void Parser::frparams(std::vector<std::wstring> &tables) {
		std::wstring table; 
		frparam(table);
		tables.push_back(table); 
		std::wstring table1; 
		while (la->kind == 10 /* "," */) {
			Get();
			frparam(table1);
			tables.push_back(table1); 
		}
}

void Parser::whparams(std::vector<Predicate> &preds ) {
		if (la->kind == 11 /* "(" */) {
			std::vector<Predicate> vecpr(2); 
			Get();
			whparam(vecpr[0]);
			if (la->kind == 12 /* "AND" */) {
				Get();
			} else if (la->kind == 13 /* "OR" */) {
				Get();
			} else SynErr(22);
			whparam(vecpr[1]);
			Expect(14 /* ")" */);
			for (int i = 0; i < vecpr.size(); ++i)
			   preds.push_back(vecpr[i]);
			
		} else if (la->kind == _attrname) {
			Predicate pr; 
			whparam(pr);
			preds.push_back(pr); 
			while (la->kind == 12 /* "AND" */ || la->kind == 13 /* "OR" */) {
				std::vector<Predicate> prs; Predicate tmppr; 
				if (la->kind == 12 /* "AND" */) {
					Get();
				} else {
					Get();
				}
				whparam(tmppr);
				preds.push_back(tmppr);            
				
			}
		} else SynErr(23);
}

void Parser::selparam(std::wstring &str) {
		Expect(_attrname);
		str = t->val; 
}

void Parser::frparam(std::wstring &str) {
		Expect(_tablename);
		str = t->val; 
}

void Parser::whparam(Predicate &pr) {
		std::wstring rel; 
		Expect(_attrname);
		pr.left = t->val; 
		relation(rel);
		pr.relation = std::move(rel); 
		std::wstring ra; 
		expr(ra);
		pr.right = ra; 
}

void Parser::relation(std::wstring &rel) {
		if (la->kind == 17 /* "=" */) {
			Get();
			rel = t->val; 
		} else if (la->kind == 18 /* "<" */) {
			Get();
			rel = t->val; 
		} else if (la->kind == 19 /* ">" */) {
			Get();
			rel = t->val; 
		} else SynErr(24);
}

void Parser::expr(std::wstring &ex) {
		std::wstring left, right; 
		term(left);
		ex += left; 
		while (la->kind == 15 /* "+" */ || la->kind == 16 /* "-" */) {
			if (la->kind == 15 /* "+" */) {
				Get();
				term(right);
				ex += L"+"; ex += right; 
			} else {
				Get();
				term(right);
				ex += L"-"; ex += right; 
			}
		}
}

void Parser::term(std::wstring &ex) {
		if (la->kind == _number || la->kind == 16 /* "-" */) {
			bool umin = false; 
			if (la->kind == 16 /* "-" */) {
				Get();
				umin = true; 
			}
			Expect(_number);
			if (umin){
			   ex += L"-"; 
			   ex += t->val;
			}
			else 
			   ex = t->val;  
			
		} else if (la->kind == _attrname) {
			Get();
			ex = t->val; 
		} else if (la->kind == 11 /* "(" */) {
			std::wstring ws; 
			Get();
			expr(ws);
			Expect(14 /* ")" */);
			ex = ws; 
		} else SynErr(25);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	whole_query();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 20;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[1][22] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"word expected"); break;
			case 2: s = coco_string_create(L"number expected"); break;
			case 3: s = coco_string_create(L"tablename expected"); break;
			case 4: s = coco_string_create(L"attrname expected"); break;
			case 5: s = coco_string_create(L"\";\" expected"); break;
			case 6: s = coco_string_create(L"\"SELECT\" expected"); break;
			case 7: s = coco_string_create(L"\"FROM\" expected"); break;
			case 8: s = coco_string_create(L"\"WHERE\" expected"); break;
			case 9: s = coco_string_create(L"\"*\" expected"); break;
			case 10: s = coco_string_create(L"\",\" expected"); break;
			case 11: s = coco_string_create(L"\"(\" expected"); break;
			case 12: s = coco_string_create(L"\"AND\" expected"); break;
			case 13: s = coco_string_create(L"\"OR\" expected"); break;
			case 14: s = coco_string_create(L"\")\" expected"); break;
			case 15: s = coco_string_create(L"\"+\" expected"); break;
			case 16: s = coco_string_create(L"\"-\" expected"); break;
			case 17: s = coco_string_create(L"\"=\" expected"); break;
			case 18: s = coco_string_create(L"\"<\" expected"); break;
			case 19: s = coco_string_create(L"\">\" expected"); break;
			case 20: s = coco_string_create(L"??? expected"); break;
			case 21: s = coco_string_create(L"invalid selparams"); break;
			case 22: s = coco_string_create(L"invalid whparams"); break;
			case 23: s = coco_string_create(L"invalid whparams"); break;
			case 24: s = coco_string_create(L"invalid relation"); break;
			case 25: s = coco_string_create(L"invalid term"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}


