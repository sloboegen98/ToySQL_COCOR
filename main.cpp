#include <iostream>
#include <vector>
#include <string>

#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>

int main (int argc, char *argv[]) {

	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		Scanner *scanner = new Scanner(fileName);
		Parser *parser = new Parser(scanner);
    	parser->Parse();
		coco_string_delete(fileName);

		(parser->q).print();

		delete parser;
		delete scanner;
	} else
		printf("-- No source file specified\n");

	return 0;

}